/* ShapeLib.DoubleLineShape.cpp */
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

/* class XDoubleLineShape */

SizeY XDoubleLineShape::getMinSize() const
 {
  Coord dxy=RoundUpLen(+cfg.width);

  return 2*dxy;
 }

void XDoubleLineShape::draw(const DrawBuf &buf) const
 {
  MPane p(pane);

  if( !p ) return;

  SmoothDrawArt art(buf.cut(pane));

  MCoord width=+cfg.width;

  MCoord x0=p.x+width/2;
  MCoord x1=p.ex-width/2;

  MCoord y=p.y+p.dy/2;
  MCoord y0=y-width/2;
  MCoord y1=y+width/2;

  art.path(width,+cfg.top,MPoint(x0,y0),MPoint(x1,y0));
  art.path(width,+cfg.bottom,MPoint(x0,y1),MPoint(x1,y1));
 }

/* class YDoubleLineShape */

SizeX YDoubleLineShape::getMinSize() const
 {
  Coord dxy=RoundUpLen(+cfg.width);

  return 2*dxy;
 }

void YDoubleLineShape::draw(const DrawBuf &buf) const
 {
  MPane p(pane);

  if( !p ) return;

  SmoothDrawArt art(buf.cut(pane));

  MCoord width=+cfg.width;

  MCoord x=p.x+p.dx/2;
  MCoord x0=x-width/2;
  MCoord x1=x+width/2;

  MCoord y0=p.y+width/2;
  MCoord y1=p.ey-width/2;

  art.path(width,+cfg.top,MPoint(x0,y0),MPoint(x0,y1));
  art.path(width,+cfg.bottom,MPoint(x1,y0),MPoint(x1,y1));
 }

} // namespace Video
} // namespace CCore


