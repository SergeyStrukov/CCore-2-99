/* EditAngleWindow.cpp */
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

#include <inc/EditAngleWindow.h>

#include <CCore/inc/video/FigureLib.h>

namespace App {

/* class EditAngleWindow */

void EditAngleWindow::newValue(Geometry::Angle value_)
 {
  value=value_;

  redraw();

  changed.assert(value);
 }

void EditAngleWindow::pin(Point point)
 {
  if( point!=base )
    {
     newValue( Geometry::Point::Arg(Map(point-base)) );
    }
 }

void EditAngleWindow::rotate(Coord delta)
 {
  newValue( value+Geometry::Angle(GradToRadian(delta)) );
 }

EditAngleWindow::EditAngleWindow(SubWindowHost &host,const Config &cfg_)
 : SubWindow(host),
   cfg(cfg_),

   value(0)
 {
 }

EditAngleWindow::~EditAngleWindow()
 {
 }

 // methods

SizeBox EditAngleWindow::getMinSize() const
 {
  return +cfg.dxy;
 }

 // drawing

bool EditAngleWindow::isGoodSize(Point size) const
 {
  return size>=getMinSize();
 }

void EditAngleWindow::layout()
 {
  Point size=getSize();

  len=Min(size.x,size.y);

  pane=Pane(Null,len);

  base=Point::Diag(len/2);
 }

void EditAngleWindow::draw(DrawBuf buf,bool) const
 {
  if( len<10 ) return;

  MCoord w=+cfg.width;

  VColor face=+cfg.face;
  VColor gray=+cfg.gray;

  SmoothDrawArt art(buf);

  MPane p(pane);

  // body

  {
   FigureBox fig(p);

   fig.solid(art,+cfg.back);

   VColor vc = focus? +cfg.focus : ( hilight? +cfg.hilight : +cfg.border ) ;

   fig.loop(art,HalfPos,w,vc);
  }

  // angle

  {
   Geometry::Real radius(len*0.4);
   Geometry::Real radius2(len*0.45);

   MPoint line_x=Map(Geometry::Point(radius2,0));
   MPoint line_y=Map(Geometry::Point(0,radius2));

   MPoint line=Map(Geometry::Point::Polar(radius,value));

   art.ball(base,2*w,face);

   art.path(w/2,gray,base-line_x,base+line_x);
   art.path(w/2,gray,base-line_y,base+line_y);

   art.path(w,face,base,base+line);
  }

  // text

  {
  }
 }

 // base

void EditAngleWindow::open()
 {
  focus=false;
  hilight=false;
 }

 // keyboard

FocusType EditAngleWindow::askFocus() const
 {
  return FocusOk;
 }

void EditAngleWindow::gainFocus()
 {
  if( Change(focus,true) )
    {
     redraw();
    }
 }

void EditAngleWindow::looseFocus()
 {
  if( Change(focus,false) )
    {
     redraw();
    }
 }

 // mouse

MouseShape EditAngleWindow::getMouseShape(Point,KeyMod) const
 {
  return Mouse_Arrow;
 }

 // user input

void EditAngleWindow::react(UserAction action)
 {
  action.dispatch(*this);
 }

void EditAngleWindow::react_Key(VKey vkey,KeyMod kmod,unsigned repeat)
 {
  switch( vkey )
    {
     case VKey_Up :
     case VKey_Left :
     case VKey_NumPlus :
      {
       if( kmod&KeyMod_Shift )
         rotate(10*Coord(repeat));
       else
         rotate(Coord(repeat));
      }
     break;

     case VKey_Down :
     case VKey_Right :
     case VKey_NumMinus :
      {
       if( kmod&KeyMod_Shift )
         rotate(-10*Coord(repeat));
       else
         rotate(-Coord(repeat));
      }
     break;
    }
 }

void EditAngleWindow::react_LeftClick(Point point,MouseKey)
 {
  pin(point);
 }

void EditAngleWindow::react_Move(Point point,MouseKey mkey)
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

void EditAngleWindow::react_Leave()
 {
  if( Change(hilight,false) ) redraw();
 }

void EditAngleWindow::react_Wheel(Point,MouseKey mkey,Coord delta)
 {
  if( mkey&MouseKey_Shift )
    {
     rotate(10*delta);
    }
  else
    {
     rotate(delta);
    }
 }

} // namespace App


