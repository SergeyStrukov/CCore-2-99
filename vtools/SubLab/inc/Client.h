/* Client.h */
//----------------------------------------------------------------------------------------
//
//  Project: SubLab 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/Application.h>

#include <CCore/inc/video/WindowLib.h>

namespace App {

/* classes */

class SpaceWindow;

class ClientWindow;

/* class SpaceWindow */

class SpaceWindow : public SubWindow
 {
  public:

   struct Config
    {
     RefVal<Coord> space_dxy = 100 ;
     RefVal<Coord> border_dxy = 10 ;

     RefVal<VColor> border = SkyBlue ;

     Config() noexcept {}
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   Point space;

   bool drag = false ;
   Point drag_base;
   Point space_base;

  private:

   void startDrag(Point point);

   void dragTo(Point point);

   void endDrag();

   void endDrag(Point point);

  public:

   SpaceWindow(SubWindowHost &host,const ConfigType &cfg);

   virtual ~SpaceWindow();

   // methods

   Point getMinSize() const { return Null; }

   Pane getInner() const { return getPlace().shrink(space); }

   // drawing

   virtual bool isGoodSize(Point size) const;

   virtual void layout();

   virtual void draw(DrawBuf buf,bool drag_active) const;

   // base

   virtual void open();

   // keyboard

   virtual FocusType askFocus() const;

   // mouse

   virtual void looseCapture();

   virtual MouseShape getMouseShape(Point point,KeyMod kmod) const;

   // user input

   virtual void react(UserAction action);

   void react_LeftClick(Point point,MouseKey);

   void react_LeftUp(Point point,MouseKey);

   void react_LeftDClick(Point point,MouseKey mkey);

   void react_Move(Point point,MouseKey mkey);

   // signals

   Signal<> changed;
 };

/* class ClientWindow */

class ClientWindow : public ComboWindow
 {
  public:

   struct Config
    {
     RefVal<VColor> back = Silver ;

     RefVal<Coord> space_dxy = 10 ;

     CtorRefVal<CheckWindow::ConfigType> check_cfg;
     CtorRefVal<LabelWindow::ConfigType> label_cfg;
     CtorRefVal<ScrollListWindow::ConfigType> list_cfg;

     // special

     RefVal<VColor> backAlt = Wheat ;

     CtorRefVal<SpaceWindow::ConfigType> space_cfg;

     Config() noexcept {}

     Config(const UserPreference &pref)
      : check_cfg(SmartBind,pref),
        label_cfg(SmartBind,pref),
        list_cfg(SmartBind,pref)
      {
       bind(pref.get());
      }

     template <class Bag>
     void bind(const Bag &bag)
      {
       back.bind(bag.back);
       space_dxy.bind(bag.space_dxy);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;
   const UserPreference &pref;

   using SubFactory = SubWindow * (*)(SubWindowHost &host,const UserPreference &pref) ;

   class TypeInfo : public ComboInfo
    {
      class Base;

     public:

      TypeInfo();

      ~TypeInfo();

      // methods

      SubFactory getFactory(ulen index) const;
    };

   TypeInfo info;

   CheckWindow check_wheat;
   LabelWindow label_wheat;
   ScrollListWindow list_type;
   SpaceWindow space;

   ulen cur_index = MaxULen ;
   OwnPtr<SubWindow> cur;

  private:

   VColor getBack() const { return check_wheat.isChecked()? +cfg.backAlt : +cfg.back ; }

   void wheat_changed(bool check);

   SignalConnector<ClientWindow,bool> connector_wheat_changed;

   void type_selected(ulen index);

   SignalConnector<ClientWindow,ulen> connector_type_selected;

   void space_changed();

   SignalConnector<ClientWindow> connector_space_changed;

  public:

   ClientWindow(SubWindowHost &host,const Config &cfg,const UserPreference &pref);

   virtual ~ClientWindow();

   // drawing

   virtual void layout();

   virtual void draw(DrawBuf buf,bool drag_active) const;

   virtual void draw(DrawBuf buf,Pane pane,bool drag_active) const;

   // base

   virtual void open();
 };

} // namespace App


