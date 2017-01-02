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

     RefVal<VColor> gray              =      Gray ;
     RefVal<VColor> snow              =      Snow ;
     RefVal<VColor> frame             =      Snow ;
     RefVal<VColor> active            = RoyalBlue ;
     RefVal<VColor> inactive          =    Silver ;
     RefVal<VColor> title             =     Black ;

     RefVal<VColor> drag              =    Silver ;
     RefVal<VColor> dragHilight       =     Green ;
     RefVal<VColor> dragActive        =       Red ;
     RefVal<VColor> dragSmall         =     Wheat ;

     RefVal<VColor> btnFace           = SteelBlue ;
     RefVal<VColor> btnFaceHilight    =     Green ;
     RefVal<VColor> btnPict           =     White ;
     RefVal<VColor> btnPictClose      =       Red ;
     RefVal<VColor> btnPictAlert      =       Red ;
     RefVal<VColor> btnPictNoAlert    =      Gray ;
     RefVal<VColor> btnPictCloseAlert =    Orange ;

     RefVal<MCoord> hintWidth = Fraction(3) ;

     RefVal<VColor> hintBack = Wheat ;
     RefVal<VColor> hintText = Blue ;
     RefVal<VColor> hintBorder = Green ;

     RefVal<VColor> shade_color        =    Violet ;
     RefVal<Clr>    shade_alpha        =        64 ;

     RefVal<Coord> frame_dxy = 12 ;
     RefVal<Coord> title_dy  = 32 ;

     RefVal<Coord> btn_dx    = 26 ;
     RefVal<Coord> btn_dy    = 24 ;

     RefVal<Font> font;
     RefVal<Font> fontHint;

     RefVal<unsigned> time   = 3_sectick ;
     RefVal<unsigned> period =    3_tick ;

     RefVal<DefString> text_No_hint = "<No hint available>"_def ;

     RefVal<DefString> hint_ResizeTopLeft     = "Resize top-left"_def ;
     RefVal<DefString> hint_ResizeLeft        = "Resize left"_def ;
     RefVal<DefString> hint_ResizeBottomLeft  = "Resize bottom-left"_def ;
     RefVal<DefString> hint_ResizeBottom      = "Resize bottom"_def ;
     RefVal<DefString> hint_ResizeBottomRight = "Resize bottom-right"_def ;
     RefVal<DefString> hint_ResizeRight       = "Resize right"_def ;
     RefVal<DefString> hint_ResizeTopRight    = "Resize top-right"_def ;

     RefVal<DefString> hint_Alert    = "Open/close alert view"_def ;
     RefVal<DefString> hint_Help     = "Help on/off"_def ;
     RefVal<DefString> hint_Minimize = "Minimize"_def ;
     RefVal<DefString> hint_Maximize = "Maximize"_def ;
     RefVal<DefString> hint_Restore  = "Restore"_def ;
     RefVal<DefString> hint_Close    = "Close"_def ;

     Config() noexcept {}

     template <class Bag>
     void bind(const Bag &bag)
      {
       width.bind(bag.width);
       gray.bind(bag.gray);
       snow.bind(bag.snow);

       frame.bind(bag.frame);
       active.bind(bag.active_frame);
       inactive.bind(bag.inactive_frame);
       title.bind(bag.title);

       drag.bind(bag.drag);
       dragHilight.bind(bag.dragHilight);
       dragActive.bind(bag.dragActive);
       dragSmall.bind(bag.dragSmall);

       btnFace.bind(bag.btnFace);
       btnFaceHilight.bind(bag.btnFaceHilight);
       btnPict.bind(bag.btnPict);
       btnPictClose.bind(bag.btnPictClose);
       btnPictAlert.bind(bag.btnPictAlert);
       btnPictNoAlert.bind(bag.btnPictNoAlert);
       btnPictCloseAlert.bind(bag.btnPictCloseAlert);

       hintWidth.bind(bag.hintWidth);

       hintBack.bind(bag.hintBack);
       hintText.bind(bag.hintText);
       hintBorder.bind(bag.hintBorder);

       shade_color.bind(bag.shade_color);
       shade_alpha.bind(bag.shade_alpha);

       frame_dxy.bind(bag.frame_dxy);
       title_dy.bind(bag.title_dy);
       btn_dx.bind(bag.btn_dx);
       btn_dy.bind(bag.btn_dy);

       font.bind(bag.title_font.font);
       fontHint.bind(bag.hint_font.font);

       time.bind(bag.blink_time);
       period.bind(bag.blink_period);

       text_No_hint.bind(bag.text_No_hint);

       hint_ResizeTopLeft.bind(bag.hint_ResizeTopLeft);
       hint_ResizeLeft.bind(bag.hint_ResizeLeft);
       hint_ResizeBottomLeft.bind(bag.hint_ResizeBottomLeft);
       hint_ResizeBottom.bind(bag.hint_ResizeBottom);
       hint_ResizeBottomRight.bind(bag.hint_ResizeBottomRight);
       hint_ResizeRight.bind(bag.hint_ResizeRight);
       hint_ResizeTopRight.bind(bag.hint_ResizeTopRight);

       hint_Alert.bind(bag.hint_Alert);
       hint_Help.bind(bag.hint_Help);
       hint_Minimize.bind(bag.hint_Minimize);
       hint_Maximize.bind(bag.hint_Maximize);
       hint_Restore.bind(bag.hint_Restore);
       hint_Close.bind(bag.hint_Close);
      }
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
   Pane btnHelp;
   Pane btnMin;
   Pane btnMax;
   Pane btnClose;

   Pane client;

  private:

   class DrawArt;

   VColor dragColor(DragType zone) const;

   void draw_Frame(DrawArt &art,Pane part) const;

   void draw_Frame(DrawArt &art) const { draw_Frame(art,Pane(Null,size)); }

   void draw_Frame(const DrawBuf &buf,DragType drag_type) const;

   void draw_TopLeft(DrawArt &art) const;

   void draw_Left(DrawArt &art) const;

   void draw_BottomLeft(DrawArt &art) const;

   void draw_Bottom(DrawArt &art) const;

   void draw_BottomRight(DrawArt &art) const;

   void draw_Right(DrawArt &art) const;

   void draw_TopRight(DrawArt &art) const;

   void draw_Bar(DrawArt &art) const;

   auto draw_Btn(DrawArt &art,Pane btn,DragType zone) const;

   void draw_Alert(DrawArt &art) const;

   void draw_Help(DrawArt &art) const;

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
   bool has_good_size = true ;

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

   Hint getHint(Point point) const;

   void shade(FrameBuf<DesktopColor> &buf) const
    {
     buf.erase(+cfg.shade_color,+cfg.shade_alpha);
    }

   void shade(FrameBuf<DesktopColor> &buf,Pane pane) const
    {
     buf.block_safe(pane,+cfg.shade_color,+cfg.shade_alpha);
    }

   bool resetTime()
    {
     bool ret = !time ;

     time=+cfg.time;

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
     return !( time % +cfg.period );
    }

   void draw(const DrawBuf &buf) const;

   void draw(const DrawBuf &buf,DragType drag_type) const;

   void drawHint(const DrawBuf &buf,Hint hint) const;
 };

} // namespace Video
} // namespace CCore

#endif

