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
#include <CCore/inc/video/SubWindow.h>

#include <CCore/inc/DeferCall.h>

#include <CCore/inc/Array.h>
#include <CCore/inc/CharProp.h>
#include <CCore/inc/algon/EuclidRotate.h>

namespace CCore {
namespace Video {

/* classes */

template <class Shape> class ButtonWindowOf;

template <class Shape> class CheckWindowOf;

class RadioItem;

class RadioGroup;

template <class Shape> class RadioWindowOf;

template <class Shape> class TextWindowOf;

template <class Shape> class DecorWindowOf;

template <class Shape> class LightWindowOf;

template <class Shape> class ScrollWindowOf;

template <class Shape> class ProgressWindowOf;

template <class Shape> class InfoWindowOf;

template <class Shape> class LineEditWindowOf;

template <class Shape> class SimpleTextListWindowOf;

struct ScrollListWindowBase;

template <class Shape> class ScrollListInnerWindowOf;

template <class Shape,class XShape,class YShape> class ScrollListWindowOf;

/* class ButtonWindowOf<Shape> */

template <class Shape>
class ButtonWindowOf : public SubWindow
 {
   Shape shape;

  private:

   void inside()
    {
     if( Change(shape.mover,true) ) redraw();
    }

   void outside()
    {
     if( shape.down && shape.mouse )
       {
        shape.mover=false;
        shape.down=false;

        releaseMouse();

        redraw();
       }
     else
       {
        if( Change(shape.mover,false) ) redraw();
       }
    }

  public:

   using ShapeType = Shape ;
   using ConfigType = typename Shape::Config ;

   template <class ... TT>
   explicit ButtonWindowOf(SubWindowHost &host,TT && ... tt)
    : SubWindow(host),
      shape( std::forward<TT>(tt)... )
    {
    }

   virtual ~ButtonWindowOf() {}

   // methods

   auto getMinSize() const { return shape.getMinSize(); }

   bool isGoodSize(Point size) const { return shape.isGoodSize(size); }

   bool isEnabled() const { return shape.enable; }

   void enable(bool enable=true)
    {
     if( Change(shape.enable,enable) ) redraw();
    }

   void disable() { enable(false); }

   using FaceType = typename Shape::FaceType ;

   FaceType getFace() const { return shape.face; }

   void setFace(const FaceType &face)
    {
     shape.face=face;

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
     shape.mover=false;
     shape.down=false;
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
        shape.focus=false;
        shape.down=false;

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
        shape.mover=false;
        shape.down=false;

        redraw();
       }
    }

   virtual MouseShape getMouseShape(Point,KeyMod) const
    {
     return shape.enable?Mouse_Hand:Mouse_Arrow;
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
        case VKey_Enter :
        case VKey_Space :
         {
          if( shape.enable && !shape.down )
            {
             shape.down=true;
             shape.mouse=false;

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
        case VKey_Enter :
        case VKey_Space :
         {
          if( shape.down && !shape.mouse )
            {
             shape.down=false;

             pressed.assert();

             redraw();
            }
         }
        break;
       }
    }

   void react_LeftClick(Point point,MouseKey)
    {
     if( shape.enable && !shape.down && shape.pane.contains(point) )
       {
        shape.down=true;
        shape.mouse=true;

        captureMouse();

        redraw();
       }
    }

   void react_LeftUp(Point point,MouseKey)
    {
     if( shape.down && shape.mouse )
       {
        shape.down=false;

        releaseMouse();

        if( shape.pane.contains(point) ) pressed.assert();

        redraw();
       }
    }

   void react_LeftDClick(Point point,MouseKey mkey)
    {
     react_LeftClick(point,mkey);
    }

   void react_Move(Point point,MouseKey)
    {
     if( shape.pane.contains(point) )
       {
        inside();
       }
     else
       {
        outside();
       }
    }

   void react_Leave()
    {
     outside();
    }

   // signals

   Signal<> pressed;
   Signal<bool> tabbed; // shift
 };

/* type ButtonWindow */

using ButtonWindow = ButtonWindowOf<ButtonShape> ;

/* type KnobWindow */

using KnobWindow = ButtonWindowOf<KnobShape> ;

/* class CheckWindowOf<Shape> */

template <class Shape>
class CheckWindowOf : public SubWindow
 {
   Shape shape;

  private:

   void inside()
    {
     if( Change(shape.mover,true) ) redraw();
    }

   void outside()
    {
     if( Change(shape.mover,false) ) redraw();
    }

   void change()
    {
     shape.check=!shape.check;

     changed.assert(shape.check);

     redraw();
    }

  public:

   using ShapeType = Shape ;
   using ConfigType = typename Shape::Config ;

   template <class ... TT>
   explicit CheckWindowOf(SubWindowHost &host,TT && ... tt)
    : SubWindow(host),
      shape( std::forward<TT>(tt)... )
    {
    }

   virtual ~CheckWindowOf() {}

   // methods

   auto getMinSize() const { return shape.getMinSize(); }

   bool isGoodSize(Point size) const { return shape.isGoodSize(size); }

   bool isEnabled() const { return shape.enable; }

   void enable(bool enable=true)
    {
     if( Change(shape.enable,enable) ) redraw();
    }

   void disable() { enable(false); }

