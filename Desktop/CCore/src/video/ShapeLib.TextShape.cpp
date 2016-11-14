/* ShapeLib.TextShape.cpp */
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

/* class TextShape */

Point TextShape::getMinSize() const
 {
  return getMinSize(text.str());
 }

Point TextShape::getMinSize(StrLen text) const
 {
  TextSize ts=cfg.font->text(text);

  IntGuard( !ts.overflow );

  MCoord width=+cfg.width;

  MCoord ex=(Fraction(ts.dy)+2*width)/4;

  Coord dx=RoundUpLen(ex);
  Coord dy=RoundUpLen(width);

  return 2*Point(dx,dy)+Point(ts.full_dx,ts.dy)+(+cfg.space);
 }

void TextShape::draw(const DrawBuf &buf) const
 {
  MPane p(pane);

  if( !p ) return;

  SmoothDrawArt art(buf);

  Font font=cfg.font.get();

  // figure

  MCoord width=+cfg.width;

  FontSize fs=font->getSize();

  MCoord ex=(Fraction(fs.dy)+2*width)/4;

  FigureButton fig(p,ex);

  // body

  fig.curveSolid(art,+cfg.back);

  // border

  auto fig_top=fig.getTop();

  fig_top.curvePath(art,HalfPos,width,+cfg.top);

  auto fig_bottom=fig.getBottom();

  fig_bottom.curvePath(art,HalfPos,width,+cfg.bottom);

  // text

  Coord dx=RoundUpLen(ex);
  Coord dy=RoundUpLen(width);

  font->text(buf,pane.shrink(dx,dy),TextPlace(align_x,align_y),text.str(),enable?+cfg.text:+cfg.inactive);
 }

} // namespace Video
} // namespace CCore


