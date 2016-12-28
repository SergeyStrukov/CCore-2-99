/* Window.Scroll.h */
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

#ifndef CCore_inc_video_lib_Window_Scroll_h
#define CCore_inc_video_lib_Window_Scroll_h

#include <CCore/inc/video/SubWindow.h>

#include <CCore/inc/video/lib/Shape.Scroll.h>

namespace CCore {
namespace Video {

/* classes */

template <class Shape> class ScrollWindowOf;

/* class ScrollWindowOf<Shape> */

template <class Shape>
class ScrollWindowOf : public SubWindow
 {
   Shape shape;

   DeferInput<ScrollWindowOf<Shape> > input;

   DeferTick defer_tick;

   bool invert_wheel = false ;

  private:

   void pressOn(ScrollType type,Point point)
    {
     shape.press_type=type;

     if( type==ScrollType_Drag )
       {
        shape.drag_base=point;
        shape.drag_pos=shape.pos;
       }
     else
       {
        pressOn();
       }
    }

   void pressOn(ScrollType type)
    {
     shape.press_type=type;

     pressOn();
    }

   void pressOn()
    {
     shape.change_count=0;

     shape.posChange();

     changed.assert(shape.pos);

     defer_tick.start();
    }

   void drag(Point point)
    {
     shape.drag(point);

     changed.assert(shape.pos);

     redraw();
    }

   void pressOff(Point point)
    {
     if( shape.press_type==ScrollType_Drag )
       {
        shape.drag(point);

        changed.assert(shape.pos);
       }
     else
       {
        defer_tick.stop();
       }
    }

   void pressOff()
    {
     if( shape.press_type!=ScrollType_Drag ) defer_tick.stop();
    }

   void inside(ScrollType type)
    {
     if( shape.down && shape.mouse )
       {
        if( Change(shape.mover,type) )
          {
           shape.down=ScrollType_None;

           releaseMouse();

           pressOff();

           redraw();
          }
       }
     else
       {
        if( Change(shape.mover,type) ) redraw();
       }
    }

   void outside()
    {
     if( shape.down && shape.mouse )
       {
        shape.down=ScrollType_None;
        shape.mover=ScrollType_None;

        releaseMouse();

        pressOff();

        redraw();
       }
     else
       {
        if( Change(shape.mover,ScrollType_None) ) redraw();
       }
    }

   void tick()
    {
     if( shape.posChange() )
       {
        changed.assert(shape.pos);

        redraw();
       }
    }

   void stopDown()
    {
     if( shape.down )
       {
        shape.down=ScrollType_None;

        if( shape.mouse )
          {
           shape.mover=ScrollType_None;

           releaseMouse();
          }

        pressOff();

        redraw();
       }
    }

  public:

   using ShapeType = Shape ;
   using ConfigType = typename Shape::Config ;

   template <class ... TT>
   ScrollWindowOf(SubWindowHost &host,TT && ... tt)
    : SubWindow(host),
      shape( std::forward<TT>(tt)... ),
      input(this)
    {
     defer_tick=input.create(&ScrollWindowOf<Shape>::tick);
    }

   virtual ~ScrollWindowOf() {}

   // methods

   auto getMinSize() const { return shape.getMinSize(); }

   bool isEnabled() const { return shape.enable; }

   void enable(bool enable=true)
    {
     if( Change(shape.enable,enable) ) redraw();
    }

   void disable() { enable(false); }

   void invertWheel(bool invert=true) { invert_wheel=invert; }

   ulen getTotal() const { return shape.total; }

   ulen getPage() const { return shape.page; }

   ulen getPos() const { return shape.pos; }

   void setRange(ulen total,ulen page,ulen pos)
    {
     stopDown();

     shape.total=total;
     shape.page=page;
     shape.pos=pos;

     shape.adjustPos();

     redraw();
    }

   void setRange(ulen total,ulen page)
    {
     stopDown();

     shape.total=total;
     shape.page=page;

     shape.adjustPos();

     redraw();
    }

   void setPos(ulen pos)
    {
     stopDown();

     shape.pos=pos;

     shape.adjustPos();

     redraw();
    }

   // drawing

   virtual bool isGoodSize(Point size) const { return shape.isGoodSize(size); }

   virtual void layout()
    {
     shape.pane=Pane(Null,getSize());
    }

   virtual void draw(DrawBuf buf,bool) const
    {
     shape.draw(buf);
    }

   // base

   virtual void open()
    {
     shape.focus=false;
     shape.down=ScrollType_None;
     shape.mover=ScrollType_None;
    }

   virtual void close()
    {
     defer_tick.stop();
    }

   // keyboard

   virtual FocusType askFocus() const
    {
     return shape.enable?FocusOk:NoFocus;
    }

   virtual void gainFocus()
    {
     if( Change(shape.focus,true) ) redraw();
    }

   virtual void looseFocus()
    {
     if( shape.down && !shape.mouse )
       {
        shape.down=ScrollType_None;
        shape.focus=false;

        pressOff();

        redraw();
       }
     else
       {
        if( Change(shape.focus,false) ) redraw();
       }
    }

   // mouse

   virtual void looseCapture()
    {
     if( shape.down && shape.mouse )
       {
        shape.down=ScrollType_None;
        shape.mover=ScrollType_None;

        pressOff();

        redraw();
       }
    }

