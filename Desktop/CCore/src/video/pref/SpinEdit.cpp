/* SpinEdit.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 3.00
//
//  Tag: Desktop
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2017 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <CCore/inc/video/pref/SpinEdit.h>

#include <CCore/inc/video/Layout.h>

namespace CCore {
namespace Video {

/* class SpinEditWindow */

Point SpinEditWindow::getTextSize(int value) const
 {
  char temp[TextBufLen];
  PrintBuf out(Range(temp));

  Putobj(out,value);

  return text.getMinSize(out.close());
 }

void SpinEditWindow::showVal()
 {
  text.printf("#;",value);

  minus.enable( value > min_value );
  plus.enable( value < max_value );
 }

void SpinEditWindow::plusVal()
 {
  value++;

  changed.assert(value);

  showVal();
 }

void SpinEditWindow::minusVal()
 {
  value--;

  changed.assert(value);

  showVal();
 }

SpinEditWindow::SpinEditWindow(SubWindowHost &host,const ConfigType &cfg_)
 : ComboWindow(host),
   cfg(cfg_),

   plus(wlist,cfg.knob_cfg,KnobShape::FacePlus),
   minus(wlist,cfg.knob_cfg,KnobShape::FaceMinus),
   text(wlist,cfg.text_cfg),

   connector_plus_pressed(this,&SpinEditWindow::plusVal,plus.pressed),
   connector_minus_pressed(this,&SpinEditWindow::minusVal,minus.pressed)
 {
  wlist.insTop(minus,text,plus);

  plus.disable();
  minus.disable();
 }

SpinEditWindow::~SpinEditWindow()
 {
 }

 // methods

Point SpinEditWindow::getMinSize() const
 {
  Coord knob_dxy=minus.getMinSize().dxy;
  Point size=Sup(getTextSize(min_value),getTextSize(max_value),getTextSize(12345));

  Coordinate dx=size.x;

  dx+=BoxExt(knob_dxy);
  dx+=BoxExt(knob_dxy);

  return Point( +dx , Max(knob_dxy,size.y) );
 }

void SpinEditWindow::setValue(int value_)
 {
  value=Cap(min_value,value_,max_value);

  showVal();
 }

void SpinEditWindow::setValue(int value_,int min_value_,int max_value_)
 {
  min_value=min_value_;
  max_value=max_value_;

  setValue(value_);
 }

 // drawing

void SpinEditWindow::layout()
 {
  PaneCut pane(getSize(),0);

  pane.place_cutLeft(minus)
      .place_cutRight(plus)
      .place(text);
 }

 // keyboard

FocusType SpinEditWindow::askFocus() const
 {
  return isEnabled()?FocusTab:NoFocus;
 }

 // user input

void SpinEditWindow::react(UserAction action)
 {
  action.dispatch(*this);

  wlist.react(action);
 }

void SpinEditWindow::react_Char(char ch)
 {
  switch( ch )
    {
     case '+' : plusVal(); break;

     case '-' : minusVal(); break;
    }
 }

} // namespace Video
} // namespace CCore
