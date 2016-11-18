/* ShapeLib.ButtonShape.cpp */
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

/* class ButtonShape */

MCoord ButtonShape::FigEX(Coord fdy,MCoord width)
 {
  return (Fraction(fdy)+2*width)/4;
 }

Point ButtonShape::getMinSize() const
 {
  TextSize ts=cfg.font->text(face.str());

  IntGuard( !ts.overflow );

  MCoord width=+cfg.width;

  MCoord ex=FigEX(ts.dy,width);

  Coord dx=RoundUpLen(ex);
  Coord dy=RoundUpLen(width);

  return 2*Point(dx,dy)+Point(ts.full_dx,ts.dy)+(+cfg.space);
 }

void ButtonShape::draw(const DrawBuf &buf) const
 {
  MPane p(pane);

  if( !p ) return;

  SmoothDrawArt art(buf.cut(pane));

  Font font=cfg.font.get();

  // figure

  MCoord width=+cfg.width;

  FontSize fs=font->getSize();

  MCoord ex=FigEX(fs.dy,width);

  VColor bottom=+cfg.bottom;

  if( ex>p.dx/3 )
    {
     art.block(pane,bottom);

     return;
    }

  FigureButton fig(p,ex);

  // body

  if( down )
    {
     fig.curveSolid(art,bottom);
    }
  else
    {
     VColor top = ( mover && enable )? +cfg.topUp : +cfg.top ;

     fig.curveSolid(art,TwoField(p.getTopLeft(),top,p.getBottomLeft(),bottom));
    }

  // text

  {
   Coord dx=RoundUpLen(ex);
   Coord dy=RoundUpLen(width);

   Point shift=Null;

   if( down ) shift=Point::Diag( (dy+1)/2 );

   VColor text = enable? +cfg.text : bottom ;

   font->text(buf,pane.shrink(dx,dy)+shift,TextPlace(AlignX_Center,AlignY_Center),face.str(),text);
  }

  // border

  {
   VColor border = focus? +cfg.focus : ( enable? +cfg.border : bottom ) ;

   fig.curveLoop(art,HalfPos,width,border);
  }
 }

/* class RefButtonShape */

Point RefButtonShape::getMinSize() const
 {
  ButtonShape temp(cfg,face.get());

  temp.pane=pane;

  temp.enable=enable;
  temp.focus=focus;
  temp.mover=mover;
  temp.down=down;

  temp.mouse=mouse;

  return temp.getMinSize();
 }

void RefButtonShape::draw(const DrawBuf &buf) const
 {
  ButtonShape temp(cfg,face.get());

  temp.pane=pane;

  temp.enable=enable;
  temp.focus=focus;
  temp.mover=mover;
  temp.down=down;

  temp.mouse=mouse;

  temp.draw(buf);
 }

} // namespace Video
} // namespace CCore


