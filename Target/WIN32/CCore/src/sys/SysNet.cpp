/* SysNet.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: Target/WIN32
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2015 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <CCore/inc/sys/SysNet.h>
#include <CCore/inc/sys/SysWaitInternal.h>

#include <CCore/inc/Exception.h>

#include <CCore/inc/win32/Win32.h>

namespace CCore {
namespace Sys {

/* GetPlanInitNode_...() */

namespace Private_SysNet {

/* class WSAInitExit */

class WSAInitExit : NoCopy
 {
  public:

   WSAInitExit()
    {
     Win32::WSAInfo info;

     AbortIf( Win32::WSAStartup(Win32::WSAVersion_2_00,&info) ,"Win32::WSAStartup()");
    }

   ~WSAInitExit()
    {
     AbortIf( Win32::WSACleanup() ,"Win32::WSACleanup()");
    }

   static const char * GetTag() { return "SysNet"; }
 };

PlanInitObject<WSAInitExit> Object CCORE_INITPRI_1 ;

/* WSAError() */

ErrorType WSAError()
 {
  if( auto ret=Win32::WSAGetLastError() ) return ErrorType(ret);

  return Error_SysErrorFault;
 }

/* class Address */

class Address : NoCopy
 {
   static const ulen Len = 16 ;

  private:

   uint8 data[Len];

  private:

   void setFamily(uint16 address_family=Win32::WSA_IPv4);

   void setPort(Net::UDPort udport);

   void setAddress(Net::IPAddress address);

  public:

   Address() {}

   explicit Address(Net::UDPort udport);

   explicit Address(Net::UDPoint udpoint);

   // get

   void get(Net::UDPoint &udpoint);

   // raw

   uint8 * getPtr() { return data; }

   const uint8 * getPtr() const { return data; }

   ulen getLen() const { return Len; }
 };

void Address::setFamily(uint16 address_family)
 {
  BufPutDev dev(data);

  dev.use<LeOrder>(address_family);
 }

void Address::setPort(Net::UDPort udport)
 {
  BufPutDev dev(data+2);

  dev.use<BeOrder>(udport);
 }

void Address::setAddress(Net::IPAddress address)
 {
  BufPutDev dev(data+4);

  dev(address);
 }

Address::Address(Net::UDPort udport)
 : data()
 {
  setFamily();
  setPort(udport);
 }

Address::Address(Net::UDPoint udpoint)
 : data()
 {
  setFamily();
  setPort(udpoint.port);
  setAddress(udpoint.address);
 }

void Address::get(Net::UDPoint &udpoint)
 {
  BufGetDev dev(data+2);

  dev.use<BeOrder>(udpoint.port,udpoint.address);
 }

/* struct Timeout */

struct Timeout : Win32::WSATimeout
 {
  explicit Timeout(MSec timeout)
   {
    auto val=+timeout;

    sec=val/1000;
    usec=(val%1000)*1000;
   }
 };

} // namespace Private_SysNet

using namespace Private_SysNet;

PlanInitNode * GetPlanInitNode_SysNet() { return &Object; }

/* struct UDPSocket */

auto UDPSocket::Open(Net::UDPort udport) noexcept -> OpenType
 {
  OpenType ret;

  ret.sockid=Win32::WSASocketA(Win32::WSA_IPv4,Win32::WSA_Datagram,Win32::WSA_UDP,0,0,Win32::WSA_AsyncIO);

  if( ret.sockid==Win32::InvalidSocket )
    {
     ret.error=WSAError();

     return ret;
    }

  Address address(udport);

  if( Win32::bind(ret.sockid,address.getPtr(),address.getLen()) )
    {
     ret.error=WSAError();

     Win32::closesocket(ret.sockid); // ignore unprobable error

     ret.sockid=Win32::InvalidSocket;

     return ret;
    }

  ret.error=NoError;

  return ret;
 }

