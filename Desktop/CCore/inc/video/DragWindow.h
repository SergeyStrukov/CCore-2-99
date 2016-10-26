/* DragWindow.h */
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

#ifndef CCore_inc_video_DragWindow_h
#define CCore_inc_video_DragWindow_h

#include <CCore/inc/video/SubWindow.h>
#include <CCore/inc/video/Font.h>
#include <CCore/inc/video/FrameGuards.h>
#include <CCore/inc/video/RefVal.h>

#include <CCore/inc/String.h>
#include <CCore/inc/DeferCall.h>

namespace CCore {
namespace Video {

/* consts */

enum DragType
 {
  DragType_None = 0,

  DragType_Top,
  DragType_TopLeft,
  DragType_Left,
  DragType_BottomLeft,
  DragType_Bottom,
  DragType_BottomRight,
  DragType_Right,
  DragType_TopRight,

  DragType_Bar,

  DragType_Alert,
  DragType_Min,
  DragType_Max,
  DragType_Close
 };

enum AlertType
 {
  AlertType_No = 0,
  AlertType_Closed,
  AlertType_Opened
 };

/* DragPane() */

void DragPane(Pane &place,Point delta,DragType drag_type);

/* classes */

class DragShape;

template <class Shape> class DragWindowOf;

/* class DragShape */

class DragShape
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<Coord> frame_dxy = 12 ;
     RefVal<Coord> title_dy  = 32 ;

     RefVal<Coord> btn_dx    = 26 ;
     RefVal<Coord> btn_dy    = 24 ;

     RefVal<VColor> top               =      Gray ;
     RefVal<VColor> bottom            =      Snow ;
     RefVal<VColor> frame             =      Snow ;

     RefVal<VColor> drag              =    Silver ;
     RefVal<VColor> dragHilight       =     Green ;
     RefVal<VColor> dragActive        =       Red ;

     RefVal<VColor> active            = RoyalBlue ;
     RefVal<VColor> inactive          =    Silver ;
     RefVal<VColor> title             =     Black ;

     RefVal<VColor> btnFace           = SteelBlue ;
     RefVal<VColor> btnFaceHilight    =     Green ;
     RefVal<VColor> btnPict           =     White ;
     RefVal<VColor> btnPictClose      =       Red ;
     RefVal<VColor> btnPictAlert      =       Red ;
     RefVal<VColor> btnPictNoAlert    =      Gray ;
     RefVal<VColor> btnPictCloseAlert =    Orange ;

     RefVal<VColor> shade_color        =    Violet ;
     RefVal<Clr>    shade_alpha        =        64 ;

     RefVal<Font> title_font;

     RefVal<unsigned> blink_time   = 3_sectick ;
     RefVal<unsigned> blink_period =    3_tick ;

     RefVal<DefString> fatal_error = "Fatal error"_def ;

     Config() noexcept {}
    };

   const Config &cfg;

  private:

   Point size;

   Pane dragTopLeft;
   Pane dragLeft;
   Pane dragBottomLeft;
   Pane dragBottom;
   Pane dragBottomRight;
   Pane dragRight;
   Pane dragTopRight;
   Pane dragBar;

   Pane titleBar;
   Pane btnAlert;
   Pane btnMin;
   Pane btnMax;
   Pane btnClose;

   Pane client;

  private:

   class DrawArt;

   VColor dragColor(DragType zone) const;

   void draw_Frame(DrawArt &art) const;

   void draw_TopLeft(DrawArt &art) const;

   void draw_Left(DrawArt &art) const;

   void draw_BottomLeft(DrawArt &art) const;

   void draw_Bottom(DrawArt &art) const;

   void draw_BottomRight(DrawArt &art) const;

   void draw_Right(DrawArt &art) const;

   void draw_TopRight(DrawArt &art) const;

   void draw_Bar(DrawArt &art) const;

   void draw_Alert(DrawArt &art) const;

   void draw_Min(DrawArt &art) const;

