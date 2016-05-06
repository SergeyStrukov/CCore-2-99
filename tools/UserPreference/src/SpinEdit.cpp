/* SpinEdit.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: UserPreference 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/SpinEdit.h>

#include <CCore/inc/video/Layout.h>

namespace App {

/* class SpinEditWindow */

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

Point SpinEditWindow::getTextSize(int value) const
 {
  char temp[TextBufLen];
  PrintBuf out(Range(temp));

  Putobj(out,value);

  return text.getMinSize(out.close());
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
  Point s=Sup(getTextSize(min_value),getTextSize(max_value),getTextSize(12345));

  return Point(2*s.y+s.x,s.y);
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
  Point size=getSize();

  Panesor psor(size,0);

  Coord len1=size.y;
  Coord len2=size.x-2*len1;

  psor.placeX(minus,len1);
  psor.placeX(text,len2);
  psor.placeX(plus,len1);
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

} // namespace App
