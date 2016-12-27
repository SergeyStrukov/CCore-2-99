/* WindowLib.h */
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

#ifndef CCore_inc_video_WindowLib_h
#define CCore_inc_video_WindowLib_h

#include <CCore/inc/video/ShapeLib.h>

#include <CCore/inc/video/lib/Window.Button.h>
#include <CCore/inc/video/lib/Window.Check.h>
#include <CCore/inc/video/lib/Window.Radio.h>
#include <CCore/inc/video/lib/Window.Light.h>
#include <CCore/inc/video/lib/Window.Text.h>
#include <CCore/inc/video/lib/Window.TextLine.h>
#include <CCore/inc/video/lib/Window.Decor.h>
#include <CCore/inc/video/lib/Window.Progress.h>
#include <CCore/inc/video/lib/Window.Info.h>
#include <CCore/inc/video/lib/Window.SimpleTextList.h>
#include <CCore/inc/video/lib/Window.LineEdit.h>

#include <CCore/inc/Array.h>
#include <CCore/inc/CharProp.h>
#include <CCore/inc/algon/EuclidRotate.h>

namespace CCore {
namespace Video {

/* classes */

template <class Shape> class ScrollWindowOf;

struct ScrollListWindowBase;

template <class Shape> class ScrollListInnerWindowOf;

template <class Shape,class XShape,class YShape> class ScrollListWindowOf;

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

   void pressOff(Point point)
    {
     if( shape.press_type==ScrollType_Drag )
       {
        shape.drag(point);

        changed.assert(shape.pos);
       }
     else
       {
        pressOff();
       }
    }

   void drag(Point point)
    {
     shape.drag(point);

     changed.assert(shape.pos);

     redraw();
    }

