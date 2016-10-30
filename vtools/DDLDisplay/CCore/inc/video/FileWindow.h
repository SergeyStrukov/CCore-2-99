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
#include <CCore/inc/video/WindowLib.h>
#include <CCore/inc/video/Menu.h>
#include <CCore/inc/video/UserPreference.h>

#include <CCore/inc/FileSystem.h>
#include <CCore/inc/MakeFileName.h>

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
     RefVal<Coord> space_dxy = 10 ;

     RefVal<VColor> back = Silver ;

     CtorRefVal<LineEditWindow::ConfigType> edit_cfg;
     CtorRefVal<SimpleTextListWindow::ConfigType> list_cfg;
     CtorRefVal<ButtonWindow::ConfigType> btn_cfg;
     CtorRefVal<KnobWindow::ConfigType> knob_cfg;

     CtorRefVal<SimpleCascadeMenu::ConfigType> hit_menu_cfg;

     Config() noexcept {}

     explicit Config(const UserPreference &pref) noexcept
      : edit_cfg(SmartBind,pref),
        list_cfg(SmartBind,pref),
        btn_cfg(SmartBind,pref),
        knob_cfg(SmartBind,pref),
        hit_menu_cfg(SmartBind,pref)
      {
       back.bind(pref.get().back);
       space_dxy.bind(pref.get().space_dxy);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   LineEditWindow dir;
   KnobWindow knob_hit;
   KnobWindow knob_add;
   KnobWindow knob_back;
   SimpleTextListWindow dir_list;
   SimpleTextListWindow file_list;

   ButtonWindow btn_Ok;
   ButtonWindow btn_Cancel;

   MenuData hit_data;
   SimpleCascadeMenu hit_menu;

   FileSystem fs;

   MakeFileName file_buf;
   StrLen file_path;

  private:

   void fillLists();

   void setDir(StrLen dir_name);

   void setDir(StrLen dir_name,StrLen sub_dir);

   void buildFilePath();

  private:

   void file_list_entered();

   SignalConnector<FileSubWindow> connector_file_list_entered;
   SignalConnector<FileSubWindow> connector_file_list_dclicked;

   void dir_list_entered();

   SignalConnector<FileSubWindow> connector_dir_list_entered;
   SignalConnector<FileSubWindow> connector_dir_list_dclicked;

   void dir_entered();

   void dir_changed();

   SignalConnector<FileSubWindow> connector_dir_entered;
   SignalConnector<FileSubWindow> connector_dir_changed;

   void btn_Ok_pressed();

   void btn_Cancel_pressed();

   SignalConnector<FileSubWindow> connector_btn_Ok_pressed;
   SignalConnector<FileSubWindow> connector_btn_Cancel_pressed;

   void knob_hit_pressed();

   void knob_add_pressed();

   void knob_back_pressed();

   SignalConnector<FileSubWindow> connector_knob_hit_pressed;
   SignalConnector<FileSubWindow> connector_knob_add_pressed;
   SignalConnector<FileSubWindow> connector_knob_back_pressed;

  public:

   FileSubWindow(SubWindowHost &host,const Config &cfg);

   virtual ~FileSubWindow();

   // methods

   Point getMinSize(StrLen sample_text) const;

   StrLen getFilePath() const { return file_path; }

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

     explicit Config(const UserPreference &pref) noexcept
      : frame_cfg(SmartBind,pref),
        file_cfg(pref)
      {
      }
    };

   using ConfigType = Config ;

  private:

   FileSubWindow sub_win;

   static const char *const SampleDir;

  public:

   FileWindow(Desktop *desktop,const Config &cfg);

   FileWindow(Desktop *desktop,const Config &cfg,Signal<> &update);

   virtual ~FileWindow();

   // methods

   StrLen getFilePath() const { return sub_win.getFilePath(); } // available after the signal "destroyed"

   // create

   Pane getPane(StrLen title,Point base) const;

   using DragWindow::create;

   void create(Point base,const DefString &title)
    {
     create(getPane(title.str(),base),title);
    }

   void create(FrameWindow *parent,Point base,const DefString &title)
    {
     create(parent,getPane(title.str(),base),title);
    }
 };

} // namespace Video
} // namespace CCore

#endif