   bool isChecked() const { return shape.check; }

   void check(bool check=true)
    {
     if( Change(shape.check,check) ) redraw();
    }

   void uncheck() { check(false); }

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
     shape.mover=false;
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
     if( Change(shape.focus,false) ) redraw();
    }

   // mouse

   virtual MouseShape getMouseShape(Point,KeyMod) const
    {
     if( shape.enable ) return Mouse_Hand;

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
        case VKey_Enter :
        case VKey_Space :
         {
          if( shape.enable ) change();
         }
        break;

        case VKey_Tab :
         {
          tabbed.assert(kmod&KeyMod_Shift);
         }
        break;
       }
    }

   void react_LeftClick(Point point,MouseKey)
    {
     if( shape.enable && shape.pane.contains(point) ) change();
    }

   void react_LeftDClick(Point point,MouseKey mkey)
    {
     react_LeftClick(point,mkey);
    }

   void react_Move(Point point,MouseKey)
    {
     if( shape.pane.contains(point) )
       {
        inside();
       }
     else
       {
        outside();
       }
    }

   void react_Leave()
    {
     outside();
    }

   // signals

   Signal<bool> changed; // check
   Signal<bool> tabbed;  // shift
 };

/* type CheckWindow */

using CheckWindow = CheckWindowOf<CheckShape> ;

/* type SwitchWindow */

using SwitchWindow = CheckWindowOf<SwitchShape> ;

/* class RadioItem */

class RadioItem : NoCopy
 {
   int radio_id;

   DLink<RadioItem> link;
   RadioGroup *group = 0 ;

  protected:

   void setCheck(bool signal);

   virtual void check(bool check)=0;

   virtual void uncheck()=0;

   friend class RadioGroup;

  public:

   explicit RadioItem(int radio_id);

   ~RadioItem();
 };

/* class RadioGroup */

class RadioGroup : NoCopy
 {
   using Algo = DLink<RadioItem>::LinearAlgo<&RadioItem::link> ;

   Algo::FirstLast list;
   RadioItem *cur = 0 ;

   friend class RadioItem;

  public:

   RadioGroup();

   ~RadioGroup();

   void add(RadioItem *item);

   void del(RadioItem *item);

   void add(RadioItem &item) { add(&item); }

   template <class ... TT>
   void add(RadioItem *item,TT * ... tt)
    {
     add(item);

     add(tt...);
    }

   template <class ... TT>
   void add(RadioItem &item,TT && ... tt)
    {
     add(item);

     add(tt...);
    }

   // signals

   Signal<int,int> changed; // new_id, prev_id
 };

/* class RadioWindowOf<Shape> */

template <class Shape>
class RadioWindowOf : public SubWindow , public RadioItem
 {
   Shape shape;

  private:

   void inside()
    {
     if( Change(shape.mover,true) ) redraw();
    }

   void outside()
    {
     if( Change(shape.mover,false) ) redraw();
    }

   void select(bool signal=true)
    {
     if( !shape.check )
       {
        shape.check=true;

        setCheck(signal);

        redraw();
       }
    }

   virtual void check(bool check)
    {
     shape.check=check;
    }

   virtual void uncheck()
    {
     shape.check=false;

     redraw();
    }

  public:

   using ShapeType = Shape ;
   using ConfigType = typename Shape::Config ;

   template <class ... TT>
   RadioWindowOf(SubWindowHost &host,int radio_id,TT && ... tt)
    : SubWindow(host),
      RadioItem(radio_id),
      shape( std::forward<TT>(tt)... )
    {
    }

   virtual ~RadioWindowOf() {}

   // methods

   auto getMinSize() const { return shape.getMinSize(); }

   bool isGoodSize(Point size) const { return shape.isGoodSize(size); }

   bool isEnabled() const { return shape.enable; }

   void enable(bool enable=true)
    {
     if( Change(shape.enable,enable) ) redraw();
    }

   void disable() { enable(false); }

   bool isChecked() const { return shape.check; }

   void check() { select(false); }

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
     shape.mover=false;
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
     if( Change(shape.focus,false) ) redraw();
    }

   // mouse

   virtual MouseShape getMouseShape(Point,KeyMod) const
    {
     if( shape.enable ) return Mouse_Hand;

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
        case VKey_Enter :
        case VKey_Space :
         {
          if( shape.enable ) select();
         }
        break;

        case VKey_Tab :
         {
          tabbed.assert(kmod&KeyMod_Shift);
         }
        break;
       }
    }

   void react_LeftClick(Point point,MouseKey)
    {
     if( shape.enable && shape.pane.contains(point) ) select();
    }

   void react_LeftDClick(Point point,MouseKey mkey)
    {
     react_LeftClick(point,mkey);
    }

   void react_Move(Point point,MouseKey)
    {
     if( shape.pane.contains(point) )
       {
        inside();
       }
     else
       {
        outside();
       }
    }

   void react_Leave()
    {
     outside();
    }

   // signals

   Signal<bool> tabbed; // shift
 };

/* type RadioWindow */

using RadioWindow = RadioWindowOf<RadioShape> ;

/* class TextWindowOf<Shape> */

