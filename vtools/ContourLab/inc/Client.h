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

#include <inc/Application.h>

#include <CCore/inc/video/WindowLib.h>
#include <CCore/inc/video/Menu.h>
#include <CCore/inc/video/MessageFrame.h>
#include <CCore/inc/video/FileFrame.h>

#include <CCore/inc/Array.h>

namespace App {

/* classes */

class ClientWindow;

/* class ClientWindow */

class ClientWindow : public ComboWindow
 {
  public:

   struct Config
    {
     Config() noexcept {}

     Config(const UserPreference &pref) noexcept
      {
       bind(pref.get(),pref.getSmartConfig());
      }

     template <class Bag,class Proxy>
     void bind(const Bag &bag,Proxy proxy)
      {
       Used(bag);
       Used(proxy);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

  public:

   ClientWindow(SubWindowHost &host,const Config &cfg);

   virtual ~ClientWindow();
 };

} // namespace App


