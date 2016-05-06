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

namespace App {

/* classes */

class TestWindow;

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

  public:

   TestWindow(SubWindowHost &host,const UserPreference &pref);

   virtual ~TestWindow();

   // drawing

   virtual void layout();

   virtual void draw(DrawBuf buf,bool drag_active) const;

   virtual void draw(DrawBuf buf,Pane pane,bool drag_active) const;
 };

/* class TestFrame */

class TestFrame : public DragWindow
 {
   TestWindow test;

   ClientFromSubWindow client;

  public:

   TestFrame(Desktop *desktop,const UserPreference &pref,Signal<> &update);

   virtual ~TestFrame();
 };

} // namespace App

#endif

