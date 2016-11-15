/* ApplyToList.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 3.00
//
//  Tag: Fundamental
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_algon_ApplyToList_h
#define CCore_inc_algon_ApplyToList_h

#include <CCore/inc/algon/ApplyToRange.h>

namespace CCore {
namespace Algon {

/* classes */

template <class Func> struct FuncAccumulator;

/* struct FuncAccumulator<Func> */

template <class Func>
struct FuncAccumulator
 {
  Func &func;

  explicit FuncAccumulator(Func &func_) : func(func_) {}

  template <class T>
  FuncAccumulator<Func> operator + (T &&t) { func( std::forward<T>(t) ); return *this; }
 };

template <class Func>
auto FuncToAccumulator(Func &func)
 {
  return FuncAccumulator<Func>(func);
 }

/* ApplyToList() */

template <class FuncInit,class ... TT>
auto ApplyToList(FuncInit func_init,TT && ... tt)
 {
  FunctorTypeOf<FuncInit> func(func_init);

  ( FuncToAccumulator(func) + ... + std::forward<TT>(tt) );

  return GetResult(func);
 }

} // namespace Algon
} // namespace CCore

#endif

