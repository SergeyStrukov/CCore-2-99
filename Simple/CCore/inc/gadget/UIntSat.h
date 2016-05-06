/* UIntSat.h */
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

#ifndef CCore_inc_gadget_UIntSat_h
#define CCore_inc_gadget_UIntSat_h

#include <CCore/inc/gadget/Len.h>
#include <CCore/inc/gadget/Meta.h>
#include <CCore/inc/gadget/UIntFunc.h>

namespace CCore {

/* classes */

template <class UInt,class=Meta::EnableIf< Meta::IsUInt<UInt> > > struct UIntSat;

template <class UInt,UInt A,UInt B> struct UIntConstAdd_ok;

template <class UInt,UInt A,UInt B> struct UIntConstSub_ok;

/* struct UIntSat<UInt> */

template <class UInt,class>
struct UIntSat
 {
  static const UInt MaxVal = MaxUInt<UInt> ;

  UInt value;
  bool overflow;

  // constructors

  constexpr UIntSat() : value(0),overflow(false) {}

  constexpr UIntSat(UIntSat<UInt> a,UIntSat<UInt> b)
   : value( (a.value<=MaxVal-b.value)? a.value+b.value : MaxVal ),
     overflow( a.overflow || b.overflow || (a.value>MaxVal-b.value) )
   {
   }

  template <class T,class=Meta::EnableIf< Meta::IsUInt<T> > >
  constexpr UIntSat(T value_)
   : value( (value_<=MaxVal)? value_ : MaxVal ),
     overflow( value_>MaxVal )
   {
   }

  // methods

  bool operator ! () const { return overflow; }

  // operators

  friend constexpr UIntSat<UInt> operator + (UIntSat<UInt> a,UIntSat<UInt> b) { return {a,b}; }

  friend UIntSat<UInt> operator += (UIntSat<UInt> &a,UIntSat<UInt> b) { a=a+b; return a; }

  bool operator < (UInt lim) const { return value<lim && !overflow ; }

  bool operator <= (UInt lim) const { return value<=lim && !overflow ; }

  bool operator > (UInt lim) const { return value>lim || overflow ; }

  bool operator >= (UInt lim) const { return value>=lim || overflow ; }
 };

/* struct UIntConstAdd_ok<UInt,UInt A,UInt B> */

template <class UInt,UInt A,UInt B>
struct UIntConstAdd_ok
 {
  enum RetType : UInt { Ret = A+B };
 };

/* const UIntConstAdd<UInt,UInt A,UInt B> */

template <class UInt,UInt A,UInt B>
const UInt UIntConstAdd = Meta::Select<( A<=MaxUInt<UInt>-B ), UIntConstAdd_ok<UInt,A,B> , Meta::Empty >::Ret ;

/* struct UIntConstSub_ok<UInt,UInt A,UInt B> */

template <class UInt,UInt A,UInt B>
struct UIntConstSub_ok
 {
  enum RetType : UInt { Ret = A-B };
 };

/* const UIntConstSub<UInt,UInt A,UInt B> */

template <class UInt,UInt A,UInt B>
const UInt UIntConstSub = Meta::Select<( A>=B ), UIntConstSub_ok<UInt,A,B> , Meta::Empty >::Ret ;

/* type ULenSat */

using ULenSat = UIntSat<ulen> ;

} // namespace CCore

#endif


