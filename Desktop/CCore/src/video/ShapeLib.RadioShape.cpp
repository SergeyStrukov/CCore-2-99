/* ShapeLib.RadioShape.cpp */
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

#include <CCore/inc/video/ShapeLib.h>

#include <CCore/inc/video/FigureLib.h>

namespace CCore {
namespace Video {

/* class RadioShape */

SizeBox RadioShape::getMinSize() const
 {
  return +cfg.dxy;
 }

void RadioShape::draw(const DrawBuf &buf) const
 {
  MPane p(pane);

  if( !p ) return;

  p.square();

  SmoothDrawArt art(buf.cut(pane));

  MCoord len=p.dy;
  MCoord width=len/10;
  MCoord radius=len/2;

  MPoint center=p.getCenter();

  VColor top=+cfg.top;

  // body

  {
   VColor bottom = ( mover && enable )? +cfg.bottomUp : +cfg.bottom ;

   MCoord d=Ratio(600,10)*radius;

   art.ball(center,radius,TwoField(center.subXY(d),top,center.addXY(d),bottom));
  }

  // mark

  if( check )
    {
     VColor mark = enable? +cfg.mark : top ;

     art.ball(center,radius/3,mark);
    }

  // border

  {
   VColor border = focus? +cfg.focus : ( enable? +cfg.border : top ) ;

   art.circle(center,radius-width/2,width,border);
  }
 }

} // namespace Video
} // namespace CCore


