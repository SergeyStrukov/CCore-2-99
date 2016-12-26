/* Window.SimpleTextList.h */
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

#ifndef CCore_inc_video_lib_Window_SimpleTextList_h
#define CCore_inc_video_lib_Window_SimpleTextList_h

#include <CCore/inc/video/SubWindow.h>

#include <CCore/inc/video/lib/Shape.SimpleTextList.h>

namespace CCore {
namespace Video {

/* classes */

template <class Shape> class SimpleTextListWindowOf;

/* class SimpleTextListWindowOf<Shape> */

template <class Shape>
class SimpleTextListWindowOf : public SubWindow
 {
   Shape shape;

  private:

   void setXOff(Coord xoff)
    {
     if( Change(shape.xoff,Cap<Coord>(0,xoff,shape.xoffMax)) ) redraw();
    }

   void shiftX(Coordinate count)
    {
     Coordinate dx=shape.xoff-count*shape.dxoff;

     setXOff(+dx);
    }

   void setYOff(ulen yoff)
    {
     if( Change(shape.yoff,Min(yoff,shape.yoffMax)) ) redraw();
    }

   void addYOff(ulen delta)
    {
     setYOff(AddToCap(shape.yoff,delta,shape.yoffMax));
    }

   void subYOff(ulen delta)
    {
     setYOff(PosSub(shape.yoff,delta));
    }

   void selectFirst()
    {
     setSelect(0);
    }

   void selectLast()
    {
     if( ulen count=shape.info->getLineCount() )
       {
        setSelect2(count-1,count);
       }
    }

   void setSelect2(ulen select,ulen count,bool signal=true)
    {
     if( select>=count )
       {
        select=count?count-1:0;
       }

     if( Change(shape.select,select) )
       {
        shape.showSelect();

        if( signal ) selected.assert(shape.select);

        redraw();
       }
    }

   void setSelect(ulen select,bool signal=true)
    {
     setSelect2(select,shape.info->getLineCount(),signal);
    }

   void addSelect(ulen delta)
    {
     if( ulen count=shape.info->getLineCount() )
       {
        setSelect2(AddToCap(shape.select,delta,count-1),count);
       }
    }

   void subSelect(ulen delta)
    {
     setSelect(PosSub(shape.select,delta));
    }

  public:

   using ShapeType = Shape ;
   using ConfigType = typename Shape::Config ;

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

   virtual bool isGoodSize(Point size) const { return shape.isGoodSize(size); }

   virtual void layout()
    {
     shape.pane=Pane(Null,getSize());

     shape.setMax();
    }

   virtual void draw(DrawBuf buf,bool) const
    {
     shape.draw(buf);
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
     if( Change(shape.focus,true) ) redraw();
    }

   virtual void looseFocus()
    {
     if( Change(shape.focus,false) ) redraw();
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
          shiftX(CountToCoordinate(repeat));
         }
        break;

        case VKey_Right :
         {
          shiftX(-CountToCoordinate(repeat));
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

     ulen delta=IntAbs(delta_);

     if( delta_>0 )
       {
        if( mkey&MouseKey_Shift )
          {
           addYOff(delta);
          }
        else
          {
           addSelect(delta);
          }
       }
     else
       {
        if( mkey&MouseKey_Shift )
          {
           subYOff(delta);
          }
        else
          {
           subSelect(delta);
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

} // namespace Video
} // namespace CCore

#endif

