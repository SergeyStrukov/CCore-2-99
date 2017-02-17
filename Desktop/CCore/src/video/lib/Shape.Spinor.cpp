/* Shape.Spinor.cpp */
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

#include <CCore/inc/video/lib/Shape.Spinor.h>

#include <CCore/inc/video/FigureLib.h>

namespace CCore {
namespace Video {

/* class SpinorShape */

Point SpinorShape::getInnerSize(int val) const
 {
  char temp[TextBufLen];
  PrintBuf out(Range(temp));

  Putobj(out,BindOpt(opt,val));

  TextSize ts=cfg.font->text_guarded(out.close());

  return Point(ts.full_dx,ts.dy);
 }

Point SpinorShape::getMinSize() const
 {
  Point s=Sup(getInnerSize(12345),getInnerSize(min_val),getInnerSize(max_val));

  Coordinate len=RoundUpLen(+cfg.width);

  Coordinate dy=s.y+2*len;
  Coordinate dx=s.x+4*dy;

  return Point(dx,dy);
 }

auto SpinorShape::getZone(Point point) const -> ZoneType
 {
  if( pane.contains(point) )
    {
     int x=point.x-pane.x;

     if( x<pane.dy ) return ZonePlus;

     if( pane.dx-x<pane.dy ) return ZoneMinus;
    }

  return ZoneNone;
 }

void SpinorShape::draw(const DrawBuf &buf) const
 {
  MPane p(pane);

  if( !p ) return;

  VColor gray=+cfg.gray;
  MCoord width=+cfg.width;

  SmoothDrawArt art(buf.cut(pane));

  if( pane.dx<4*pane.dy )
    {
     art.block(pane,gray);

     return;
    }

  // body

  MCoord radius=p.dy/2;
  FigureRoundBox fig(p,radius);

  {
   fig.curveSolid(art,+cfg.back);

   VColor border = focus? +cfg.focus : ( enable? +cfg.border : gray ) ;

   fig.curveLoop(art,HalfPos,width,border);
  }

  // text

  {
   Coord dx=pane.dy;
   Coord dy=RoundUpLen(width);

   Pane inner=pane.shrink(dx,dy);

   char temp[TextBufLen];
   PrintBuf out(Range(temp));

   Putobj(out,BindOpt(opt,val));

   VColor text = enable? +cfg.text : gray ;

   cfg.font->text(buf,inner,TextPlace(AlignX_Center,AlignY_Center),out.close(),text);
  }

  // buttons

  {
   MPoint c1=fig.getTopLeft().addY(radius);
   MPoint c2=fig.getTopRight().addY(radius);

   MCoord radius2=Div(9,10)*radius;

   MPoint f1=c1.subY(radius2);
   MPoint f2=c1.addY(radius2);

   if( enable )
     {
      TwoField field(f1,+cfg.snow,f2,gray);
      TwoField fieldUp(f1,+cfg.snowUp,f2,gray);

      if( down==ZonePlus )
        {
         art.ball(c1,radius2,gray);
        }
      else if( mover==ZonePlus )
        {
         art.ball(c1,radius2,fieldUp);
        }
      else
        {
         art.ball(c1,radius2,field);
        }

      if( down==ZoneMinus )
        {
         art.ball(c2,radius2,gray);
        }
      else if( mover==ZoneMinus )
        {
         art.ball(c2,radius2,fieldUp);
        }
      else
        {
         art.ball(c2,radius2,field);
        }
     }
   else
     {
      VColor top=+cfg.snow;

      TwoField field(f1,top,f2,gray);

      art.ball(c1,radius2,field);

      art.ball(c2,radius2,field);
     }

   VColor fc = enable? +cfg.face : gray ;

   MCoord a=radius2/2;
   MCoord w=radius2/3;

   art.path(w,fc,c1.subX(a),c1.addX(a));
   art.path(w,fc,c1.subY(a),c1.addY(a));

   art.path(w,fc,c2.subX(a),c2.addX(a));

   VColor border = enable? +cfg.border : gray ;

   art.circle(c1,radius2-width/2,width,border);
   art.circle(c2,radius2-width/2,width,border);
  }
 }

} // namespace Video
} // namespace CCore

