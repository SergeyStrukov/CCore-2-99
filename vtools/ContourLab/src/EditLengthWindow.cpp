/* EditLengthWindow.cpp */
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

#include <inc/EditLengthWindow.h>

#include <CCore/inc/video/FigureLib.h>

namespace App {

/* class EditLengthWindow */

void EditLengthWindow::newValue(Geometry::Length value_)
 {
  value=value_;

  redraw();

  changed.assert(value);
 }

void EditLengthWindow::pin(Point point)
 {
  newValue( Geometry::Length(point.x-base.x) );
 }

void EditLengthWindow::shift(Coord delta)
 {
  newValue( Geometry::Add(value,Geometry::Length(delta)) );
 }

EditLengthWindow::EditLengthWindow(SubWindowHost &host,const Config &cfg_)
 : SubWindow(host),
   cfg(cfg_),

   value(0)
 {
 }

EditLengthWindow::~EditLengthWindow()
 {
 }

 // methods

Point EditLengthWindow::getMinSize() const
 {
  return {+cfg.dx,+cfg.dy};
 }

 // drawing

bool EditLengthWindow::isGoodSize(Point size) const
 {
  return size>=getMinSize();
 }

void EditLengthWindow::layout()
 {
  Point size=getSize();

  pane=Pane(Null,size);
  base=size/2;

  text=Pane(0,base.y+size.y/12,size.x,size.y/3);
 }

void EditLengthWindow::draw(DrawBuf buf,bool) const // TODO
 {
  if( pane.dx<10 || pane.dy<10 ) return;

  MCoord w=+cfg.width;

  VColor face=+cfg.face;
  VColor gray=+cfg.gray;

  SmoothDrawArt art(buf.cut(pane));

  MPane p(pane);

  // body

  {
   FigureBox fig(p);

   fig.solid(art,+cfg.back);

   VColor vc = focus? +cfg.focus : ( hilight? +cfg.hilight : +cfg.border ) ;

   fig.loop(art,HalfPos,w,vc);
  }

  // length

  {
   MPoint end=base;

   end.x+=Map(value);

   art.ball(base,2*w,face);
   art.ball(end,2*w,face);

   art.path(w,face,base,end);
  }

  // text

  {
   char temp[TextBufLen];
   PrintBuf out(Range(temp));

   Putobj(out,value);

   cfg.font->text(buf,text,TextPlace(AlignX_Center,AlignY_Top),out.close(),+cfg.text);
  }
 }

 // base

void EditLengthWindow::open()
 {
  focus=false;
  hilight=false;
 }

 // keyboard

FocusType EditLengthWindow::askFocus() const
 {
  return FocusOk;
 }

void EditLengthWindow::gainFocus()
 {
  if( Change(focus,true) ) redraw();
 }

void EditLengthWindow::looseFocus()
 {
  if( Change(focus,false) ) redraw();
 }

 // mouse

MouseShape EditLengthWindow::getMouseShape(Point,KeyMod) const
 {
  return Mouse_Arrow;
 }

 // user input

void EditLengthWindow::react(UserAction action)
 {
  action.dispatch(*this);
 }

void EditLengthWindow::react_Key(VKey vkey,KeyMod kmod,unsigned repeat)
 {
  switch( vkey )
    {
     case VKey_Left :
     case VKey_NumPlus :
      {
       if( kmod&KeyMod_Shift )
         shift(-10*Coord(repeat));
       else
         shift(-Coord(repeat));
      }
     break;

     case VKey_Right :
     case VKey_NumMinus :
      {
       if( kmod&KeyMod_Shift )
         shift(10*Coord(repeat));
       else
         shift(Coord(repeat));
      }
     break;
    }
 }

void EditLengthWindow::react_LeftClick(Point point,MouseKey)
 {
  pin(point);
 }

void EditLengthWindow::react_Move(Point point,MouseKey mkey)
 {
  if( pane.contains(point) )
    {
     if( Change(hilight,true) ) redraw();
    }
  else
    {
     if( Change(hilight,false) ) redraw();
    }

  if( mkey&MouseKey_Left )
    {
     pin(point);
    }
 }

void EditLengthWindow::react_Leave()
 {
  if( Change(hilight,false) ) redraw();
 }

void EditLengthWindow::react_Wheel(Point,MouseKey mkey,Coord delta)
 {
  if( mkey&MouseKey_Shift )
    {
     shift(10*delta);
    }
  else
    {
     shift(delta);
    }
 }

} // namespace App

