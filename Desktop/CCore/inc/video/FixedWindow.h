/* FixedWindow.h */
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

#ifndef CCore_inc_video_FixedWindow_h
#define CCore_inc_video_FixedWindow_h

#include <CCore/inc/video/SubWindow.h>
#include <CCore/inc/video/Font.h>
#include <CCore/inc/video/FrameGuards.h>
#include <CCore/inc/video/RefVal.h>

#include <CCore/inc/String.h>
#include <CCore/inc/DeferCall.h>
#include <CCore/inc/Signal.h>

namespace CCore {
namespace Video {

/* consts */

enum HitFrameType
 {
  HitFrame_None = 0,

  HitFrame_Move,

  HitFrame_Close
 };

/* classes */

class FixedShape;

template <class Shape> class FixedWindowOf;

/* class FixedShape */

class FixedShape
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<Coord> frame_dxy = 12 ;
     RefVal<Coord> title_dy  = 32 ;

     RefVal<Coord> btn_dx    = 26 ;
     RefVal<Coord> btn_dy    = 24 ;

     RefVal<VColor> top            =      Gray ;
     RefVal<VColor> bottom         =      Snow ;

     RefVal<VColor> frame          =      Snow ;
     RefVal<VColor> frameHilight   = PaleGreen ;
     RefVal<VColor> frameDrag      =      Pink ;

     RefVal<VColor> active         = RoyalBlue ;
     RefVal<VColor> inactive       =    Silver ;
     RefVal<VColor> title          =     Black ;

     RefVal<VColor> btnFace        = SteelBlue ;
     RefVal<VColor> btnFaceHilight =     Green ;
     RefVal<VColor> btnPictClose   =       Red ;

     RefVal<VColor> shade_color    =    Violet ;
     RefVal<Clr>    shade_alpha    =        64 ;

     RefVal<Font> title_font;

     Config() {}
    };

   const Config &cfg;

  private:

   // layout

   Point size;

   Pane client;
   Pane titleBar;
   Pane btnClose;

  private:

   class DrawArt;

   void draw_Frame(DrawArt &art) const;

   void draw_Title(DrawArt &art) const;

   void draw_Close(DrawArt &art) const;

  public:

   // state

   bool has_focus = false ;

   HitFrameType hilight  = HitFrame_None ;
   HitFrameType hit_type = HitFrame_None ;

   String title;

   // methods

   explicit FixedShape(const Config &cfg_) : cfg(cfg_) {}

   Point getDeltaSize() const;

   Coord getMinDx(StrLen title) const;

   Pane getClient() const { return client; }

   void reset(const String &title);

   void layout(Point size);

   void draw(const DrawBuf &buf) const;

   void draw(const DrawBuf &buf,HitFrameType hit_type) const;

   Pane getPane(HitFrameType hit_type) const;

   HitFrameType hitTest(Point point) const;
 };

/* class FixedWindowOf<Shape> */

