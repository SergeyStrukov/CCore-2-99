/* UtilFunc.h */
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

#ifndef CCore_inc_gadget_UtilFunc_h
#define CCore_inc_gadget_UtilFunc_h

#include <CCore/inc/gadget/Meta.h>

namespace CCore {

/* functions */

template <class T>
const T & ToConst(T &obj) { return obj; }

template <class T>
constexpr T Min(T a,T b) { return (a<b)?a:b; }

template <class T>
constexpr T Max(T a,T b) { return (a<b)?b:a; }

template <class T>
constexpr T Cap(T a,T x,T b) { return (x<a)?a:( (b<x)?b:x ); }

template <class T>
constexpr bool Fit(T a,T x,T b) { return a<=x && x<=b; }

template <class T>
constexpr T Sq(T x) { return x*x; }

template <class T>
T Diff(T &obj,T a)
 {
  T ret(a-obj);

  obj=a;

  return ret;
 }

template <class T>
bool Change(T &obj,T val)
 {
  if( obj!=val )
    {
     obj=val;

     return true;
    }

  return false;
 }

/* functions */

template <class UInt,class S>
Meta::EnableIf< Meta::IsUInt<UInt> > BitSet(UInt &obj,S bits)
 {
  obj |= UInt(bits) ;
 }

template <class UInt,class S>
Meta::EnableIf< Meta::IsUInt<UInt> > BitClear(UInt &obj,S bits)
 {
  obj &= ~UInt(bits) ;
 }

template <class UInt,class S>
Meta::EnableIf< Meta::IsUInt<UInt> , UInt > BitTest(UInt val,S bits)
 {
  return UInt( val&UInt(bits) );
 }

template <class UInt,class S>
Meta::EnableIf< Meta::IsUInt<UInt>  , bool > BitTestAll(UInt val,S bits_)
 {
  UInt bits=UInt(bits_);

  return UInt( val&bits )==bits;
 }

/* MaxOf() */

namespace Meta {

template <class RetType,class T>
constexpr RetType MaxOf(T t)
 {
  return t;
 }

template <class RetType,class T,class ... TT>
constexpr RetType MaxOf(T t,TT ... tt)
 {
  return Max<RetType>(t,MaxOf<RetType>(tt...));
 }

} // namespace Meta

} // namespace CCore

#endif


