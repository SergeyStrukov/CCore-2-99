/* FileWindow.cpp */
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
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <CCore/inc/video/FileWindow.h>
#include <CCore/inc/video/Layout.h>

#include <CCore/inc/video/SmoothDrawArt.h>

namespace CCore {
namespace Video {

/* class FileSubWindow */

FileSubWindow::FileSubWindow(SubWindowHost &host,const Config &cfg_)
 : ComboWindow(host),
   cfg(cfg_)
 {
 }

FileSubWindow::~FileSubWindow()
 {
 }

 // methods

Point FileSubWindow::getMinSize() const
 {
  return Point(20,10);
 }

StrLen FileSubWindow::getFilePath() const
 {
  return Empty;
 }

 // base

void FileSubWindow::open() // TODO
 {
  ComboWindow::open();


 }

 // drawing

void FileSubWindow::layout() // TODO
 {
 }

void FileSubWindow::draw(DrawBuf buf,bool drag_active) const
 {
  buf.erase(+cfg.back);

  wlist.draw(buf,drag_active);
 }

void FileSubWindow::draw(DrawBuf buf,Pane pane,bool drag_active) const
 {
  SmoothDrawArt art(buf);

  art.block(pane,+cfg.back);

  wlist.draw(buf,pane,drag_active);
 }

/* class FileWindow */

FileWindow::FileWindow(Desktop *desktop,const Config &cfg)
 : DragWindow(desktop,cfg.frame_cfg),
   sub_win(*this,cfg.file_cfg)
 {
  bindClient(sub_win);
 }

FileWindow::FileWindow(Desktop *desktop,const Config &cfg,Signal<> &update)
 : FileWindow(desktop,cfg)
 {
  bindUpdate(update);
 }

FileWindow::~FileWindow()
 {
 }

 // create

Pane FileWindow::getPane(StrLen title,Point base) const
 {
  Point size=getMinSize(false,title,sub_win.getMinSize());

  Point screen_size=getDesktop()->getScreenSize();

  return FitToScreen(base,size,screen_size);
 }

} // namespace Video
} // namespace CCore

