/* Shape.LineEdit.h */
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

#ifndef CCore_inc_video_lib_Shape_LineEdit_h
#define CCore_inc_video_lib_Shape_LineEdit_h

#include <CCore/inc/video/MinSizeType.h>
#include <CCore/inc/video/Color.h>
#include <CCore/inc/video/Font.h>
#include <CCore/inc/video/RefVal.h>

#include <CCore/inc/DeferCall.h>

namespace CCore {
namespace Video {

/* classes */

class LineEditShape;

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

   bool isGoodSize(Point size) const { return size>=getMinSize(); }

   void setMax();

   bool tick()
    {
     if( count )
       {
        count--;

        return false;
       }
     else
       {
        count=PosSub(+cfg.period,1u);

        return true;
       }
    }

   void showCursor();

   ulen getPosition(Point point) const;

   virtual void drawText(Font font,const DrawBuf &buf,Pane pane,TextPlace place,StrLen text,VColor vc) const;

   void draw(const DrawBuf &buf) const;
 };

} // namespace Video
} // namespace CCore

#endif