ErrorType UDPSocket::Close(Type sockid) noexcept
 {
  if( Win32::closesocket(sockid) ) return WSAError();

  return NoError;
 }

WaitResult UDPSocket::Wait_in(Type sockid,MSec timeout_) noexcept
 {
  Win32::WSASockSet set;

  set.count=1;
  set.set[0]=sockid;

  Timeout timeout(timeout_);

  switch( Win32::select(0,&set,0,0,&timeout) )
    {
     case 0 : return Wait_timeout;
     case 1 : return WaitResult(0);

     default: return Wait_error;
    }
 }

ErrorType UDPSocket::Outbound(Type sockid,Net::UDPoint dst,PtrLen<const uint8> data) noexcept
 {
  Address address(dst);

  auto result=Win32::sendto(sockid,data.ptr,data.len,0,address.getPtr(),address.getLen());

  if( result!=data.len ) return WSAError();

  return NoError;
 }

auto UDPSocket::Inbound(Type sockid,PtrLen<uint8> buf) noexcept -> InResult
 {
  InResult ret;

  Address address;
  Win32::ulen_t address_len=address.getLen();

  auto result=Win32::recvfrom(sockid,buf.ptr,buf.len,0,address.getPtr(),&address_len);

  if( result>buf.len )
    {
     ret.len=0;
     ret.error=WSAError();
    }
  else if( address_len!=address.getLen() )
    {
     ret.len=0;
     ret.error=Error_Socket;
    }
  else
    {
     address.get(ret.src);

     ret.len=result;
     ret.error=NoError;
    }

  return ret;
 }

/* struct AsyncUDPSocket */

struct AsyncUDPSocket::AsyncState
 {
  Win32::WSAOverlapped olap;
  Win32::WSABuf buf;

  Address address;
  Win32::ulen_t address_len;
 };

auto AsyncUDPSocket::Open(Net::UDPort udport) noexcept -> OpenType
 {
  OpenType ret;

  ret.sockid=Win32::WSASocketA(Win32::WSA_IPv4,Win32::WSA_Datagram,Win32::WSA_UDP,0,0,Win32::WSA_AsyncIO);

  if( ret.sockid==Win32::InvalidSocket )
    {
     ret.error=WSAError();

     return ret;
    }

  Address address(udport);

  if( Win32::bind(ret.sockid,address.getPtr(),address.getLen()) )
    {
     ret.error=WSAError();

     Win32::closesocket(ret.sockid); // ignore unprobable error

     ret.sockid=Win32::InvalidSocket;

     return ret;
    }

  ret.error=NoError;

  return ret;
 }

ErrorType AsyncUDPSocket::Close(Type sockid) noexcept
 {
  if( Win32::closesocket(sockid) ) return WSAError();

  return NoError;
 }

auto AsyncUDPSocket::StartOutbound(Type sockid,Async async,Net::UDPoint dst,PtrLen<const uint8> data) noexcept -> OutResult
 {
  OutResult ret;

  async->olap.internal=0;
  async->olap.offset=0;

  async->buf.ptr=const_cast<uint8 *>(data.ptr);
  async->buf.len=data.len;

  Win32::ulen_t ret_len;

  Address address(dst);

  if( Win32::WSASendTo(sockid,&async->buf,1,&ret_len,0,address.getPtr(),address.getLen(),&async->olap,0) )
    {
     auto error=WSAError();

     if( error==Win32::WSAErrorIOPending )
       {
        ret.pending=true;
        ret.error=NoError;
       }
     else
       {
        ret.pending=false;
        ret.error=error;
       }
    }
  else
    {
     ret.pending=false;
     ret.error=(ret_len!=data.len)?Error_Socket:NoError;
    }

  return ret;
 }

ErrorType AsyncUDPSocket::CompleteOutbound(Type sockid,Async async) noexcept
 {
  Win32::ulen_t ret_len;
  Win32::flags_t flags;

  if( Win32::WSAGetOverlappedResult(sockid,&async->olap,&ret_len,false,&flags) )
    {
     return (ret_len!=async->buf.len)?Error_Socket:NoError;
    }
  else
    {
     return WSAError();
    }
 }

