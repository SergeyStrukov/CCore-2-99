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
#include <CCore/inc/video/UserPreference.h>

#include <CCore/inc/FileSystem.h>
#include <CCore/inc/ElementPool.h>
#include <CCore/inc/Array.h>
#include <CCore/inc/Sort.h>
#include <CCore/inc/MakeString.h>

namespace CCore {
namespace Video {

/* classes */

class InfoBuilder;

class FileSubWindow;

class FileWindow;

/* class InfoBuilder */

class InfoBuilder : NoCopy
 {
   ElementPool pool;
   DynArray<StrLen> list;

   class PoolInfo;

  public:

   InfoBuilder();

   ~InfoBuilder();

   void add(StrLen text);

   void sort(ObjLessFuncType<StrLen> less) { IncrSort(Range(list),less); }

   Info complete();
 };

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

     Config() noexcept {}

     explicit Config(const UserPreference &pref) noexcept
      : edit_cfg(SmartBind,pref),
        list_cfg(SmartBind,pref),
        btn_cfg(SmartBind,pref)
      {
       back.bind(pref.get().back);
       space_dxy.bind(pref.get().space_dxy);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   LineEditWindow dir;
   SimpleTextListWindow dir_list;
   SimpleTextListWindow file_list;

   ButtonWindow btn_Ok;
   ButtonWindow btn_Cancel;

   FileSystem fs;

   class MakeFileName : public MakeString<MaxPathLen>
    {
     public:

      MakeFileName() {}

      MakeFileName(StrLen dir_name,StrLen file_name);

      StrLen operator () (StrLen dir_name,StrLen file_name);
    };

   MakeFileName file_buf;
   StrLen file_path;

  private:

   static CmpResult NameCmp(StrLen a,StrLen b);

   static bool ExtNameLess(StrLen a,StrLen b);

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

  public:

   FileSubWindow(SubWindowHost &host,const Config &cfg);

   virtual ~FileSubWindow();

   // methods

   Point getMinSize() const;

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

