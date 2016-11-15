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

#include <CCore/inc/video/MinSizeType.h>
#include <CCore/inc/video/Color.h>
#include <CCore/inc/video/Font.h>
#include <CCore/inc/video/RefVal.h>
#include <CCore/inc/video/Info.h>

#include <CCore/inc/String.h>
#include <CCore/inc/DeferCall.h>

namespace CCore {
namespace Video {

/* classes */

class ButtonShape;

class KnobShape;

class CheckShape;

class RadioShape;

class LabelShape;

class TextShape;

class SingleLineShape;

class XSingleLineShape;

class YSingleLineShape;

class DoubleLineShape;

class XDoubleLineShape;

class YDoubleLineShape;

class ContourShape;

class TextContourShape;

class SwitchShape;

class LightShape;

class ScrollShape;

class XScrollShape;

class YScrollShape;

class ProgressShape;

class InfoShape;

class LineEditShape;

class SimpleTextListShape;

class ScrollListShape;

/* class ButtonShape */

class ButtonShape
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<Point> space = Point(6,4) ;

     RefVal<VColor> border =      Blue ;
     RefVal<VColor> focus  = OrangeRed ;

     RefVal<VColor> bottom =      Gray ;
     RefVal<VColor> topUp  =     Green ;
     RefVal<VColor> top    =      Snow ;
     RefVal<VColor> text   =     Black ;

     RefVal<Font> font;

     Config() noexcept {}
    };

   // parameters

   using FaceType = DefString ;

   const Config &cfg;
   FaceType face;
   Pane pane;

   // state

   bool enable =  true ;
   bool focus  = false ;
   bool mover  = false ;
   bool down   = false ;

   bool mouse  = false ;

   // methods

   ButtonShape(const Config &cfg_,const FaceType &face_) : cfg(cfg_),face(face_) {}

   Point getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void draw(const DrawBuf &buf) const;
 };

/* class KnobShape */

class KnobShape
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<Coord> dxy = 30 ;

     RefVal<VColor> border =      Blue ;
     RefVal<VColor> focus  = OrangeRed ;

     RefVal<VColor> bottom =      Gray ;
     RefVal<VColor> topUp  =     Green ;
     RefVal<VColor> top    =      Snow ;
     RefVal<VColor> face   =     Black ;

     Config() noexcept {}
    };

   // parameters

   enum FaceType
    {
     FaceOk,
     FacePlus,
     FaceMinus,
     FaceCross,
     FaceAsterisk,
     FaceExclamation,
     FaceQuestion,
     FaceLeft,
     FaceRight,
     FaceUp,
     FaceDown,
     FaceStop,
     FaceRightRight,
     FaceLeftLeft,

     FaceLim
    };

   const Config &cfg;
   FaceType face;
   Pane pane;

   // state

   bool enable =  true ;
   bool focus  = false ;
   bool mover  = false ;
   bool down   = false ;

   bool mouse  = false ;

   // methods

   KnobShape(const Config &cfg_,const FaceType &face_) : cfg(cfg_),face(face_) {}

   SizeBox getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void draw(const DrawBuf &buf) const;
 };

/* class CheckShape */

class CheckShape
 {
  public:

   struct Config
    {
     RefVal<Coord> dxy = 20 ;

     RefVal<VColor> border   =      Blue ;
     RefVal<VColor> focus    = OrangeRed ;

     RefVal<VColor> bottomUp =     Green ;
     RefVal<VColor> bottom   =      Snow ;
     RefVal<VColor> top      =      Gray ;
     RefVal<VColor> mark     =     Black ;

     Config() noexcept {}
    };

   const Config &cfg;
   Pane pane;

   // state

   bool enable =  true ;
   bool focus  = false ;
   bool mover  = false ;
   bool check  = false ;

   // methods

   explicit CheckShape(const Config &cfg_,bool check_=false) : cfg(cfg_),check(check_) {}

   SizeBox getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void draw(const DrawBuf &buf) const;
 };

/* class RadioShape */

class RadioShape
 {
  public:

   struct Config
    {
     RefVal<Coord> dxy = 20 ;

     RefVal<VColor> border   =      Blue ;
     RefVal<VColor> focus    = OrangeRed ;

     RefVal<VColor> bottomUp =     Green ;
     RefVal<VColor> bottom   =      Snow ;
     RefVal<VColor> top      =      Gray ;
     RefVal<VColor> mark     =     Black ;

     Config() noexcept {}
    };

   const Config &cfg;
   Pane pane;

   // state

   bool enable =  true ;
   bool focus  = false ;
   bool mover  = false ;
   bool check  = false ;

   // methods

   explicit RadioShape(const Config &cfg_,bool check_=false) : cfg(cfg_),check(check_) {}

   SizeBox getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void draw(const DrawBuf &buf) const;
 };

