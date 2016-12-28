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
#include <CCore/inc/video/lib/Shape.SimpleTextList.h>
#include <CCore/inc/video/lib/Shape.LineEdit.h>
#include <CCore/inc/video/lib/Shape.Scroll.h>

namespace CCore {
namespace Video {

/* classes */

class ScrollListShape;

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


