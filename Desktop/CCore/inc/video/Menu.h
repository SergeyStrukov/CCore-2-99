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

#include <CCore/inc/video/MinSizeType.h>
#include <CCore/inc/video/Color.h>
#include <CCore/inc/video/Font.h>
#include <CCore/inc/video/RefVal.h>

#include <CCore/inc/video/ToolFrame.h>
#include <CCore/inc/video/Layout.h>

namespace CCore {
namespace Video {

/* consts */

enum MenuType
 {
  MenuText,
  MenuDisabled,
  MenuHidden,
  MenuSeparator,

  MenuTextNoHot
 };

enum MenuState : unsigned
 {
  MenuNone    =    0,
  MenuHilight = 0x01,
  MenuSelect  = 0x02
 };

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

  MenuPoint() noexcept : type(MenuHidden),text("<not defined>"_def),hotindex(0),hotkey(0),id(-1) {}

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

  Signal<> updated;

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

  FindResult findFirst() const;

  bool isGood(ulen index) const
   {
    return index<list.getLen() && list[index].type==MenuText ;
   }
 };

/* class SimpleTopMenuShape */

class SimpleTopMenuShape
 {
   static Coordinate GetDX(const MenuPoint &point,Font font,Coordinate space,Coordinate dy);

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

   void draw(const DrawBuf &buf,const MenuPoint &point,Pane pane,Font font,VColor vc,bool showhot=false) const;

   void draw(const DrawBuf &buf,Pane pane) const;

  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<VColor> gray     =      Gray ;
     RefVal<VColor> snow     =      Snow ;
     RefVal<VColor> back     =    Silver ;
     RefVal<VColor> text     =     Black ;
     RefVal<VColor> inactive =      Gray ;
     RefVal<VColor> hilight  =      Blue ;
     RefVal<VColor> select   = OrangeRed ;
     RefVal<VColor> hot      =       Red ;

     RefVal<Point> space = Point(4,4) ;

     RefVal<bool> hotcolor = true ;

     RefVal<Font> font;

     Config() noexcept {}

     template <class Bag>
     void bind(const Bag &bag)
      {
       width.bind(bag.width);
       gray.bind(bag.gray);
       snow.bind(bag.snow);
       inactive.bind(bag.inactive);

       back.bind(bag.menu_back);
       text.bind(bag.menu_text);
       hilight.bind(bag.menu_hilight);
       select.bind(bag.menu_select);
       hot.bind(bag.menu_hot);
       space.bind(bag.menu_space);
       hotcolor.bind(bag.menu_hotcolor);
       font.bind(bag.menu_font.font);
      }
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
   Coord xoff = 0 ;

   Coord xoffMax = 0 ;
   Coord dxoff = 0 ;

   // methods

   SimpleTopMenuShape(const Config &cfg_,MenuData &data_) : cfg(cfg_),data(data_) {}

   SizeY getMinSize() const;

   bool isGoodSize(Point size) const;

   void layout();

   void draw(const DrawBuf &buf) const;
 };

/* class SimpleTopMenuWindowOf<Shape> */

template <class Shape>
class SimpleTopMenuWindowOf : public SubWindow
 {
   Shape shape;

  private:

   void setXOff(Coord xoff)
    {
     if( Change(shape.xoff,Cap<Coord>(0,xoff,shape.xoffMax)) ) redraw();
    }

   void addXOff(Coordinate delta)
    {
     Coordinate dx=shape.xoff+delta*shape.dxoff;

     setXOff(+dx);
    }

   void assert()
    {
     const MenuPoint &point=shape.data.list.at(shape.select_index);

     selected.assert(point.id,toScreen(point.place.addDY()-Point(shape.xoff,0)));
    }