/* class LabelShape */

class LabelShape
 {
  public:

   struct Config
    {
     RefVal<VColor> inactive =  Gray ;
     RefVal<VColor> text     = Black ;

     RefVal<Font> font;

     Config() noexcept {}
    };

   const Config &cfg;
   DefString text;
   AlignX align_x;
   AlignY align_y;
   Pane pane;

   // state

   bool enable = true ;

   // methods

   LabelShape(const Config &cfg_,const DefString &text_,AlignX align_x_=AlignX_Center,AlignY align_y_=AlignY_Center)
    : cfg(cfg_),text(text_),align_x(align_x_),align_y(align_y_) {}

   Point getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void draw(const DrawBuf &buf) const;
 };

/* class TextShape */

class TextShape
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<Point> space = Point(6,4) ;

     RefVal<VColor> bottom   =   Snow ;
     RefVal<VColor> top      =   Gray ;
     RefVal<VColor> back     = Silver ;
     RefVal<VColor> text     =  Black ;
     RefVal<VColor> inactive =   Gray ;

     RefVal<Font> font;

     Config() noexcept {}
    };

   const Config &cfg;
   DefString text;
   AlignX align_x;
   AlignY align_y;
   Pane pane;

   // state

   bool enable = true ;

   // methods

   TextShape(const Config &cfg_,const DefString &text_,AlignX align_x_=AlignX_Center,AlignY align_y_=AlignY_Center)
    : cfg(cfg_),text(text_),align_x(align_x_),align_y(align_y_) {}

   explicit TextShape(const Config &cfg_,AlignX align_x_=AlignX_Center,AlignY align_y_=AlignY_Center)
    : cfg(cfg_),align_x(align_x_),align_y(align_y_) {}

   Point getMinSize() const;

   Point getMinSize(StrLen text) const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void draw(const DrawBuf &buf) const;
 };

/* class SingleLineShape */

class SingleLineShape
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<VColor> line = Gray ;

     Config() noexcept {}
    };

   const Config &cfg;
   Pane pane;

   // methods

   explicit SingleLineShape(const Config &cfg_) : cfg(cfg_) {}
 };

/* class XSingleLineShape */

class XSingleLineShape : public SingleLineShape
 {
  public:

   // methods

   explicit XSingleLineShape(const Config &cfg) : SingleLineShape(cfg) {}

   SizeY getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void draw(const DrawBuf &buf) const;
 };

/* class YSingleLineShape */

class YSingleLineShape : public SingleLineShape
 {
  public:

   // methods

   explicit YSingleLineShape(const Config &cfg) : SingleLineShape(cfg) {}

   SizeX getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void draw(const DrawBuf &buf) const;
 };

/* class DoubleLineShape */

class DoubleLineShape
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<VColor> top    = Snow ;
     RefVal<VColor> bottom = Gray ;

     Config() noexcept {}
    };

   const Config &cfg;
   Pane pane;

   // methods

   explicit DoubleLineShape(const Config &cfg_) : cfg(cfg_) {}
 };

/* class XDoubleLineShape */

class XDoubleLineShape : public DoubleLineShape
 {
  public:

   // methods

   explicit XDoubleLineShape(const Config &cfg) : DoubleLineShape(cfg) {}

   SizeY getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void draw(const DrawBuf &buf) const;
 };

/* class YDoubleLineShape */

class YDoubleLineShape : public DoubleLineShape
 {
  public:

   // methods

   explicit YDoubleLineShape(const Config &cfg) : DoubleLineShape(cfg) {}

   SizeX getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void draw(const DrawBuf &buf) const;
 };

/* class ContourShape */

class ContourShape
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<VColor> top    = Gray ;
     RefVal<VColor> bottom = Snow ;

     Config() noexcept {}
    };

   const Config &cfg;
   Pane pane;

   // methods

   explicit ContourShape(const Config &cfg_) : cfg(cfg_) {}

   Point getMinSize() const;

   Point getMinSize(Point inner_size) const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   Pane getInner() const;

   void draw(const DrawBuf &buf) const;
 };

/* class TextContourShape */

class TextContourShape
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<VColor> top    =  Gray ;
     RefVal<VColor> bottom =  Snow ;
     RefVal<VColor> text   = Black ;

     RefVal<Font> font;

     Config() noexcept {}
    };

   const Config &cfg;
   DefString title;
   AlignX align_x;
   Pane pane;

   // methods

   TextContourShape(const Config &cfg_,const DefString &title_,AlignX align_x_=AlignX_Left) : cfg(cfg_),title(title_),align_x(align_x_) {}

   Point getMinSize() const;

   Point getMinSize(Point inner_size) const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   Pane getInner() const;

   void draw(const DrawBuf &buf) const;
 };

