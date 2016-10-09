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

/* functions */

inline Point TopCascadePoint(Pane pane)
 {
  return pane.getBase().addY(pane.dy);
 }

/* classes */

struct MenuPoint;

struct MenuData;

class SimpleTopMenuShape;

template <class Shape> class SimpleTopMenuWindowOf;

class SimpleCascadeMenuShape;

template <class Shape> class SimpleCascadeMenuWindowOf;

/* struct MenuPoint */

struct MenuPoint
 {
  static const char Hot = '@' ;

  MenuType type;
  DefString text;
  ulen hotindex;
  char hotkey;
  int id;

  // internal

  Pane place;

  void pickhot();

  // constructors

  MenuPoint() noexcept : type(MenuHidden),text("<not defined>"),hotindex(0),hotkey(0),id(-1) {}

  MenuPoint(DefString text_,int id_) noexcept : type(MenuText),text(text_),id(id_) { pickhot(); }

  MenuPoint(MenuType type_,DefString text_,int id_) noexcept : type(type_),text(text_),id(id_) { pickhot(); }

  MenuPoint(MenuType type_) noexcept : MenuPoint() { type=type_; }

  ~MenuPoint() {}

  // methods

  bool test(char ch) const { return type==MenuText && hotindex && ToLowerCase(hotkey)==ToLowerCase(ch) ; }

  bool test(Point point) const { return type==MenuText && place.contains(point); }
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

  // methods

  template <class ... SS>
  MenuData & operator () (SS && ... ss)
   {
    list.append_fill( std::forward<SS>(ss)... );

    return *this;
   }

  struct FindResult
   {
    ulen index;
    bool found;
   };

  FindResult find(char ch) const;

  FindResult find(Point point) const;

  FindResult findDown(ulen index) const;

  FindResult findUp(ulen index) const;
 };

/* class SimpleTopMenuShape */

class SimpleTopMenuShape
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<Point> space = Point(4,4) ;

     RefVal<bool> use_hotcolor = false ;

     RefVal<VColor> ground   =    Silver ;
     RefVal<VColor> text     =     Black ;
     RefVal<VColor> inactive =      Gray ;
     RefVal<VColor> hilight  =      Blue ;
     RefVal<VColor> select   = OrangeRed ;
     RefVal<VColor> hot      =       Red ;
     RefVal<VColor> left     =      Snow ;
     RefVal<VColor> right    =      Gray ;

     RefVal<Font> font;

     Config() noexcept {}
    };

   // parameters

   const Config &cfg;
   MenuData &data;
   Pane pane;

   // state

   bool focus = false ;
   MenuState state = MenuNone ;
   ulen index = 0 ;
   Coord off = 0 ;
   Coord max_off = 0 ;

  private:

   static Coord GetDX(const MenuPoint &point,Font font,Coord space,Coord dy);

   struct PlaceFunc : Funchor
    {
     VColor vc;
     ulen index;
     Point base;
     Point delta;

     PlaceFunc(VColor vc_,ulen index_) : vc(vc_),index(index_) {}

     VColor place(ulen index,char ch,Point base,Point delta);

     CharFunction function_place() { return FunctionOf(this,&PlaceFunc::place); }
    };

   struct HotFunc : Funchor
    {
     VColor vc;
     ulen index;
     VColor hotc;

     HotFunc(VColor vc_,ulen index_,VColor hotc_) : vc(vc_),index(index_),hotc(hotc_) {}

     VColor hot(ulen index,char ch,Point base,Point delta);

     CharFunction function_hot() { return FunctionOf(this,&HotFunc::hot); }
    };

   static void Draw(const DrawBuf &buf,const MenuPoint &point,Pane pane,Font font,VColor vc,const Config &cfg,bool showhot=false);

   static void Draw(const DrawBuf &buf,Pane pane,const Config &cfg);

  public:

   SimpleTopMenuShape(const Config &cfg_,MenuData &data_) : cfg(cfg_),data(data_) {}

   void layout();

   Point getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void draw(const DrawBuf &buf) const;
 };

/* class SimpleTopMenuWindowOf<Shape> */

