/* ShapeLib.SingleLineShape.cpp */
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

/* class XSingleLineShape */

SizeY XSingleLineShape::getMinSize() const
 {
  Coord dy=RoundUpLen(+cfg.width);

  return dy;
 }

void XSingleLineShape::draw(const DrawBuf &buf) const
 {
  MPane p(pane);

  if( !p ) return;

  SmoothDrawArt art(buf.cut(pane));

  MCoord width=+cfg.width;

  art.path(width,+cfg.line,p.getLeftMid(),p.getRightMid());
 }

/* class YSingleLineShape */

SizeX YSingleLineShape::getMinSize() const
 {
  Coord dx=RoundUpLen(+cfg.width);

  return dx;
 }

void YSingleLineShape::draw(const DrawBuf &buf) const
 {
  MPane p(pane);

  if( !p ) return;

  SmoothDrawArt art(buf.cut(pane));

  MCoord width=+cfg.width;

  art.path(width,+cfg.line,p.getTopMid(),p.getBottomMid());
 }

} // namespace Video
} // namespace CCore


