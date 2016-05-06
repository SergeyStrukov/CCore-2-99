/* PosSub.h */
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

#ifndef CCore_inc_gadget_PosSub_h
#define CCore_inc_gadget_PosSub_h

#include <CCore/inc/gadget/Meta.h>

namespace CCore {

/* classes */

template <class UInt,class=Meta::EnableIf< Meta::IsUInt<UInt> > > struct PosSubResult;

/* struct PosSubResult<UInt> */

template <class UInt,class>
struct PosSubResult
 {
  // result

  UInt value;
  bool ok;

  // constructors

  explicit PosSubResult(UInt value_) : value(value_),ok(true) {}

  // methods

  operator UInt() const { return value; }

  void sub_single(UInt a)
   {
    if( a<=value )
      {
       value-=a;
      }
    else
      {
       value=0;
       ok=false;
      }
   }

  void sub() {}

  template <class ... TT>
  void sub(UInt a,TT ... tt)
   {
    sub_single(a);

    sub(tt...);
   }
 };

/* PosSub() */

template <class UInt,class ... TT>
Meta::EnableIf< Meta::IsUInt<UInt> , PosSubResult<UInt> > PosSub(UInt value,TT ... tt)
 {
  PosSubResult<UInt> ret(value);

  ret.sub(tt...);

  return ret;
 }

} // namespace CCore

#endif


