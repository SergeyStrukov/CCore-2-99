/* GeometryWindow.cpp */
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

#include <inc/GeometryWindow.h>

#include <CCore/inc/video/FigureLib.h>

namespace App {

/* class GeometryWindow */

void GeometryWindow::pin(Point point) // TODO
 {
 }

void GeometryWindow::shift_x(Coord delta) // TODO
 {
 }

void GeometryWindow::shift_y(Coord delta) // TODO
 {
 }

GeometryWindow::GeometryWindow(SubWindowHost &host,const Config &cfg_)
 : SubWindow(host),
   cfg(cfg_)
 {
 }

GeometryWindow::~GeometryWindow()
 {
 }

 // methods

Point GeometryWindow::getMinSize() const
 {
  return Point::Diag(+cfg.dxy);
 }

 // drawing

bool GeometryWindow::isGoodSize(Point size) const
 {
  size>=getMinSize();
 }

void GeometryWindow::layout()
 {
  Point size=getSize();
  Coord s=+cfg.shade_dxy;

  pane=Pane(Null,size.x-s,size.y-s);
  shade1=Pane(s,size.y-s,size.x-s,s);
  shade2=Pane(size.x-s,s,s,size.y-2*s);
 }

void GeometryWindow::draw(DrawBuf buf,bool) const // TODO
 {
  if( pane.dx<10 || pane.dy<10 ) return;

  MCoord w=+cfg.width;

  VColor face=+cfg.face;
  VColor gray=+cfg.gray;

  SmoothDrawArt art(buf.cut(pane));

  MPane p(pane);
  FigureBox fig(p);

  // body

  {
   fig.solid(art,+cfg.back);
  }

  // border

  {
   VColor vc = focus? +cfg.focus : ( hilight? +cfg.hilight : +cfg.border ) ;

   fig.loop(art,HalfPos,w,vc);
  }

  // shade

  {
   buf.erase(shade1,gray);
   buf.erase(shade2,gray);
  }
 }

 // base

void GeometryWindow::open()
 {
  focus=false;
  hilight=false;
 }

 // keyboard

FocusType GeometryWindow::askFocus() const
 {
  return FocusOk;
 }

void GeometryWindow::gainFocus()
 {
  if( Change(focus,true) ) redraw();
 }

void GeometryWindow::looseFocus()
 {
  if( Change(focus,false) ) redraw();
 }

 // mouse

MouseShape GeometryWindow::getMouseShape(Point,KeyMod) const
 {
  return Mouse_Arrow;
 }

 // user input

void GeometryWindow::react(UserAction action)
 {
  action.dispatch(*this);
 }

void GeometryWindow::react_Key(VKey vkey,KeyMod kmod,unsigned repeat)
 {
  switch( vkey )
    {
     case VKey_Left :
      {
       if( kmod&KeyMod_Shift )
         shift_x(-10*Coord(repeat));
       else
         shift_x(-Coord(repeat));
      }
     break;

     case VKey_Right :
      {
       if( kmod&KeyMod_Shift )
         shift_x(10*Coord(repeat));
       else
         shift_x(Coord(repeat));
      }
     break;

     case VKey_Up :
      {
       if( kmod&KeyMod_Shift )
         shift_y(-10*Coord(repeat));
       else
         shift_y(-Coord(repeat));
      }
     break;

     case VKey_Down :
      {
       if( kmod&KeyMod_Shift )
         shift_y(10*Coord(repeat));
       else
         shift_y(Coord(repeat));
      }
     break;
    }
 }

void GeometryWindow::react_LeftClick(Point point,MouseKey)
 {
  if( pane.contains(point) ) pin(point);
 }

void GeometryWindow::react_Move(Point point,MouseKey mkey)
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

void GeometryWindow::react_Leave()
 {
  if( Change(hilight,false) ) redraw();
 }

void GeometryWindow::react_Wheel(Point,MouseKey mkey,Coord delta)
 {
  if( mkey&MouseKey_Shift )
    {
     shift_y(delta);
    }
  else
    {
     shift_x(delta);
    }
 }

} // namespace App
