/* ItemListWindow.h */
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

#ifndef ItemListWindow_h
#define ItemListWindow_h

#include <inc/Application.h>

namespace App {

/* classes */

class ItemListWindow;

/* class ItemListWindow */

class ItemListWindow : public ComboWindow
 {
  public:

   struct Config
    {
     RefVal<Coord> space_dxy = 10 ;

     CtorRefVal<ScrollListWindow::ConfigType> list_cfg;
     CtorRefVal<LineEditWindow::ConfigType> edit_cfg;
     CtorRefVal<KnobWindow::ConfigType> knob_cfg;

     Config() noexcept {}

     template <class AppPref>
     Config(const UserPreference &pref,const AppPref &app_pref) noexcept
      {
       bind(pref.get(),pref.getSmartConfig());
       bindApp(app_pref.get());
      }

     template <class Bag,class Proxy>
     void bind(const Bag &bag,Proxy proxy)
      {
       space_dxy.bind(bag.space_dxy);

       list_cfg.bind(proxy);
       edit_cfg.bind(proxy);
       knob_cfg.bind(proxy);
      }

     template <class Bag>
     void bindApp(const Bag &bag)
      {
       Used(bag);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   ScrollListWindow list;

   LineEditWindow edit;

   KnobWindow knob_down;
   KnobWindow knob_up;
   KnobWindow knob_del;
   KnobWindow knob_add;

  private:

   void list_selected(ulen index);

   SignalConnector<ItemListWindow,ulen> connector_list_selected;

   void knob_down_pressed();

   void knob_up_pressed();

   void knob_del_pressed();

   void knob_add_pressed();

   SignalConnector<ItemListWindow> connector_edit_entered;

   SignalConnector<ItemListWindow> connector_knob_down_pressed;
   SignalConnector<ItemListWindow> connector_knob_up_pressed;
   SignalConnector<ItemListWindow> connector_knob_del_pressed;
   SignalConnector<ItemListWindow> connector_knob_add_pressed;

  public:

   ItemListWindow(SubWindowHost &host,const Config &cfg);

   virtual ~ItemListWindow();

   // methods

   Point getMinSize(Point cap=Point::Max()) const;

   void setInfo(const ComboInfo &info) { list.setInfo(info); }

   bool select(ulen index) { return list.select(index); }

   bool reselect() { return list.reselect(); }

   void ping() { list.ping(); }

   void updateList()
    {
     list.layout();

     list.redraw();
    }

   StrLen getText() const { return edit.getText(); }

   // drawing

   virtual void layout();

   // signals

   Signal<ulen> command_up;
   Signal<ulen> command_down;
   Signal<ulen> command_del;
   Signal<ulen> command_add;

   Signal<ulen> selected;
 };

} // namespace App

#endif
