/* AltWindow.h */
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

#ifndef CCore_inc_video_AltWindow_h
#define CCore_inc_video_AltWindow_h

#include <CCore/inc/video/WindowLib.h>

namespace CCore {
namespace Video {

/* classes */

class AltShape;

template <class Shape> class AltWindowOf;

/* class AltShape */

class AltShape
 {
  public:

   struct Config
    {
     RefVal<Coord> dy = 32 ;

     RefVal<VColor> border   =      Blue ;
     RefVal<VColor> focus    = OrangeRed ;

     RefVal<VColor> topUp    = PaleGreen ;
     RefVal<VColor> top      =      Snow ;
     RefVal<VColor> bottom   =      Gray ;

     RefVal<VColor> mark_false    = RGBColor(64,0,0) ;
     RefVal<VColor> mark_true     = RGBColor(0,64,0) ;
     RefVal<VColor> mark_false_on = RGBColor(255,100,100) ;
     RefVal<VColor> mark_true_on  = RGBColor(100,255,100) ;

     Config() noexcept {}
    };

   const Config &cfg;
   Pane pane;

   // state

   using CheckType = bool ;

   static CheckType Next(CheckType check) { return !check; }

   bool enable     =  true ;
   bool focus      = false ;
   bool mover      = false ;
   CheckType zone  = false ;
   CheckType check = false ;

   // methods

   explicit AltShape(const Config &cfg_,CheckType check_=false) : cfg(cfg_),check(check_) {}

   Point getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   CheckType getZone(Point point) const;

   void draw(const DrawBuf &buf) const;
 };

/* class AltWindowOf<Shape> */

template <class Shape>
class AltWindowOf : public SubWindow
 {
  public:

   using CheckType = typename Shape::CheckType ;

  private:

   Shape shape;

  private:

   void inside(CheckType zone)
    {
     if( Change(shape.mover,true) )
       {
        shape.zone=zone;

        redraw();
       }
     else
       {
        if( Change(shape.zone,zone) ) redraw();
       }
    }

   void outside()
    {
     if( Change(shape.mover,false) ) redraw();
    }

   void change()
    {
     shape.check=Shape::Next(shape.check);

     changed.assert(shape.check);

     redraw();
    }

   void change(CheckType zone)
    {
     if( Change(shape.check,zone) )
       {
        changed.assert(shape.check);

        redraw();
       }
    }

  public:

   using ShapeType = Shape ;
   using ConfigType = typename Shape::Config ;

   template <class ... TT>
   explicit AltWindowOf(SubWindowHost &host,TT && ... tt)
    : SubWindow(host),
      shape( std::forward<TT>(tt)... )
    {
    }

   virtual ~AltWindowOf() {}

   // methods

   auto getMinSize() const { return shape.getMinSize(); }

   bool isGoodSize(Point size) const { return shape.isGoodSize(size); }

   bool isEnabled() const { return shape.enable; }

   void enable(bool enable=true)
    {
     if( Change(shape.enable,enable) ) redraw();
    }

   void disable() { enable(false); }

   CheckType getCheck() const { return shape.check; }

   void check(CheckType check)
    {
     if( Change(shape.check,check) ) redraw();
    }

   // bool

   bool isChecked() const requires ( IsType<CheckType,bool> ) { return shape.check; }

   void check() requires ( IsType<CheckType,bool> ) { check(true); }

   void uncheck() requires ( IsType<CheckType,bool> ) { check(false); }

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
     if( shape.enable && shape.pane.contains(point) ) change(shape.getZone(point));
    }

   void react_LeftDClick(Point point,MouseKey mkey)
    {
     react_LeftClick(point,mkey);
    }

   void react_Move(Point point,MouseKey)
    {
     if( shape.pane.contains(point) )
       {
        inside(shape.getZone(point));
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

   Signal<CheckType> changed; // check
   Signal<bool> tabbed;       // shift
 };

/* type AltWindow */

using AltWindow = AltWindowOf<AltShape> ;

} // namespace Video
} // namespace CCore

#endif
