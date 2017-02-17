/* Shape.Spinor.h */
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

#ifndef CCore_inc_video_lib_Shape_Spinor_h
#define CCore_inc_video_lib_Shape_Spinor_h

#include <CCore/inc/video/MinSizeType.h>
#include <CCore/inc/video/Color.h>
#include <CCore/inc/video/Font.h>
#include <CCore/inc/video/RefVal.h>

namespace CCore {
namespace Video {

/* classes */

class SpinorShape;

/* class SpinorShape */

class SpinorShape
 {
   Point getInnerSize(int val) const;

  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<VColor> back   =    Silver ;
     RefVal<VColor> border =      Blue ;
     RefVal<VColor> focus  = OrangeRed ;
     RefVal<VColor> gray   =      Gray ;
     RefVal<VColor> snow   =      Snow ;
     RefVal<VColor> snowUp = PaleGreen ;
     RefVal<VColor> face   =     Black ;
     RefVal<VColor> text   =     Black ;

     RefVal<Font> font;

     Config() noexcept {}

     template <class Bag>
     void bind(const Bag &bag)
      {
       width.bind(bag.width);
       back.bind(bag.back);
       border.bind(bag.border);
       focus.bind(bag.focus);
       gray.bind(bag.gray);
       snow.bind(bag.snow);
       snowUp.bind(bag.snowUp);
       face.bind(bag.face);

       text.bind(bag.spinor_text);
       font.bind(bag.spinor_font.font);
      }
    };

   // parameters

   const Config &cfg;
   int min_val = 0 ;
   int max_val = 0 ;
   int val = 0 ;
   IntPrintOpt opt;
   Pane pane;

   // state

   enum ZoneType
    {
     ZoneNone = 0,

     ZonePlus,
     ZoneMinus
    };

   bool enable =  true ;
   bool focus  = false ;
   ZoneType mover = ZoneNone ;
   ZoneType down  = ZoneNone ;

   bool mouse = false ;

   // methods

   explicit SpinorShape(const Config &cfg_) : cfg(cfg_) {}

   Point getMinSize() const;

   bool isGoodSize(Point size) const { return size >= getMinSize() && size.x >= 4*size.y ; }

   ZoneType getZone(Point point) const;

   void draw(const DrawBuf &buf) const;
 };

} // namespace Video
} // namespace CCore

#endif

