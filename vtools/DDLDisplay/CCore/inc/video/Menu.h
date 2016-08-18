/* Menu.h */
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

#ifndef CCore_inc_video_Menu_h
#define CCore_inc_video_Menu_h

#include <CCore/inc/video/SubWindow.h>
#include <CCore/inc/video/Font.h>
#include <CCore/inc/video/RefVal.h>

namespace CCore {
namespace Video {

/* consts */

enum MenuType
 {
  MenuText,
  MenuDisabled,
  MenuHidden,
  MenuSeparator
 };

enum MenuState
 {
  MenuNone,
  MenuHilight,
  MenuSelect
 };

/* classes */

struct MenuPoint;

struct MenuData;

class SimpleTopMenuShape;

template <class Shape> class SimpleTopMenuOf;

/* struct MenuPoint */

struct MenuPoint
 {
  MenuType type;
  DefString text;
  int id;

  // internal

  Pane place;
  char hotkey = 0 ;

  // constructors

  MenuPoint() noexcept : type(MenuHidden),text("<not defined>"),id(-1) {}

  MenuPoint(DefString text_,int id_) noexcept : type(MenuText),text(text_),id(id_) {}

  MenuPoint(MenuType type_,DefString text_,int id_) noexcept : type(type_),text(text_),id(id_) {}

  MenuPoint(MenuType type_) noexcept : MenuPoint() { type=type_; }

  ~MenuPoint() {}
 };

/* struct MenuData */

struct MenuData : NoCopy
 {
  DynArray<MenuPoint> list;

  Signal<> update;

  // constructors

  MenuData() noexcept {}

  template <class ... TT> requires ( ... && IsType<TT,MenuPoint> )
  explicit MenuData(TT ... tt) : list{tt...} {}

  template <class ... SS>
  MenuData & operator () (SS && ... ss)
   {
    list.append_fill( std::forward<SS>(ss)... );

    return *this;
   }
 };

/* class SimpleTopMenuShape */

class SimpleTopMenuShape
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<Point> space = Point(8,8) ;

     RefVal<VColor> ground   = Silver ;
     RefVal<VColor> text     =  Black ;
     RefVal<VColor> inactive =   Gray ;
     RefVal<VColor> hilight  =  Green ;

     RefVal<Font> font;

     Config() noexcept {}
    };

   // parameters

   const Config &cfg;
   MenuData &data;
   Pane pane;

   // state

   MenuState state = MenuNone ;
   ulen index = 0 ;

   // methods

   SimpleTopMenuShape(const Config &cfg_,MenuData &data_) : cfg(cfg_),data(data_) {}

   void prepare();

   Point getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void draw(const DrawBuf &buf) const;
 };

/* class SimpleTopMenuOf<Shape> */

template <class Shape>
class SimpleTopMenuOf : public SubWindow
 {
   Shape shape;

  public:

   using ShapeType = Shape ;
   using ConfigType = typename Shape::Config ;

   template <class ... TT>
   SimpleTopMenuOf(SubWindowHost &host,TT && ... tt)
    : SubWindow(host),
      shape( std::forward<TT>(tt)... )
    {
    }

   virtual ~SimpleTopMenuOf() {}

   // methods

   Point getMinSize() const { return shape.getMinSize(); }

   bool isGoodSize(Point size) const { return shape.isGoodSize(size); }

   void unselect()
    {
     if( Change(shape.state,MenuNone) ) redraw();
    }

   bool forward(char ch)
    {
     Used(ch);

     return false;
    }

   // drawing

   virtual void layout()
    {
     shape.pane=Pane(Null,getSize());

     shape.prepare();
    }

   virtual void draw(DrawBuf buf,bool) const
    {
     try { shape.draw(buf); } catch(CatchType) {}
    }

   // base

   virtual void open()
    {
     shape.state=MenuNone;
    }

   // user input

   virtual void react(UserAction action)
    {
     action.dispatch(*this);
    }

   void react_Key(VKey vkey,KeyMod kmod)
    {
    }

   void react_Char(char ch)
    {
    }

   void react_LeftClick(Point point,MouseKey)
    {
    }

   void react_LeftDClick(Point point,MouseKey mkey)
    {
     react_LeftClick(point,mkey);
    }

   void react_Move(Point point,MouseKey)
    {
     Used(point);
    }

   void react_Leave()
    {
    }

   // signals

   Signal<int,Point> selected; // id , cascade menu point
 };

/* type SimpleTopMenu */

using SimpleTopMenu = SimpleTopMenuOf<SimpleTopMenuShape> ;

} // namespace Video
} // namespace CCore

#endif