template <class Shape>
class TextWindowOf : public SubWindow
 {
   Shape shape;

  public:

   using ShapeType = Shape ;
   using ConfigType = typename Shape::Config ;

   template <class ... TT>
   TextWindowOf(SubWindowHost &host,TT && ... tt)
    : SubWindow(host),
      shape( std::forward<TT>(tt)... )
    {
    }

   virtual ~TextWindowOf() {}

   // methods

   auto getMinSize() const { return shape.getMinSize(); }

   Point getMinSize(StrLen text) const { return shape.getMinSize(text); }

   bool isGoodSize(Point size) const { return shape.isGoodSize(size); }

   bool isEnabled() const { return shape.enable; }

   void enable(bool enable=true)
    {
     if( Change(shape.enable,enable) ) redraw();
    }

   void disable() { enable(false); }

   DefString getText() const { return shape.text; }

   void setText(const DefString &text)
    {
     shape.text=text;

     redraw();
    }

   template <class ... TT>
   void printf(const char *format,const TT & ... tt)
    {
     setText( Stringf(format,tt...) );
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

   // keyboard

   virtual FocusType askFocus() const
    {
     return NoFocus;
    }
 };

/* type TextWindow */

using TextWindow = TextWindowOf<TextShape> ;

/* type LabelWindow */

using LabelWindow = TextWindowOf<LabelShape> ;

/* class DecorWindowOf<Shape> */

template <class Shape>
class DecorWindowOf : public SubWindow
 {
   Shape shape;

  public:

   using ShapeType = Shape ;
   using ConfigType = typename Shape::Config ;

   template <class ... TT>
   DecorWindowOf(SubWindowHost &host,TT && ... tt)
    : SubWindow(host),
      shape( std::forward<TT>(tt)... )
    {
    }

   virtual ~DecorWindowOf() {}

   // methods

   auto getMinSize() const { return shape.getMinSize(); }

   Point getMinSize(Point inner_size) const { return shape.getMinSize(inner_size); }

   bool isGoodSize(Point size) const { return shape.isGoodSize(size); }

   Pane getInner() const { return shape.getInner()+getPlace().getBase(); }

   // drawing

   virtual void layout()
    {
     shape.pane=Pane(Null,getSize());
    }

   virtual void draw(DrawBuf buf,bool) const
    {
     try { shape.draw(buf); } catch(CatchType) {}
    }

   // keyboard

   virtual FocusType askFocus() const
    {
     return NoFocus;
    }
 };

/* type XSingleLineWindow */

using XSingleLineWindow = DecorWindowOf<XSingleLineShape> ;

/* type YSingleLineWindow */

using YSingleLineWindow = DecorWindowOf<YSingleLineShape> ;

/* type XDoubleLineWindow */

using XDoubleLineWindow = DecorWindowOf<XDoubleLineShape> ;

/* type YDoubleLineWindow */

using YDoubleLineWindow = DecorWindowOf<YDoubleLineShape> ;

/* type ContourWindow */

using ContourWindow = DecorWindowOf<ContourShape> ;

/* type TextContourWindow */

using TextContourWindow = DecorWindowOf<TextContourShape> ;

/* class LightWindowOf<Shape> */

template <class Shape>
class LightWindowOf : public SubWindow
 {
   Shape shape;

  public:

   using ShapeType = Shape ;
   using ConfigType = typename Shape::Config ;

   template <class ... TT>
   LightWindowOf(SubWindowHost &host,TT && ... tt)
    : SubWindow(host),
      shape( std::forward<TT>(tt)... )
    {
    }

   virtual ~LightWindowOf() {}

   // methods

   auto getMinSize() const { return shape.getMinSize(); }

   bool isGoodSize(Point size) const { return shape.isGoodSize(size); }

   VColor getFace() const { return shape.face; }

   void setFace(VColor face)
    {
     if( Change(shape.face,face) ) redraw();
    }

   bool isOn() const { return shape.on; }

   void turn(bool on)
    {
     if( Change(shape.on,on) ) redraw();
    }

   void turnOn() { turn(true); }

   void turnOff() { turn(false); }

   // drawing

   virtual void layout()
    {
     shape.pane=Pane(Null,getSize());
    }

   virtual void draw(DrawBuf buf,bool) const
    {
     try { shape.draw(buf); } catch(CatchType) {}
    }

   // keyboard

   virtual FocusType askFocus() const
    {
     return NoFocus;
    }
 };

/* type LightWindow */

using LightWindow = LightWindowOf<LightShape> ;

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

/* class ProgressWindowOf<Shape> */

template <class Shape>
class ProgressWindowOf : public SubWindow
 {
   Shape shape;

   DeferInput<ProgressWindowOf<Shape> > input;

   DeferTick defer_tick;

  private:

   void tick()
    {
     if( shape.time )
       {
        shape.time--;

        if( shape.tick() )
          {
           shape.nextActive();

           redraw();
          }
       }
     else
       {
        stopPing();
       }
    }

  public:

   using ShapeType = Shape ;
   using ConfigType = typename Shape::Config ;

   template <class ... TT>
   ProgressWindowOf(SubWindowHost &host,TT && ... tt)
    : SubWindow(host),
      shape( std::forward<TT>(tt)... ),
      input(this)
    {
     defer_tick=input.create(&ProgressWindowOf<Shape>::tick);
    }

   virtual ~ProgressWindowOf() {}

   // methods

   auto getMinSize() const { return shape.getMinSize(); }

   bool isGoodSize(Point size) const { return shape.isGoodSize(size); }

   unsigned getPos() const { return shape.pos; }

   unsigned getTotal() const { return shape.total; }

   void setTotal(unsigned total)
    {
     shape.total=total;
     shape.pos=0;
     shape.stopActive();

     redraw();
    }

   void setPos(unsigned pos)
    {
     if( Change(shape.pos,pos) )
       {
        shape.adjustPos();

        redraw();
       }
    }

   void setPosPing(unsigned pos)
    {
     if( Change(shape.pos,pos) )
       {
        shape.adjustPos();

        shape.resetTime();

        defer_tick.start();

        shape.startActive();

        redraw();
       }
    }

   void ping()
    {
     shape.resetTime();

     defer_tick.start();

     if( shape.startActive() ) redraw();
    }

   void stopPing()
    {
     defer_tick.stop();

     shape.stopActive();

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

   // keyboard

   virtual FocusType askFocus() const
    {
     return NoFocus;
    }
 };

/* type ProgressWindow */

using ProgressWindow = ProgressWindowOf<ProgressShape> ;

/* class InfoWindowOf<Shape> */

template <class Shape>
class InfoWindowOf : public SubWindow
 {
   Shape shape;

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

        redraw();
       }
    }

   void setYOff(ulen yoff)
    {
     if( yoff>shape.yoffMax ) yoff=shape.yoffMax;

     if( yoff!=shape.yoff )
       {
        shape.yoff=yoff;

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

   void drag(Point point)
    {
     Coord delta=IntSub(point.x,shape.drag_base.x);

     setXOff( IntSub(shape.xoff_base,delta) );
    }

  public:

   using ShapeType = Shape ;
   using ConfigType = typename Shape::Config ;

   template <class ... TT>
   InfoWindowOf(SubWindowHost &host,TT && ... tt)
    : SubWindow(host),
      shape( std::forward<TT>(tt)... )
    {
    }

   virtual ~InfoWindowOf()
    {
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

   void setInfo(const Info &info)
    {
     shape.info=info;
     shape.yoff=0;
     shape.xoff=0;

     shape.setMax();

     redraw();
    }

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
     shape.drag=false;
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
     shape.drag=false;
    }

   virtual MouseShape getMouseShape(Point,KeyMod) const
    {
     if( shape.xoffMax>0 || shape.xoff>0 ) return Mouse_SizeLeftRight;

     return Mouse_Arrow;
    }

   // user input

   virtual void react(UserAction action)
    {
     action.dispatch(*this);
    }

   void react_Key(VKey vkey,KeyMod,unsigned repeat)
    {
     switch( vkey )
       {
        case VKey_Left :
         {
          Coord delta_x=Coord(repeat)*shape.dxoff;

          setXOff( IntSub(shape.xoff,delta_x) );
         }
        break;

        case VKey_Right :
         {
          Coord delta_x=Coord(repeat)*shape.dxoff;

          setXOff( IntAdd(shape.xoff,delta_x) );
         }
        break;

        case VKey_Up :
         {
          subYOff(repeat);
         }
        break;

        case VKey_Down :
         {
          addYOff(repeat);
         }
        break;
       }
    }

   void react_LeftClick(Point point,MouseKey)
    {
     if( !shape.drag )
       {
        shape.drag=true;

        shape.drag_base=point;
        shape.xoff_base=shape.xoff;

        captureMouse();
       }
    }

   void react_LeftUp(Point point,MouseKey)
    {
     if( shape.drag )
       {
        shape.drag=false;

        releaseMouse();

        drag(point);
       }
    }

   void react_LeftDClick(Point point,MouseKey mkey)
    {
     react_LeftClick(point,mkey);
    }

   void react_Move(Point point,MouseKey mkey)
    {
     if( shape.drag )
       {
        if( mkey&MouseKey_Left )
          {
           drag(point);
          }
        else
          {
           shape.drag=false;

           releaseMouse();
          }
       }
    }

   void react_Wheel(Point,MouseKey,Coord delta)
    {
     if( delta>0 )
       {
        addYOff(IntDist<Coord>(0,delta));
       }
     else
       {
        subYOff(IntDist<Coord>(delta,0));
       }
    }
 };

/* type InfoWindow */

using InfoWindow = InfoWindowOf<InfoShape> ;

/* class LineEditWindowOf<Shape> */

template <class Shape>
class LineEditWindowOf : public SubWindow
 {
   static const ulen DefBufLen = 1_KByte ;

   SimpleArray<char> text_buf;

   Shape shape;

   DeferInput<LineEditWindowOf<Shape> > input;

   DeferTick defer_tick;

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

        redraw();
       }
    }

   void insChar(char ch)
    {
     if( shape.len<text_buf.getLen() )
       {
        shape.len++;

        auto r=RangeReverse(text_buf.getPtr()+shape.pos,shape.len-shape.pos);

        for(; r.len>1 ;++r) r[0]=r[1];

        r[0]=ch;

        shape.pos++;
       }
    }

   void delRange(ulen off,ulen len)
    {
     if( off>=shape.len || len==0 ) return;

     Replace_min<ulen>(len,shape.len-off);

     ulen tail_off=off+len;
     ulen tail_len=shape.len-tail_off;

     char *base=text_buf.getPtr();
     auto src=Range(base+tail_off,tail_len);
     auto dst=base+off;

     for(; +src ;++src,++dst) *dst=*src;

     shape.len-=len;
    }

   void delChar(unsigned repeat=1)
    {
     delRange(shape.pos,repeat);

     shape.setMax();

     Replace_min(shape.xoff,shape.xoffMax);

     shape.showCursor();

     changed.assert();

     redraw();
    }

   void delTextSelection()
    {
     if( !shape.select_len ) return;

     delRange(shape.select_off,shape.select_len);

     shape.pos=shape.select_off;
     shape.select_off=0;
     shape.select_len=0;
    }

   void delSelection()
    {
     delTextSelection();

     shape.setMax();

     shape.cursor=true;

     shape.showCursor();

     changed.assert();

     redraw();
    }

   void cut()
    {
     copy();

     delSelection();
    }

   void copy()
    {
     if( !shape.select_len ) return;

     getWindowHost()->textToClipboard(Range(text_buf.getPtr()+shape.select_off,shape.select_len));
    }

   void past()
    {
     delTextSelection();

     CopyFunction func(text_buf.getPtr()+shape.len,text_buf.getLen()-shape.len);

     getWindowHost()->textFromClipboard(func.function_copy());

     func.cutEOL();

     ulen len=func.buf.len;

     shape.len+=len;

     Algon::EuclidRotate_suffix(Range(text_buf.getPtr()+shape.pos,shape.len-shape.pos),len);

     shape.pos+=len;

     shape.setMax();

     shape.cursor=true;

     shape.showCursor();

     changed.assert();

     redraw();
    }

   void startDrag(Point point)
    {
     if( !shape.drag )
       {
        shape.drag=true;

        shape.drag_base=point;
        shape.xoff_base=shape.xoff;

        captureMouse();
       }
    }

   void dragTo(Point point)
    {
     Coord delta=IntSub(point.x,shape.drag_base.x);

     setXOff( IntSub(shape.xoff_base,delta) );
    }

   void endDrag()
    {
     shape.drag=false;

     releaseMouse();
    }

   void endDrag(Point point)
    {
     shape.drag=false;

     releaseMouse();

     dragTo(point);
    }

   void select(ulen pos1,ulen pos2)
    {
     if( pos1<pos2 )
       {
        shape.select_off=pos1;
        shape.select_len=pos2-pos1;
       }
     else
       {
        shape.select_off=pos2;
        shape.select_len=pos1-pos2;
       }
    }

   void select(ulen new_pos)
    {
     ulen epos=shape.select_off+shape.select_len;

     if( shape.pos==shape.select_off )
       {
        select(epos,new_pos);
       }
     else if( shape.pos==epos )
       {
        select(shape.select_off,new_pos);
       }
     else
       {
        shape.select_off=0;
        shape.select_len=0;
       }
    }

   void posCursor(Point point)
    {
     ulen new_pos=shape.getPosition(point);

     if( shape.mouse_pos )
       {
        if( shape.select_len )
          {
           select(new_pos);
          }
        else
          {
           select(shape.pos,new_pos);
          }

        shape.pos=new_pos;
       }
     else
       {
        shape.mouse_pos=true;

        shape.pos=new_pos;
        shape.select_off=0;
        shape.select_len=0;
       }

     redraw();
    }

   void posCursorEnd()
    {
     shape.mouse_pos=false;
    }

   void posCursorEnd(Point point)
    {
     posCursor(point);

     posCursorEnd();
    }

   void keyLeft(bool shift,unsigned repeat=1)
    {
     if( repeat>shape.pos ) repeat=(unsigned)shape.pos;

     if( repeat )
       {
        if( shift )
          {
           if( shape.select_len )
             {
              if( shape.pos==shape.select_off )
                {
                 shape.select_off-=repeat;
                 shape.select_len+=repeat;
                }
              else if( shape.pos==shape.select_off+shape.select_len )
                {
                 if( shape.select_len>repeat )
                   {
                    shape.select_len-=repeat;
                   }
                 else
                   {
                    shape.select_off=shape.pos-repeat;
                    shape.select_len=repeat-(unsigned)shape.select_len;
                   }
                }
              else
                {
                 shape.select_off=0;
                 shape.select_len=0;
                }
             }
           else
             {
              shape.select_off=shape.pos-repeat;
              shape.select_len=repeat;
             }
          }
        else
          {
           shape.select_off=0;
           shape.select_len=0;
          }

        shape.cursor=true;
        shape.pos-=repeat;

        shape.showCursor();

        redraw();
       }
    }

   void keyRight(bool shift,unsigned repeat=1)
    {
     ulen rest=shape.len-shape.pos;

     if( repeat>rest ) repeat=(unsigned)rest;

     if( repeat )
       {
        if( shift )
          {
           if( shape.select_len )
             {
              if( shape.pos==shape.select_off )
                {
                 if( shape.select_len>repeat )
                   {
                    shape.select_off+=repeat;
                    shape.select_len-=repeat;
                   }
                 else
                   {
                    shape.select_off+=shape.select_len;
                    shape.select_len=repeat-(unsigned)shape.select_len;
                   }
                }
              else if( shape.pos==shape.select_off+shape.select_len )
                {
                 shape.select_len+=repeat;
                }
              else
                {
                 shape.select_off=0;
                 shape.select_len=0;
                }
             }
           else
             {
              shape.select_off=shape.pos;
              shape.select_len=repeat;
             }
          }
        else
          {
           shape.select_off=0;
           shape.select_len=0;
          }

        shape.cursor=true;
        shape.pos+=repeat;

        shape.showCursor();

        redraw();
       }
    }

   void keyHome(bool shift)
    {
     if( shape.pos )
       {
        if( shift )
          {
           if( shape.select_len )
             {
              if( shape.pos==shape.select_off )
                {
                 shape.select_off-=shape.pos;
                 shape.select_len+=shape.pos;
                }
              else if( shape.pos==shape.select_off+shape.select_len )
                {
                 shape.select_len=shape.select_off;
                 shape.select_off=0;
                }
              else
                {
                 shape.select_off=0;
                 shape.select_len=0;
                }
             }
           else
             {
              shape.select_off=0;
              shape.select_len=shape.pos;
             }
          }
        else
          {
           shape.select_off=0;
           shape.select_len=0;
          }

        shape.cursor=true;
        shape.pos=0;
        shape.xoff=0;

        redraw();
       }
     else
       {
        shape.xoff=0;

        redraw();
       }
    }

   void keyEnd(bool shift)
    {
     if( shape.pos<shape.len )
       {
        if( shift )
          {
           if( shape.select_len )
             {
              if( shape.pos==shape.select_off )
                {
                 shape.select_off=shape.select_off+shape.select_len;
                 shape.select_len=shape.len-shape.select_off;
                }
              else if( shape.pos==shape.select_off+shape.select_len )
                {
                 shape.select_len+=shape.len-shape.pos;
                }
              else
                {
                 shape.select_off=0;
                 shape.select_len=0;
                }
             }
           else
             {
              shape.select_off=shape.pos;
              shape.select_len=shape.len-shape.pos;
             }
          }
        else
          {
           shape.select_off=0;
           shape.select_len=0;
          }

        shape.cursor=true;
        shape.pos=shape.len;
        shape.xoff=shape.xoffMax;

        redraw();
       }
     else
       {
        shape.xoff=shape.xoffMax;

        redraw();
       }
    }

   void tick()
    {
     if( shape.tick()  )
       {
        shape.cursor=!shape.cursor;

        redraw();
       }
    }

  public:

   using ShapeType = Shape ;
   using ConfigType = typename Shape::Config ;

   template <class ... TT>
   LineEditWindowOf(SubWindowHost &host,TT && ... tt)
    : SubWindow(host),
      text_buf(DefBufLen),
      shape(Range(text_buf), std::forward<TT>(tt)... ),
      input(this)
    {
     defer_tick=input.create(&LineEditWindowOf<Shape>::tick);
    }

   template <class ... TT>
   LineEditWindowOf(SubWindowHost &host,ulen buf_len,TT && ... tt)
    : SubWindow(host),
      text_buf(buf_len),
      shape(Range(text_buf), std::forward<TT>(tt)... ),
      input(this)
    {
     defer_tick=input.create(&LineEditWindowOf<Shape>::tick);
    }

   virtual ~LineEditWindowOf()
    {
    }

   // methods

   auto getMinSize() const { return shape.getMinSize(); }

   Point getMinSize(StrLen sample_text) const { return shape.getMinSize(sample_text); }

   bool isGoodSize(Point size) const { return shape.isGoodSize(size); }

   bool isEnabled() const { return shape.enable; }

   bool isAlerted() const { return shape.alert; }

   void enable(bool enable=true)
    {
     if( enable )
       {
        if( !shape.enable )
          {
           shape.enable=true;

           if( shape.focus )
             {
              shape.cursor=true;

              defer_tick.start();
             }

           redraw();
          }
       }
     else
       {
        if( shape.enable )
          {
           shape.enable=false;
           shape.cursor=false;

           defer_tick.stop();

           redraw();
          }
       }
    }

   void disable() { enable(false); }

   void hideInactiveCursor(bool flag=true)
    {
     if( Change(shape.hide_cursor,flag) ) redraw();
    }

   void alert(bool on=true)
    {
     if( Change(shape.alert,on) ) redraw();
    }

   StrLen getText() const { return Range(text_buf.getPtr(),shape.len); }

   PtrLen<char> getBuf() { return Range(text_buf); }

   void setTextLen(ulen len)
    {
     shape.cursor=false;
     shape.drag=false;

     shape.len=Min(len,text_buf.getLen());
     shape.xoff=0;
     shape.pos=0;
     shape.select_off=0;
     shape.select_len=0;

     shape.setMax();

     redraw();
    }

   void setText(StrLen text)
    {
     auto out=getBuf();

     Replace_min(text.len,out.len);

     text.copyTo(out.ptr);

     setTextLen(text.len);
    }

   template <class ... TT>
   void printf(const char *format,const TT & ... tt)
    {
     PrintBuf out(getBuf());

     Printf(out,format,tt...);

     out.guardOverflow();

     setTextLen( out.close().len );
    }

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
     shape.cursor=false;
     shape.drag=false;
     shape.mouse_pos=false;

     shape.xoff=0;
     shape.pos=0;
     shape.select_off=0;
     shape.select_len=0;
    }

   // keyboard

   virtual FocusType askFocus() const
    {
     return shape.enable?FocusOk:NoFocus;
    }

   virtual void gainFocus()
    {
     shape.focus=true;

     if( shape.enable )
       {
        shape.cursor=true;

        defer_tick.start();
       }

     redraw();
    }

   virtual void looseFocus()
    {
     shape.focus=false;
     shape.cursor=false;

     defer_tick.stop();

     redraw();
    }

   // mouse

   virtual void looseCapture()
    {
     shape.drag=false;
    }

   virtual MouseShape getMouseShape(Point,KeyMod kmod) const
    {
     if( !shape.enable ) return Mouse_Arrow;

     if( ( shape.xoffMax>0 || shape.xoff>0 ) && (kmod&KeyMod_Ctrl) ) return Mouse_SizeLeftRight;

     return Mouse_IBeem;
    }

   // user input

   virtual void react(UserAction action)
    {
     action.dispatch(*this);
    }

   void react_Key(VKey vkey,KeyMod kmod)
    {
     if( shape.enable )
       {
        switch( vkey )
          {
           case VKey_Left :
            {
             if( kmod&KeyMod_Ctrl )
               {
                Coord delta_x=shape.dxoff;

                setXOff( IntSub(shape.xoff,delta_x) );
               }
             else
               {
                keyLeft(kmod&KeyMod_Shift);
               }
            }
           break;

           case VKey_Right :
            {
             if( kmod&KeyMod_Ctrl )
               {
                Coord delta_x=shape.dxoff;

                setXOff( IntAdd(shape.xoff,delta_x) );
               }
             else
               {
                keyRight(kmod&KeyMod_Shift);
               }
            }
           break;

           case VKey_Home :
            {
             keyHome(kmod&KeyMod_Shift);
            }
           break;

           case VKey_End :
            {
             keyEnd(kmod&KeyMod_Shift);
            }
           break;

           case VKey_c :
            {
             if( kmod&KeyMod_Ctrl ) copy();
            }
           break;

           case VKey_v :
            {
             if( kmod&KeyMod_Ctrl ) past();
            }
           break;

           case VKey_Insert :
            {
             if( kmod&KeyMod_Ctrl )
               copy();
             else if( kmod&KeyMod_Shift )
               past();
            }
           break;

           case VKey_Delete :
            {
             if( shape.select_len )
               {
                if( kmod&KeyMod_Shift )
                  cut();
                else
                  delSelection();
               }
             else
               {
                if( !(kmod&KeyMod_Shift) )
                  {
                   if( shape.pos<shape.len ) delChar();
                  }
               }
            }
           break;

           case VKey_BackSpace :
            {
             if( shape.select_len )
               {
                delSelection();
               }
             else
               {
                if( shape.pos )
                  {
                   shape.cursor=true;
                   shape.pos--;

                   delChar();
                  }
               }
            }
           break;

           case VKey_Enter :
            {
             entered.assert();
            }
           break;
          }
       }
     else
       {
        switch( vkey )
          {
           case VKey_Left :
            {
             if( kmod&KeyMod_Ctrl )
               {
                Coord delta_x=shape.dxoff;

                setXOff( IntSub(shape.xoff,delta_x) );
               }
            }
           break;

           case VKey_Right :
            {
             if( kmod&KeyMod_Ctrl )
               {
                Coord delta_x=shape.dxoff;

                setXOff( IntAdd(shape.xoff,delta_x) );
               }
            }
           break;
          }
       }
    }

   void react_Key(VKey vkey,KeyMod kmod,unsigned repeat)
    {
     if( shape.enable )
       {
        switch( vkey )
          {
           case VKey_Left :
            {
             if( kmod&KeyMod_Ctrl )
               {
                Coord delta_x=shape.dxoff*repeat;

                setXOff( IntSub(shape.xoff,delta_x) );
               }
             else
               {
                keyLeft(kmod&KeyMod_Shift,repeat);
               }
            }
           break;

           case VKey_Right :
            {
             if( kmod&KeyMod_Ctrl )
               {
                Coord delta_x=shape.dxoff*repeat;

                setXOff( IntAdd(shape.xoff,delta_x) );
               }
             else
               {
                keyRight(kmod&KeyMod_Shift,repeat);
               }
            }
           break;

           case VKey_Home :
            {
             keyHome(kmod&KeyMod_Shift);
            }
           break;

           case VKey_End :
            {
             keyEnd(kmod&KeyMod_Shift);
            }
           break;

           case VKey_c :
            {
             if( kmod&KeyMod_Ctrl ) copy();
            }
           break;

           case VKey_v :
            {
             if( kmod&KeyMod_Ctrl ) past();
            }
           break;

           case VKey_Insert :
            {
             if( kmod&KeyMod_Ctrl )
               copy();
             else if( kmod&KeyMod_Shift )
               past();
            }
           break;

           case VKey_Delete :
            {
             if( shape.select_len )
               {
                if( kmod&KeyMod_Shift )
                  cut();
                else
                  delSelection();
               }
             else
               {
                if( !(kmod&KeyMod_Shift) )
                  {
                   delChar(repeat);
                  }
               }
            }
           break;

           case VKey_BackSpace :
            {
             if( shape.select_len )
               {
                delSelection();
               }
             else
               {
                if( repeat>shape.pos ) repeat=(unsigned)shape.pos;

                if( repeat )
                  {
                   shape.cursor=true;
                   shape.pos-=repeat;

                   delChar(repeat);
                  }
               }
            }
           break;

           case VKey_Enter :
            {
             entered.assert();
            }
           break;
          }
       }
     else
       {
        switch( vkey )
          {
           case VKey_Left :
            {
             if( kmod&KeyMod_Ctrl )
               {
                Coord delta_x=shape.dxoff*repeat;

                setXOff( IntSub(shape.xoff,delta_x) );
               }
            }
           break;

           case VKey_Right :
            {
             if( kmod&KeyMod_Ctrl )
               {
                Coord delta_x=shape.dxoff*repeat;

                setXOff( IntAdd(shape.xoff,delta_x) );
               }
            }
           break;
          }
       }
    }

   void react_Char(char ch)
    {
     if( shape.enable && CharIsPrintable(ch) )
       {
        delTextSelection();

        insChar(ch);

        shape.setMax();

        shape.showCursor();

        changed.assert();

        redraw();
       }
    }

   void react_LeftClick(Point point,MouseKey mkey)
    {
     if( !shape.enable ) return;

     if( mkey&MouseKey_Ctrl )
       {
        startDrag(point);
       }
     else
       {
        posCursor(point);
       }
    }

   void react_LeftUp(Point point,MouseKey)
    {
     if( shape.drag )
       {
        endDrag(point);
       }
     else
       {
        if( shape.enable ) posCursorEnd(point); else posCursorEnd();
       }
    }

   void react_LeftDClick(Point point,MouseKey mkey)
    {
     react_LeftClick(point,mkey);
    }

   void react_Move(Point point,MouseKey mkey)
    {
     if( shape.drag )
       {
        if( mkey&MouseKey_Left )
          {
           dragTo(point);
          }
        else
          {
           endDrag();
          }
       }
     else if( mkey&MouseKey_Left )
       {
        if( shape.enable ) posCursor(point);
       }
     else
       {
        posCursorEnd();
       }
    }

   void react_Leave()
    {
     posCursorEnd();
    }

   void react_Wheel(Point,MouseKey mkey,Coord delta)
    {
     if( mkey&MouseKey_Ctrl )
       {
        if( delta>0 )
          {
           unsigned repeat=IntDist<Coord>(0,delta);

           Coord delta_x=shape.dxoff*repeat;

           setXOff( IntSub(shape.xoff,delta_x) );
          }
        else if( delta<0 )
          {
           unsigned repeat=IntDist<Coord>(delta,0);

           Coord delta_x=shape.dxoff*repeat;

           setXOff( IntAdd(shape.xoff,delta_x) );
          }
       }
     else if( shape.enable )
       {
        if( delta>0 )
          {
           unsigned repeat=IntDist<Coord>(0,delta);

           keyLeft(mkey&MouseKey_Shift,repeat);
          }
        else if( delta<0 )
          {
           unsigned repeat=IntDist<Coord>(delta,0);

           keyRight(mkey&MouseKey_Shift,repeat);
          }
       }
    }

   // signals

   Signal<> entered;
   Signal<> changed;
 };

