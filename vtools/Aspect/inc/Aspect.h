/* Aspect.h */
//----------------------------------------------------------------------------------------
//
//  Project: Aspect 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2017 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef Aspect_h
#define Aspect_h

#include <inc/Application.h>

namespace App {

/* classes */

class AspectWindow;

/* class AspectWindow */

class AspectWindow : public ComboWindow
 {
  public:

   struct Config
    {
     RefVal<VColor> back = Silver ;

     RefVal<Coord> space_dxy = 10 ;

     RefVal<DefString> text_Error = "Error"_def ;

     CtorRefVal<TextLineWindow::ConfigType> text_cfg;
     CtorRefVal<MessageFrame::AlertConfigType> msg_cfg;

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
       back.bind(bag.back);
       space_dxy.bind(bag.space_dxy);
       text_Error.bind(bag.text_Error);

       text_cfg.bind(proxy);
       msg_cfg.bind(proxy);
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

  public:

   AspectWindow(SubWindowHost &host,const Config &cfg);

   virtual ~AspectWindow();

   // methods

   Point getMinSize() const;

   bool isModified() const;

   void blank();

   void load(StrLen file_name);

   bool save();

   void save(StrLen file_name);

   // drawing

   virtual void layout();

   virtual void drawBack(DrawBuf buf,bool drag_active) const;
 };

} // namespace App

#endif
