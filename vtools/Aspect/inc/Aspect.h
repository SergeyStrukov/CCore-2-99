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

/* functions */

MPoint MCenter(Pane pane);

/* classes */

class HideControl;

class CountControl;

class InnerDataWindow;

class DataWindow;

class AspectWindow;

/* class HideControl */

class HideControl : public ComboWindow
 {
  public:

   struct Config
    {
     RefVal<Coord> space_dxy = 10 ;

     CtorRefVal<RefLabelWindow::ConfigType> label_cfg;
     CtorRefVal<CheckWindow::ConfigType> check_cfg;
     CtorRefVal<RefButtonWindow::ConfigType> btn_cfg;

     // app

     RefVal<Coord> status_dxy = 30 ;

     RefVal<DefString> text_Hide    = "Hide"_def ;
     RefVal<DefString> text_ShowAll = "Show all"_def ;

     RefVal<VColor> status_New    = SkyBlue ;
     RefVal<VColor> status_Ignore = Gray ;
     RefVal<VColor> status_Red    = Red ;
     RefVal<VColor> status_Yellow = Yellow ;
     RefVal<VColor> status_Green  = Green ;

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

       label_cfg.bind(proxy);
       check_cfg.bind(proxy);
       btn_cfg.bind(proxy);
      }

     template <class Bag>
     void bindApp(const Bag &bag)
      {
       status_dxy.bind(bag.status_dxy);

       text_Hide.bind(bag.text_Hide);
       text_ShowAll.bind(bag.text_ShowAll);

       status_New.bind(bag.status_New);
       status_Ignore.bind(bag.status_Ignore);
       status_Red.bind(bag.status_Red);
       status_Yellow.bind(bag.status_Yellow);
       status_Green.bind(bag.status_Green);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   RefLabelWindow label_Hide;

   CheckWindow check_New;
   CheckWindow check_Ignore;
   CheckWindow check_Red;
   CheckWindow check_Yellow;
   CheckWindow check_Green;

   RefButtonWindow btn_ShowAll;

   Pane place_New;
   Pane place_Ignore;
   Pane place_Red;
   Pane place_Yellow;
   Pane place_Green;

  private:

   static Pane Inner(Pane pane,Coord dxy);

  private:

   void check_changed(bool);

   SignalConnector<HideControl,bool> connector_check_New_changed;
   SignalConnector<HideControl,bool> connector_check_Ignore_changed;
   SignalConnector<HideControl,bool> connector_check_Red_changed;
   SignalConnector<HideControl,bool> connector_check_Yellow_changed;
   SignalConnector<HideControl,bool> connector_check_Green_changed;

   void btn_pressed();

   SignalConnector<HideControl> connector_btn_pressed;

  public:

   HideControl(SubWindowHost &host,const Config &cfg);

   virtual ~HideControl();

   // methods

   Point getMinSize() const;

   bool operator [] (ItemStatus status) const;

   // drawing

   virtual void layout();

   virtual void drawBack(DrawBuf buf,bool drag_active) const;

   // signals

   Signal<> changed;
 };

/* class CountControl */

class CountControl : public ComboWindow
 {
  public:

   struct Config
    {
     CtorRefVal<TextLineWindow::ConfigType> text_cfg;

     // app

     RefVal<Coord> status_dxy = 15 ;

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

       text_cfg.bind(proxy);
      }

     template <class Bag>
     void bindApp(const Bag &bag)
      {
       status_dxy.bind(bag.count_status_dxy);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   TextLineWindow text;

   Pane place_status;
   VColor color;

  public:

   CountControl(SubWindowHost &host,const Config &cfg,VColor color);

   virtual ~CountControl();

   // methods

   Point getMinSize() const;

   void setCount(ulen count);

   // drawing

   virtual void layout();

   virtual void drawBack(DrawBuf buf,bool drag_active) const;
 };

/* class InnerDataWindow */

class InnerDataWindow : public SubWindow
 {
  public:

   struct Config
    {
     // app

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

   AspectData &data;

   bool focus = false ;

   ulen off_x = 0 ;
   ulen off_y = 0 ;

   ulen total_x = 0 ;
   ulen total_y = 0 ;

   ulen page_x = 1 ;
   ulen page_y = 1 ;

  private:

   void setMax();

  private:

   void posX(ulen pos);

   void posY(ulen pos);

   SignalConnector<InnerDataWindow,ulen> connector_posX;
   SignalConnector<InnerDataWindow,ulen> connector_posY;

  public:

   InnerDataWindow(SubWindowHost &host,const Config &cfg,AspectData &data);

   virtual ~InnerDataWindow();

   // special methods

   bool shortDX() const;

   bool shortDY() const;

   template <class W>
   void setScrollXRange(W &window)
    {
     window.setRange(total_x,page_x,off_x);
    }

   template <class W>
   void setScrollYRange(W &window)
    {
     window.setRange(total_x,page_x,off_y);
    }

   void connect(Signal<ulen> &scroll_x,Signal<ulen> &scroll_y)
    {
     connector_posX.connect(scroll_x);
     connector_posY.connect(scroll_y);
    }

   // methods

   Point getMinSize(Point cap=Point::Max()) const;

   void update(bool new_data);

   // drawing

   virtual bool isGoodSize(Point size) const;

   virtual void layout();

   virtual void draw(DrawBuf buf,bool) const;

   // base

   virtual void open();

   // keyboard

   virtual FocusType askFocus() const;

   virtual void gainFocus();

   virtual void looseFocus();

   // mouse

   virtual void looseCapture();

   virtual MouseShape getMouseShape(Point,KeyMod) const;

   // user input

   virtual void react(UserAction action);

   // signals

   Signal<ulen> scroll_x;
   Signal<ulen> scroll_y;
 };

/* class DataWindow */

class DataWindow : public ComboWindow
 {
  public:

   struct Config : InnerDataWindow::Config
    {
     CtorRefVal<XScrollWindow::ConfigType> x_cfg;
     CtorRefVal<YScrollWindow::ConfigType> y_cfg;

     // app

     Config() noexcept {}

     template <class AppPref>
     Config(const UserPreference &pref,const AppPref &app_pref) noexcept
      : InnerDataWindow::Config(pref,app_pref)
      {
       bind(pref.get(),pref.getSmartConfig());
      }

     template <class Bag,class Proxy>
     void bind(const Bag &,Proxy proxy)
      {
       x_cfg.bind(proxy);
       y_cfg.bind(proxy);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   InnerDataWindow inner;
   XScrollWindow scroll_x;
   YScrollWindow scroll_y;

  private:

   void setScroll();

  private:

   SignalConnector<XScrollWindow,ulen> connector_posx;
   SignalConnector<YScrollWindow,ulen> connector_posy;

  public:

   DataWindow(SubWindowHost &host,const Config &cfg,AspectData &data);

   virtual ~DataWindow();

   // methods

   Point getMinSize(Point cap=Point::Max()) const;

   void update(bool new_data);

   // drawing

   virtual void layout();
 };

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
     CtorRefVal<XDoubleLineWindow::ConfigType> line_cfg;

     // app

     RefVal<DefString> text_Path   = "Path"_def ;
     RefVal<DefString> text_Aspect = "Aspect"_def ;

     RefVal<DefString> text_Nothing = "Nothing to save!"_def ;

     HideControl::ConfigType hide_cfg;
     CountControl::ConfigType count_cfg;
     DataWindow::ConfigType data_cfg;

     Config() noexcept {}

     template <class AppPref>
     Config(const UserPreference &pref,const AppPref &app_pref) noexcept
      : hide_cfg(pref,app_pref),
        count_cfg(pref,app_pref),
        data_cfg(pref,app_pref)
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
       line_cfg.bind(proxy);
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

   // subs

   RefLabelWindow label_path;
   RefLabelWindow label_aspect;

   TextLineWindow text_path;
   TextLineWindow text_aspect;

   XDoubleLineWindow line1;

   HideControl hide;

   CountControl count_red;
   CountControl count_yellow;
   CountControl count_green;

   XDoubleLineWindow line2;

   DataWindow data_window;

   // frames

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

   void updateCount();

   void update(bool new_data);

   // drawing

   virtual void layout();

   virtual void drawBack(DrawBuf buf,bool drag_active) const;
 };

} // namespace App

#endif
