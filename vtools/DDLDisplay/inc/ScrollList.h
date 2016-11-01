/* ScrollList.h */
//----------------------------------------------------------------------------------------
//
//  Project: DDLDisplay 1.00
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

class ScrollListShape;

struct ScrollListWindowBase;

template <class Shape> class ScrollListInnerWindowOf;

template <class Shape,class XShape,class YShape> class ScrollListWindowOf;

/* class ScrollListShape */

class ScrollListShape
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     //RefVal<Point> space = Point(8,8) ;

     RefVal<VColor> back   = Silver ;
     RefVal<VColor> top    =   Gray ;
     RefVal<VColor> bottom =   Snow ;

     RefVal<VColor> text     =     Black ;
     RefVal<VColor> title    =     Black ;
     RefVal<VColor> inactive =      Gray ;
     RefVal<VColor> focus    = OrangeRed ;
     RefVal<VColor> select   =    Yellow ;

     RefVal<Font> font;

     Config() noexcept {}
    };

   // parameters

   const Config &cfg;
   ComboInfo info;
   Pane pane;

   // state

   bool enable =  true ;
   bool focus  = false ;
   ulen select = 0 ;

   ulen yoff  = 0 ;
   Coord xoff = 0 ;

   ulen page     = 0 ;
   ulen yoffMax  = 0 ;
   Coord xoffMax = 0 ;
   Coord dxoff   = 0 ;

   // methods

   explicit ScrollListShape(const Config &cfg_) : cfg(cfg_) {}

   ScrollListShape(const Config &cfg_,const ComboInfo &info_) : cfg(cfg_),info(info_) {}

   Point getMinSize() const;

   void setMax();

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void showSelect();

   ulen getPosition(Point point) const;

   void draw(const DrawBuf &buf) const;
 };

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
     if( shape.select )
       {
        shape.select=0;

        shape.showSelect();

        outer->selected.assert(shape.select);

        redraw();
       }
    }

   void selectLast()
    {
     ulen count=shape.info->getLineCount();

     if( count )
       {
        if( shape.select!=count-1 )
          {
           shape.select=count-1;

           shape.showSelect();

           outer->selected.assert(shape.select);

           redraw();
          }
       }
     else
       {
        selectFirst();
       }
    }

   void addSelect(ulen delta)
    {
     ulen count=shape.info->getLineCount();

     if( delta && count && shape.select<count-1 )
       {
        shape.select+=Min<ulen>(delta,count-1-shape.select);

        shape.showSelect();

        outer->selected.assert(shape.select);

        redraw();
       }
    }

   void subSelect(ulen delta)
    {
     if( delta && shape.select )
       {
        shape.select=PosSub(shape.select,delta);

        shape.showSelect();

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

     if( shape.select!=select )
       {
        shape.select=select;

        shape.showSelect();

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
      outer(outer_),
      shape( std::forward<TT>(tt)... ),
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

   Point getMinSize() const { return shape.getMinSize(); }

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
     shape.select=0;

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
     RefVal<Coord> scroll_dxy = 20 ;

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

   Point getMinSize() const { return inner.getMinSize(); }

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
     Coord delta=+cfg.scroll_dxy;

     inner.setPlace(pane);

     if( inner.shortDY() )
       {
        Pane px=SplitX(pane,delta);

        inner.setPlace(pane);
        scroll_x.setPlace(px);

        if( !scroll_x.isListed() ) wlist.insBottom(scroll_x);

        if( inner.shortDX() )
          {
           Pane py=SplitY(pane,delta);

           inner.setPlace(pane);
           scroll_y.setPlace(py);

           if( !scroll_y.isListed() ) wlist.insBottom(scroll_y);
          }
        else
          {
           if( scroll_y.isListed() ) wlist.del(scroll_y);
          }
       }
     else
       {
        if( inner.shortDX() )
          {
           Pane py=SplitY(pane,delta);

           inner.setPlace(pane);

           if( inner.shortDY() )
             {
              pane=all;
              Pane px=SplitX(pane,delta);
              Pane py=SplitY(pane,delta);

              inner.setPlace(pane);
              scroll_x.setPlace(px);
              scroll_y.setPlace(py);

              if( !scroll_x.isListed() ) wlist.insBottom(scroll_x);

              if( !scroll_y.isListed() ) wlist.insBottom(scroll_y);
             }
           else
             {
              scroll_y.setPlace(py);

              if( !scroll_y.isListed() ) wlist.insBottom(scroll_y);

              if( scroll_x.isListed() ) wlist.del(scroll_x);
           }
          }
        else
          {
           if( scroll_x.isListed() ) wlist.del(scroll_x);

           if( scroll_y.isListed() ) wlist.del(scroll_y);
          }
       }

     setScroll();
    }
 };

/* type ScrollListWindow */

using ScrollListWindow = ScrollListWindowOf<ScrollListShape,XScrollShape,YScrollShape> ;

} // namespace App


