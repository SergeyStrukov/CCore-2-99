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
#include <CCore/inc/video/ToolWindow.h>
#include <CCore/inc/video/Layout.h>

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

enum MenuState : unsigned
 {
  MenuNone    =    0,
  MenuHilight = 0x01,
  MenuSelect  = 0x02
 };

/* functions */

inline Point TopCascadePoint(Pane pane)
 {
  return pane.getBase().addY(pane.dy);
 }

inline Point CascadePoint(Pane pane)
 {
  return pane.getBase().addX(pane.dx);
 }

/* classes */

struct MenuPoint;

struct MenuData;

class SimpleTopMenuShape;

template <class Shape> class SimpleTopMenuWindowOf;

class SimpleCascadeMenuShape;

template <class Shape> class SimpleCascadeMenuWindowOf;

template <class Shape> class SimpleCascadeMenuOf;

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

     RefVal<bool> use_hotcolor = true ;

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
   unsigned state = MenuNone ;
   ulen hilight_index = 0 ;
   ulen select_index = 0 ;
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
     const MenuPoint &point=shape.data.list.at(shape.select_index);

     selected.assert(point.id,toScreen(TopCascadePoint(point.place)-Point(shape.off,0)));
    }

   void select(ulen index)
    {
     if( index>=shape.data.list.getLen() ) return;

     if( BitTest(shape.state,MenuSelect) )
       {
        if( Change(shape.select_index,index) )
          {
           assert();

           redraw();
          }
       }
     else
       {
        BitSet(shape.state,MenuSelect);

        shape.select_index=index;

        assert();

        redraw();
       }
    }

   void hilight(ulen index)
    {
     if( index>=shape.data.list.getLen() ) return;

     if( BitTest(shape.state,MenuHilight) )
       {
        if( Change(shape.hilight_index,index) )
          {
           redraw();
          }
       }
     else
       {
        BitSet(shape.state,MenuHilight);

        shape.hilight_index=index;

        redraw();
       }
    }

   void hilightOff()
    {
     if( BitTest(shape.state,MenuHilight) && !shape.focus )
       {
        BitClear(shape.state,MenuHilight);

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

   unsigned getState() const { return shape.state; }

   void unselect()
    {
     if( Change<unsigned>(shape.state,MenuNone) ) redraw();
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
     if( Change(shape.focus,true) )
       {
        if( !BitTest(shape.state,MenuHilight) && shape.data.list.getLen() )
          {
           BitSet(shape.state,MenuHilight);

           shape.hilight_index=0;
          }

        redraw();
       }
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
     switch( vkey )
       {
        case VKey_Enter :
        case VKey_Space :
        case VKey_Down :
         {
          if( BitTest(shape.state,MenuHilight) )
            {
             select(shape.hilight_index);
            }
         }
        break;

        case VKey_Left :
         {
          if( kmod&KeyMod_Shift )
            {
             changeOff(-1);
            }
          else
            {
             if( BitTest(shape.state,MenuSelect) )
               {
                auto result=shape.data.findDown(shape.select_index);

                if( result.found )
                  {
                   select(result.index);
                  }
               }
             else if( BitTest(shape.state,MenuHilight) )
               {
                auto result=shape.data.findDown(shape.hilight_index);

                if( result.found )
                  {
                   hilight(result.index);
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
             if( BitTest(shape.state,MenuSelect) )
               {
                auto result=shape.data.findUp(shape.select_index);

                if( result.found )
                  {
                   select(result.index);
                  }
               }
             else if( BitTest(shape.state,MenuHilight) )
               {
                auto result=shape.data.findUp(shape.hilight_index);

                if( result.found )
                  {
                   hilight(result.index);
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
     auto result=shape.data.find(point+Point(shape.off,0));

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

     RefVal<bool> use_hotcolor = true ;

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
   MenuData *data = 0 ;
   Pane pane;

   // state

   bool focus = false ;
   unsigned state = MenuNone ;
   ulen hilight_index = 0 ;
   ulen select_index = 0 ;
   Coord off = 0 ;
   Coord max_off = 0 ;
   Coord cell_dy = 0 ;

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

   void draw_Frame(const DrawBuf &buf) const;

   void draw_Menu(const DrawBuf &buf) const;

  public:

   explicit SimpleCascadeMenuShape(const Config &cfg_) : cfg(cfg_) {}

   void layout();

   Point getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void draw(const DrawBuf &buf) const;
 };

/* class SimpleCascadeMenuWindowOf<Shape> */

template <class Shape>
class SimpleCascadeMenuWindowOf : public SubWindow
 {
   Shape shape;

  private:

   void assert()
    {
     const MenuPoint &point=shape.data->list.at(shape.select_index);

     selected.assert(point.id,toScreen(CascadePoint(point.place)-Point(0,shape.off)));
    }

   void select(ulen index)
    {
     if( !shape.data || index>=shape.data->list.getLen() ) return;

     if( BitTest(shape.state,MenuSelect) )
       {
        if( Change(shape.select_index,index) )
          {
           assert();

           redraw();
          }
       }
     else
       {
        BitSet(shape.state,MenuSelect);

        shape.select_index=index;

        assert();

        redraw();
       }
    }

   void hilight(ulen index)
    {
     if( !shape.data || index>=shape.data->list.getLen() ) return;

     if( BitTest(shape.state,MenuHilight) )
       {
        if( Change(shape.hilight_index,index) )
          {
           redraw();
          }
       }
     else
       {
        BitSet(shape.state,MenuHilight);

        shape.hilight_index=index;

        redraw();
       }
    }

   void hilightOff()
    {
     if( BitTest(shape.state,MenuHilight) && !shape.focus )
       {
        BitClear(shape.state,MenuHilight);

        redraw();
       }
    }

   void changeOff(Coord delta)
    {
     delta*=shape.cell_dy/2;

     Coord new_off=Cap<Coord>(0,shape.off+delta,shape.max_off);

     if( Change(shape.off,new_off) ) redraw();
    }

  public:

   using ShapeType = Shape ;
   using ConfigType = typename Shape::Config ;

   template <class ... TT>
   SimpleCascadeMenuWindowOf(SubWindowHost &host,TT && ... tt)
    : SubWindow(host),
      shape( std::forward<TT>(tt)... )
    {
    }

   virtual ~SimpleCascadeMenuWindowOf() {}

   // methods

   Point getMinSize() const { return shape.getMinSize(); }

   bool isGoodSize(Point size) const { return shape.isGoodSize(size); }

   unsigned getState() const { return shape.state; }

   void bind(MenuData &data) { shape.data=&data; }

   void unselect()
    {
     if( Change<unsigned>(shape.state,MenuNone) ) redraw();
    }

   bool forward(char ch)
    {
     if( !shape.data ) return false;

     auto result=shape.data->find(ch);

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
     if( Change(shape.focus,true) )
       {
        if( !BitTest(shape.state,MenuHilight) && shape.data && shape.data->list.getLen() )
          {
           BitSet(shape.state,MenuHilight);

           shape.hilight_index=0;
          }

        redraw();
       }
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
     switch( vkey )
       {
        case VKey_Enter :
        case VKey_Space :
         {
          if( BitTest(shape.state,MenuHilight) )
            {
             select(shape.hilight_index);
            }
         }
        break;

        case VKey_Esc :
         {
          askFrameClose();
         }
        break;

        case VKey_Left :
        case VKey_Right :
         {
          pressed.assert(vkey,kmod);
         }
        break;

        case VKey_Up :
         {
          if( kmod&KeyMod_Shift )
            {
             changeOff(-1);
            }
          else
            {
             if( !shape.data ) break;

             if( BitTest(shape.state,MenuSelect) )
               {
                auto result=shape.data->findDown(shape.select_index);

                if( result.found )
                  {
                   select(result.index);
                  }
               }
             else if( BitTest(shape.state,MenuHilight) )
               {
                auto result=shape.data->findDown(shape.hilight_index);

                if( result.found )
                  {
                   hilight(result.index);
                  }
               }
            }
         }
        break;

        case VKey_Down :
         {
          if( kmod&KeyMod_Shift )
            {
             changeOff(+1);
            }
          else
            {
             if( !shape.data ) break;

             if( BitTest(shape.state,MenuSelect)  )
               {
                auto result=shape.data->findUp(shape.select_index);

                if( result.found )
                  {
                   select(result.index);
                  }
               }
             else if( BitTest(shape.state,MenuHilight) )
               {
                auto result=shape.data->findUp(shape.hilight_index);

                if( result.found )
                  {
                   hilight(result.index);
                  }
               }
            }
         }
        break;
       }
    }

   void react_Char(char ch)
    {
     if( !shape.data ) return;

     auto result=shape.data->find(ch);

     if( result.found )
       {
        select(result.index);
       }
    }

   void react_LeftClick(Point point,MouseKey)
    {
     if( !shape.data ) return;

     auto result=shape.data->find(point+Point(0,shape.off));

     if( result.found )
       {
        select(result.index);
       }
    }

   void react_LeftDClick(Point point,MouseKey mkey)
    {
     react_LeftClick(point,mkey);
    }

   void react_RightClick(Point,MouseKey)
    {
     askFrameClose();
    }

   void react_Move(Point point,MouseKey)
    {
     if( !shape.data ) return;

     auto result=shape.data->find(point+Point(0,shape.off));

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

   Signal<VKey,KeyMod> pressed; // for Left and Right keys
 };

/* type SimpleCascadeMenuWindow */

using SimpleCascadeMenuWindow = SimpleCascadeMenuWindowOf<SimpleCascadeMenuShape> ;

/* class SimpleCascadeMenuOf<Shape> */

template <class Shape>
class SimpleCascadeMenuOf
 {
  public:

   struct Config
    {
     CtorRefVal<ToolWindow::ConfigType> frame_cfg;
     CtorRefVal<typename Shape::Config> menu_cfg;

     Config() noexcept {}
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   ToolWindow frame;
   SimpleCascadeMenuWindowOf<Shape> client;

   SignalConnector<SimpleCascadeMenuOf<Shape>,Point> connector_moved;

  private:

   void moved(Point delta)
    {
     if( frame.isAlive() ) frame.move(delta);
    }

  public:

   template <class ... TT>
   SimpleCascadeMenuOf(Desktop *desktop,const Config &cfg_,TT && ... tt)
    : cfg(cfg_),
      frame(desktop,cfg.frame_cfg),
      client(frame,cfg.menu_cfg, std::forward<TT>(tt)... ),
      connector_moved(this,&SimpleCascadeMenuOf<Shape>::moved)
    {
     frame.bindClient(client);
    }

   ~SimpleCascadeMenuOf() {}

   // methods

   bool isAlive() const { return frame.isAlive(); }

   bool isDead() const { return frame.isDead(); }

   void create(FrameWindow *parent,MenuData &data,Point base)
    {
     client.bind(data);

     Point size=client.getMinSize();

     Point screen_size=frame.getDesktop()->getScreenSize();

     Pane pane=FitToScreen(base,size,screen_size);

     frame.create(parent,pane);

     frame.getHost()->setFocus();

     connector_moved.disconnect();
     connector_moved.connect(parent->moved);
    }

   void unselect() { client.unselect(); }

   void destroy() { frame.destroy(); }

   // signals

   Signal<int,Point> & takeSelected() { return client.selected; } // id , cascade menu point

   Signal<VKey,KeyMod> & takePressed() { return client.pressed; } // for Left and Right keys
 };

/* type SimpleCascadeMenu */

using SimpleCascadeMenu = SimpleCascadeMenuOf<SimpleCascadeMenuShape> ;

} // namespace Video
} // namespace CCore

#endif

