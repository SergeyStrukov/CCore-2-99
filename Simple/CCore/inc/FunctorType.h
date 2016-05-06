/* FunctorType.h */
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

#ifndef CCore_inc_FunctorType_h
#define CCore_inc_FunctorType_h

#include <CCore/inc/Gadget.h>

namespace CCore {

/* classes */

struct ProbeSet_FunctorType;

template <bool has_FunctorType,class FuncInit> struct FunctorTypes;

template <class T> struct FunctorRefType;

/* struct ProbeSet_FunctorType */

struct ProbeSet_FunctorType
 {
  template <class T,class C=typename T::FunctorType> struct Condition;
 };

/* const Has_FunctorType<T> */

template <class T>
const bool Has_FunctorType = Meta::Detect<ProbeSet_FunctorType,T> ;

/* struct FunctorTypes<bool has_FunctorType,FuncInit> */

template <class FuncInit>
struct FunctorTypes<true,FuncInit>
 {
  using Ret = typename FuncInit::FunctorType ;
 };

template <class FuncInit>
struct FunctorTypes<false,FuncInit>
 {
  using Ret = FuncInit ;
 };

/* type FunctorTypeOf<FuncInit> */

template <class FuncInit>
using FunctorTypeOf = typename FunctorTypes<Has_FunctorType<FuncInit>,FuncInit>::Ret ;

/* struct FunctorRefType<T> */

template <class T>
struct FunctorRefType
 {
  T *obj;

  explicit FunctorRefType(T &obj_) : obj(&obj_) {}

  using FunctorType = T & ;

  operator FunctorType() const { return *obj; }
 };

/* FunctorRef() */

template <class T>
FunctorRefType<T> FunctorRef(T &obj) { return FunctorRefType<T>(obj); }

} // namespace CCore

#endif


