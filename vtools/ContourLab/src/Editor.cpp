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

void EditorWindow::split1_dragged(Point point)
 {
  Point size=getSize();
  Point s1=split1.getSize();

  Coord min_dx=10;
  Coord max_dx=size.x-s1.x-10;

  if( min_dx>max_dx ) return;

  Coord dx=Cap<Coord>(min_dx,left_dx+point.x,max_dx);

  if( Change(left_dx,dx) )
    {
     layout();

     redraw();
    }
 }

EditorWindow::EditorWindow(SubWindowHost &host,const Config &cfg_)
 : ComboWindow(host),
   cfg(cfg_),

   left(wlist,Blue),
   split1(wlist,cfg.split_cfg),
   right(wlist,Blue),

   connector_split1_dragged(this,&EditorWindow::split1_dragged,split1.dragged)
 {
  wlist.insTop(left,split1,right);
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
     pane.place_cutLeft(left,Div(1,3),0).place_cutLeft(split1).place(right);

     left_dx=left.getSize().x;
    }
  else
    {
     pane.place_cutLeft(left,left_dx,0).place_cutLeft(split1).place(right);
    }
 }

void EditorWindow::drawBack(DrawBuf buf,bool) const
 {
  buf.erase(+cfg.back);
 }

} // namespace App


