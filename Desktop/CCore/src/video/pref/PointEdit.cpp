/* PointEdit.cpp */
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

#include <CCore/inc/video/pref/PointEdit.h>

#include <CCore/inc/video/FigureLib.h>

namespace CCore {
namespace Video {

/* class PointEditWindow */

void PointEditWindow::drawBox(const DrawBuf &buf) const
 {
  try
    {
     SmoothDrawArt art(buf);

     Pane pane=Envelope(pos,pos+getPoint());

     MCoord width=+cfg.width;

     FigureBox fig(pane);

     fig.loop(art,HalfPos,width,+cfg.line);
    }
  catch(CatchType)
    {
    }
 }

void PointEditWindow::changePoint(int)
 {
  changed.assert(getPoint());

  redraw();
 }

PointEditWindow::PointEditWindow(SubWindowHost &host,const ConfigType &cfg_)
 : ComboWindow(host),
   cfg(cfg_),

   x_spin(wlist,cfg.spin_cfg),
   y_spin(wlist,cfg.spin_cfg),

   connector_x_spin_changed(this,&PointEditWindow::changePoint,x_spin.changed),
   connector_y_spin_changed(this,&PointEditWindow::changePoint,y_spin.changed)
 {
  wlist.insTop(x_spin,y_spin);

  x_spin.setValue(0,-1000,1000);
  y_spin.setValue(0,-1000,1000);
 }

PointEditWindow::~PointEditWindow()
 {
 }

 // methods

Point PointEditWindow::getMinSize() const
 {
  Point s=x_spin.getMinSize();

  Coord space_dxy=+cfg.space_dxy;

  Coord dx=2*s.x+space_dxy;

  return Point(dx,s.y+dx);
 }

Point PointEditWindow::getPoint() const
 {
  return Point(Coord(x_spin.getValue()),Coord(y_spin.getValue()));
 }

void PointEditWindow::setPoint(Point value)
 {
  x_spin.setValue(value.x);
  y_spin.setValue(value.y);

  redraw();
 }

 // drawing

void PointEditWindow::layout()
 {
  Point s=x_spin.getMinSize();

  Coord space_dxy=+cfg.space_dxy;

  x_spin.setPlace(Pane(Null,s));

  y_spin.setPlace(Pane(Point(s.x+space_dxy,0),s));

  Coord dx=s.x+space_dxy/2;

  pos=Point(dx,s.y+dx);
 }

void PointEditWindow::draw(DrawBuf buf,bool drag_active) const
 {
  drawBox(buf);

  wlist.draw(buf,drag_active);
 }

void PointEditWindow::draw(DrawBuf buf,Pane pane,bool drag_active) const
 {
  drawBox(buf);

  wlist.draw(buf,pane,drag_active);
 }

} // namespace Video
} // namespace CCore