template <class Shape>
class FixedWindowOf : public FrameWindow , public SubWindowHost
 {
  public:

   using ShapeType = Shape ;

   using ConfigType = typename Shape::Config ;

  private:

   Shape shape;

   SubWindow *client = 0 ;

   AliveControl *client_ac = &AliveControl::Default ;

   Point size;

   Point drag_from;
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

   void replace(Pane place,Point delta)
    {
     place+=delta;

     Point new_size(place.dx,place.dy);

     if( new_size>Null && new_size<=host->getMaxSize() )
       {
        Pane screen=Pane(Null,desktop->getScreenSize());

        if( +Inf(place,screen) )
          {
           host->move(place);

           host->invalidate(1);
          }
       }
    }

   void replace(Point delta)
    {
     Pane place=host->getPlace();

     replace(place,delta);
    }

   void startDrag(Point point)
    {
     shape.hit_type=HitFrame_Move;

     if( !client_capture ) host->captureMouse();

     Pane place=host->getPlace();

     drag_from=point+place.getBase();

     redrawAll();
    }

   void dragTo(Point point)
    {
     Pane place=host->getPlace();
     Point delta=Diff(drag_from,point+place.getBase());

     replace(place,delta);
    }

   void endDrag()
    {
     shape.hit_type=HitFrame_None;

     redrawAll();
    }

   void endDrag(Point point)
    {
     dragTo(point);

     shape.hit_type=HitFrame_None;

     if( !client_capture ) host->releaseMouse();

     redrawAll();
    }

   void btnDown(HitFrameType hit_type)
    {
     shape.hit_type=hit_type;

     redrawFrame(hit_type);
    }

   void btnUp(Point point)
    {
     auto type=Replace(shape.hit_type,HitFrame_None);

     redrawFrame(type);

     if( shape.hitTest(point)==type )
       {
        switch( type )
          {
           case HitFrame_Close : destroy(); return;
          }
       }
    }

   void moveBtn(Point point)
    {
     if( shape.hitTest(point)!=shape.hit_type ) endBtn();
    }

   void endBtn()
    {
     redrawFrame(Replace(shape.hit_type,HitFrame_None));
    }

   void hilightFrame(Point point)
    {
     if( Change(shape.hilight,shape.hitTest(point)) ) redrawFrame();
    }

   void endHilightFrame()
    {
     if( Change(shape.hilight,HitFrame_None) ) redrawFrame();
    }

   bool forwardKey(VKey vkey,KeyMod kmod,unsigned repeat=1)
    {
     if( kmod&KeyMod_Alt )
       {
        switch( vkey )
          {
           case VKey_Left  : replace(Point(-(Coord)repeat,0)); return true;

           case VKey_Right : replace(Point((Coord)repeat,0)); return true;

           case VKey_Up    : replace(Point(0,-(Coord)repeat)); return true;

           case VKey_Down  : replace(Point(0,(Coord)repeat)); return true;

           case VKey_F4    : destroy(); return true;

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
           case VKey_F4    : return true;

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

   void redrawFrame(HitFrameType hit_type)
    {
     Pane pane=shape.getPane(hit_type);

     if( !pane ) return;

     redraw( [this,hit_type,pane] (FrameBuf<DesktopColor> &buf)
                                  {
                                   try { shape.draw(buf,hit_type); } catch(CatchType) {}

                                   shade(buf);

                                   host->invalidate(pane,1);

                                  } );
    }

  public:

   FixedWindowOf(Desktop *desktop,const ConfigType &cfg)
    : FrameWindow(desktop),
      shape(cfg),
      input(this),
      connector_updateConfig(this,&FixedWindowOf<Shape>::updateConfig)
    {
    }

   FixedWindowOf(Desktop *desktop,const ConfigType &cfg,Signal<> &update)
    : FixedWindowOf(desktop,cfg)
    {
     connector_updateConfig.connect(update);
    }

   virtual ~FixedWindowOf()
    {
    }

   // methods

   Point getDeltaSize() const { return shape.getDeltaSize(); }

   Point getMinSize(StrLen title,Point size) const
    {
     size+=shape.getDeltaSize();

     return Point(Max(size.x,shape.getMinDx(title)),size.y);
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

   void createMain(Pane pane,const String &title)
    {
     guardClient();

     shape.reset(title);

     host->createMain(pane,pane.getSize());
     host->setTitle(Range(title));
     host->display(CmdDisplay_Normal);
    }

   void create(Pane pane,const String &title)
    {
     guardClient();

     shape.reset(title);

     host->create(pane,pane.getSize());
     host->show();
    }

   void create(FrameWindow *parent,Pane pane,const String &title)
    {
     guardClient();

     shape.reset(title);

     host->create(parent->getHost(),pane,pane.getSize());
     host->show();
    }

   void destroy()
    {
     if( client_ac->askDestroy() )
       {
        host->destroy();

        destroyed.assert();
       }
    }

   void redrawAll(bool do_layout=false)
    {
     if( do_layout )
       {
        shape.layout(size);

        if( client ) client->setPlace(shape.getClient());
       }

     redraw( [this] (FrameBuf<DesktopColor> &buf)
                    {
                     try { shape.draw(buf); } catch(CatchType) {}

                     getClient().forward_draw(buf,shape.hit_type==HitFrame_Move);

                     shade(buf);

                     host->invalidate(1);

                    } );
    }

   void redrawClient()
    {
     redraw( [this] (FrameBuf<DesktopColor> &buf)
                    {
                     getClient().forward_draw(buf,shape.hit_type==HitFrame_Move);

                     shade(buf);

                     host->invalidate(shape.getClient(),1);

                    } );
    }

   void redrawClient(Pane pane)
    {
     redraw( [this,pane] (FrameBuf<DesktopColor> &buf)
                         {
                          getClient().forward_draw(buf,pane,shape.hit_type==HitFrame_Move);

                          shade(buf);

                          host->invalidate(pane,1);

                         } );
    }

   unsigned getToken() { return host->getToken(); }

   void setTitle(const String &title)
    {
     shape.title=title;

     host->setTitle(Range(title));

     redrawFrame();
    }

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

        if( shape.hit_type!=HitFrame_Move ) host->captureMouse();
       }
    }

   virtual void releaseMouse(SubWindow *)
    {
     if( client_capture )
       {
        client_capture=false;

        if( shape.hit_type!=HitFrame_Move ) host->releaseMouse();
       }
    }

   // base

   virtual void alive()
    {
     reset();

     host->trackMouseHover();
     host->trackMouseLeave();

     client_ac->alive();

     getClient().open();
    }

   virtual void dead()
    {
     getClient().close();

     client_ac->dead();
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
     if( shape.hit_type==HitFrame_Move ) endDrag();

     if( client_capture )
       {
        client_capture=false;

        getClient().looseCapture();
       }
    }

   virtual void setMouseShape(Point point,KeyMod kmod)
    {
     switch( shape.hitTest(point) )
       {
        case HitFrame_Move : host->setMouseShape(Mouse_SizeAll); break;

        case HitFrame_Close : host->setMouseShape(Mouse_Stop); break;

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
        if( shape.hit_type ) return;

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
     switch( auto hit_type=shape.hitTest(point) )
       {
        case HitFrame_None :
         {
          if( client_capture || shape.getClient().contains(point) )
            {
             getClient().forward().put_LeftClick(point,mkey);
            }
         }
        break;

        case HitFrame_Close :
         {
          if( !shape.hit_type ) btnDown(hit_type);
         }
        break;

        case HitFrame_Move :
         {
          if( !shape.hit_type ) startDrag(point);
         }
       }
    }

   void react_LeftUp(Point point,MouseKey mkey)
    {
     switch( shape.hit_type )
       {
        case HitFrame_Move : endDrag(point); break;

        case HitFrame_Close : btnUp(point); break;

        default:
         {
          if( client_capture || shape.getClient().contains(point) )
            {
             getClient().forward().put_LeftUp(point,mkey);
            }
         }
       }

     hilightFrame(point);
    }

   void react_Move(Point point,MouseKey mkey)
    {
     switch( shape.hit_type )
       {
        case HitFrame_Move :
         {
          if( mkey&MouseKey_Left )
            dragTo(point);
          else
            endDrag(point);
         }
        break;

        case HitFrame_Close :
         {
          if( mkey&MouseKey_Left )
            moveBtn(point);
          else
            endBtn();
         }
        break;

        default:
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
    }

   void react_Leave()
    {
     if( shape.hit_type )
       {
        if( shape.hit_type!=HitFrame_Move ) endBtn();
       }
     else
       {
        endHilightFrame();
       }

     if( client_enter )
       {
        client_enter=false;

        getClient().put_Leave();
       }
    }

   // signals

   Signal<> destroyed;

   // DeferInput

   class Input : DeferInput<FixedWindowOf<Shape> >
    {
      friend class FixedWindowOf<Shape>;

      explicit Input(FixedWindowOf<Shape> *window) : DeferInput<FixedWindowOf<Shape> >(window) {}

      ~Input() {}

      using DeferInput<FixedWindowOf<Shape> >::try_post;

     public:

      void redrawAll(bool do_layout=false) { try_post(&FixedWindowOf<Shape>::redrawAll,do_layout); }

      void redrawClient() { try_post(&FixedWindowOf<Shape>::redrawClient); }

      void redrawClient(Pane pane) { try_post(&FixedWindowOf<Shape>::redrawClient,pane); }
    };

   Input input;

  private:

   void updateConfig()
    {
     input.redrawAll(true);
    }

   SignalConnector<FixedWindowOf<Shape> > connector_updateConfig;
 };

/* type FixedWindow */

using FixedWindow = FixedWindowOf<FixedShape> ;

} // namespace Video
} // namespace CCore

#endif


