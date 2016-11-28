/* TextLineWindow.h */
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

#ifndef CCore_inc_video_TextLineWindow_h
#define CCore_inc_video_TextLineWindow_h

#include <CCore/inc/video/WindowLib.h>

namespace CCore {
namespace Video {

/* classes */

class TextLineShape;

template <class Shape> class TextLineWindowOf;

/* class TextLineShape */

class TextLineShape
 {
   static MCoord FigEX(Coord fdy,MCoord width);

  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<Point> space = Point(6,4) ;

     RefVal<VColor> bottom   =      Snow ;
     RefVal<VColor> top      =      Gray ;
     RefVal<VColor> back     =    Silver ;
     RefVal<VColor> focus    = OrangeRed ;
     RefVal<VColor> text     =     Black ;
     RefVal<VColor> inactive =      Gray ;
     RefVal<VColor> alert    =      Pink ;

     RefVal<Font> font;

     Config() noexcept {}
    };

   const Config &cfg;
   DefString text;
   Pane pane;

   // state

   bool enable =  true ;
   bool focus  = false ;
   bool alert  = false ;
   Coord xoff  =     0 ;

   Coord xoffMax = 0 ;
   Coord dxoff   = 0 ;

   // methods

   TextLineShape(const Config &cfg_,const DefString &text_) : cfg(cfg_),text(text_) {}

   explicit TextLineShape(const Config &cfg_) : cfg(cfg_) {}

   Point getMinSize() const;

   Point getMinSize(StrLen text) const;

   void setMax();

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void draw(const DrawBuf &buf) const;
 };

/* class TextLineWindowOf<Shape> */

template <class Shape>
class TextLineWindowOf : public SubWindow
 {
   Shape shape;

  public:

   using ShapeType = Shape ;
   using ConfigType = typename Shape::Config ;

   template <class ... TT>
   TextLineWindowOf(SubWindowHost &host,TT && ... tt)
    : SubWindow(host),
      shape( std::forward<TT>(tt)... )
    {
    }

   virtual ~TextLineWindowOf()
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
     if( Change(shape.enable,enable) ) redraw();
    }

   void disable() { enable(false); }

   void alert(bool on=true)
    {
     if( Change(shape.alert,on) ) redraw();
    }

   DefString getText() const { return shape.text; }

   void setText(const DefString &text)
    {
     shape.text=text;

     shape.setMax();

     shape.xoff=0;

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
     shape.xoff=0;
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

   virtual void looseCapture()
    {
    }

   virtual MouseShape getMouseShape(Point,KeyMod kmod) const
    {
     if( !shape.enable ) return Mouse_Arrow;

     if( ( shape.xoffMax>0 || shape.xoff>0 ) && (kmod&KeyMod_Ctrl) ) return Mouse_SizeLeftRight;

     return Mouse_Arrow;
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
 };

/* type TextLineWindow */

using TextLineWindow = TextLineWindowOf<TextLineShape> ;

} // namespace Video
} // namespace CCore

#endif

