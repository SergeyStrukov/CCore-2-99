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

Coord EditorWindow::getMaxLeftDX() const
 {
  Coord space=+cfg.space_dxy;

  Coord total_dx=getSize().x;
  Coord sx=split1.getMinSize().dx;

  return Max_cast(MinDXY,total_dx-2*space-MinDXY-sx);
 }

Coord EditorWindow::getMaxTopDY() const
 {
  Coord space=+cfg.space_dxy;

  Coord total_dy=getSize().y;
  Coord sy=split2.getMinSize().dy;

  return Max_cast(MinDXY,total_dy-2*space-MinDXY-sy);
 }

bool EditorWindow::adjustSplitX(Coord dx)
 {
  Coord max_dx=getMaxLeftDX();

  return Change(left_dx, Cap<Coord>(MinDXY,left_dx+dx,max_dx) );
 }

bool EditorWindow::adjustSplitY(Coord dy)
 {
  Coord max_dy=getMaxTopDY();

  return Change(top_dy, Cap<Coord>(MinDXY,top_dy+dy,max_dy) );
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

   top(wlist,Blue),
   split2(wlist,cfg.split_cfg),
   bottom(wlist,Blue),
   split1(wlist,cfg.split_cfg),
   right(wlist,Blue),

   connector_split1_dragged(this,&EditorWindow::split1_dragged,split1.dragged),
   connector_split2_dragged(this,&EditorWindow::split2_dragged,split2.dragged)
 {
  wlist.insTop(top,split2,bottom,split1,right);
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
  PaneCut pane(getSize(),+cfg.space_dxy);

  pane.shrink();

  if( Change(layout_first,false) )
    {
     PaneCut p=pane.cutLeft(Div(1,3),0);

     left_dx=p.getSize().x;

     pane.place_cutLeft(split1).place(right);

     p.place_cutTop(top,Div(1,2),0).place_cutTop(split2).place(bottom);

     top_dy=top.getSize().y;
    }
  else
    {
     left_dx=Cap(MinDXY,left_dx,getMaxLeftDX());

     top_dy=Cap(MinDXY,top_dy,getMaxTopDY());

     PaneCut p=pane.cutLeft(left_dx,0);

     pane.place_cutLeft(split1).place(right);

     p.place_cutTop(top,top_dy,0).place_cutTop(split2).place(bottom);
    }
 }

void EditorWindow::drawBack(DrawBuf buf,bool) const
 {
  buf.erase(+cfg.back);
 }

} // namespace App


