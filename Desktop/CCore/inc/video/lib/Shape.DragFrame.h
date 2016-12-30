/* Shape.DragFrame.h */
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

#ifndef CCore_inc_video_lib_Shape_DragFrame_h
#define CCore_inc_video_lib_Shape_DragFrame_h

#include <CCore/inc/video/FrameBase.h>
#include <CCore/inc/video/Color.h>
#include <CCore/inc/video/Font.h>
#include <CCore/inc/video/RefVal.h>

#include <CCore/inc/DeferCall.h>

namespace CCore {
namespace Video {

/* classes */

class DragFrameShape;

/* class DragFrameShape */

class DragFrameShape
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<Coord> frame_dxy = 12 ;
     RefVal<Coord> title_dy  = 32 ;

     RefVal<Coord> btn_dx    = 26 ;
     RefVal<Coord> btn_dy    = 24 ;

     RefVal<VColor> top               =      Gray ;
     RefVal<VColor> bottom            =      Snow ;
     RefVal<VColor> frame             =      Snow ;

     RefVal<VColor> drag              =    Silver ;
     RefVal<VColor> dragHilight       =     Green ;
     RefVal<VColor> dragActive        =       Red ;

     RefVal<VColor> active            = RoyalBlue ;
     RefVal<VColor> inactive          =    Silver ;
     RefVal<VColor> title             =     Black ;

     RefVal<VColor> btnFace           = SteelBlue ;
     RefVal<VColor> btnFaceHilight    =     Green ;
     RefVal<VColor> btnPict           =     White ;
     RefVal<VColor> btnPictClose      =       Red ;
     RefVal<VColor> btnPictAlert      =       Red ;
     RefVal<VColor> btnPictNoAlert    =      Gray ;
     RefVal<VColor> btnPictCloseAlert =    Orange ;

     RefVal<VColor> shade_color        =    Violet ;
     RefVal<Clr>    shade_alpha        =        64 ;

     RefVal<Font> title_font;

     RefVal<unsigned> blink_time   = 3_sectick ;
     RefVal<unsigned> blink_period =    3_tick ;

     RefVal<DefString> fatal_error = "Fatal error"_def ;

     Config() noexcept {}
    };

   const Config &cfg;

  private:

   Point size;

   Pane dragTopLeft;
   Pane dragLeft;
   Pane dragBottomLeft;
   Pane dragBottom;
   Pane dragBottomRight;
   Pane dragRight;
   Pane dragTopRight;
   Pane dragBar;

   Pane titleBar;
   Pane btnAlert;
   Pane btnMin;
   Pane btnMax;
   Pane btnClose;

   Pane client;

  private:

   class DrawArt;

   VColor dragColor(DragType zone) const;

   void draw_Frame(DrawArt &art) const;

   void draw_TopLeft(DrawArt &art) const;

   void draw_Left(DrawArt &art) const;

   void draw_BottomLeft(DrawArt &art) const;

   void draw_Bottom(DrawArt &art) const;

   void draw_BottomRight(DrawArt &art) const;

   void draw_Right(DrawArt &art) const;

   void draw_TopRight(DrawArt &art) const;

   void draw_Bar(DrawArt &art) const;

   void draw_Alert(DrawArt &art) const;

   void draw_Min(DrawArt &art) const;

   void draw_Max(DrawArt &art) const;

   void draw_Close(DrawArt &art) const;

  public:

   static VKey Filter(VKey vkey,KeyMod kmod)
    {
     Used(kmod);

     return vkey;
    }

   // state

   bool has_focus = false ;
   bool max_button = true ;
   bool is_main = true ;

   DragType drag_type = DragType_None ;
   DragType hilight = DragType_None ;
   DragType btn_type = DragType_None ;
   AlertType alert_type = AlertType_No ;
   bool alert_blink = false ;
   bool help = false ;

   DefString title;

   unsigned time = 0 ;

   // methods

   explicit DragFrameShape(const Config &cfg_) : cfg(cfg_) {}

   void reset(const DefString &title,bool is_main,bool max_button);

   void layout(Point size);

   Point getDeltaSize() const;

   Coord getMinDx(bool is_main,StrLen title) const;

   Point getMinSize(bool is_main,StrLen title,Point size) const
    {
     size+=getDeltaSize();

     return Point( Max(size.x,getMinDx(is_main,title)) , size.y );
    }

   Point getMinSize(Point size=Point(10,10)) const
    {
     StrLen str=title.str();

     Replace_min(str.len,10);

     return getMinSize(is_main,str,size);
    }

   Pane getClient() const { return client; }

   DragType dragTest(Point point) const;

   Pane getPane(DragType drag_type) const;

   void shade(FrameBuf<DesktopColor> &buf) const;

   bool resetTime()
    {
     bool ret = !time ;

     time=+cfg.blink_time;

     return ret;
    }

   bool checkTime()
    {
     if( time )
       {
        time--;

        return true;
       }
     else
       {
        return false;
       }
    }

   bool tick() const
    {
     return !( time % +cfg.blink_period );
    }

   void draw(const DrawBuf &buf) const;

   void draw(const DrawBuf &buf,DragType drag_type) const;
 };

} // namespace Video
} // namespace CCore

#endif

