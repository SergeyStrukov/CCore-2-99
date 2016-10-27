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

void FileSubWindow::fillLists() // TODO
 {
  dir_list.enable();
  file_list.enable();

  DirCursor cur(fs,dir.getText());

  cur.apply( [] (StrLen name,FileType ft)
                {
                 switch( ft )
                   {
                    case FileType_dir :
                    break;

                    case FileType_file :
                    break;
                   }

                } );
 }

void FileSubWindow::setDir(StrLen dir_name) // TODO
 {
  char temp[MaxPathLen+1];

  StrLen path=fs.pathOf(dir_name,temp);

  if( fs.getFileType(path)==FileType_dir )
    {
     dir.setText(path);

     fillLists();
    }
  else
    {
     dir_list.disable();
     file_list.disable();

     // TODO
    }
 }

void FileSubWindow::buildFilePath() // TODO
 {
 }

void FileSubWindow::dir_entered()
 {
  StrLen dir_name=dir.getText();

  setDir(dir_name);
 }

void FileSubWindow::dir_changed()
 {
  dir_list.disable();
  file_list.disable();
 }

void FileSubWindow::btn_Ok_pressed()
 {
  buildFilePath();

  askFrameClose();
 }

void FileSubWindow::btn_Cancel_pressed()
 {
  askFrameClose();
 }

FileSubWindow::FileSubWindow(SubWindowHost &host,const Config &cfg_)
 : ComboWindow(host),
   cfg(cfg_),

   dir(wlist,cfg.edit_cfg),
   dir_list(wlist,cfg.list_cfg),
   file_list(wlist,cfg.list_cfg),
   btn_Ok(wlist,cfg.btn_cfg,"Ok"_def),
   btn_Cancel(wlist,cfg.btn_cfg,"Cancel"_def),

   connector_dir_entered(this,&FileSubWindow::dir_entered,dir.entered),
   connector_dir_changed(this,&FileSubWindow::dir_changed,dir.changed),
   connector_btn_Ok_pressed(this,&FileSubWindow::btn_Ok_pressed,btn_Ok.pressed),
   connector_btn_Cancel_pressed(this,&FileSubWindow::btn_Cancel_pressed,btn_Cancel.pressed)
 {
  wlist.insTop(dir,dir_list,file_list,btn_Ok,btn_Cancel);
 }

FileSubWindow::~FileSubWindow()
 {
 }

 // methods

Point FileSubWindow::getMinSize() const
 {
  Coord space=+cfg.space_dxy;

  Point dir_size=dir.getMinSize();

  Point btn_size=SupMinSize(btn_Ok,btn_Cancel);

  Coord dx=2*space+Max<Coord>(dir_size.x,2*btn_size.x+space);
  Coord dy=5*space+12*dir_size.y+btn_size.y;

  return Point(dx,dy);
 }

 // base

void FileSubWindow::open()
 {
  ComboWindow::open();

  file_path=Empty;

  setDir(".");
 }

 // drawing

void FileSubWindow::layout()
 {
  Coord space=+cfg.space_dxy;
  Panesor psor(getSize(),space);

  psor.shrink();

  // dir

  {
   Point s=dir.getMinSize();

   psor.placeY(dir,s.y);
  }

  // dir_list

  {
   psor.placeY(dir_list,Rational(1,3));
  }

  // file_list

  Point btn_size=SupMinSize(btn_Ok,btn_Cancel);

  {
   psor.placeY_rest(file_list,btn_size.y);
  }

  // btn

  {
   PlaceRow row(psor,btn_size,space,2);

   btn_Ok.setPlace(*row);

   ++row;

   btn_Cancel.setPlace(*row);
  }
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

