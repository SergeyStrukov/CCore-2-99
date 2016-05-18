/* ApplyToRange.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 3.00
//
//  Tag: Fundamental Mini
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2015 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_algon_ApplyToRange_h
#define CCore_inc_algon_ApplyToRange_h

#include <CCore/inc/FunctorType.h>

namespace CCore {
namespace Algon {

/* concept Has_getResult<T> */

template <class T>
concept bool Has_getResult = requires(T &obj) { obj.getResult(); } ;

/* concept No_getResult<T> */

template <class T>
concept bool No_getResult = !Has_getResult<T> ;

/* GetResult() */

auto GetResult(Has_getResult &obj) { return obj.getResult(); }

void GetResult(No_getResult &) {}

/* ApplyToRange() */

template <CursorType R,FuncInitArgType<Meta::PtrObjType<R> &> FuncInit>
auto ApplyToRange(R r,FuncInit func_init)
 {
  FunctorTypeOf<FuncInit> func(func_init);

  for(; +r ;++r) func(*r);

  return GetResult(func);
 }

template <CursorType R,FuncInitArgType<Meta::PtrObjType<R> &> FuncInit>
auto ApplyToRange(R r,FuncInit func_init) requires( FuncInitType<FuncInit,bool,Meta::PtrObjType<R> &> )
 {
  FunctorTypeOf<FuncInit> func(func_init);

  for(; +r ;++r) if( !func(*r) ) break;

  return GetResult(func);
 }

} // namespace Algon
} // namespace CCore

#endif