   void draw_Max(DrawArt &art) const;

   void draw_Close(DrawArt &art) const;

  public:

   // state

   bool has_focus = false ;
   bool max_button = true ;
   bool is_main = true ;

   DragType drag_type = DragType_None ;
   DragType hilight = DragType_None ;
   DragType btn_type = DragType_None ;
   AlertType alert_type = AlertType_No ;
   bool alert_blink = false ;

   DefString title;

   // methods

   explicit DragShape(const Config &cfg_) : cfg(cfg_) {}

   Point getDeltaSize() const;

   Coord getMinDx(bool is_main,StrLen title) const;

   Pane getClient() const { return client; }

   void reset(const DefString &title,bool is_main,bool max_button);

   void layout(Point size);

   void draw(const DrawBuf &buf) const;

   void draw(const DrawBuf &buf,DragType drag_type) const;

   Pane getPane(DragType drag_type) const;

   DragType dragTest(Point point) const;
 };

/* class DragWindowOf<Shape> */

template <class Shape>
class DragWindowOf : public FrameWindow , public SubWindowHost
 {
  public:

   using ShapeType = Shape ;

   using ConfigType = typename Shape::Config ;

  private:

   Shape shape;

   SubWindow *client = 0 ;
   SubWindow *alert_client = 0 ;

   AliveControl *client_ac = &AliveControl::Default ;
   AliveControl *alert_client_ac = &AliveControl::Default ;

   Point size;

   Point drag_from;
   bool client_enter = false ;
   bool client_capture = false ;
   bool delay_draw = false ;
   bool enable_react = true ;

   unsigned tick_count = 0 ;

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
     tick_count=0;
    }

   void assert_moved(Point from,Point to)
    {
     if( from!=to ) moved.assert(to-from);
    }

   void replace(Pane place,Point delta,DragType drag_type)
    {
     Point base=place.getBase();

     DragPane(place,delta,drag_type);

     Point new_size(place.dx,place.dy);

     if( new_size>Null && new_size<=host->getMaxSize() )
       {
        Pane screen=Pane(Null,desktop->getScreenSize());

        if( +Inf(place,screen) )
          {
           if( !shape.max_button )
             {
              shape.max_button=true;

              redrawFrame(DragType_Max);
             }

           host->move(place);

           host->invalidate(1);

           assert_moved(base,place.getBase());
          }
       }
    }

   void replace(Point delta,DragType drag_type)
    {
     Pane place=host->getPlace();

     replace(place,delta,drag_type);
    }

   void startDrag(Point point,DragType drag_type)
    {
     shape.drag_type=drag_type;

     if( !client_capture ) host->captureMouse();

     Pane place=host->getPlace();

     drag_from=point+place.getBase();

     redrawAll();
    }

   void dragTo(Point point)
    {
     Pane place=host->getPlace();
     Point delta=Diff(drag_from,point+place.getBase());

     replace(place,delta,shape.drag_type);
    }

   void endDrag()
    {
     shape.drag_type=DragType_None;

     redrawAll();
    }

   void endDrag(Point point)
    {
     dragTo(point);

     shape.drag_type=DragType_None;

     if( !client_capture ) host->releaseMouse();

     redrawAll();
    }

   void btnDown(DragType drag_type)
    {
     shape.btn_type=drag_type;

     redrawFrame(drag_type);
    }

   void btnUp(Point point)
    {
     auto type=Replace(shape.btn_type,DragType_None);

     redrawFrame(type);

     if( shape.dragTest(point)==type )
       {
        switch( type )
          {
           case DragType_Alert : switchClients(); break;

           case DragType_Min   : minimize(); break;

           case DragType_Max   : maximize(); break;

           case DragType_Close : destroy(); break;
          }
       }
    }

   void moveBtn(Point point)
    {
     if( shape.dragTest(point)!=shape.btn_type ) endBtn();
    }

   void endBtn()
    {
     redrawFrame(Replace(shape.btn_type,DragType_None));
    }

   void hilightFrame(Point point)
    {
     auto drag_type=shape.dragTest(point);

     if( drag_type==DragType_Bar ) drag_type=DragType_None;

     if( drag_type!=shape.hilight )
       {
        auto type=Replace(shape.hilight,drag_type);

        redrawFrame(type);
        redrawFrame(drag_type);
       }
    }

   void endHilightFrame()
    {
     if( shape.hilight )
       {
        redrawFrame(Replace(shape.hilight,DragType_None));
       }
    }

   bool forwardKey(VKey vkey,KeyMod kmod,unsigned repeat=1)
    {
     if( kmod&KeyMod_Alt )
       {
        switch( vkey )
          {
           case VKey_Left  : replace(Point(-(Coord)repeat,0),(kmod&KeyMod_Shift)?DragType_Right:DragType_Bar); return true;

           case VKey_Right : replace(Point((Coord)repeat,0),(kmod&KeyMod_Shift)?DragType_Right:DragType_Bar); return true;

           case VKey_Up    : replace(Point(0,-(Coord)repeat),(kmod&KeyMod_Shift)?DragType_Bottom:DragType_Bar); return true;

           case VKey_Down  : replace(Point(0,(Coord)repeat),(kmod&KeyMod_Shift)?DragType_Bottom:DragType_Bar); return true;

           case VKey_F2    : minimize(); return true;

           case VKey_F3    : maximize(); return true;

           case VKey_F4    : destroy(); return true;

           case VKey_F12   : switchClients(); return true;

           default: return false;
          }
       }
     else
       {
        return false;
       }
    }

   bool forwardKeyUp(VKey vkey,KeyMod kmod,unsigned =1)
    {
     if( kmod&KeyMod_Alt )
       {
        switch( vkey )
          {
           case VKey_Left  :
           case VKey_Right :
           case VKey_Up    :
           case VKey_Down  :
           case VKey_F2    :
           case VKey_F3    :
           case VKey_F4    :
           case VKey_F12   : return true;

           default: return false;
          }
       }
     else
       {
        return false;
       }
    }

   SubWindow & getClient()
    {
     if( alert_client && shape.alert_type==AlertType_Opened ) return *alert_client;

     guardClient();

     return *client;
    }

   void shade(FrameBuf<DesktopColor> &buf)
    {
     if( !enable_react ) buf.erase(+shape.cfg.shade_color,+shape.cfg.shade_alpha);
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

   void redrawFrame()
    {
     redraw( [this] (FrameBuf<DesktopColor> &buf)
                    {
                     try { shape.draw(buf); } catch(CatchType) {}

                     shade(buf);

                     host->invalidate(1);

                    } );
    }

   void redrawFrame(DragType drag_type)
    {
     Pane pane=shape.getPane(drag_type);

     if( !pane ) return;

     redraw( [this,drag_type,pane] (FrameBuf<DesktopColor> &buf)
                                   {
                                    try { shape.draw(buf,drag_type); } catch(CatchType) {}

                                    shade(buf);

                                    host->invalidate(pane,1);

                                   } );
    }

   void pushAlertBlink()
    {
     if( !tick_count )
       {
        tick_count=+shape.cfg.blink_time;

        defer_tick.start();
       }
     else
       {
        tick_count=+shape.cfg.blink_time;
       }
    }

   void tick()
    {
     if( tick_count )
       {
        if( !( tick_count % +shape.cfg.blink_period ) )
          {
           shape.alert_blink=!shape.alert_blink;

           redrawFrame(DragType_Alert);
          }

        tick_count--;
       }
     else
       {
        defer_tick.stop();

        shape.alert_blink=false;

        redrawFrame(DragType_Alert);
       }
    }

   void switchClients()
    {
     if( shape.alert_type && alert_client )
       {
        getClient().close();

        shape.alert_type=(shape.alert_type==AlertType_Closed)?AlertType_Opened:AlertType_Closed;

        getClient().open();

        redrawAll();
       }
    }

  public:

   DragWindowOf(Desktop *desktop,const ConfigType &cfg)
    : FrameWindow(desktop),
      shape(cfg),
      input(this),
      connector_updateConfig(this,&DragWindowOf<Shape>::updateConfig),
      connector_alert(this,&DragWindowOf<Shape>::alert)
    {
     defer_tick=input.create(&DragWindowOf<Shape>::tick);
    }

   DragWindowOf(Desktop *desktop,const ConfigType &cfg,Signal<> &update)
    : DragWindowOf(desktop,cfg)
    {
     connector_updateConfig.connect(update);
    }

   virtual ~DragWindowOf()
    {
    }

   // methods

   Point getDeltaSize() const { return shape.getDeltaSize(); }

   Point getMinSize(bool is_main,StrLen title,Point size) const
    {
     size+=shape.getDeltaSize();

     return Point(Max(size.x,shape.getMinDx(is_main,title)),size.y);
    }

   void bindUpdate(Signal<> &update)
    {
     connector_updateConfig.disconnect();
     connector_updateConfig.connect(update);
    }

   void bindClient(SubWindow &client_)
    {
     guardDead();

     client=&client_;
     client_ac=client_.getAliveControl();
    }

   void bindAlertClient(SubWindow &alert_client_)
    {
     guardDead();

     alert_client=&alert_client_;
     alert_client_ac=alert_client_.getAliveControl();
    }

   void createMain(CmdDisplay cmd_display,Point max_size,const DefString &title)
    {
     guardClient();

     shape.reset(title,true, cmd_display!=CmdDisplay_Maximized );

     host->createMain(max_size);
     host->setTitle(title.str());
     host->display(cmd_display);
    }

   void createMain(CmdDisplay cmd_display,Pane pane,Point max_size,const DefString &title)
    {
     guardClient();

     shape.reset(title,true, cmd_display!=CmdDisplay_Maximized );

     host->createMain(pane,max_size);
     host->setTitle(title.str());
     host->display(cmd_display);
    }

   void createMain(CmdDisplay cmd_display,const DefString &title)
    {
     Point max_size=getDesktop()->getScreenSize();

     createMain(cmd_display,max_size,title);
    }

   void createMain(CmdDisplay cmd_display,Pane pane,const DefString &title)
    {
     Point max_size=getDesktop()->getScreenSize();

     createMain(cmd_display,pane,max_size,title);
    }

   void create(Pane pane,Point max_size,const DefString &title)
    {
     guardClient();

     shape.reset(title,false,true);

     host->create(pane,max_size);
     host->show();
    }

   void create(FrameWindow *parent,Pane pane,Point max_size,const DefString &title)
    {
     guardClient();

     shape.reset(title,false,true);

     host->create(parent->getHost(),pane,max_size);
     host->show();
    }

   void create(Pane pane,const DefString &title)
    {
     Point max_size=getDesktop()->getScreenSize();

     create(pane,max_size,title);
    }

   void create(FrameWindow *parent,Pane pane,const DefString &title)
    {
     Point max_size=getDesktop()->getScreenSize();

     create(parent,pane,max_size,title);
    }

   void destroy()
    {
     if( client_ac->askDestroy() )
       {
        host->destroy();

        destroyed.assert();
       }
    }

   void minimize()
    {
     if( shape.is_main ) host->display(CmdDisplay_Minimized);
    }

   void maximize()
    {
     if( shape.max_button )
       {
        shape.max_button=false;

        redrawFrame(DragType_Max);

        host->display(CmdDisplay_Maximized);
       }
     else
       {
        shape.max_button=true;

        redrawFrame(DragType_Max);

        host->display(CmdDisplay_Restore);
       }
    }

   void redrawAll(bool do_layout=false)
    {
     if( do_layout )
       {
        shape.layout(size);

        Pane place=shape.getClient();

        if( client ) client->setPlace(place);

        if( alert_client ) alert_client->setPlace(place);
       }

     redraw( [this] (FrameBuf<DesktopColor> &buf)
                    {
                     try { shape.draw(buf); } catch(CatchType) {}

                     getClient().forward_draw(buf,shape.drag_type);

                     shade(buf);

                     host->invalidate(1);

                    } );
    }

   void redrawClient()
    {
     redraw( [this] (FrameBuf<DesktopColor> &buf)
                    {
                     getClient().forward_draw(buf,shape.drag_type);

                     shade(buf);

                     host->invalidate(shape.getClient(),1);

                    } );
    }

   void redrawClient(Pane pane)
    {
     redraw( [this,pane] (FrameBuf<DesktopColor> &buf)
                         {
                          getClient().forward_draw(buf,pane,shape.drag_type);

                          shade(buf);

                          host->invalidate(pane,1);

                         } );
    }

   unsigned getToken() { return host->getToken(); }

   void setTitle(const DefString &title)
    {
     shape.title=title;

     host->setTitle(title.str());

     redrawFrame();
    }

   void connectAlert(Signal<> &signal) { connector_alert.connect(signal); }

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
     if( shape.has_focus )
       {
        getClient().gainFocus();
       }
    }

   virtual void captureMouse(SubWindow *)
    {
     if( !client_capture )
       {
        client_capture=true;

        if( !shape.drag_type ) host->captureMouse();
       }
    }

   virtual void releaseMouse(SubWindow *)
    {
     if( client_capture )
       {
        client_capture=false;

        if( !shape.drag_type ) host->releaseMouse();
       }
    }

   // base

   virtual void alive()
    {
     reset();

     host->trackMouseHover();
     host->trackMouseLeave();

     client_ac->alive();
     alert_client_ac->alive();

     getClient().open();
    }

   virtual void dead()
    {
     getClient().close();

     client_ac->dead();
     alert_client_ac->dead();
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
     shape.has_focus=true;

     redrawFrame();

     getClient().gainFocus();
    }

   virtual void looseFocus()
    {
     shape.has_focus=false;

     redrawFrame();

     getClient().looseFocus();
    }

   // mouse

   virtual void looseCapture()
    {
     if( shape.drag_type ) endDrag();

     if( client_capture )
       {
        client_capture=false;

        getClient().looseCapture();
       }
    }

   virtual void setMouseShape(Point point,KeyMod kmod)
    {
     switch( shape.dragTest(point) )
       {
        case DragType_Top         : host->setMouseShape(Mouse_SizeUpDown); break;

        case DragType_TopLeft     : host->setMouseShape(Mouse_SizeUpLeft); break;

        case DragType_Left        : host->setMouseShape(Mouse_SizeLeftRight); break;

        case DragType_BottomLeft  : host->setMouseShape(Mouse_SizeUpRight); break;

        case DragType_Bottom      : host->setMouseShape(Mouse_SizeUpDown); break;

        case DragType_BottomRight : host->setMouseShape(Mouse_SizeUpLeft); break;

        case DragType_Right       : host->setMouseShape(Mouse_SizeLeftRight); break;

        case DragType_TopRight    : host->setMouseShape(Mouse_SizeUpRight); break;

        case DragType_Alert       :
        case DragType_Min         :
        case DragType_Max         : host->setMouseShape(Mouse_Hand); break;

        case DragType_Close       : host->setMouseShape(Mouse_Stop); break;

        case DragType_Bar         : host->setMouseShape(Mouse_SizeAll); break;

        default: host->setMouseShape(getClient().forward_getMouseShape(point,kmod));
       }
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
        if( shape.drag_type || shape.btn_type ) return;

        Point point=action.getPoint();

        if( client_capture || shape.getClient().contains(point) )
          {
           getClient().forward_react(action);
          }
       }
    }

   void react_Key(VKey vkey,KeyMod kmod)
    {
     if( !forwardKey(vkey,kmod) ) getClient().put_Key(vkey,kmod);
    }

   void react_Key(VKey vkey,KeyMod kmod,unsigned repeat)
    {
     if( !forwardKey(vkey,kmod,repeat) ) getClient().put_Key(vkey,kmod,repeat);
    }

   void react_KeyUp(VKey vkey,KeyMod kmod)
    {
     if( !forwardKeyUp(vkey,kmod) ) getClient().put_KeyUp(vkey,kmod);
    }

   void react_KeyUp(VKey vkey,KeyMod kmod,unsigned repeat)
    {
     if( !forwardKeyUp(vkey,kmod,repeat) ) getClient().put_KeyUp(vkey,kmod,repeat);
    }

   void react_LeftClick(Point point,MouseKey mkey)
    {
     switch( auto drag_type=shape.dragTest(point) )
       {
        case DragType_None :
         {
          if( client_capture || shape.getClient().contains(point) )
            {
             getClient().forward().put_LeftClick(point,mkey);
            }
         }
        break;

        case DragType_Alert :
        case DragType_Min   :
        case DragType_Max   :
        case DragType_Close :
         {
          if( !shape.drag_type && !shape.btn_type ) btnDown(drag_type);
         }
        break;

        default:
         {
          if( !shape.drag_type && !shape.btn_type ) startDrag(point,drag_type);
         }
       }
    }

   void react_LeftUp(Point point,MouseKey mkey)
    {
     if( shape.drag_type )
       {
        endDrag(point);
       }
     else if( shape.btn_type )
       {
        btnUp(point);
       }
     else if( client_capture || shape.getClient().contains(point) )
       {
        getClient().forward().put_LeftUp(point,mkey);
       }

     hilightFrame(point);
    }

   void react_Move(Point point,MouseKey mkey)
    {
     if( shape.drag_type )
       {
        if( mkey&MouseKey_Left )
          dragTo(point);
        else
          endDrag(point);
       }
     else if( shape.btn_type )
       {
        if( mkey&MouseKey_Left )
          moveBtn(point);
        else
          endBtn();
       }
     else
       {
        hilightFrame(point);

        if( shape.getClient().contains(point) )
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
    }

   void react_Leave()
    {
     if( shape.btn_type )
       {
        endBtn();
       }
     else if( !shape.drag_type )
       {
        endHilightFrame();
       }

     if( client_enter )
       {
        client_enter=false;

        getClient().put_Leave();
       }
    }

   // DeferInput

   class Input : DeferInput<DragWindowOf<Shape> >
    {
      friend class DragWindowOf<Shape>;

      explicit Input(DragWindowOf<Shape> *window) : DeferInput<DragWindowOf<Shape> >(window) {}

      ~Input() {}

      using DeferInput<DragWindowOf<Shape> >::try_post;

     public:

      void redrawAll(bool do_layout=false) { try_post(&DragWindowOf<Shape>::redrawAll,do_layout); }

      void redrawClient() { try_post(&DragWindowOf<Shape>::redrawClient); }

      void redrawClient(Pane pane) { try_post(&DragWindowOf<Shape>::redrawClient,pane); }
    };

   Input input;

  private:

   DeferTick defer_tick;

   void updateConfig()
    {
     input.redrawAll(true);
    }

   void alert()
    {
     switch( shape.alert_type )
       {
        case AlertType_No :
         {
          shape.alert_type=AlertType_Closed;

          redrawFrame(DragType_Alert);

          pushAlertBlink();
         }
        break;

        case AlertType_Closed :
         {
          pushAlertBlink();
         }
        break;

        case AlertType_Opened :
         {
          redrawClient();
         }
        break;
       }
    }

   SignalConnector<DragWindowOf<Shape> > connector_updateConfig;
   SignalConnector<DragWindowOf<Shape> > connector_alert;
 };

/* type DragWindow */

using DragWindow = DragWindowOf<DragShape> ;

} // namespace Video
} // namespace CCore

#endif


