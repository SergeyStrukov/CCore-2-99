/* FileWindow.h */
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

#ifndef CCore_inc_video_FileWindow_h
#define CCore_inc_video_FileWindow_h

#include <CCore/inc/video/DragWindow.h>

namespace CCore {
namespace Video {

/* classes */

class FileSubWindow;

class FileWindow;

/* class FileSubWindow */

class FileSubWindow : public ComboWindow
 {
  public:

   struct Config
    {
     Config() noexcept {}
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

  public:

   FileSubWindow(SubWindowHost &host,const Config &cfg);

   virtual ~FileSubWindow();

   // methods

   Point getMinSize() const;

   StrLen getFilePath() const;

   // drawing

   virtual void layout();

   virtual void draw(DrawBuf buf,bool drag_active) const;

   virtual void draw(DrawBuf buf,Pane pane,bool drag_active) const;

   // base

   virtual void open();
 };

/* class FileWindow */

class FileWindow : public DragWindow
 {
  public:

   struct Config
    {
     CtorRefVal<DragWindow::ConfigType> frame_cfg;
     CtorRefVal<FileSubWindow::ConfigType> file_cfg;

     Config() noexcept {}
    };

   using ConfigType = Config ;

  private:

   FileSubWindow sub_win;

  public:

   FileWindow(Desktop *desktop,const Config &cfg);

   FileWindow(Desktop *desktop,const Config &cfg,Signal<> &update);

   virtual ~FileWindow();

   // methods

   StrLen getFilePath() const { return sub_win.getFilePath(); } // available after the signal "destroyed"

   // create

   Pane getPane(Point base) const;

   using DragWindow::create;

   void create(Point base,const DefString &title)
    {
     create(getPane(base),title);
    }

   void create(FrameWindow *parent,Point base,const DefString &title)
    {
     create(parent,getPane(base),title);
    }
 };

} // namespace Video
} // namespace CCore

#endif

