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
   SubWindow *client = 0 ;

   Point size;

   bool has_focus = false ;
   bool client_enter = false ;
   bool client_capture = false ;
   bool delay_draw = false ;
   bool enable_react = true ;

  private:

   void guardClient()
    {
     if( !client ) GuardNoClient();
    }

   void guardDead()
    {
     if( isAlive() ) GuardNotDead();
    }

   void reset()
    {
     size=Null;
     client_enter=false;
     client_capture=false;
     delay_draw=false;
     enable_react=true;
    }

   SubWindow & getClient()
    {
     guardClient();

     return *client;
    }

   void shade(FrameBuf<DesktopColor> &buf)
    {
     //if( !enable_react ) buf.erase(+shape.cfg.shade_color,+shape.cfg.shade_alpha);
    }

   void redraw(FuncArgType<FrameBuf<DesktopColor> &> func)
    {
     if( host->isDead() ) return;

     if( host->getToken() )
       {
        delay_draw=true;

        return;
       }

     FrameBuf<DesktopColor> buf(host->getDrawPlane());

     if( size<=buf.getSize() )
       {
        func(buf);
       }
     else
       {
        buf.erase(Black);

        host->invalidate(1);
       }
    }

  public:

   explicit ToolWindow(Desktop *desktop)
    : FrameWindow(desktop),
      input(this)
    {
    }

   virtual ~ToolWindow()
    {
    }

   // methods

   void bindClient(SubWindow &client_)
    {
     guardDead();

     client=&client_;
    }

   void createMain(Pane pane,const DefString &title)
    {
     guardClient();

     has_focus=false;

     host->createMain(pane,pane.getSize());
     host->setTitle(title.str());
     host->display(CmdDisplay_Normal);
    }

   void create(Pane pane)
    {
     guardClient();

     has_focus=false;

     host->create(pane,pane.getSize());
     host->show();
    }

   void create(FrameWindow *parent,Pane pane)
    {
     guardClient();

     has_focus=false;

     host->create(parent->getHost(),pane,pane.getSize());
     host->show();
    }

   void destroy()
    {
     host->destroy();

     destroyed.assert();
    }

   void redrawAll(bool do_layout=false)
    {
     if( do_layout )
       {
        if( client ) client->setPlace(Pane(Null,size));
       }

     redraw( [this] (FrameBuf<DesktopColor> &buf)
                    {
                     getClient().forward_draw(buf,false);

                     shade(buf);

                     host->invalidate(1);

                    } );
    }

   void redrawClient()
    {
     redraw( [this] (FrameBuf<DesktopColor> &buf)
                    {
                     getClient().forward_draw(buf,false);

                     shade(buf);

                     host->invalidate(Pane(Null,size),1);

                    } );
    }

   void redrawClient(Pane pane)
    {
     redraw( [this,pane] (FrameBuf<DesktopColor> &buf)
                         {
                          getClient().forward_draw(buf,pane,false);

                          shade(buf);

                          host->invalidate(pane,1);

                         } );
    }

   unsigned getToken() { return host->getToken(); }

   // SubWindowHost

   virtual FrameWindow * getFrame()
    {
     return this;
    }

   virtual Point getScreenOrigin()
    {
     Pane pane=host->getPlace();

     return pane.getBase();
    }

   virtual void redraw(Pane pane)
    {
     input.redrawClient(pane);
    }

   virtual void setFocus(SubWindow *)
    {
     if( has_focus )
       {
        getClient().gainFocus();
       }
    }

   virtual void captureMouse(SubWindow *)
    {
     if( !client_capture )
       {
        client_capture=true;

        host->captureMouse();
       }
    }

   virtual void releaseMouse(SubWindow *)
    {
     if( client_capture )
       {
        client_capture=false;

        host->releaseMouse();
       }
    }

   // base

   virtual void alive()
    {
     reset();

     host->trackMouseHover();
     host->trackMouseLeave();

     getClient().open();
    }

   virtual void dead()
    {
     getClient().close();
    }

   virtual void askClose()
    {
     destroy();
    }

   virtual void setSize(Point size_,bool buf_dirty)
    {
     if( size!=size_ || buf_dirty )
       {
        size=size_;

        redrawAll(true);
       }
    }

   virtual void paintDone(unsigned)
    {
     if( delay_draw )
       {
        delay_draw=false;

        redrawAll();
       }
    }

   // keyboard

   virtual void gainFocus()
    {
     has_focus=true;

     getClient().gainFocus();
    }

   virtual void looseFocus()
    {
     has_focus=false;

     getClient().looseFocus();
    }

   // mouse

   virtual void looseCapture()
    {
     if( client_capture )
       {
        client_capture=false;

        getClient().looseCapture();
       }
    }

   virtual void setMouseShape(Point point,KeyMod kmod)
    {
     host->setMouseShape(getClient().forward_getMouseShape(point,kmod));
    }

   // user input

   virtual void disableReact() { enable_react=false; redrawAll(); }

   virtual void enableReact() { enable_react=true; redrawAll(); }

   virtual void react(UserAction action)
    {
     if( enable_react ) action.dispatch(*this);
    }

   void react_other(UserAction action)
    {
     if( action.fromKeyboard() )
       {
        getClient().react(action);
       }
     else
       {
        Point point=action.getPoint();

        if( client_capture || Pane(Null,size).contains(point) )
          {
           getClient().forward_react(action);
          }
       }
    }

   void react_Move(Point point,MouseKey mkey)
    {
     if( Pane(Null,size).contains(point) )
       {
        client_enter=true;

        getClient().forward().put_Move(point,mkey);
       }
     else
       {
        if( client_capture ) getClient().forward().put_Move(point,mkey);

        if( client_enter )
          {
           client_enter=false;

           getClient().put_Leave();
          }
       }
    }

   void react_Leave()
    {
     if( client_enter )
       {
        client_enter=false;

        getClient().put_Leave();
       }
    }

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

