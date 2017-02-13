/* CoordEdit.cpp */
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

#include <CCore/inc/video/pref/CoordEdit.h>

#include <CCore/inc/video/SmoothDrawArt.h>

namespace CCore {
namespace Video {

/* class CoordEditWindow */

void CoordEditWindow::drawLine(const DrawBuf &buf) const
 {
  Coord mark_dy=+cfg.mark_dy;
  MCoord width=+cfg.width;

  SmoothDrawArt art(buf);

  Point end=pos.addX(getCoord());

  art.path(width,+cfg.line,pos,end);

  art.path(width,+cfg.line,pos.subY(mark_dy),pos.addY(mark_dy));

  art.path(width,+cfg.line,end.subY(mark_dy),end.addY(mark_dy));
 }

void CoordEditWindow::valueChanged(int value)
 {
  redraw();

  changed.assert(Coord(value));
 }

CoordEditWindow::CoordEditWindow(SubWindowHost &host,const ConfigType &cfg_)
 : ComboWindow(host),
   cfg(cfg_),

   spin(wlist,cfg.spin_cfg),

   spin_changed(this,&CoordEditWindow::valueChanged,spin.changed)
 {
  wlist.insTop(spin);

  spin.setValue(0,MinCoord,MaxCoord);
 }

CoordEditWindow::~CoordEditWindow()
 {
 }

 // methods

Point CoordEditWindow::getMinSize() const
 {
  Point s=spin.getMinSize();

  return Point(s.x,2*Coordinate(s.y));
 }

 // drawing

void CoordEditWindow::layout()
 {
  Point size=getSize();

  spin.setPlace(Pane(Null,size.x,size.y/2));

  pos.x=size.x/2;
  pos.y=Div(3,4)*size.y;
 }

void CoordEditWindow::draw(DrawBuf buf,bool drag_active) const
 {
  drawLine(buf);

  wlist.draw(buf,drag_active);
 }

void CoordEditWindow::draw(DrawBuf buf,Pane pane,bool drag_active) const
 {
  drawLine(buf);

  wlist.draw(buf,pane,drag_active);
 }

} // namespace Video
} // namespace CCore


