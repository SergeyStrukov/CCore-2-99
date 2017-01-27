/* Contour.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: ContourLab 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2017 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/Contour.h>

#include <CCore/inc/Exception.h>

namespace App {

/* functions */

void GuardTypeMismatch()
 {
  Printf(Exception,"App::GuardType(...) : type mismatch");
 }

} // namespace App

