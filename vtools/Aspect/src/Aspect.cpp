/* Aspect.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: Aspect 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2017 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/Aspect.h>

namespace App {

/* class AspectWindow */

AspectWindow::AspectWindow(SubWindowHost &host,const Config &cfg_)
 : SubWindow(host),
   cfg(cfg_)
 {
 }

AspectWindow::~AspectWindow()
 {
 }

 // methods

Point AspectWindow::getMinSize() const
 {
  return Point(100,100);
 }

bool AspectWindow::isModified() const
 {
  return false;
 }

void AspectWindow::blank()
 {
 }

void AspectWindow::load(StrLen file_name)
 {
  Used(file_name);
 }

bool AspectWindow::save()
 {
  return true;
 }

void AspectWindow::save(StrLen file_name)
 {
  Used(file_name);
 }

} // namespace App

