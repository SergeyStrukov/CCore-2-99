/* Display.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: DDLDisplay 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/Display.h>

namespace App {

/* class DisplayWindow */

DisplayWindow::DisplayWindow(SubWindowHost &host,const Config &cfg_)
 : SubWindow(host),
   cfg(cfg_),
   msg(getFrame()->getDesktop(),cfg.msg_cfg)
 {
 }

DisplayWindow::~DisplayWindow()
 {
 }

 // methods

void DisplayWindow::open(StrLen file_name)
 {
  msg.setInfo(InfoFromString(file_name));

  if( msg.isDead() ) msg.create(getFrame(),"Open"_def);
 }

} // namespace App



