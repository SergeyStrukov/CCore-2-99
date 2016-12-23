/* ShapeLib.h */
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

#ifndef CCore_inc_video_ShapeLib_h
#define CCore_inc_video_ShapeLib_h

#include <CCore/inc/video/lib/Shape.Button.h>
#include <CCore/inc/video/lib/Shape.Check.h>
#include <CCore/inc/video/lib/Shape.Switch.h>
#include <CCore/inc/video/lib/Shape.Radio.h>
#include <CCore/inc/video/lib/Shape.Light.h>
#include <CCore/inc/video/lib/Shape.Label.h>
#include <CCore/inc/video/lib/Shape.Text.h>
#include <CCore/inc/video/lib/Shape.TextLine.h>
#include <CCore/inc/video/lib/Shape.Line.h>
#include <CCore/inc/video/lib/Shape.Contour.h>
#include <CCore/inc/video/lib/Shape.Progress.h>
#include <CCore/inc/video/lib/Shape.Info.h>

namespace CCore {
namespace Video {

/* classes */

class ScrollShape;

class XScrollShape;

class YScrollShape;

class LineEditShape;

class SimpleTextListShape;

class ScrollListShape;

/* enum ScrollType */

enum ScrollType
 {
  ScrollType_None = 0,

  ScrollType_Down,
  ScrollType_DownPage,
  ScrollType_Drag,
  ScrollType_UpPage,
  ScrollType_Up
 };

/* class ScrollShape */

class ScrollShape
 {
  protected:

   static unsigned Accelerate(unsigned change_count,unsigned period);

   static ulen Delta(ulen m,uCoord p,uCoord q);

   ulen delta(uCoord delta,uCoord len,uCoord dlen) const;

   void dragPos(Coord from,Coord to,uCoord len,uCoord dlen);

  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<Coord> dxy = 20 ;

     RefVal<VColor> top    =      Snow ;
     RefVal<VColor> back   =    Silver ;
     RefVal<VColor> bottom =      Gray ;
     RefVal<VColor> focus  = OrangeRed ;
     RefVal<VColor> face   =     Black ;
     RefVal<VColor> faceUp =      Blue ;

     RefVal<unsigned> speedUpPeriod = 12_tick ;

     Config() noexcept {}
    };

   const Config &cfg;
   Pane pane;

   // state

   ulen total = 0 ;
   ulen page  = 0 ; // page <= total
   ulen pos   = 0 ; // pos <= total-page

   bool enable=true;
   bool focus=false;
   ScrollType mover = ScrollType_None ;
   ScrollType down = ScrollType_None ;

   ScrollType press_type = ScrollType_None ;
   bool mouse = false ;
   Point drag_base;
   ulen drag_pos = 0 ;
   unsigned change_count = 0 ;

   // methods

   explicit ScrollShape(const Config &cfg_) : cfg(cfg_) {}

   ulen getMaxPos() const
    {
     if( page<total ) return total-page;

     return 0;
    }

   void adjustPos()
    {
     Replace_min(pos,getMaxPos());
    }

   void posMin()
    {
     pos=0;
    }

   void posMax()
    {
     pos=getMaxPos();
    }

   void posDown()
    {
     if( pos ) pos--;
    }

   void posDown(ulen d)
    {
     if( !d ) return;

     if( pos>d )
       pos-=d;
     else
       pos=0;
    }

   void posDownPage() { posDown(page); }

   void posUp()
    {
     ulen cap=getMaxPos();

     if( pos<cap ) pos++;
    }

   void posUp(ulen d)
    {
     if( !d ) return;

     ulen cap=getMaxPos();

     if( d<cap && pos<cap-d )
       pos+=d;
     else
       pos=cap;
    }

   void posUpPage() { posUp(page); }

   bool posChange()
    {
     ulen old_pos=pos;

     unsigned period=+cfg.speedUpPeriod;

     switch( press_type )
       {
        case ScrollType_Down :
         {
          posDown(Accelerate(change_count,period));
         }
        break;

        case ScrollType_Up :
         {
          posUp(Accelerate(change_count,period));
         }
        break;

        case ScrollType_DownPage :
         {
          if( (change_count%period)==0 ) posDownPage();
         }
        break;

        case ScrollType_UpPage :
         {
          if( (change_count%period)==0 ) posUpPage();
         }
        break;
       }

     change_count++;

     return old_pos!=pos;
    }
 };

/* class XScrollShape */

class XScrollShape : public ScrollShape
 {
  public:

   static const VKey DownKey = VKey_Left ;

   static const VKey UpKey = VKey_Right ;

   // methods

   explicit XScrollShape(const Config &cfg) : ScrollShape(cfg) {}

