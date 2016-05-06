/* ShapeLib.LabelShape.cpp */
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

/* class LabelShape */

Point LabelShape::getMinSize() const
 {
  TextSize ts=cfg.font.get()->text(Range(text));

  IntGuard( !ts.overflow );

  return Point(ts.full_dx,ts.dy).addXY(2);
 }

void LabelShape::draw(const DrawBuf &buf) const
 {
  cfg.font.get()->text(buf,pane,TextPlace(align_x,align_y),Range(text),enable?+cfg.text:+cfg.inactive);
 }

} // namespace Video
} // namespace CCore


