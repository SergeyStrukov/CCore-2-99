/* EditRatioWindow.cpp */
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

#include <inc/EditRatioWindow.h>

#include <CCore/inc/video/FigureLib.h>

namespace App {

/* class EditRatioWindow */

void EditRatioWindow::newValue(Geometry::Ratio value_)
 {
  value=value_;

  redraw();

  changed.assert(value);
 }

void EditRatioWindow::pin(Point point) // TODO
 {
  Used(point);
 }

void EditRatioWindow::shift(Coord delta) // TODO
 {
  Used(delta);
 }

EditRatioWindow::EditRatioWindow(SubWindowHost &host,const Config &cfg_)
 : SubWindow(host),
   cfg(cfg_),

   value(0)
 {
 }

EditRatioWindow::~EditRatioWindow()
 {
 }

 // methods

Point EditRatioWindow::getMinSize() const
 {
  return {+cfg.dx,+cfg.dy};
 }

 // drawing

bool EditRatioWindow::isGoodSize(Point size) const
 {
  return size>=getMinSize();
 }

void EditRatioWindow::layout() // TODO
 {
  Coord s=+cfg.shade_dxy;

  Point size=getSize().subXY(s);

  pane=Pane(Null,size.x,size.y);
  shade1=Pane(s,size.y,size.x,s);
  shade2=Pane(size.x,s,s,size.y-s);

  base=size/2;

  text=Pane(0,base.y+size.y/12,size.x,size.y/3);
 }

void EditRatioWindow::draw(DrawBuf buf,bool) const // TODO
 {
  if( pane.dx<10 || pane.dy<10 ) return;

  MCoord w=+cfg.width;

  //VColor face=+cfg.face;
  VColor gray=+cfg.gray;

  SmoothDrawArt art(buf.cut(pane));

  MPane p(pane);
  FigureBox fig(p);

  // body

  {
   fig.solid(art,+cfg.back);

   buf.erase(shade1,gray);
   buf.erase(shade2,gray);
  }

  // text

  {
   char temp[TextBufLen];
   PrintBuf out(Range(temp));

   Putobj(out,value);

   cfg.font->text(buf,text,TextPlace(AlignX_Center,AlignY_Top),out.close(),+cfg.text);
  }

  // border

  {
   VColor vc = focus? +cfg.focus : ( hilight? +cfg.hilight : +cfg.border ) ;

   fig.loop(art,HalfPos,w,vc);
  }
 }

 // base

void EditRatioWindow::open()
 {
  focus=false;
  hilight=false;
 }

 // keyboard

FocusType EditRatioWindow::askFocus() const
 {
  return FocusOk;
 }

void EditRatioWindow::gainFocus()
 {
  if( Change(focus,true) ) redraw();
 }

void EditRatioWindow::looseFocus()
 {
  if( Change(focus,false) ) redraw();
 }

 // mouse

MouseShape EditRatioWindow::getMouseShape(Point,KeyMod) const
 {
  return Mouse_Arrow;
 }

 // user input

void EditRatioWindow::react(UserAction action)
 {
  action.dispatch(*this);
 }

void EditRatioWindow::react_Key(VKey vkey,KeyMod kmod,unsigned repeat) // TODO
 {
  Used(vkey);
  Used(kmod);
  Used(repeat);
 }

void EditRatioWindow::react_LeftClick(Point point,MouseKey)
 {
  if( pane.contains(point) ) pin(point);
 }

void EditRatioWindow::react_Move(Point point,MouseKey mkey)
 {
  if( pane.contains(point) )
    {
     if( Change(hilight,true) ) redraw();

     if( mkey&MouseKey_Left ) pin(point);
    }
  else
    {
     if( Change(hilight,false) ) redraw();
    }
 }

void EditRatioWindow::react_Leave()
 {
  if( Change(hilight,false) ) redraw();
 }

void EditRatioWindow::react_Wheel(Point,MouseKey mkey,Coord delta) // TODO
 {
  Used(mkey);
  Used(delta);
 }

} // namespace App

