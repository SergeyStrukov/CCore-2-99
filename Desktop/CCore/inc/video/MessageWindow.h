/* MessageWindow.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 3.00
//
//  Tag: Desktop
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_video_MessageWindow_h
#define CCore_inc_video_MessageWindow_h

#include <CCore/inc/video/FixedWindow.h>
#include <CCore/inc/video/WindowLib.h>
#include <CCore/inc/video/ButtonId.h>

#include <CCore/inc/OwnPtr.h>
#include <CCore/inc/Array.h>

namespace CCore {
namespace Video {

/* classes */

class MessageSubWindow;

class MessageWindow;

/* class MessageSubWindow */

class MessageSubWindow : public ComboWindow
 {
  public:

   struct Config
    {
     RefVal<Coord> space_dxy = 10 ;

     RefVal<VColor> back = Silver ;

     CtorRefVal<InfoWindow::ConfigType> info_cfg;
     CtorRefVal<KnobWindow::ConfigType> knob_cfg;
     CtorRefVal<ButtonWindow::ConfigType> btn_cfg;
     CtorRefVal<XDoubleLineWindow::ConfigType> dline_cfg;

     RefVal<Coord> knob_dxy  = 50 ;

     Config() noexcept {}

     template <class Bag,class Proxy>
     void bind(const Bag &bag,Proxy proxy)
      {
       space_dxy.bind(bag.space_dxy);

       back.bind(bag.back);

       info_cfg.bind(proxy);
       knob_cfg.bind(proxy);
       btn_cfg.bind(proxy);
       dline_cfg.bind(proxy);

       knob_dxy.bind(bag.message_knob_dxy);
      }
    };

   using ConfigType = Config ;

  private:

   class Btn : public ButtonWindow
    {
      int btn_id;

      MessageSubWindow *owner;

     private:

      void finish();

      SignalConnector<Btn> connector_pressed;

     public:

      Btn(SubWindowHost &host,const ButtonWindow::ConfigType &cfg,const DefString &name,int btn_id,MessageSubWindow *owner);

      virtual ~Btn();
    };

  private:

   const Config &cfg;
   const ButtonWindow::ConfigType &btn_cfg;

   InfoWindow info;

   XDoubleLineWindow dline;

   KnobWindow knob;

   DynArray<OwnPtr<Btn> > btn_list;

   ulen btn_count = 0 ; // <= btn_list.getLen()

  private:

   static Point BtnSize(AnyType list)
    {
     Point ret;

     for(const OwnPtr<Btn> &obj : list ) ret=Sup(ret,obj->getMinSize());

     return ret;
    }

   void knob_pressed();

   SignalConnector<MessageSubWindow> connector_knob_pressed;

  public:

   MessageSubWindow(SubWindowHost &host,const Config &cfg);

   virtual ~MessageSubWindow();

   // methods

   Point getMinSize() const;

   MessageSubWindow & setInfo(const Info &info);

   MessageSubWindow & setInfo(StrLen str) { return setInfo(InfoFromString(str)); }

   MessageSubWindow & add(const DefString &name,int btn_id);

   // drawing

   virtual bool isGoodSize(Point size) const;

   virtual void layout();

   virtual void draw(DrawBuf buf,bool drag_active) const;

   virtual void draw(DrawBuf buf,Pane pane,bool drag_active) const;

   // base

   virtual void open();

   // signals

   Signal<int> finish; // btn_id
 };

/* class MessageWindow */

class MessageWindow : public FixedWindow
 {
  public:

   struct Config
    {
     CtorRefVal<FixedWindow::ConfigType> frame_cfg;
     CtorRefVal<MessageSubWindow::ConfigType> msg_cfg;

     RefVal<Ratio> pos_ry = Div(5,12) ;

     Config() noexcept {}

     template <class Bag,class Proxy>
     void bind(const Bag &bag,Proxy proxy)
      {
       frame_cfg.bind(proxy);
       msg_cfg.bind(proxy);

       pos_ry.bind(bag.message_pos_ry);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   MessageSubWindow sub_win;

   int btn_id = Button_Cancel ;

   SignalConnector<MessageWindow,int> connector_finish;

  private:

   void finish(int btn_id);

  public:

   MessageWindow(Desktop *desktop,const Config &cfg);

   MessageWindow(Desktop *desktop,const Config &cfg,Signal<> &update);

   virtual ~MessageWindow();

   // methods

   MessageWindow & setInfo(const Info &info) { sub_win.setInfo(info); return *this; }

   MessageWindow & setInfo(StrLen str) { return setInfo(InfoFromString(str)); }

   MessageWindow & add(const DefString &name,int btn_id) { sub_win.add(name,btn_id); return *this; }

   int getButtonId() const { return btn_id; } // available after the signal "destroyed"

   // base

   virtual void alive();

   // create

   Pane getPane(StrLen title) const;

   using FixedWindow::createMain;
   using FixedWindow::create;

   void createMain(const DefString &title) { createMain(getPane(title.str()),title); }

   void create(const DefString &title) { create(getPane(title.str()),title); }

   void create(FrameWindow *parent,const DefString &title) { create(parent,getPane(title.str()),title); }
 };

} // namespace Video
} // namespace CCore

#endif


