/* CoordEdit.cpp */
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

#include <inc/CoordEdit.h>

#include <CCore/inc/video/SmoothDrawArt.h>

namespace App {

/* class CoordEditWindow */

void CoordEditWindow::drawLine(const DrawBuf &buf) const
 {
  try
    {
     Coord mark_dy=+cfg.mark_dy;

     SmoothDrawArt art(buf);

     Point end=pos.addX(getCoord());

     MCoord width=+cfg.width;

     art.path(width,+cfg.line,pos,end);

     art.path(width,+cfg.line,pos.subY(mark_dy),pos.addY(mark_dy));

     art.path(width,+cfg.line,end.subY(mark_dy),end.addY(mark_dy));
    }
  catch(CatchType)
    {
    }
 }

void CoordEditWindow::valueChanged(int value)
 {
  changed.assert(Coord(value));

  redraw();
 }

CoordEditWindow::CoordEditWindow(SubWindowHost &host,const ConfigType &cfg_)
 : ComboWindow(host),
   cfg(cfg_),

   spin(wlist,cfg.spin_cfg),

   spin_changed(this,&CoordEditWindow::valueChanged,spin.changed)
 {
  wlist.insTop(spin);

  spin.setValue(0,-1000,1000);
 }

CoordEditWindow::~CoordEditWindow()
 {
 }

 // methods

Point CoordEditWindow::getMinSize() const
 {
  Point s=spin.getMinSize();

  return Point(s.x,2*s.y);
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

} // namespace App


