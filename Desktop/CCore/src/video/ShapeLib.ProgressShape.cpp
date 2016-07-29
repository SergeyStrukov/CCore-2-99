/* ShapeLib.ProgressShape.cpp */
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

#include <CCore/inc/video/SmoothDrawArt.h>
#include <CCore/inc/video/FigureLib.h>

namespace CCore {
namespace Video {

/* class ProgressShape */

Point ProgressShape::getMinSize() const
 {
  return Point(10,40);
 }

void ProgressShape::draw(const DrawBuf &buf) const
 {
  MPane p(pane);

  if( !p ) return;

  SmoothDrawArt art(buf);

  MCoord x0=p.x;
  MCoord x2=p.ex;

  MCoord y0=p.y;
  MCoord y1=p.ey;

  MCoord x1=Position(pos,total,x0,x2);

  MCoord width=+cfg.width;

  VColor border=+cfg.border;

  {
   FigureBox fig(x0,x1,y0,y1);

   fig.solid(art,TwoField({x0,y0},+cfg.active_top,{x0,y1},+cfg.active_bottom));
  }
  {
   FigureBox fig(x1,x2,y0,y1);

   fig.solid(art,TwoField({x0,y0},+cfg.top,{x0,y1},+cfg.bottom));
  }
  {
   FigureBox fig(p);

   fig.loop(art,HalfPos,width,border);
  }

  if( has_active )
    {
     MCoord x=x0+width;
     MCoord y=y0+Rational(1,10)*p.dy;
     MCoord dy=Rational(8,10)*p.dy;
     MCoord ey=y+dy;
     MCoord dx=dy+Rational(active_pos,MaxActivePos)*dy;

     MCoord len=dx/3;
     MCoord shift=len-width;

     MCoord radius=Rational(40,100)*Min(len,dy);

     TwoField field({x,y},+cfg.ping_top,{x,ey},+cfg.ping_bottom);

     for(unsigned count=5; count ;count--,x+=shift)
       {
        FigureRoundBox fig(x,x+len,y,ey,radius);

        fig.curveSolid(art,field);

        fig.curveLoop(art,HalfPos,width,border);
       }
    }
 }

} // namespace Video
} // namespace CCore


