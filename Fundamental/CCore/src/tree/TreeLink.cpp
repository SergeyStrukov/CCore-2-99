/* TreeLink.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
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

#include <CCore/inc/tree/TreeLink.h>

#include <CCore/inc/Exception.h>

namespace CCore {

/* functions */

void GuardRadixTreeCheckFailed(const char *text)
 {
  Printf(Exception,"CCore::GuardRadixTreeCheck(...) : ( #; ) is false",text);
 }

} // namespace CCore


