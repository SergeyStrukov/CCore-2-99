/* Test.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: Test 1.00
//
//  Tag: VideoLabs
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/Test.h>

#include <CCore/inc/video/SmoothDrawArt.h>

#include <CCore/inc/Exception.h>

namespace App {

/* class Test */

void Test::setShape(MouseShape mshape_)
 {
  mshape=mshape_;

  text=Stringf("#;",mshape);

  redraw();
 }

Test::Test(SubWindowHost &host,const Config &cfg_)
 : SubWindow(host),
   cfg(cfg_)
 {
 }

Test::~Test()
 {
 }

 // drawing

void Test::layout()
 {
 }

void Test::draw(DrawBuf buf,bool drag_active) const
 {
  Used(drag_active);

  buf.erase(cfg.back);

  Pane pane(Null,getSize());

  cfg.font->text(buf,pane,TextPlace(AlignX_Left,AlignY_Top),Range(text),cfg.text);
 }

 // mouse

MouseShape Test::getMouseShape(Point,KeyMod) const
 {
  return mshape;
 }

 // user input

void Test::react(UserAction action)
 {
  action.dispatch(*this);
 }

void Test::react_Key(VKey vkey,KeyMod kmod)
 {
  Used(vkey);
  Used(kmod);
 }

void Test::react_Char(char ch)
 {
  switch( ch )
    {
     case '1' : return setShape(Mouse_Arrow);
     case '2' : return setShape(Mouse_IBeem);
     case '3' : return setShape(Mouse_Wait);
     case '4' : return setShape(Mouse_Cross);
     case '5' : return setShape(Mouse_Hand);
     case '6' : return setShape(Mouse_Stop);
     case '7' : return setShape(Mouse_Help);
     case '8' : return setShape(Mouse_SizeLeftRight);
     case '9' : return setShape(Mouse_SizeUpDown);
     case '0' : return setShape(Mouse_SizeUpLeft);
     case 'q' : return setShape(Mouse_SizeUpRight);
     case 'w' : return setShape(Mouse_SizeAll);
     case 'e' : return setShape(Mouse_UpArrow);
     case 'r' : return setShape(Mouse_Progress);
    }
 }

void Test::react_AltChar(char ch)
 {
  Used(ch);
 }

} // namespace App

