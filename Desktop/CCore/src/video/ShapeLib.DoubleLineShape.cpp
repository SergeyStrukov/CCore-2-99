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

#include <CCore/inc/video/FigureLib.h>

namespace CCore {
namespace Video {

/* class XDoubleLineShape */

SizeY XDoubleLineShape::getMinSize() const
 {
  Coordinate dy=RoundUpLen(+cfg.width);

  return 2*dy;
 }

void XDoubleLineShape::draw(const DrawBuf &buf) const
 {
  MPane p(pane);

  if( !p ) return;

  SmoothDrawArt art(buf.cut(pane));

  MCoord width=+cfg.width;
  MCoord d=width/2;

  art.path(width,+cfg.top,p.getLeftMid().subY(d),p.getRightMid().subY(d));
  art.path(width,+cfg.bottom,p.getLeftMid().addY(d),p.getRightMid().addY(d));
 }

/* class YDoubleLineShape */

SizeX YDoubleLineShape::getMinSize() const
 {
  Coordinate dx=RoundUpLen(+cfg.width);

  return 2*dx;
 }

void YDoubleLineShape::draw(const DrawBuf &buf) const
 {
  MPane p(pane);

  if( !p ) return;

  SmoothDrawArt art(buf.cut(pane));

  MCoord width=+cfg.width;
  MCoord d=width/2;

  art.path(width,+cfg.top,p.getTopMid().subX(d),p.getBottomMid().subX(d));
  art.path(width,+cfg.bottom,p.getTopMid().addX(d),p.getBottomMid().addX(d));
 }

} // namespace Video
} // namespace CCore