   SizeY getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize() && size.x>=5*size.y ; }

   ScrollType getType(MPoint point) const;

   void drag(Point point) { dragPos(drag_base.x,point.x,pane.dx,pane.dy); }

   void draw(const DrawBuf &buf) const;
 };

/* class YScrollShape */

class YScrollShape : public ScrollShape
 {
  public:

   static const VKey DownKey = VKey_Up ;

   static const VKey UpKey = VKey_Down ;

   // methods

   explicit YScrollShape(const Config &cfg) : ScrollShape(cfg) {}

   SizeX getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize() && size.y>=5*size.x ; }

   ScrollType getType(MPoint point) const;

   void drag(Point point) { dragPos(drag_base.y,point.y,pane.dy,pane.dx); }

   void draw(const DrawBuf &buf) const;
 };

/* class LineEditShape */

class LineEditShape
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<Point> space = Point(6,4) ;

     RefVal<Coord> ex = 3 ;
     RefVal<Coord> cursor_dx = 3 ;

     RefVal<VColor> back     =    Silver ;
     RefVal<VColor> bottom   =      Snow ;
     RefVal<VColor> top      =      Gray ;
     RefVal<VColor> focus    = OrangeRed ;

     RefVal<VColor> text     =     Black ;
     RefVal<VColor> inactive =      Gray ;
     RefVal<VColor> select   =    Yellow ;
     RefVal<VColor> alert    =      Pink ;

     RefVal<VColor> cursor   =      Blue ;

     RefVal<Font> font;

     RefVal<unsigned> period = 10_tick ;

     Config() noexcept {}
    };

   const Config &cfg;
   PtrLen<char> text_buf;
   Pane pane;

   // state

   bool enable =  true ;
   bool focus  = false ;
   bool cursor = false ;
   bool hide_cursor = false ;
   bool alert  = false ;
   ulen len    =     0 ;
   ulen pos    =     0 ;
   ulen select_off = 0 ;
   ulen select_len = 0 ;
   Coord xoff  =     0 ;

   Coord xoffMax = 0 ;
   Coord dxoff   = 0 ;

   bool drag = false ;
   Point drag_base;
   Coord xoff_base = 0 ;
   bool mouse_pos = false ;

   unsigned count = 0 ;

   // methods

   LineEditShape(PtrLen<char> text_buf_,const Config &cfg_) : cfg(cfg_),text_buf(text_buf_) {}

   Point getMinSize() const;

   Point getMinSize(StrLen sample_text) const;

   void setMax();

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   bool tick()
    {
     if( ++count >= +cfg.period )
       {
        count=0;

        return true;
       }

     return false;
    }

   void showCursor();

   ulen getPosition(Point point) const;

   virtual void drawText(Font font,const DrawBuf &buf,Pane pane,TextPlace place,StrLen text,VColor vc) const;

   void draw(const DrawBuf &buf) const;
 };

/* class SimpleTextListShape */

class SimpleTextListShape
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<Point> space = Point(8,8) ;

     RefVal<VColor> back   = Silver ;
     RefVal<VColor> top    =   Gray ;
     RefVal<VColor> bottom =   Snow ;

     RefVal<VColor> text     =     Black ;
     RefVal<VColor> inactive =      Gray ;
     RefVal<VColor> focus    = OrangeRed ;
     RefVal<VColor> select   =    Yellow ;

     RefVal<Font> font;

     Config() noexcept {}
    };

   // parameters

   const Config &cfg;
   Info info;
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

   explicit SimpleTextListShape(const Config &cfg_) : cfg(cfg_) {}

   SimpleTextListShape(const Config &cfg_,const Info &info_) : cfg(cfg_),info(info_) {}

   Point getMinSize() const;

   void setMax();

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void showSelect();

   ulen getPosition(Point point) const;

   void draw(const DrawBuf &buf) const;
 };

/* class ScrollListShape */

class ScrollListShape
 {
   static StrLen GetText(ComboInfoItem item) { return (item.type==ComboInfoText)?item.text:Empty; }

  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<Point> space = Point(8,8) ;

     RefVal<VColor> back   = Silver ;
     RefVal<VColor> top    =   Gray ;
     RefVal<VColor> bottom =   Snow ;

     RefVal<VColor> title_top    = Aqua ;
     RefVal<VColor> title_bottom = Gray ;

     RefVal<VColor> text     =     Black ;
     RefVal<VColor> title    =      Navy ;
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

   void initSelect();

   bool setSelectDown(ulen pos);

   bool setSelectUp(ulen pos);

   bool showSelect();

   ulen getPosition(Point point) const;

   void draw(const DrawBuf &buf) const;
 };

} // namespace Video
} // namespace CCore

#endif


