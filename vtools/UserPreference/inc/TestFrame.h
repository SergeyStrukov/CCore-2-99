/* TestFrame.h */
//----------------------------------------------------------------------------------------
//
//  Project: UserPreference 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef TestFrame_h
#define TestFrame_h

#include <inc/Application.h>

#include <CCore/inc/video/UserPreference.h>
#include <CCore/inc/video/MessageWindow.h>
#include <CCore/inc/video/Menu.h>

namespace App {

/* classes */

class TestWindow;

class TestClient;

class TestFrame;

/* class TestWindow */

class TestWindow : public ComboWindow
 {
   const UserPreference &pref;

   ButtonWindow btn;
   KnobWindow knob;
   CheckWindow check;
   SwitchWindow swtch;

   RadioGroup group;
   RadioWindow rad1;
   RadioWindow rad2;
   RadioWindow rad3;

   XScrollWindow xscroll;

   InfoWindow info;
   LineEditWindow edit;
   SimpleTextListWindow text_list;

   LabelWindow label1;
   LabelWindow label2;
   LabelWindow label3;
   LabelWindow label;

   TextWindow text;

   XSingleLineWindow xsingle;
   YSingleLineWindow ysingle;
   XDoubleLineWindow xdouble;
   YDoubleLineWindow ydouble;

   ContourWindow contour;
   TextContourWindow text_contour;
   LightWindow light;
   ProgressWindow progress;

   ButtonWindow shade_btn;

   MessageWindow enable_window;

  private:

   static const char *const InfoText;

   static const char *const ListText;

   void changeColor(int new_id,int prev_id);

   SignalConnector<TestWindow,int,int> connector_group_changed;

   void lightOnOff(bool check);

   SignalConnector<TestWindow,bool> connector_check_changed;

   void knobPressed();

   SignalConnector<TestWindow> connector_knob_pressed;

   void enableAll(bool en);

   SignalConnector<TestWindow,bool> connector_swtch_changed;

   void setFace(ulen index);

   SignalConnector<TestWindow,ulen> connector_xscroll_changed;

   void push();

   SignalConnector<TestWindow> connector_btn_pressed;

   void shade();

   SignalConnector<TestWindow> connector_shade_btn_pressed;

   void unshade();

   SignalConnector<TestWindow> connector_enable_window_destoyed;

  public:

   TestWindow(SubWindowHost &host,const UserPreference &pref);

   virtual ~TestWindow();

   // drawing

   virtual void layout();

   virtual void draw(DrawBuf buf,bool drag_active) const;

   virtual void draw(DrawBuf buf,Pane pane,bool drag_active) const;
 };

/* class TestClient */

class TestClient : public ComboWindow
 {
   MenuData menu_data;

   MenuData menu_file_data;
   MenuData menu_edit_data;
   MenuData menu_options_data;
   MenuData menu_window_data;

   SimpleTopMenuWindow menu;
   SimpleCascadeMenu cascade_menu;

   TestWindow test;

  private:

   void menu_off();

  private:

   void menu_selected(int id,Point point);

   void cascade_menu_selected(int id,Point point);

   void cascade_menu_pressed(VKey vkey,KeyMod kmod);

   SignalConnector<TestClient,int,Point> connector_menu_selected;
   SignalConnector<TestClient,int,Point> connector_cascade_menu_selected;
   SignalConnector<TestClient,VKey,KeyMod> connector_cascade_menu_pressed;

   void update();

   SignalConnector<TestClient> connector_update;

  public:

   TestClient(SubWindowHost &host,const UserPreference &pref,Signal<> &update);

   virtual ~TestClient();

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

/* class TestFrame */

class TestFrame : public DragWindow
 {
   TestClient client;

  public:

   TestFrame(Desktop *desktop,const UserPreference &pref,Signal<> &update);

   virtual ~TestFrame();
 };

} // namespace App

#endif

