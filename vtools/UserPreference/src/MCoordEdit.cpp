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
  Coord knob_dxy=minus.getMinSize().dxy;

  Point size=text.getMinSize("123435 3/16");

  Coordinate dx=size.x;

  dx+=BoxExt(knob_dxy);
  dx+=BoxExt(knob_dxy);

  Coordinate dy=Max(size.y,knob_dxy);

  dy=(Rational(5,3)+Rational(1,10))*dy;

  return Point( +dx , +dy );
 }

void MCoordEditWindow::setMCoord(MCoord value_)
 {
  value=value_;

  showVal();
 }

 // drawing

void MCoordEditWindow::layout()
 {
  PaneCut pane(getSize(),0);

  PaneCut p=pane.cutTop(Rational(3,5));

  auto knob_minus=CutBox(minus);

  p.place_cutLeft(knob_minus)
   .place_cutRight(plus)
   .place(text);

  Coord offx=BoxExt(knob_minus.dxy);

  pane.cutLeft(offx);

  pane.place_cutLeftTop(CutPoint(small_minus,Rational(2,3)));

  pane.cutRight(offx);

  pane.place_cutRightTop(CutPoint(small_plus,Rational(2,3)));
 }

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