/* type LineEditWindow */

using LineEditWindow = LineEditWindowOf<LineEditShape> ;

/* class SimpleTextListWindowOf<Shape> */

template <class Shape>
class SimpleTextListWindowOf : public SubWindow
 {
   Shape shape;

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

        selected.assert(shape.select);

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

           selected.assert(shape.select);

           redraw();
          }
       }
    }

   void addSelect(ulen delta)
    {
     ulen count=shape.info->getLineCount();

     if( delta && count && shape.select<count-1 )
       {
        shape.select+=Min<ulen>(delta,count-1-shape.select);

        shape.showSelect();

        selected.assert(shape.select);

        redraw();
       }
    }

   void subSelect(ulen delta)
    {
     if( delta && shape.select )
       {
        shape.select=PosSub(shape.select,delta);

        shape.showSelect();

        selected.assert(shape.select);

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

        if( signal ) selected.assert(shape.select);

        redraw();
       }
    }

  public:

   using ShapeType = Shape ;
   using ConfigType = typename ShapeType::Config ;

   template <class ... TT>
   SimpleTextListWindowOf(SubWindowHost &host,TT && ... tt)
    : SubWindow(host),
      shape( std::forward<TT>(tt)... )
    {
    }

   virtual ~SimpleTextListWindowOf()
    {
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

   void setInfo(const Info &info)
    {
     shape.info=info;
     shape.yoff=0;
     shape.xoff=0;
     shape.select=0;

     shape.setMax();

     redraw();
    }

   const Info & getInfo() const { return shape.info; }

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
          entered.assert();
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

     dclicked.assert();
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

   Signal<> entered;
   Signal<> dclicked;
   Signal<ulen> selected; // select
 };

/* type SimpleTextListWindow */

using SimpleTextListWindow = SimpleTextListWindowOf<SimpleTextListShape> ;

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