template <class Shape>
class SimpleTopMenuWindowOf : public SubWindow
 {
   Shape shape;

  private:

   void assert()
    {
     const MenuPoint &point=shape.data.list.at(shape.index);

     selected.assert(point.id,TopCascadePoint(point.place)-Point(shape.off,0));
    }

   void select(ulen index)
    {
     if( Change(shape.state,MenuSelect) )
       {
        shape.index=index;

        assert();

        redraw();
       }
     else
       {
        if( Change(shape.index,index) )
          {
           assert();

           redraw();
          }
       }
    }

   void hilight(ulen index)
    {
     if( shape.state==MenuNone )
       {
        shape.state=MenuHilight;
        shape.index=index;

        redraw();
       }
     else if( shape.state==MenuHilight )
       {
        if( Change(shape.index,index) )
          {
           redraw();
          }
       }
    }

   void hilightOff()
    {
     if( shape.state==MenuHilight )
       {
        shape.state=MenuNone;

        redraw();
       }
    }

   void changeOff(Coord delta)
    {
     delta*=shape.pane.dy/2;

     Coord new_off=Cap<Coord>(0,shape.off+delta,shape.max_off);

     if( Change(shape.off,new_off) ) redraw();
    }

  public:

   using ShapeType = Shape ;
   using ConfigType = typename Shape::Config ;

   template <class ... TT>
   SimpleTopMenuWindowOf(SubWindowHost &host,TT && ... tt)
    : SubWindow(host),
      shape( std::forward<TT>(tt)... )
    {
    }

   virtual ~SimpleTopMenuWindowOf() {}

   // methods

   Point getMinSize() const { return shape.getMinSize(); }

   bool isGoodSize(Point size) const { return shape.isGoodSize(size); }

   MenuState getState() const { return shape.state; }

   void unselect()
    {
     if( Change(shape.state,MenuNone) ) redraw();
    }

   bool forward(char ch)
    {
     auto result=shape.data.find(ch);

     if( result.found )
       {
        setFocus();

        select(result.index);

        return true;
       }

     return false;
    }

   // drawing

   virtual void layout()
    {
     shape.pane=Pane(Null,getSize());

     shape.layout();
    }

   virtual void draw(DrawBuf buf,bool) const
    {
     try { shape.draw(buf); } catch(CatchType) {}
    }

   // base

   virtual void open()
    {
     shape.focus=false;
     shape.state=MenuNone;
     shape.off=0;
     shape.max_off=0;
    }

   // keyboard

   virtual void gainFocus()
    {
     if( Change(shape.focus,true) ) redraw();
    }

   virtual void looseFocus()
    {
     if( Change(shape.focus,false) ) redraw();
    }

   // user input

   virtual void react(UserAction action)
    {
     action.dispatch(*this);
    }

   void react_Key(VKey vkey,KeyMod kmod)
    {
     Used(kmod);

     switch( vkey )
       {
        case VKey_Left :
         {
          if( kmod&KeyMod_Shift )
            {
             changeOff(-1);
            }
          else
            {
             if( shape.state==MenuSelect )
               {
                auto result=shape.data.findDown(shape.index);

                if( result.found )
                  {
                   select(result.index);
                  }
               }
            }
         }
        break;

        case VKey_Right :
         {
          if( kmod&KeyMod_Shift )
            {
             changeOff(+1);
            }
          else
            {
             if( shape.state==MenuSelect )
               {
                auto result=shape.data.findUp(shape.index);

                if( result.found )
                  {
                   select(result.index);
                  }
               }
            }
         }
        break;
       }
    }

   void react_Char(char ch)
    {
     auto result=shape.data.find(ch);

     if( result.found )
       {
        select(result.index);
       }
    }

   void react_LeftClick(Point point,MouseKey)
    {
     auto result=shape.data.find(point+Point(shape.off,0));

     if( result.found )
       {
        select(result.index);
       }
    }

   void react_LeftDClick(Point point,MouseKey mkey)
    {
     react_LeftClick(point,mkey);
    }

   void react_Move(Point point,MouseKey)
    {
     auto result=shape.data.find(point);

     if( result.found )
       {
        hilight(result.index);
       }
     else
       {
        hilightOff();
       }
    }

   void react_Leave()
    {
     hilightOff();
    }

   void react_Wheel(Point,MouseKey,Coord delta)
    {
     changeOff(delta);
    }

   // signals

   Signal<int,Point> selected; // id , cascade menu point
 };

/* type SimpleTopMenuWindow */

using SimpleTopMenuWindow = SimpleTopMenuWindowOf<SimpleTopMenuShape> ;

/* class SimpleCascadeMenuShape */

class SimpleCascadeMenuShape
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<Point> space = Point(4,4) ;

     RefVal<bool> use_hotcolor = false ;

     RefVal<VColor> ground   =    Silver ;
     RefVal<VColor> text     =     Black ;
     RefVal<VColor> inactive =      Gray ;
     RefVal<VColor> hilight  =      Blue ;
     RefVal<VColor> select   = OrangeRed ;
     RefVal<VColor> hot      =       Red ;
     RefVal<VColor> top      =      Snow ;
     RefVal<VColor> bottom   =      Gray ;

     RefVal<Font> font;

     Config() noexcept {}
    };

   // parameters

   const Config &cfg;
   MenuData &data;
   Pane pane;

   // state

   bool focus = false ;
   MenuState state = MenuNone ;
   ulen index = 0 ;
   Coord off = 0 ;
   Coord max_off = 0 ;

  private:

   static Coord GetDX(const MenuPoint &point,Font font,Coord space);

   struct PlaceFunc : Funchor
    {
     VColor vc;
     ulen index;
     Point base;
     Point delta;

     PlaceFunc(VColor vc_,ulen index_) : vc(vc_),index(index_) {}

     VColor place(ulen index,char ch,Point base,Point delta);

     CharFunction function_place() { return FunctionOf(this,&PlaceFunc::place); }
    };

   struct HotFunc : Funchor
    {
     VColor vc;
     ulen index;
     VColor hotc;

     HotFunc(VColor vc_,ulen index_,VColor hotc_) : vc(vc_),index(index_),hotc(hotc_) {}

     VColor hot(ulen index,char ch,Point base,Point delta);

     CharFunction function_hot() { return FunctionOf(this,&HotFunc::hot); }
    };

   static void Draw(const DrawBuf &buf,const MenuPoint &point,Pane pane,Font font,VColor vc,const Config &cfg,bool showhot=false);

   static void Draw(const DrawBuf &buf,Pane pane,const Config &cfg);

  public:

   SimpleCascadeMenuShape(const Config &cfg_,MenuData &data_) : cfg(cfg_),data(data_) {}

   void layout();

   Point getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void draw(const DrawBuf &buf) const;
 };

/* class SimpleCascadeMenuWindowOf<Shape> */

template <class Shape>
class SimpleCascadeMenuWindowOf
 {
 };

/* type SimpleCascadeMenuWindow */

using SimpleCascadeMenuWindow = SimpleCascadeMenuWindowOf<SimpleCascadeMenuShape> ;

} // namespace Video
} // namespace CCore

#endif