/* class SwitchShape */

class SwitchShape
 {
  public:

   struct Config
    {
     RefVal<Coord> dxy = 30 ;

     RefVal<VColor> border =      Blue ;
     RefVal<VColor> focus  = OrangeRed ;

     RefVal<VColor> top    =      Snow ;
     RefVal<VColor> bottom =      Gray ;
     RefVal<VColor> faceUp =      Blue ;
     RefVal<VColor> face   =     Black ;
     RefVal<VColor> on     =     Green ;
     RefVal<VColor> off    =       Red ;

     Config() noexcept {}
    };

   const Config &cfg;
   Pane pane;

   // state

   bool enable =  true ;
   bool focus  = false ;
   bool mover  = false ;
   bool check  = false ;

   // methods

   explicit SwitchShape(const Config &cfg_,bool check_=false) : cfg(cfg_),check(check_) {}

   SizeBox getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void draw(const DrawBuf &buf) const;
 };

/* class LightShape */

class LightShape
 {
  public:

   struct Config
    {
     RefVal<Coord> dxy = 20 ;

     RefVal<VColor> top      = Gray ;
     RefVal<VColor> bottom   = Snow ;
     RefVal<VColor> inactive = Gray ;

     Config() noexcept {}
    };

   const Config &cfg;
   VColor face = Green ;
   Pane pane;

   // state

   bool on;

   // methods

   explicit LightShape(const Config &cfg_,bool on_=false) : cfg(cfg_),on(on_) {}

   LightShape(const Config &cfg_,VColor face_,bool on_=false) : cfg(cfg_),face(face_),on(on_) {}

   SizeBox getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void draw(const DrawBuf &buf) const;
 };

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

   Point getMinSize() const;

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

   Point getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize() && size.y>=5*size.x ; }

   ScrollType getType(MPoint point) const;

   void drag(Point point) { dragPos(drag_base.y,point.y,pane.dy,pane.dx); }

   void draw(const DrawBuf &buf) const;
 };

/* class ProgressShape */

class ProgressShape
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<VColor> border        =     Black ;

     RefVal<VColor> top           =      Snow ;
     RefVal<VColor> bottom        =      Gray ;

     RefVal<VColor> active_top    =     Green ;
     RefVal<VColor> active_bottom = DarkGreen ;

     RefVal<VColor> ping_top      =      Snow ;
     RefVal<VColor> ping_bottom   =     Black ;

     RefVal<unsigned> time   = 3_sectick ;
     RefVal<unsigned> period =    2_tick ;

     Config() noexcept {}
    };

   const Config &cfg;
   Pane pane;

   // state

   static const unsigned MaxActivePos = 100 ;

   unsigned total      =   100 ;
   unsigned pos        =     0 ;
   unsigned active_pos =     0 ;
   bool has_active     = false ;

   unsigned time  = 0 ;
   unsigned count = 0 ;

   // methods

   explicit ProgressShape(const Config &cfg_) : cfg(cfg_) {}

   Point getMinSize() const;

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void adjustPos()
    {
     Replace_min(pos,total);
    }

   void resetTime() { time=+cfg.time; }

   bool tick()
    {
     if( ++count >= +cfg.period )
       {
        count=0;

        return true;
       }

     return false;
    }

   bool startActive()
    {
     if( !has_active )
       {
        has_active=true;
        active_pos=0;

        return true;
       }

     return false;
    }

   void nextActive()
    {
     if( (active_pos+=MaxActivePos/10)>MaxActivePos ) active_pos=0;
    }

   void stopActive()
    {
     has_active=false;
    }

   void draw(const DrawBuf &buf) const;
 };

/* class InfoShape */

class InfoShape
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<Point> space = Point(8,8) ;

     RefVal<VColor> text     =     Black ;
     RefVal<VColor> inactive =      Gray ;
     RefVal<VColor> focus    = OrangeRed ;

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

   ulen yoff  = 0 ;
   Coord xoff = 0 ;

   ulen yoffMax  = 0 ;
   Coord xoffMax = 0 ;
   Coord dxoff   = 0 ;

   bool drag = false ;
   Point drag_base;
   Coord xoff_base = 0 ;

   // methods

   explicit InfoShape(const Config &cfg_) : cfg(cfg_) {}

   InfoShape(const Config &cfg_,const Info &info_) : cfg(cfg_),info(info_) {}

   Point getMinSize() const;

   void setMax();

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

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


