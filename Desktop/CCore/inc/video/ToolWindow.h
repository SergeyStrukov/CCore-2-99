/* ToolWindow.h */
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

#ifndef CCore_inc_video_ToolWindow_h
#define CCore_inc_video_ToolWindow_h

#include <CCore/inc/video/SubWindow.h>
#include <CCore/inc/video/FrameGuards.h>
#include <CCore/inc/video/RefVal.h>

#include <CCore/inc/DeferCall.h>

namespace CCore {
namespace Video {

/* classes */

class ToolWindow;

/* class ToolWindow */

class ToolWindow : public FrameWindow , public SubWindowHost
 {
  public:

   struct Config
    {
     RefVal<VColor> shade_color = Violet ;
     RefVal<Clr>    shade_alpha =     64 ;

     Config() noexcept {}
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   SubWindow *client = 0 ;

   Point size;

   bool has_focus = false ;
   bool client_enter = false ;
   bool client_capture = false ;
   bool delay_draw = false ;
   bool enable_react = true ;

  private:

   void guardClient();

   void guardDead();

   void reset();

   SubWindow & getClient();

   void shade(FrameBuf<DesktopColor> &buf);

   void redraw(FuncArgType<FrameBuf<DesktopColor> &> func);

  public:

   ToolWindow(Desktop *desktop,const Config &cfg);

   virtual ~ToolWindow();

   // methods

   void bindClient(SubWindow &client_);

   void createMain(Pane pane,const DefString &title);

   void create(Pane pane);

   void create(FrameWindow *parent,Pane pane);

   void destroy();

   void redrawAll(bool do_layout=false);

   void redrawClient();

   void redrawClient(Pane pane);

   unsigned getToken() { return host->getToken(); }

   // SubWindowHost

   virtual FrameWindow * getFrame();

   virtual Point getScreenOrigin();

   virtual void redraw(Pane pane);

   virtual void setFocus(SubWindow *);

   virtual void captureMouse(SubWindow *);

   virtual void releaseMouse(SubWindow *);

   // base

   virtual void alive();

   virtual void dead();

   virtual void askClose();

   virtual void setSize(Point size_,bool buf_dirty);

   virtual void paintDone(unsigned);

   // keyboard

   virtual void gainFocus();

   virtual void looseFocus();

   // mouse

   virtual void looseCapture();

   virtual void setMouseShape(Point point,KeyMod kmod);

   // user input

   virtual void disableReact();

   virtual void enableReact();

   virtual void react(UserAction action);

   void react_other(UserAction action);

   void react_Move(Point point,MouseKey mkey);

   void react_Leave();

   // DeferInput

   class Input : DeferInput<ToolWindow>
    {
      friend class ToolWindow;

      explicit Input(ToolWindow *window) : DeferInput<ToolWindow>(window) {}

      ~Input() {}

      using DeferInput<ToolWindow>::try_post;

     public:

      void redrawAll(bool do_layout=false) { try_post(&ToolWindow::redrawAll,do_layout); }

      void redrawClient() { try_post(&ToolWindow::redrawClient); }

      void redrawClient(Pane pane) { try_post(&ToolWindow::redrawClient,pane); }
    };

   Input input;
 };

} // namespace Video
} // namespace CCore

#endif

