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

namespace App {

/* class EditorWindow */

EditorWindow::EditorWindow(SubWindowHost &host,const Config &cfg_)
 : ComboWindow(host),
   cfg(cfg_)
 {
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
 }

void EditorWindow::drawBack(DrawBuf buf,bool) const
 {
  buf.erase(+cfg.back);
 }

} // namespace App


