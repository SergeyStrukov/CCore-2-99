/* UIntSat.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 3.00
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

template <UIntType UInt> struct UIntSat;

/* struct UIntSat<UInt> */

template <UIntType UInt>
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

  constexpr UIntSat(UIntType value_)
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

/* const UIntConstAdd<UInt,UInt A,UInt B> */

template <UIntType UInt,UInt A,UInt B> requires ( A <= MaxUInt<UInt> - B )
const UInt UIntConstAdd = A + B ;

/* const UIntConstSub<UInt,UInt A,UInt B> */

template <UIntType UInt,UInt A,UInt B> requires ( A >= B )
const UInt UIntConstSub = A - B ;

/* type ULenSat */

using ULenSat = UIntSat<ulen> ;

} // namespace CCore

#endif


