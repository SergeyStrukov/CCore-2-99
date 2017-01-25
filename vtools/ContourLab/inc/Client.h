/* Client.h */
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

#include <inc/Application.h>

#include <CCore/inc/video/WindowLib.h>
#include <CCore/inc/video/Menu.h>
#include <CCore/inc/video/MessageFrame.h>
#include <CCore/inc/video/FileFrame.h>

#include <CCore/inc/Array.h>

namespace App {

/* classes */

class EditorWindow;

class ClientWindow;

/* class EditorWindow */

class EditorWindow : public ComboWindow
 {
  public:

   struct Config
    {
     RefVal<VColor> back;

     Config() noexcept {}

     Config(const UserPreference &pref) noexcept
      {
       bind(pref.get(),pref.getSmartConfig());
      }

     template <class Bag,class Proxy>
     void bind(const Bag &bag,Proxy proxy)
      {
       Used(proxy);

       back.bind(bag.back);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   bool modified = false ;

  public:

   EditorWindow(SubWindowHost &host,const Config &cfg);

   virtual ~EditorWindow();

   // methods

   bool isModified() const { return modified; }

   void load();

   void load(StrLen file_name);

   bool save();

   void save(StrLen file_name);

   // drawing

   virtual void layout();

   virtual void drawBack(DrawBuf buf,bool drag_active) const;
 };

/* class ClientWindow */

class ClientWindow : public ComboWindow , public AliveControl
 {
  public:

   struct Config
    {
     RefVal<DefString> load_file = "Select a file to load from"_def ;
     RefVal<DefString> save_file = "Select a file to save to"_def ;

     EditorWindow::ConfigType editor_cfg;

     CtorRefVal<FileFrame::ConfigType> file_cfg;
     CtorRefVal<SimpleTopMenuWindow::ConfigType> menu_cfg;
     CtorRefVal<SimpleCascadeMenu::ConfigType> cascade_menu_cfg;

     Config() noexcept {}

     Config(const UserPreference &pref) noexcept
      : editor_cfg(pref)
      {
       bind(pref.get(),pref.getSmartConfig());
      }

     template <class Bag,class Proxy>
     void bind(const Bag &bag,Proxy proxy)
      {
       Used(bag);

       file_cfg.bind(proxy);
       menu_cfg.bind(proxy);
       cascade_menu_cfg.bind(proxy);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   // menu

   MenuData menu_data;

   MenuData menu_file_data;
   MenuData menu_opt_data;

   SimpleTopMenuWindow menu;
   SimpleCascadeMenu cascade_menu;

   // editor

   EditorWindow editor;

   // file frame

   FileFrame file_frame;

   // continuation

   enum Continue
    {
     ContinueNone = 0,

     ContinueOpen,
     ContinueSave,
     ContinueSaveAs
    };

   Continue cont = ContinueNone ;

  private:

   void menuOff();

   void fileOff();

  private:

   enum MenuId
    {
     MenuFile = 1,

     MenuFileNew = 101,
     MenuFileOpen = 102,
     MenuFileSave = 103,
     MenuFileSaveAs = 104,
     MenuFileClose = 105,
     MenuFileExit = 110,

     MenuOptions = 2,

     MenuOptionsUserPref = 201,
     MenuOptionsAppPref = 202
    };

   void menu_selected(int id,Point point);

   void cascade_menu_selected(int id,Point point);

   void cascade_menu_pressed(VKey vkey,KeyMod kmod);

   void file_destroyed();

   SignalConnector<ClientWindow,int,Point> connector_menu_selected;
   SignalConnector<ClientWindow,int,Point> connector_cascade_menu_selected;
   SignalConnector<ClientWindow,VKey,KeyMod> connector_cascade_menu_pressed;
   SignalConnector<ClientWindow> connector_file_destroyed;

  public:

   ClientWindow(SubWindowHost &host,const Config &cfg);

   virtual ~ClientWindow();

   // base

   virtual void open();

   // drawing

   virtual void layout();

   // user input

   virtual void react(UserAction action);

   void react_Key(VKey vkey,KeyMod kmod);

   void react_LeftClick(Point point,MouseKey mkey);

   void react_RightClick(Point point,MouseKey mkey);

   void react_other(UserAction action);

   // AliveControl

   virtual bool askDestroy();
 };

} // namespace App


