/* Shape.FrameBase.h */
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
//  Copyright (c) 2017 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_video_lib_Shape_FrameBase_h
#define CCore_inc_video_lib_Shape_FrameBase_h

#include <CCore/inc/video/FrameBase.h>
#include <CCore/inc/video/Color.h>
#include <CCore/inc/video/Font.h>
#include <CCore/inc/video/RefVal.h>

#include <CCore/inc/DeferCall.h>

namespace CCore {
namespace Video {

/* classes */

class FrameShapeBase;

/* class FrameShapeBase */

class FrameShapeBase
 {
  public:

   struct ConfigBase
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<VColor> gray              =      Gray ;
     RefVal<VColor> snow              =      Snow ;
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

     RefVal<MCoord> hintWidth = Fraction(3) ;

     RefVal<VColor> hintBack = Wheat ;
     RefVal<VColor> hintText = Blue ;
     RefVal<VColor> hintBorder = Green ;

     RefVal<VColor> shade_color        =    Violet ;
     RefVal<Clr>    shade_alpha        =        64 ;

     RefVal<Font> font;
     RefVal<Font> fontHint;

     RefVal<unsigned> time   = 3_sectick ;
     RefVal<unsigned> period =    3_tick ;

     RefVal<DefString> text_No_hint = "<No hint available>"_def ;

     ConfigBase() noexcept {}

     template <class Bag>
     void bind(const Bag &bag)
      {
       width.bind(bag.width);

       gray.bind(bag.gray);
       snow.bind(bag.snow);
       active.bind(bag.active_frame);
       inactive.bind(bag.inactive_frame);
       title.bind(bag.title);

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

       font.bind(bag.title_font.font);
       fontHint.bind(bag.hint_font.font);

       time.bind(bag.blink_time);
       period.bind(bag.blink_period);

       text_No_hint.bind(bag.text_No_hint);
      }
    };

  private:

   const ConfigBase &cfg;

   auto drawBtn(const DrawBuf &buf,Pane btn,DragType zone) const;

  protected:

   void drawFrame(const DrawBuf &buf,Pane all,Pane client,VColor frame,VColor frameSmall,Pane part) const;

   void drawTopLeft(const DrawBuf &buf,Pane pane,VColor vc) const;

   void drawLeft(const DrawBuf &buf,Pane pane,VColor vc) const;

   void drawBottomLeft(const DrawBuf &buf,Pane pane,VColor vc) const;

   void drawBottom(const DrawBuf &buf,Pane pane,VColor vc) const;

   void drawBottomRight(const DrawBuf &buf,Pane pane,VColor vc) const;

   void drawRight(const DrawBuf &buf,Pane pane,VColor vc) const;

   void drawTopRight(const DrawBuf &buf,Pane pane,VColor vc) const;

   void drawBar(const DrawBuf &buf,Pane pane) const;

   void drawAlert(const DrawBuf &buf,Pane btn) const;

   void drawHelp(const DrawBuf &buf,Pane btn) const;

   void drawMin(const DrawBuf &buf,Pane btn) const;

   void drawMax(const DrawBuf &buf,Pane btn,bool max_button) const;

   void drawClose(const DrawBuf &buf,Pane btn) const;

   void drawHint(const DrawBuf &buf,Pane title,Hint hint) const;

  public:

   // state

   bool has_focus = false ;
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

   explicit FrameShapeBase(const ConfigBase &cfg_) : cfg(cfg_) {}

   void reset(const DefString &title,bool is_main);

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
 };

} // namespace Video
} // namespace CCore

#endif

