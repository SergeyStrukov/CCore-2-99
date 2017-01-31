/* Editor.cpp */
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

#include <inc/Editor.h>

#include <CCore/inc/video/FigureLib.h>

namespace App {

/* class EditorWindow */

void EditorWindow::activate(SubWindow &editor) // TODO
 {
  wlist.del(edit_angle,edit_length);

  wlist.insTop(editor);

  redraw();
 }

void EditorWindow::select(const Contour::Object &obj,Geometry::Angle &angle)
 {
  angle_pad.select(obj,angle);

  edit_angle.setValue(angle);

  activate(edit_angle);
 }

void EditorWindow::select(const Contour::Object &obj,Geometry::Length &length)
 {
  length_pad.select(obj,length);

  edit_length.setValue(length);

  activate(edit_length);
 }

void EditorWindow::select(const Contour::Object &obj,Geometry::Ratio &ratio) // TODO
 {
  ratio_pad.select(obj,ratio);

  //

  //
 }

void EditorWindow::select(const Contour::Object &obj,Geometry::Point &point)
 {
  geom.selectPoint(obj,point);
 }

struct EditorWindow::SelectPad
 {
  EditorWindow *ptr;

  template <class T>
  void operator () (const String &,const Contour::Object &,T &)
   {
   }

  void operator () (const String &,const Contour::Object &obj,OneOfTypes<Geometry::Angle,Geometry::Length,Geometry::Ratio,Geometry::Point> &s)
   {
    ptr->select(obj,s);
   }
 };

void EditorWindow::selectPad(ulen index)
 {
  geom.contour.pad(index,SelectPad{this});
 }

void EditorWindow::angle_changed(Geometry::Angle angle)
 {
  if( angle_pad.set(angle) ) geom.redraw();
 }

void EditorWindow::length_changed(Geometry::Length length)
 {
  if( length_pad.set(length) ) geom.redraw();
 }

void EditorWindow::ratio_changed(Geometry::Ratio ratio)
 {
  if( ratio_pad.set(ratio) ) geom.redraw();
 }

Coord EditorWindow::getMinDXY() const
 {
  return 2*(+cfg.space_dxy);
 }

Coord EditorWindow::getMaxLeftDX() const
 {
  Coord space=+cfg.space_dxy;
  Coord min_dx=2*space;

  Coord total_dx=getSize().x;
  Coord sx=split1.getMinSize().dx;

  return Max_cast(min_dx,total_dx-2*space-min_dx-sx);
 }

Coord EditorWindow::getMaxTopDY() const
 {
  Coord space=+cfg.space_dxy;
  Coord min_dy=2*space;

  Coord total_dy=getSize().y;
  Coord sy=split2.getMinSize().dy;

  return Max_cast(min_dy,total_dy-2*space-min_dy-sy);
 }

bool EditorWindow::adjustSplitX(Coord dx)
 {
  Coord min_dx=getMinDXY();
  Coord max_dx=getMaxLeftDX();

  return Change(left_dx, Cap<Coord>(min_dx,left_dx+dx,max_dx) );
 }

bool EditorWindow::adjustSplitY(Coord dy)
 {
  Coord min_dy=getMinDXY();
  Coord max_dy=getMaxTopDY();

  return Change(top_dy, Cap<Coord>(min_dy,top_dy+dy,max_dy) );
 }

void EditorWindow::adjustSplit(Point point)
 {
  if( adjustSplitX(point.x) | adjustSplitY(point.y) )
    {
     layout();

     redraw();
    }
 }

void EditorWindow::split1_dragged(Point point)
 {
  adjustSplit(point);
 }

void EditorWindow::split2_dragged(Point point)
 {
  adjustSplit(point);
 }

EditorWindow::EditorWindow(SubWindowHost &host,const Config &cfg_)
 : ComboWindow(host),
   cfg(cfg_),

   split1(wlist,cfg.split_cfg),
   split2(wlist,cfg.split_cfg),
   edit_angle(wlist,cfg.edit_angle_cfg),
   edit_length(wlist,cfg.edit_length_cfg),
   geom(wlist,cfg.geom_cfg),

   connector_angle_changed(this,&EditorWindow::angle_changed,edit_angle.changed),
   connector_length_changed(this,&EditorWindow::length_changed,edit_length.changed),
   connector_ratio_changed(this,&EditorWindow::ratio_changed), // TODO

   connector_split1_dragged(this,&EditorWindow::split1_dragged,split1.dragged),
   connector_split2_dragged(this,&EditorWindow::split2_dragged,split2.dragged)
 {
  wlist.insTop(split1,split2,geom);

  selectPad(0); // TODO
 }

EditorWindow::~EditorWindow()
 {
 }

 // methods

void EditorWindow::load() // TODO
 {
 }

void EditorWindow::load(StrLen file_name)  // TODO
 {
  Used(file_name);
 }

bool EditorWindow::save() // TODO
 {
  modified=false;

  return true;
 }

void EditorWindow::save(StrLen file_name) // TODO
 {
  Used(file_name);

  modified=false;
 }

 // drawing

void EditorWindow::layout()
 {
  Coord space=+cfg.space_dxy;

  PaneCut right(space);
  PaneCut top(space);
  PaneCut bottom(space);

  // first split

  {
   PaneCut pane(getSize(),space);

   pane.shrink();

   if( Change(layout_first,false) )
     {
      PaneCut p=pane.cutLeft(Div(1,3),0);

      left_dx=p.getSize().x;

      pane.place_cutLeft(split1);

      right=pane;

      p.place_cutTop(top,Div(1,2),0).place_cutTop(split2).place(bottom);

      top_dy=top.getSize().y;
     }
   else
     {
      Coord min_dxy=getMinDXY();

      left_dx=Cap(min_dxy,left_dx,getMaxLeftDX());

      top_dy=Cap(min_dxy,top_dy,getMaxTopDY());

      PaneCut p=pane.cutLeft(left_dx,0);

      pane.place_cutLeft(split1);

      right=pane;

      p.place_cutTop(top,top_dy,0).place_cutTop(split2).place(bottom);
     }
  }

  // right

  {
   PaneCut pane(space);

   right.place_cutBottom(pane,Div(20,100)).place(geom);

   pane.placeSmart(edit_angle);
   pane.placeSmart(edit_length);
  }

  // top

  {
  }

  // bottom

  {
  }
 }

void EditorWindow::drawBack(DrawBuf buf,bool) const
 {
  buf.erase(+cfg.back);
 }

} // namespace App


