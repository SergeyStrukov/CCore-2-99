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

namespace App {

/* classes */

class DisplayWindow;

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

   // methods

   void open(StrLen file_name) { Used(file_name); }
 };

} // namespace App