   virtual MouseShape getMouseShape(Point,KeyMod) const
    {
     if( shape.enable && shape.page<shape.total ) return Mouse_Hand;

     return Mouse_Arrow;
    }

   // user input

   virtual void react(UserAction action)
    {
     action.dispatch(*this);
    }

   void react_Key(VKey vkey,KeyMod kmod)
    {
     switch( vkey )
       {
        case VKey_Home :
         {
          if( shape.enable && !shape.down )
            {
             shape.posMin();

             changed.assert(shape.pos);

             redraw();
            }
         }
        break;

        case VKey_End :
         {
          if( shape.enable && !shape.down )
            {
             shape.posMax();

             changed.assert(shape.pos);

             redraw();
            }
         }
        break;

        case Shape::DownKey :
         {
          if( shape.enable && !shape.down )
            {
             shape.down=ScrollType_Down;
             shape.mouse=false;

             pressOn(ScrollType_Down);

             redraw();
            }
         }
        break;

        case Shape::UpKey :
         {
          if( shape.enable && !shape.down )
            {
             shape.down=ScrollType_Up;
             shape.mouse=false;

             pressOn(ScrollType_Up);

             redraw();
            }
         }
        break;

        case VKey_PageUp :
         {
          if( shape.enable && !shape.down )
            {
             shape.down=ScrollType_DownPage;
             shape.mouse=false;

             pressOn(ScrollType_DownPage);

             redraw();
            }
         }
        break;

        case VKey_PageDown :
         {
          if( shape.enable && !shape.down )
            {
             shape.down=ScrollType_UpPage;
             shape.mouse=false;

             pressOn(ScrollType_UpPage);

             redraw();
            }
         }
        break;

        case VKey_Tab :
         {
          tabbed.assert(kmod&KeyMod_Shift);
         }
        break;
       }
    }

   void react_KeyUp(VKey vkey,KeyMod)
    {
     switch( vkey )
       {
        case Shape::DownKey :
         {
          if( shape.down && !shape.mouse && shape.press_type==ScrollType_Down )
            {
             shape.down=ScrollType_None;

             pressOff();

             redraw();
            }
         }
        break;

        case Shape::UpKey :
         {
          if( shape.down && !shape.mouse && shape.press_type==ScrollType_Up )
            {
             shape.down=ScrollType_None;

             pressOff();

             redraw();
            }
         }
        break;

        case VKey_PageUp :
         {
          if( shape.down && !shape.mouse && shape.press_type==ScrollType_DownPage )
            {
             shape.down=ScrollType_None;

             pressOff();

             redraw();
            }
         }
        break;

        case VKey_PageDown :
         {
          if( shape.down && !shape.mouse && shape.press_type==ScrollType_UpPage )
            {
             shape.down=ScrollType_None;

             pressOff();

             redraw();
            }
         }
        break;
       }
    }

   void react_LeftClick(Point point,MouseKey)
    {
     if( shape.enable && !shape.down )
       {
        if( ScrollType type=shape.getType(point) )
          {
           shape.mover=type;
           shape.down=type;
           shape.mouse=true;

           captureMouse();

           pressOn(type,point);

           redraw();
          }
       }
    }

   void react_LeftUp(Point point,MouseKey)
    {
     if( shape.down && shape.mouse )
       {
        shape.down=ScrollType_None;
        shape.mover=shape.getType(point);

        releaseMouse();

        pressOff(point);

        redraw();
       }
    }

   void react_LeftDClick(Point point,MouseKey mkey)
    {
     react_LeftClick(point,mkey);
    }

   void react_Move(Point point,MouseKey mkey)
    {
     if( shape.down && shape.mouse && shape.press_type==ScrollType_Drag )
       {
        if( mkey&MouseKey_Left )
          drag(point);
        else
          react_LeftUp(point,mkey);
       }
     else
       {
        if( shape.down && shape.mouse && !(mkey&MouseKey_Left) )
          {
           react_LeftUp(point,mkey);
          }
        else
          {
           if( ScrollType type=shape.getType(point) )
             inside(type);
           else
             outside();
          }
       }
    }

   void react_Leave()
    {
     if( shape.down && shape.mouse && shape.press_type==ScrollType_Drag )
       {
        // do nothing
       }
     else
       {
        outside();
       }
    }

   void react_Wheel(Point,MouseKey mkey,Coord delta_)
    {
     if( shape.enable && !shape.down )
       {
        ulen delta=IntAbs(delta_);

        if( invert_wheel? delta_<0 : ( delta_>0 ) )
          {
           if( mkey&MouseKey_Shift )
             shape.posDown(delta*shape.page);
           else
             shape.posDown(delta);
          }
        else
          {
           if( mkey&MouseKey_Shift )
             shape.posUp(delta*shape.page);
           else
             shape.posUp(delta);
          }

        changed.assert(shape.pos);

        redraw();
       }
    }

   // signals

   Signal<ulen> changed; // pos
   Signal<bool> tabbed;  // shift
 };

/* type XScrollWindow */

using XScrollWindow = ScrollWindowOf<XScrollShape> ;

/* type YScrollWindow */

using YScrollWindow = ScrollWindowOf<YScrollShape> ;

} // namespace Video
} // namespace CCore

#endif

