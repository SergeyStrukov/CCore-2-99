/* Display.h */
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

#include <inc/Application.h>

#include <CCore/inc/video/Menu.h>

namespace App {

/* classes */

class DisplayWindow;

class ClientWindow;

/* class DisplayWindow */

class DisplayWindow : public SubWindow
 {
  public:

   struct Config
    {
     Config() noexcept {}

     explicit Config(const UserPreference &pref)
      {
       Used(pref);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

  public:

   DisplayWindow(SubWindowHost &host,const Config &cfg);

   virtual ~DisplayWindow();
 };

/* class ClientWindow */

class ClientWindow : public ComboWindow
 {
  public:

   struct Config
    {
     CtorRefVal<SimpleTopMenuWindow::ConfigType> menu_cfg;
     CtorRefVal<DisplayWindow::ConfigType> display_cfg;

     Config() noexcept {}

     explicit Config(const UserPreference &pref)
      : //menu_cfg(SmartBind,pref),
        display_cfg(pref)
      {
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   MenuData menu_data;

   SimpleTopMenuWindow menu;
   DisplayWindow display;

  private:

   void menu_selected(int id,Point point);

   SignalConnector<ClientWindow,int,Point> connector_menu_selected;

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

   void react_other(UserAction action);
 };

} // namespace App