   void pressOff()
    {
     defer_tick.stop();
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

   bool isGoodSize(Point size) const { return shape.isGoodSize(size); }

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

   void setRange(ulen total,ulen page)
    {
     shape.total=total;
     shape.page=page;

     shape.adjustPos();

     redraw();
    }

   void setRange(ulen total,ulen page,ulen pos)
    {
     shape.total=total;
     shape.page=page;
     shape.pos=pos;

     shape.adjustPos();

     redraw();
    }

   void setPos(ulen pos)
    {
     shape.pos=pos;

     shape.adjustPos();

     redraw();
    }

   // drawing

   virtual void layout()
    {
     shape.pane=Pane(Null,getSize());
    }

   virtual void draw(DrawBuf buf,bool) const
    {
     try { shape.draw(buf); } catch(CatchType) {}
    }

   // base

   virtual void open()
    {
     shape.focus=false;
     shape.down=ScrollType_None;
     shape.mover=ScrollType_None;
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

   void react_Move(Point point,MouseKey)
    {
     if( shape.down && shape.mouse && shape.mover==ScrollType_Drag )
       {
        drag(point);
       }
     else
       {
        if( ScrollType type=shape.getType(point) )
          inside(type);
        else
          outside();
       }
    }

   void react_Leave()
    {
     if( shape.down && shape.mouse && shape.mover==ScrollType_Drag )
       {
        // do nothing
       }
     else
       {
        outside();
       }
    }

   void react_Wheel(Point,MouseKey mkey,Coord delta)
    {
     if( shape.enable && !shape.down )
       {
        if( delta>0 )
          {
           auto d=IntDist<Coord>(0,delta);

           if( invert_wheel )
             {
              if( mkey&MouseKey_Shift )
                shape.posUp(d*shape.page);
              else
                shape.posUp(d);
             }
           else
             {
              if( mkey&MouseKey_Shift )
                shape.posDown(d*shape.page);
              else
                shape.posDown(d);
             }
          }
        else
          {
           auto d=IntDist<Coord>(delta,0);

           if( invert_wheel )
             {
              if( mkey&MouseKey_Shift )
                shape.posDown(d*shape.page);
              else
                shape.posDown(d);
             }
           else
             {
              if( mkey&MouseKey_Shift )
                shape.posUp(d*shape.page);
              else
                shape.posUp(d);
             }
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

/* struct ScrollListWindowBase */

struct ScrollListWindowBase
 {
  // signals

  Signal<> entered;
  Signal<> dclicked;
  Signal<ulen> selected; // select
 };

/* class ScrollListInnerWindowOf<Shape> */

template <class Shape>
class ScrollListInnerWindowOf : public SubWindow
 {
   Shape shape;

   ScrollListWindowBase *outer;

  private:

   void showSelect()
    {
     if( shape.showSelect() ) scroll_y.assert(shape.yoff);
    }

   void setXOff(Coord xoff)
    {
     if( xoff<0 )
       xoff=0;
     else if( xoff>shape.xoffMax )
       xoff=shape.xoffMax;

     if( xoff!=shape.xoff )
       {
        shape.xoff=xoff;

        scroll_x.assert((ulen)xoff);

        redraw();
       }
    }

   void addXOff(Coord delta)
    {
     setXOff( IntAdd(shape.xoff,delta) );
    }

   void subXOff(Coord delta)
    {
     setXOff( IntSub(shape.xoff,delta) );
    }

   void setYOff(ulen yoff)
    {
     if( yoff>shape.yoffMax ) yoff=shape.yoffMax;

     if( yoff!=shape.yoff )
       {
        shape.yoff=yoff;

        scroll_y.assert(yoff);

        redraw();
       }
    }

   void addYOff(ulen delta)
    {
     if( shape.yoff<shape.yoffMax )
       {
        ulen cap=shape.yoffMax-shape.yoff;

        setYOff(shape.yoff+Min(delta,cap));
       }
    }

   void subYOff(ulen delta)
    {
     ulen yoff=shape.yoff;

     if( yoff>delta )
       yoff-=delta;
     else
       yoff=0;

     setYOff(yoff);
    }

   void selectFirst()
    {
     if( shape.setSelectDown(0) )
       {
        showSelect();

        outer->selected.assert(shape.select);

        redraw();
       }
    }

   void selectLast()
    {
     ulen count=shape.info->getLineCount();

     if( count && shape.setSelectUp(count-1) )
       {
        showSelect();

        outer->selected.assert(shape.select);

        redraw();
       }
    }

   void addSelect(ulen delta)
    {
     ulen count=shape.info->getLineCount();

     if( delta && count && shape.select<count-1 && shape.setSelectDown(shape.select+Min<ulen>(delta,count-1-shape.select)) )
       {
        showSelect();

        outer->selected.assert(shape.select);

        redraw();
       }
    }

   void subSelect(ulen delta)
    {
     if( delta && shape.select && shape.setSelectUp(PosSub(shape.select,delta)) )
       {
        showSelect();

        outer->selected.assert(shape.select);

        redraw();
       }
    }

   void setSelect(ulen select,bool signal=true)
    {
     ulen count=shape.info->getLineCount();

     if( select>=count )
       {
        select=count?count-1:0;
       }

     if( shape.select!=select && shape.setSelectDown(select) )
       {
        showSelect();

        if( signal ) outer->selected.assert(shape.select);

        redraw();
       }
    }

  private:

   void posX(ulen pos)
    {
     shape.xoff=(Coord)pos;

     redraw();
    }

   void posY(ulen pos)
    {
     shape.yoff=pos;

     redraw();
    }

   SignalConnector<ScrollListInnerWindowOf<Shape>,ulen> connector_posX;
   SignalConnector<ScrollListInnerWindowOf<Shape>,ulen> connector_posY;

  public:

   using ShapeType = Shape ;
   using ConfigType = typename ShapeType::Config ;

   template <class ... TT>
   ScrollListInnerWindowOf(SubWindowHost &host,ScrollListWindowBase *outer_,TT && ... tt)
    : SubWindow(host),
      shape( std::forward<TT>(tt)... ),
      outer(outer_),
      connector_posX(this,&ScrollListInnerWindowOf<Shape>::posX),
      connector_posY(this,&ScrollListInnerWindowOf<Shape>::posY)
    {
    }

   virtual ~ScrollListInnerWindowOf()
    {
    }

   // special methods

   bool shortDX() const { return shape.xoffMax>0; }

   bool shortDY() const { return shape.yoffMax>0; }

   template <class W>
   void setScrollX(W &window)
    {
     window.setRange((ulen)(shape.xoffMax+shape.dxoff),(ulen)shape.dxoff,(ulen)shape.xoff);
    }

   template <class W>
   void setScrollY(W &window)
    {
     window.setRange(shape.yoffMax+shape.page,shape.page,shape.yoff);
    }

   void bind(Signal<ulen> &scroll_x,Signal<ulen> &scroll_y)
    {
     connector_posX.connect(scroll_x);
     connector_posY.connect(scroll_y);
    }

   // methods

   auto getMinSize() const { return shape.getMinSize(); }

   bool isGoodSize(Point size) const { return shape.isGoodSize(size); }

   bool isEnabled() const { return shape.enable; }

   void enable(bool enable=true)
    {
     if( Change(shape.enable,enable) ) redraw();
    }

   void disable() { enable(false); }

   void setInfo(const ComboInfo &info)
    {
     shape.info=info;
     shape.yoff=0;
     shape.xoff=0;

     shape.initSelect();

     shape.setMax();

     redraw();
    }

   const ComboInfo & getInfo() const { return shape.info; }

   ulen getSelect() const { return shape.select; }

   void select(ulen select) { setSelect(select,false); }

   // drawing

   virtual void layout()
    {
     shape.pane=Pane(Null,getSize());

     shape.setMax();
    }

   virtual void draw(DrawBuf buf,bool) const
    {
     try { shape.draw(buf); } catch(CatchType) {}
    }

   // base

   virtual void open()
    {
     shape.focus=false;
    }

   // keyboard

   virtual FocusType askFocus() const
    {
     return shape.enable?FocusOk:NoFocus;
    }

   virtual void gainFocus()
    {
     shape.focus=true;

     redraw();
    }

   virtual void looseFocus()
    {
     shape.focus=false;

     redraw();
    }

   // mouse

   virtual void looseCapture()
    {
     // do nothing
    }

   virtual MouseShape getMouseShape(Point,KeyMod) const
    {
     return Mouse_Arrow;
    }

   // user input

   virtual void react(UserAction action)
    {
     action.dispatch(*this);
    }

   void react_Key(VKey vkey,KeyMod kmod,unsigned repeat)
    {
     if( !shape.enable ) return;

     switch( vkey )
       {
        case VKey_Left :
         {
          subXOff(Coord(repeat)*shape.dxoff);
         }
        break;

        case VKey_Right :
         {
          addXOff(Coord(repeat)*shape.dxoff);
         }
        break;

        case VKey_Up :
         {
          if( kmod&KeyMod_Shift )
            {
             subYOff(repeat);
            }
          else
            {
             subSelect(repeat);
            }
         }
        break;

        case VKey_Down :
         {
          if( kmod&KeyMod_Shift )
            {
             addYOff(repeat);
            }
          else
            {
             addSelect(repeat);
            }
         }
        break;

        case VKey_PageUp :
         {
          ulen delta=shape.page*repeat;

          subYOff(delta);
          subSelect(delta);
         }
        break;

        case VKey_PageDown :
         {
          ulen delta=shape.page*repeat;

          addYOff(delta);
          addSelect(delta);
         }
        break;

        case VKey_Home :
         {
          selectFirst();
         }
        break;

        case VKey_End :
         {
          selectLast();
         }
        break;

        case VKey_Enter :
         {
          outer->entered.assert();
         }
        break;
       }
    }

   void react_LeftClick(Point point,MouseKey)
    {
     if( !shape.enable ) return;

     setSelect(shape.getPosition(point));
    }

   void react_LeftDClick(Point point,MouseKey mkey)
    {
     if( !shape.enable ) return;

     react_LeftClick(point,mkey);

     outer->dclicked.assert();
    }

   void react_Wheel(Point,MouseKey mkey,Coord delta_)
    {
     if( !shape.enable ) return;

     if( delta_>0 )
       {
        ulen delta=IntDist<Coord>(0,delta_);

        if( mkey&MouseKey_Shift )
          {
           subYOff(delta);
          }
        else
          {
           subSelect(delta);
          }
       }
     else if( delta_<0 )
       {
        ulen delta=IntDist<Coord>(delta_,0);

        if( mkey&MouseKey_Shift )
          {
           addYOff(delta);
          }
        else
          {
           addSelect(delta);
          }
       }
    }

   // signals

   Signal<ulen> scroll_x;
   Signal<ulen> scroll_y;
 };

/* class ScrollListWindowOf<Shape,XShape,YShape> */

template <class Shape,class XShape,class YShape>
class ScrollListWindowOf : public ComboWindow , public ScrollListWindowBase
 {
  public:

   struct Config : Shape::Config
    {
     CtorRefVal<typename XShape::Config> x_cfg;
     CtorRefVal<typename YShape::Config> y_cfg;

     Config() noexcept {}
    };

  private:

   const Config &cfg;

   ScrollListInnerWindowOf<Shape> inner;
   ScrollWindowOf<XShape> scroll_x;
   ScrollWindowOf<YShape> scroll_y;

  private:

   void setScroll()
    {
     if( scroll_x.isListed() ) inner.setScrollX(scroll_x);

     if( scroll_y.isListed() ) inner.setScrollY(scroll_y);
    }

  private:

   SignalConnector<ScrollWindowOf<XShape>,ulen> connector_posx;
   SignalConnector<ScrollWindowOf<YShape>,ulen> connector_posy;

  public:

   using ShapeType = Shape ;
   using ConfigType = Config ;

   template <class ... TT>
   ScrollListWindowOf(SubWindowHost &host,const Config &cfg_,TT && ... tt)
    : ComboWindow(host),
      cfg(cfg_),
      inner(wlist,this,cfg_, std::forward<TT>(tt)... ),
      scroll_x(wlist,cfg_.x_cfg),
      scroll_y(wlist,cfg_.y_cfg),
      connector_posx(&scroll_x,&ScrollWindowOf<XShape>::setPos,inner.scroll_x),
      connector_posy(&scroll_y,&ScrollWindowOf<YShape>::setPos,inner.scroll_y)
    {
     wlist.insTop(inner);

     inner.bind(scroll_x.changed,scroll_y.changed);
    }

   virtual ~ScrollListWindowOf()
    {
    }

   // methods

   auto getMinSize() const { return inner.getMinSize()+Point(scroll_y.getMinSize().dx,0); }

   bool isGoodSize(Point size) const { return inner.isGoodSize(size); }

   bool isEnabled() const { return inner.isEnabled(); }

   void enable(bool enable=true)
    {
     inner.enable(enable);
     scroll_x.enable(enable);
     scroll_y.enable(enable);
    }

   void disable() { enable(false); }

   void setInfo(const ComboInfo &info)
    {
     inner.setInfo(info);

     layout();

     redraw();
    }

   const ComboInfo & getInfo() const { return inner.getInfo(); }

   ulen getSelect() const { return inner.getSelect(); }

   void select(ulen select) { inner.setSelect(select); }

   // drawing

   virtual void layout()
    {
     Pane all(Null,getSize());
     Pane pane(all);
     Coord delta_x=scroll_y.getMinSize().dx;
     Coord delta_y=scroll_x.getMinSize().dy;

     inner.setPlace(pane);

     if( inner.shortDY() )
       {
        Pane py=SplitX(pane,delta_x);

        inner.setPlace(pane);
        scroll_y.setPlace(py);

        wlist.insBottom(scroll_y);

        if( inner.shortDX() )
          {
           Pane px=SplitY(pane,delta_y);

           inner.setPlace(pane);
           scroll_x.setPlace(px);

           wlist.insBottom(scroll_x);
          }
        else
          {
           wlist.del(scroll_x);
          }
       }
     else
       {
        if( inner.shortDX() )
          {
           Pane px=SplitY(pane,delta_y);

           inner.setPlace(pane);

           if( inner.shortDY() )
             {
              pane=all;
              Pane py=SplitX(pane,delta_x);
              Pane px=SplitY(pane,delta_y);

              inner.setPlace(pane);
              scroll_x.setPlace(px);
              scroll_y.setPlace(py);

              wlist.insBottom(scroll_x);

              wlist.insBottom(scroll_y);
             }
           else
             {
              scroll_x.setPlace(px);

              wlist.insBottom(scroll_x);

              wlist.del(scroll_y);
             }
          }
        else
          {
           wlist.del(scroll_x);

           wlist.del(scroll_y);
          }
       }

     setScroll();
    }
 };

/* type ScrollListWindow */

using ScrollListWindow = ScrollListWindowOf<ScrollListShape,XScrollShape,YScrollShape> ;

} // namespace Video
} // namespace CCore

#endif


