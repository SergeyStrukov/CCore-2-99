/* Window.Spinor.h */
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
//  Copyright (c) 2017 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_video_lib_Window_Spinor_h
#define CCore_inc_video_lib_Window_Spinor_h

#include <CCore/inc/video/SubWindow.h>

#include <CCore/inc/video/lib/Shape.Spinor.h>

namespace CCore {
namespace Video {

/* classes */

template <class Shape> class SpinorWindowOf;

/* class SpinorWindowOf<Shape> */

template <class Shape>
class SpinorWindowOf : public SubWindow
 {
   Shape shape;

  private:

   void btnDown(typename Shape::ZoneType type) // TODO
    {
     Used(type);
    }

   void btnUp() // TODO
    {
    }

  public:

   using ShapeType = Shape ;
   using ConfigType = typename Shape::Config ;

   template <class ... TT>
   explicit SpinorWindowOf(SubWindowHost &host,TT && ... tt)
    : SubWindow(host),
      shape( std::forward<TT>(tt)... )
    {
    }

   virtual ~SpinorWindowOf() {}

   // methods

   auto getMinSize() const { return shape.getMinSize(); }

   bool isEnabled() const { return shape.enable; }

   void enable(bool enable=true)
    {
     if( Change(shape.enable,enable) ) redraw();
    }

   void disable() { enable(false); }

   int getValue() const { return shape.val; }

   void setValue(int val)
    {
     shape.val=Cap(shape.min_val,val,shape.max_val);

     redraw();
    }

   void setRange(int min_val,int max_val)
    {
     shape.min_val=min_val;
     shape.max_val=max_val;

     redraw();
    }

   void setOptions(const IntPrintOpt &opt)
    {
     shape.opt=opt;

     redraw();
    }

   void setOptions(StrLen str)
    {
     IntPrintOpt opt(str.ptr,str.ptr+str.len);

     setOptions(opt);
    }

   // drawing

   virtual bool isGoodSize(Point size) const
    {
     return shape.isGoodSize(size);
    }

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
     shape.mover=Shape::ZoneNone;
     shape.down=Shape::ZoneNone;
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
        shape.down=Shape::ZoneNone;

        btnUp();

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
        shape.mover=Shape::ZoneNone;
        shape.down=Shape::ZoneNone;

        btnUp();

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

   // signals

   Signal<int> changed; // value
   Signal<bool> tabbed; // shift
 };

/* type SpinorWindow */

using SpinorWindow = SpinorWindowOf<SpinorShape> ;

} // namespace Video
} // namespace CCore

#endif

