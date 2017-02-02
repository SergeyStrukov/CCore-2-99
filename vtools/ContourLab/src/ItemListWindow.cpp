/* ItemListWindow.cpp */
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

#include <inc/ItemListWindow.h>

#include <CCore/inc/video/Layout.h>

namespace App {

/* class ItemListWindow */

void ItemListWindow::list_selected(ulen index)
 {
  selected.assert(index);
 }

void ItemListWindow::knob_down_pressed()
 {
  command_down.assert(list.getSelect());
 }

void ItemListWindow::knob_up_pressed()
 {
  command_up.assert(list.getSelect());
 }

void ItemListWindow::knob_del_pressed()
 {
  command_del.assert(list.getSelect());
 }

void ItemListWindow::knob_add_pressed()
 {
  command_add.assert(list.getSelect());
 }

ItemListWindow::ItemListWindow(SubWindowHost &host,const Config &cfg_)
 : ComboWindow(host),
   cfg(cfg_),

   list(wlist,cfg.list_cfg),
   edit(wlist,cfg.edit_cfg),

   knob_down(wlist,cfg.knob_cfg,KnobShape::FaceDown),
   knob_up(wlist,cfg.knob_cfg,KnobShape::FaceUp),
   knob_del(wlist,cfg.knob_cfg,KnobShape::FaceCross),
   knob_add(wlist,cfg.knob_cfg,KnobShape::FacePlus),

   connector_list_selected(this,&ItemListWindow::list_selected,list.selected),
   connector_edit_entered(this,&ItemListWindow::knob_add_pressed,edit.entered),
   connector_knob_down_pressed(this,&ItemListWindow::knob_down_pressed,knob_down.pressed),
   connector_knob_up_pressed(this,&ItemListWindow::knob_up_pressed,knob_up.pressed),
   connector_knob_del_pressed(this,&ItemListWindow::knob_del_pressed,knob_del.pressed),
   connector_knob_add_pressed(this,&ItemListWindow::knob_add_pressed,knob_add.pressed)
 {
  wlist.insTop(list,edit,knob_down,knob_up,knob_del,knob_add);
 }

ItemListWindow::~ItemListWindow()
 {
 }

 // methods

Point ItemListWindow::getMinSize(Point cap) const
 {
  Coord space=+cfg.space_dxy;

  Coord knob_dxy=knob_down.getMinSize().dxy;
  Coord kspace=BoxSpace(knob_dxy);

  Point edit_size=edit.getMinSize();

  Coord dy=2*space+knob_dxy+Max(edit_size.y,knob_dxy);

  Point list_size=list.getMinSize(Point(cap.x,cap.y-dy));

  return Point( Max_cast( list_size.x , edit_size.x+knob_dxy+kspace , 3*knob_dxy+2*kspace ) , list_size.y+dy );
 }

 // drawing

void ItemListWindow::layout()
 {
  Coord space=+cfg.space_dxy;

  PaneCut pane(getSize(),space);

  // knob_down , knob_up , knob_del

  {
   Coord dy=knob_down.getMinSize().dxy;

   PaneCut p=pane.cutTop(dy);

   p.place_cutLeft(knob_down).place_cutLeft(knob_up).place_cutRight(knob_del);
  }

  // edit , knob_add

  {
   auto knob__add=CutBox(knob_add);
   auto edit_=CutPoint(edit);

   Coord dy=SupDY(knob__add,edit_);

   PaneCut p=pane.cutBottom(dy);

   p.place_cutRight(knob__add).place(edit_);
  }

  // list

  {
   pane.place(list);
  }
 }

} // namespace App


