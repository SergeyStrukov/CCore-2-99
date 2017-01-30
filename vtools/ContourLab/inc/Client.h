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

#ifndef Client_h
#define Client_h

#include <inc/Editor.h>

namespace App {

/* classes */

class ClientWindow;

/* class ClientWindow */

class ClientWindow : public ComboWindow , public AliveControl
 {
  public:

   struct Config
    {
     RefVal<DefString> text_LoadFile = "Select a file to load from"_def ;
     RefVal<DefString> text_SaveFile = "Select a file to save to"_def ;

     RefVal<DefString> text_Alert = "Alert"_def ;
     RefVal<DefString> text_AskSave = "Save modifications?"_def ;

     RefVal<DefString> text_Yes    = "Yes"_def ;
     RefVal<DefString> text_No     = "No"_def ;
     RefVal<DefString> text_Cancel = "Cancel"_def ;

     CtorRefVal<FileFrame::ConfigType> file_cfg;
     CtorRefVal<MessageFrame::AlertConfigType> msg_cfg;
     CtorRefVal<SimpleTopMenuWindow::ConfigType> menu_cfg;
     CtorRefVal<SimpleCascadeMenu::ConfigType> cascade_menu_cfg;

     // app

     EditorWindow::ConfigType editor_cfg;

     Config() noexcept {}

     Config(const UserPreference &pref) noexcept
      : editor_cfg(pref)
      {
       bind(pref.get(),pref.getSmartConfig());
      }

     template <class Bag,class Proxy>
     void bind(const Bag &bag,Proxy proxy)
      {
       text_LoadFile.bind(bag.text_LoadFile);
       text_SaveFile.bind(bag.text_SaveFile);

       text_Alert.bind(bag.text_Alert);
       text_AskSave.bind(bag.text_AskSave);

       text_Yes.bind(bag.text_Yes);
       text_No.bind(bag.text_No);
       text_Cancel.bind(bag.text_Cancel);

       file_cfg.bind(proxy);
       msg_cfg.bind(proxy);
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

   // frames

   FileFrame file_frame;
   MessageFrame msg_frame;

   // continuation

   enum Continue
    {
     ContinueNone = 0,

     ContinueNew,
     ContinueOpen,
     ContinueStartOpen,
     ContinueSave,
     ContinueSaveAs,
     ContinueExit
    };

   Continue cont = ContinueNone ;
   Point file_point;

  private:

   void menuOff();

   void fileOff();

   void msgOff();

   void askSave(Continue cont);

   void startOpen(Point point);

   void startSave(Point point);

  private:

   enum MenuId
    {
     MenuFile = 1,

     MenuFileNew = 101,
     MenuFileOpen = 102,
     MenuFileSave = 103,
     MenuFileSaveAs = 104,
     MenuFileExit = 110,

     MenuOptions = 2,

     MenuOptionsUserPref = 201,
     MenuOptionsAppPref = 202
    };

   void menu_selected(int id,Point point);

   void cascade_menu_selected(int id,Point point);

   void cascade_menu_pressed(VKey vkey,KeyMod kmod);

   void file_destroyed();

   void msg_destroyed();

   SignalConnector<ClientWindow,int,Point> connector_menu_selected;
   SignalConnector<ClientWindow,int,Point> connector_cascade_menu_selected;
   SignalConnector<ClientWindow,VKey,KeyMod> connector_cascade_menu_pressed;
   SignalConnector<ClientWindow> connector_file_destroyed;
   SignalConnector<ClientWindow> connector_msg_destroyed;

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

#endif

