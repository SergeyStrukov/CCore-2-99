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

/* class Contour */

Contour::Contour()
 {
  addPad("A"_c,Point{10,10});

  addPad("B"_c,Point{20,30});

  addFormula("(AB)"_c,FormulaType<decltype(LineOf)>::Create<LineOf>,pads[0].obj,pads[1].obj);
 }

Contour::~Contour()
 {
 }

} // namespace App

