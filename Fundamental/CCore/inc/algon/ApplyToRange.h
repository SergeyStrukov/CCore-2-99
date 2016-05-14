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

/* ApplyToRange() */

template <RangeType R,FuncInitArgType<Meta::RangeObjType<R> &> FuncInit>
void ApplyToRange(R r,FuncInit func_init)
 {
  FunctorTypeOf<FuncInit> func(func_init);

  for(; +r ;++r) func(*r);
 }

} // namespace Algon
} // namespace CCore

#endif