auto AsyncUDPSocket::StartInbound(Type sockid,Async async,PtrLen<uint8> buf) noexcept -> StartInResult
 {
  StartInResult ret;

  async->olap.internal=0;
  async->olap.offset=0;

  async->buf.ptr=buf.ptr;
  async->buf.len=buf.len;

  async->address_len=async->address.getLen();

  Win32::ulen_t ret_len;
  Win32::flags_t flags=0;

  if( Win32::WSARecvFrom(sockid,&async->buf,1,&ret_len,&flags,async->address.getPtr(),&async->address_len,&async->olap,0) )
    {
     auto error=WSAError();

     if( error==Win32::WSAErrorIOPending )
       {
        ret.pending=true;

        ret.result.src={};
        ret.result.len=0;
        ret.result.error=NoError;
       }
     else
       {
        ret.pending=false;

        ret.result.src={};
        ret.result.len=0;
        ret.result.error=error;
       }
    }
  else
    {
     ret.pending=false;

     if( ret_len>buf.len || async->address_len!=async->address.getLen() )
       {
        ret.result.src={};
        ret.result.len=0;
        ret.result.error=Error_Socket;
       }
     else
       {
        async->address.get(ret.result.src);

        ret.result.len=ret_len;
        ret.result.error=NoError;
       }
    }

  return ret;
 }

auto AsyncUDPSocket::CompleteInbound(Type sockid,Async async) noexcept -> InResult
 {
  InResult ret;

  Win32::ulen_t ret_len;
  Win32::flags_t flags;

  if( Win32::WSAGetOverlappedResult(sockid,&async->olap,&ret_len,false,&flags) )
    {
     if( ret_len>async->buf.len || async->address_len!=async->address.getLen() )
       {
        ret.src={};
        ret.len=0;
        ret.error=Error_Socket;
       }
     else
       {
        async->address.get(ret.src);

        ret.len=ret_len;
        ret.error=NoError;
       }
    }
  else
    {
     ret.src={};
     ret.len=0;
     ret.error=WSAError();
    }

  return ret;
 }

/* class AsyncUDPSocketWait::Engine */

class AsyncUDPSocketWait::Engine : public WaitBase<AsyncUDPSocket::AsyncState,WaitNet>
 {
  public:

   explicit Engine(ulen async_count) : WaitBase<AsyncUDPSocket::AsyncState,WaitNet>(async_count) {}

   ~Engine() {}
 };

/* struct AsyncUDPSocketWait */

ErrorType AsyncUDPSocketWait::init(ulen async_count) noexcept
 {
  if( async_count>MaxAsyncs ) return ErrorType(Win32::ErrorInvalidParameter);

  SilentReportException report;

  try
    {
     obj=new Engine(async_count);

     asyncs=obj->getAsyncs();

     return NoError;
    }
  catch(CatchType)
    {
     return ErrorType(Win32::ErrorNotEnoughMemory);
    }
 }

void AsyncUDPSocketWait::exit() noexcept
 {
  delete Replace_null(obj);

  asyncs=0;
 }

bool AsyncUDPSocketWait::addWait(ulen index) noexcept
 {
  return obj->addWait(index);
 }

bool AsyncUDPSocketWait::delWait(ulen index) noexcept
 {
  return obj->delWait(index);
 }

WaitResult AsyncUDPSocketWait::wait(MSec timeout) noexcept
 {
  return obj->wait(timeout);
 }

void AsyncUDPSocketWait::interrupt() noexcept
 {
  obj->interrupt();
 }

WaitResult AsyncUDPSocketWait::waitAll(MSec timeout) noexcept
 {
  return obj->waitAll(timeout);
 }

} // namespace Sys
} // namespace CCore


