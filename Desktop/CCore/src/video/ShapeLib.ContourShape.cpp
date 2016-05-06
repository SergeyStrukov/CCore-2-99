/* ShapeLib.ContourShape.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
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

/* class ContourShape */

Point ContourShape::getMinSize() const
 {
  Coord dxy=RoundUpLen(+cfg.width);

  return Point::Diag(2*dxy+1);
 }

Point ContourShape::getMinSize(Point inner_size) const
 {
  Coord dxy=RoundUpLen(+cfg.width);

  return inner_size.addXY(2*dxy);
 }

Pane ContourShape::getInner() const
 {
  Coord dxy=RoundUpLen(+cfg.width);

  return pane.shrink(dxy);
 }

void ContourShape::draw(const DrawBuf &buf) const
 {
  MPane p(pane);

  if( !p ) return;

  Smooth::DrawArt art(buf);

  MCoord width=+cfg.width;

  FigureTopBorder fig_top(p,width);

  fig_top.solid(art,+cfg.top);

  FigureBottomBorder fig_bottom(p,width);

  fig_bottom.solid(art,+cfg.bottom);
 }

} // namespace Video
} // namespace CCore


