/* Editor.h */
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

#ifndef Editor_h
#define Editor_h

#include <inc/Contour.h>

#include <CCore/inc/video/WindowLib.h>

namespace App {

/* classes */

class EditorWindow;

/* class EditorWindow */

class EditorWindow : public ComboWindow
 {
  public:

   struct Config
    {
     RefVal<VColor> back;

     Config() noexcept {}

     Config(const UserPreference &pref) noexcept
      {
       bind(pref.get(),pref.getSmartConfig());
      }

     template <class Bag,class Proxy>
     void bind(const Bag &bag,Proxy proxy)
      {
       Used(proxy);

       back.bind(bag.back);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   bool modified = false ;

  public:

   EditorWindow(SubWindowHost &host,const Config &cfg);

   virtual ~EditorWindow();

   // methods

   bool isModified() const { return modified; }

   void load();

   void load(StrLen file_name);

   bool save();

   void save(StrLen file_name);

   void close() { load(); }

   // drawing

   virtual void layout();

   virtual void drawBack(DrawBuf buf,bool drag_active) const;
 };

} // namespace App

#endif
