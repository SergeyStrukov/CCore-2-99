/* UIntSplit.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: Simple Mini
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2015 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_UIntSplit_h
#define CCore_inc_UIntSplit_h

#include <CCore/inc/Gadget.h>

namespace CCore {

/* types */

template <class UIntBig,class UIntSmall>
using UIntSplitEnable = Meta::EnableIf< Meta::IsUInt<UIntBig> && Meta::IsUInt<UIntSmall> &&
                                        ( Meta::UIntBits<UIntBig> % Meta::UIntBits<UIntSmall> == 0 ) &&
                                        ( Meta::UIntBits<UIntBig> > Meta::UIntBits<UIntSmall> ) > ;

/* classes */

template <class UIntBig,class UIntSmall,class=UIntSplitEnable<UIntBig,UIntSmall> > class UIntSplit;

/* class UIntSplit<UIntBig,UIntSmall> */

template <class UIntBig,class UIntSmall,class>
class UIntSplit
 {
   static const unsigned BitLen    = Meta::UIntBits<UIntSmall> ;
   static const unsigned BigBitLen = Meta::UIntBits<UIntBig> ;

  public:

   static const unsigned Len       = BigBitLen/BitLen ;

  private:

   UIntSmall buf[Len]; // big-endian order

  public:

   // access

   UIntSmall operator [] (unsigned index) const { return buf[index]; }

   UIntSmall & operator [] (unsigned index) { return buf[index]; }

   PtrLen<UIntSmall> take() { return Range(buf); }

   // get

   template <unsigned Ind,unsigned Off> struct Get_loop;

   template <unsigned Ind,unsigned Off> struct Get_last;

   template <unsigned Ind,unsigned Off>
   using Get = Meta::Select<( Ind>0 ), Get_loop<Ind,Off> , Get_last<Ind,Off> > ;

   UIntBig get() const
    {
     UIntBig ret=buf[Len-1];

     Get<Len-2,BitLen>::Do(ret,buf);

     return ret;
    }

   // set

   template <unsigned Ind,unsigned Off> struct Set_loop;

   template <unsigned Ind,unsigned Off> struct Set_last;

   template <unsigned Ind,unsigned Off>
   using Set = Meta::Select<( Ind>0 ), Set_loop<Ind,Off> , Set_last<Ind,Off> > ;

   void set(UIntBig value)
    {
     buf[Len-1]=UIntSmall( value );

     Set<Len-2,BitLen>::Do(value,buf);
    }
 };

template <class UIntBig,class UIntSmall,class E>
template <unsigned Ind,unsigned Off>
struct UIntSplit<UIntBig,UIntSmall,E>::Get_loop
 {
  static void Do(UIntBig &ret,const UIntSmall buf[])
   {
    ret|=( UIntBig(buf[Ind])<<Off );

    Get<Ind-1,Off+BitLen>::Do(ret,buf);
   }
 };

template <class UIntBig,class UIntSmall,class E>
template <unsigned Ind,unsigned Off>
struct UIntSplit<UIntBig,UIntSmall,E>::Get_last
 {
  static void Do(UIntBig &ret,const UIntSmall buf[])
   {
    ret|=( UIntBig(buf[Ind])<<Off );
   }
 };

template <class UIntBig,class UIntSmall,class E>
template <unsigned Ind,unsigned Off>
struct UIntSplit<UIntBig,UIntSmall,E>::Set_loop
 {
  static void Do(UIntBig value,UIntSmall buf[])
   {
    buf[Ind]=UIntSmall( value>>Off );

    Set<Ind-1,Off+BitLen>::Do(value,buf);
   }
 };

template <class UIntBig,class UIntSmall,class E>
template <unsigned Ind,unsigned Off>
struct UIntSplit<UIntBig,UIntSmall,E>::Set_last
 {
  static void Do(UIntBig value,UIntSmall buf[])
   {
    buf[Ind]=UIntSmall( value>>Off );
   }
 };

} // namespace CCore

#endif


