/* MCoordEdit.cpp */
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

#include <inc/MCoordEdit.h>

#include <CCore/inc/video/Layout.h>

namespace App {

/* class MCoordEditWindow */

void MCoordEditWindow::showVal()
 {
  MCoord a=IntRShift(value,MPoint::Precision);
  MCoord b=IntMask(value,MPoint::One-1u);
  MCoord d=MPoint::One;

  unsigned n=MPoint::Precision-(+cfg.precision);

  text.printf("#; #;/#;",a,b>>n,d>>n);
 }

void MCoordEditWindow::plusVal()
 {
  value+=Fraction(1);

  changed.assert(value);

  showVal();
 }

void MCoordEditWindow::minusVal()
 {
  value-=Fraction(1);

  changed.assert(value);

  showVal();
 }

void MCoordEditWindow::plusSmallVal()
 {
  value+=Fraction(1,+cfg.precision);

  changed.assert(value);

  showVal();
 }

void MCoordEditWindow::minusSmallVal()
 {
  value-=Fraction(1,+cfg.precision);

  changed.assert(value);

  showVal();
 }

MCoordEditWindow::MCoordEditWindow(SubWindowHost &host,const ConfigType &cfg_)
 : ComboWindow(host),
   cfg(cfg_),

   plus(wlist,cfg.knob_cfg,KnobShape::FacePlus),
   minus(wlist,cfg.knob_cfg,KnobShape::FaceMinus),
   small_plus(wlist,cfg.knob_cfg,KnobShape::FacePlus),
   small_minus(wlist,cfg.knob_cfg,KnobShape::FaceMinus),
   text(wlist,cfg.text_cfg),

   connector_plus_pressed(this,&MCoordEditWindow::plusVal,plus.pressed),
   connector_minus_pressed(this,&MCoordEditWindow::minusVal,minus.pressed),
   connector_small_plus_pressed(this,&MCoordEditWindow::plusSmallVal,small_plus.pressed),
   connector_small_minus_pressed(this,&MCoordEditWindow::minusSmallVal,small_minus.pressed)
 {
  wlist.insTop(minus,text,plus,small_plus,small_minus);
 }

MCoordEditWindow::~MCoordEditWindow()
 {
 }

 // methods

Point MCoordEditWindow::getMinSize() const
 {
  Point s=text.getMinSize("123435 3/16");

  return Point(s.x+2*s.y,Rational(5,3)*s.y);
 }

void MCoordEditWindow::setMCoord(MCoord value_)
 {
  value=value_;

  showVal();
 }

 // drawing

#if 0

void MCoordEditWindow::layout()
 {
  Point size=getSize();

  Panesor psor(size,0);

  Coord len1=Rational(3,5)*size.y;
  Coord len2=size.x-2*len1;
  Coord len3=Rational(2,3)*len1;

  Panesor psor1=psor.cutY(len1);

  psor1.placeX(minus,len1);
  psor1.placeX(text,len2);
  psor1.placeX(plus,len1);

  psor.cutX(len1);

  psor.placeX(small_minus,len3);

  psor.cutX(len2-2*len3);

  psor.placeX(small_plus,len3);
 }

#endif

 // user input

void MCoordEditWindow::react(UserAction action)
 {
  action.dispatch(*this);

  wlist.react(action);
 }

void MCoordEditWindow::react_Char(char ch)
 {
  switch( ch )
     {
      case '+' : plusSmallVal(); break;

      case '-' : minusSmallVal(); break;
     }
 }

} // namespace App

