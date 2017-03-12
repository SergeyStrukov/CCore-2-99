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

#include <inc/AspectData.h>

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

     CtorRefVal<RefLabelWindow::ConfigType> label_cfg;
     CtorRefVal<TextLineWindow::ConfigType> text_cfg;
     CtorRefVal<MessageFrame::AlertConfigType> msg_cfg;

     // app

     RefVal<DefString> text_Path   = "Path"_def ;
     RefVal<DefString> text_Aspect = "Aspect"_def ;

     RefVal<DefString> text_Nothing = "Nothing to save!"_def ;

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

       label_cfg.bind(proxy);
       text_cfg.bind(proxy);
       msg_cfg.bind(proxy);
      }

     template <class Bag>
     void bindApp(const Bag &bag)
      {
       text_Path.bind(bag.text_Path);
       text_Aspect.bind(bag.text_Aspect);
       text_Nothing.bind(bag.text_Nothing);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   AspectData data;

   String aspect_file_name;
   bool has_file = false ;

   RefLabelWindow label_path;
   RefLabelWindow label_aspect;

   TextLineWindow text_path;
   TextLineWindow text_aspect;

   MessageFrame msg_frame;

  private:

   void setModified() { text_aspect.alert(true); }

   void clearModified() { text_aspect.alert(false); }

   void setAspect(StrLen file_name);

   void clearAspect();

  private:

   void errorMsg(StrLen text);

   void errorMsg(const DefString &text) { errorMsg(text.str()); }

   void msg_destroyed();

   SignalConnector<AspectWindow> connector_msg_destroyed;

  public:

   AspectWindow(SubWindowHost &host,const Config &cfg);

   virtual ~AspectWindow();

   // methods

   Point getMinSize() const;

   bool isModified() const { return text_aspect.isAlerted(); }

   void blank(StrLen path);

   void load(StrLen file_name);

   bool save();

   void save(StrLen file_name);

   // drawing

   virtual void layout();

   virtual void drawBack(DrawBuf buf,bool drag_active) const;
 };

} // namespace App

#endif
