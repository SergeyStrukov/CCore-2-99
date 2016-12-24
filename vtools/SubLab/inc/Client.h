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

class ClientWindow;

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
     RefVal<Coord> spaceInner_dxy = 100 ;

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

   ulen cur_index = MaxULen ;
   OwnPtr<SubWindow> cur;

  private:

   VColor getBack() const { return check_wheat.isChecked()? +cfg.backAlt : +cfg.back ; }

   void wheat_changed(bool check);

   SignalConnector<ClientWindow,bool> connector_wheat_changed;

   void type_selected(ulen index);

   SignalConnector<ClientWindow,ulen> connector_type_selected;

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


