/* AltWindow.cpp */
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

#include <CCore/inc/video/AltWindow.h>

#include <CCore/inc/video/FigureLib.h>

namespace CCore {
namespace Video {

/* class AltShape */

Point AltShape::getMinSize() const
 {
  Coordinate dy=+cfg.dy;

  return Point(2*dy,dy);
 }

auto AltShape::getZone(Point point) const -> CheckType
 {
  return point.x-pane.x > pane.dx/2 ;
 }

void AltShape::draw(const DrawBuf &buf) const // TODO
 {
  MPane p(pane);

  if( !p ) return;

  MCoord x_med=p.x+p.dx/2;

  MPane a(p.x,x_med,p.y,p.ey);
  MPane b(x_med,p.ex,p.y,p.ey);

  SmoothDrawArt art(buf.cut(pane));

  VColor top=+cfg.top;
  VColor bottom=+cfg.bottom;
  VColor topUp=+cfg.topUp;

  // body

  {
   FigureBox fig(a);

   if( enable && check )
     {
      VColor top_ = ( mover && !zone )?topUp:top ;

      fig.solid(art,TwoField(a.getTopLeft(),top_,a.getBottomLeft(),bottom));
     }
   else
     {
      fig.solid(art,bottom);
     }
  }
  {
   FigureBox fig(b);

   if( enable && !check )
     {
      VColor top_ = ( mover && zone )?topUp:top ;

      fig.solid(art,TwoField(b.getTopLeft(),top_,b.getTopRight(),bottom));
     }
   else
     {
      fig.solid(art,bottom);
     }
  }

  // mark

  {
   FigureDownArrow fig(a.shrink(p.dy/5));

   VColor mark = ( enable && !check )?+cfg.mark_false_on:+cfg.mark_false ;

   fig.curveSolid(art,mark);
  }
  {
   FigureRightArrow fig(b.shrink(p.dy/5));

   VColor mark = ( enable && check )?+cfg.mark_true_on:+cfg.mark_true ;

   fig.curveSolid(art,mark);
  }

  // border

  {
   FigureBox fig(p);

   MCoord width=p.dy/10;

   VColor border=enable?+cfg.border:top;

   fig.loop(art,HalfPos,width,border);

   art.path(width,border,MPoint(x_med,p.y),MPoint(x_med,p.ey));

   if( focus )
     fig.loop(art,HalfPos,width/2,+cfg.focus);
  }
 }

} // namespace Video
} // namespace CCore

