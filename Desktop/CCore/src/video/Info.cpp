/* Info.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: Desktop
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <CCore/inc/video/Info.h>

#include <CCore/inc/CharProp.h>

namespace CCore {
namespace Video {

namespace Private_Info {

class DefInfo : public InfoBase
 {
  public:

   DefInfo() {}

   virtual ~DefInfo() {}

   // AbstractInfo

   virtual ulen getLineCount() const { return 0; }

   virtual StrLen getLine(ulen) const { return Empty; }
 };

DefInfo Object CCORE_INITPRI_3 ;

} // namespace Private_Info

using namespace Private_Info;

/* class Info */

Info::Info() noexcept
 : ptr(&Object)
 {
  ptr->incRef();
 }

/* class InfoFromString */

InfoFromString::StringSet::StringSet(StrLen str)
 : buf(DoReserve,100)
 {
  while( +str )
    {
     StrLen line=CutLine(str);

     buf.append_fill(line);
    }

  buf.shrink_extra();
 }

InfoFromString::StringSet::~StringSet()
 {
 }

ulen InfoFromString::StringSet::getLineCount() const
 {
  return buf.getLen();
 }

StrLen InfoFromString::StringSet::getLine(ulen index) const
 {
  return Range(buf.at(index));
 }

InfoFromString::InfoFromString(StrLen str)
 : Info(new StringSet(str))
 {
 }

InfoFromString::~InfoFromString()
 {
 }

/* class InfoBuilder::PoolInfo */

class InfoBuilder::PoolInfo : public Info
 {
   class Base : public InfoBase
    {
      ElementPool pool;
      DynArray<StrLen> list;

     public:

      Base(ElementPool &pool_,DynArray<StrLen> &list_)
       {
        Swap(pool,pool_);
        Swap(list,list_);

        pool.shrink_extra();
        list.shrink_extra();
       }

      virtual ~Base()
       {
       }

      // AbstractInfo

      virtual ulen getLineCount() const
       {
        return list.getLen();
       }

      virtual StrLen getLine(ulen index) const
       {
        return list.at(index);
       }
    };

  public:

   PoolInfo(ElementPool &pool,DynArray<StrLen> &list)
    : Info(new Base(pool,list))
    {
    }

   ~PoolInfo()
    {
    }
 };

/* class InfoBuilder */

InfoBuilder::InfoBuilder()
 {
 }

InfoBuilder::~InfoBuilder()
 {
 }

void InfoBuilder::add(StrLen line)
 {
  list.append_copy(pool.dup(line));
 }

Info InfoBuilder::complete()
 {
  return PoolInfo(pool,list);
 }

} // namespace Video
} // namespace CCore


