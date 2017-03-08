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

class AspectWindow : public SubWindow
 {
  public:

   struct Config
    {
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
       Used(bag);
       Used(proxy);
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

   void load();

   void load(StrLen file_name);

   bool save();

   void save(StrLen file_name);
 };

} // namespace App

#endif
