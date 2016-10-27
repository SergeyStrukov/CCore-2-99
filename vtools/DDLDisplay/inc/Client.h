/* Client.h */
//----------------------------------------------------------------------------------------
//
//  Project: DDLDisplay 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/Display.h>

#include <CCore/inc/video/Menu.h>
#include <CCore/inc/video/FileWindow.h>

namespace App {

/* classes */

class ClientWindow;

/* class ClientWindow */

class ClientWindow : public ComboWindow
 {
  public:

   struct Config
    {
     CtorRefVal<SimpleTopMenuWindow::ConfigType> menu_cfg;
     CtorRefVal<SimpleCascadeMenu::ConfigType> cascade_menu_cfg;
     CtorRefVal<DisplayWindow::ConfigType> display_cfg;
     CtorRefVal<FileWindow::ConfigType> file_cfg;

     Config() noexcept {}

     explicit Config(const UserPreference &pref) // TODO
      : menu_cfg(SmartBind,pref),
        cascade_menu_cfg(SmartBind,pref),
        display_cfg(pref),
        file_cfg(pref)
      {
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   enum CommandId
    {
     Cmd_File    =   1,
     Cmd_Options =   2,

     Cmd_Open    = 101,
     Cmd_Exit    = 110
    };

   MenuData menu_data;

   MenuData menu_file_data;
   MenuData menu_options_data;

   SimpleTopMenuWindow menu;
   SimpleCascadeMenu cascade_menu;

   DisplayWindow display;

   FileWindow open_file;

  private:

   void menu_off();

  private:

   void menu_selected(int id,Point point);

   void cascade_menu_selected(int id,Point point);

   void cascade_menu_pressed(VKey vkey,KeyMod kmod);

   SignalConnector<ClientWindow,int,Point> connector_menu_selected;
   SignalConnector<ClientWindow,int,Point> connector_cascade_menu_selected;
   SignalConnector<ClientWindow,VKey,KeyMod> connector_cascade_menu_pressed;

   void open_file_destroyed();

   SignalConnector<ClientWindow> connector_open_file_destroyed;

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
 };

} // namespace App


