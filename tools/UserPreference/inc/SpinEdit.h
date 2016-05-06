/* SpinEdit.h */
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

#ifndef SpinEdit_h
#define SpinEdit_h

#include <inc/Application.h>

#include <CCore/inc/video/WindowLib.h>

namespace App {

/* classes */

class SpinEditWindow;

/* class SpinEditWindow */

class SpinEditWindow : public ComboWindow
 {
  public:

   struct Config
    {
     CtorRefVal<KnobWindow::ConfigType> knob_cfg;
     CtorRefVal<TextWindow::ConfigType> text_cfg;

     Config()
      {
      }

     explicit Config(const UserPreference &pref)
      {
       knob_cfg.bind(pref.getKnobWindowConfig());
       text_cfg.bind(pref.getTextWindowConfig());
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   int value = 0 ;
   int min_value = 0 ;
   int max_value = 0 ;

   KnobWindow plus;
   KnobWindow minus;
   TextWindow text;

  private:

   void showVal();

   void plusVal();

   void minusVal();

   SignalConnector<SpinEditWindow> connector_plus_pressed;
   SignalConnector<SpinEditWindow> connector_minus_pressed;

   Point getTextSize(int value) const;

  public:

   SpinEditWindow(SubWindowHost &host,const ConfigType &cfg);

   virtual ~SpinEditWindow();

   // methods

   Point getMinSize() const;

   bool isEnabled() const { return plus.isEnabled(); }

   void enable(bool enable=true)
    {
     plus.enable(enable);
     minus.enable(enable);
     text.enable(enable);
    }

   void disable() { enable(false); }

   int getValue() const { return value; }

   void setValue(int value);

   void setValue(int value,int min_value,int max_value);

   // drawing

   virtual void layout();

   // keyboard

   virtual FocusType askFocus() const;

   // user input

   virtual void react(UserAction action);

   void react_Char(char ch);

   // signals

   Signal<int> changed; // value
 };

} // namespace App

#endif