   void select(ulen index)
    {
     if( !shape.data.isGood(index) ) return;

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
     if( !shape.data.isGood(index) ) return;

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

   void getUp()
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

   void getDown()
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

   auto getMinSize() const { return shape.getMinSize(); }

   unsigned getState() const { return shape.state; }

   void unselect()
    {
     if( Change<unsigned>(shape.state,MenuNone) ) redraw();
    }

   bool forwardChar(char ch)
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

   virtual bool isGoodSize(Point size) const
    {
     return shape.isGoodSize(size);
    }

   virtual void layout()
    {
     shape.pane=Pane(Null,getSize());

     shape.layout();
    }

   virtual void draw(DrawBuf buf,bool) const
    {
     shape.draw(buf);
    }

   // base

   virtual void open()
    {
     shape.focus=false;
     shape.state=MenuNone;
     shape.xoff=0;
     shape.xoffMax=0;
    }

   // keyboard

   virtual void gainFocus()
    {
     if( Change(shape.focus,true) )
       {
        if( !BitTest(shape.state,MenuHilight) )
          {
           auto result=shape.data.findFirst();

           if( result.found )
             {
              BitSet(shape.state,MenuHilight);

              shape.hilight_index=result.index;
             }
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
             addXOff(-1);
            }
          else
            {
             getUp();
            }
         }
        break;

        case VKey_Right :
         {
          if( kmod&KeyMod_Shift )
            {
             addXOff(+1);
            }
          else
            {
             getDown();
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
     auto result=shape.data.find(point+Point(shape.xoff,0));

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
     auto result=shape.data.find(point+Point(shape.xoff,0));

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
     setFocus();

     getFrameHost()->setFocus();

     addXOff(delta);
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

     RefVal<VColor> back     =    Silver ;
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
   Coord xoff = 0 ;

   Coord xoffMax = 0 ;
   Coord dxoff = 0 ;

  private:

   Coord cell_dy = 0 ;

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

   void setXOff(Coord xoff)
    {
     if( Change(shape.xoff,Cap<Coord>(0,xoff,shape.xoffMax)) ) redraw();
    }

   void addXOff(Coordinate delta)
    {
     Coordinate dx=shape.xoff+delta*shape.dxoff;

     setXOff(+dx);
    }

   void assert()
    {
     const MenuPoint &point=shape.data->list.at(shape.select_index);

     selected.assert(point.id,toScreen(point.place.addDX()-Point(0,shape.xoff)));
    }

   void select(ulen index)
    {
     if( !shape.data || !shape.data->isGood(index) ) return;

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

   void del(ulen index)
    {
     if( !shape.data || !shape.data->isGood(index) ) return;

     const MenuPoint &point=shape.data->list[index];

     deleted.assert(point.id);
    }

   void hilight(ulen index)
    {
     if( !shape.data || !shape.data->isGood(index) ) return;

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

   void getUp()
    {
     if( !shape.data ) return;

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

   void getDown()
    {
     if( !shape.data ) return;

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

   auto getMinSize() const { return shape.getMinSize(); }

   unsigned getState() const { return shape.state; }

   void bind(MenuData &data) { shape.data=&data; }

   void unselect()
    {
     if( Change<unsigned>(shape.state,MenuNone) ) redraw();
    }

   bool forwardChar(char ch)
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

   virtual bool isGoodSize(Point size) const
    {
     return shape.isGoodSize(size);
    }

   virtual void layout()
    {
     shape.pane=Pane(Null,getSize());

     shape.layout();
    }

   virtual void draw(DrawBuf buf,bool) const
    {
     shape.draw(buf);
    }

   // base

   virtual void open()
    {
     shape.focus=false;
     shape.state=MenuNone;
     shape.xoff=0;
     shape.xoffMax=0;
    }

   // keyboard

   virtual void gainFocus()
    {
     if( Change(shape.focus,true) )
       {
        if( !BitTest(shape.state,MenuHilight) && shape.data )
          {
           auto result=shape.data->findFirst();

           if( result.found )
             {
              BitSet(shape.state,MenuHilight);

              shape.hilight_index=result.index;
             }
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

        case VKey_Delete :
         {
          if( BitTest(shape.state,MenuHilight) )
            {
             del(shape.hilight_index);
            }
         }
        break;

        case VKey_Esc :
         {
          askFrameClose();
         }
        break;

        case VKey_Left :
         {
          if( kmod&KeyMod_Shift )
            {
             addXOff(-1);
            }
          else
            {
             pressed.assert(vkey,kmod);
            }
         }
        break;

        case VKey_Right :
         {
          if( kmod&KeyMod_Shift )
            {
             addXOff(+1);
            }
          else
            {
             pressed.assert(vkey,kmod);
            }
         }
        break;

        case VKey_Up :
         {
          getUp();
         }
        break;

        case VKey_Down :
         {
          getDown();
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

     auto result=shape.data->find(point+Point(0,shape.xoff));

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

     auto result=shape.data->find(point+Point(0,shape.xoff));

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
     setFocus();

     getFrameHost()->setFocus();

     addXOff(delta);
    }

   // signals

   Signal<int,Point> selected; // id , cascade menu point

   Signal<VKey,KeyMod> pressed; // for Left and Right keys

   Signal<int> deleted; // id
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
     CtorRefVal<ToolFrame::ConfigType> frame_cfg;
     CtorRefVal<typename Shape::Config> menu_cfg;

     Config() noexcept {}

     template <class Proxy>
     void bind(Proxy proxy)
      {
       frame_cfg.bind(proxy);
       menu_cfg.bind(proxy);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   ToolFrame frame;
   SimpleCascadeMenuWindowOf<Shape> client;
   Point screen_size;

  private:

   void move(Point delta)
    {
     if( frame.isAlive() )
       {
        frame.move(delta);
       }
    }

   SignalConnector<SimpleCascadeMenuOf<Shape>,Point> connector_moved;

   SignalConnector<SimpleCascadeMenuOf<Shape> > connector_updated;

  public:

   template <class ... TT>
   SimpleCascadeMenuOf(Desktop *desktop,const Config &cfg_,TT && ... tt)
    : cfg(cfg_),
      frame(desktop,cfg.frame_cfg),
      client(frame,cfg.menu_cfg, std::forward<TT>(tt)... ),

      connector_moved(this,&SimpleCascadeMenuOf<Shape>::move),
      connector_updated(this,&SimpleCascadeMenuOf<Shape>::update)
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

     screen_size=frame.getScreenSize();

     Pane pane=FitToScreen(base,size,screen_size);

     frame.create(parent,pane);

     frame.grabFocus();

     connector_moved.connect(parent->moved);

     connector_updated.connect(data.updated);
    }

   void update()
    {
     if( frame.isAlive() )
       {
        Point size=client.getMinSize();

        if( size<=screen_size )
          {
           frame.setMaxSize(size);

           frame.resize(size);
          }

        frame.input.redrawAll(true);
       }
    }

   void unselect() { client.unselect(); }

   void destroy() { frame.destroy(); }

   // signals

   Signal<> & takeDestroyed() { return frame.destroyed; }

   Signal<int,Point> & takeSelected() { return client.selected; } // id , cascade menu point

   Signal<int> & takeDeleted() { return client.deleted; } // id

   Signal<VKey,KeyMod> & takePressed() { return client.pressed; } // for Left and Right keys
 };

/* type SimpleCascadeMenu */

using SimpleCascadeMenu = SimpleCascadeMenuOf<SimpleCascadeMenuShape> ;

} // namespace Video
} // namespace CCore

#endif

