/* ShapeLib.SimpleTextListWindow.cpp */
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

/* class SimpleTextListShape */

Point SimpleTextListShape::getMinSize() const
 {
  Font font=cfg.font.get();

  Coord dx=0;
  Coord dy=0;

  for(ulen index=0,count=info->getLineCount(); index<count ;index++)
    {
     TextSize ts=font->text(info->getLine(index));

     IntGuard( !ts.overflow );

     Replace_max(dx,ts.full_dx);

     dy=IntAdd(dy,ts.dy);
    }

  return 2*(+cfg.space)+Point(dx,dy);
 }

void SimpleTextListShape::setMax()
 {
  Pane inner=pane.shrink(+cfg.space);

  if( +inner )
    {
     ulen count=info->getLineCount();

     Font font=cfg.font.get();

     FontSize fs=font->getSize();

     Coord dx=0;

     for(ulen index=0; index<count ;index++)
       {
        TextSize ts=font->text(info->getLine(index));

        IntGuard( !ts.overflow );

        Replace_max(dx,ts.full_dx);
       }

     if( dx>inner.dx )
       xoffMax=dx-inner.dx;
     else
       xoffMax=0;

     page=ulen(inner.dy/fs.dy);

     if( count>page )
       yoffMax=count-page;
     else
       yoffMax=0;

     dxoff=fs.medDx();
    }
  else
    {
     xoffMax=0;
     yoffMax=0;
     dxoff=0;
     page=0;
    }
 }

void SimpleTextListShape::showSelect()
 {
  if( select<yoff )
    {
     yoff=select;
    }
  else
    {
     ulen i=select-yoff;

     if( i>=page && page>0 )
       {
        yoff=Min<ulen>(select-page+1,yoffMax);
       }
    }
 }

ulen SimpleTextListShape::getPosition(Point point) const
 {
  Pane inner=pane.shrink(+cfg.space);

  if( !inner ) return 0;

  if( point.y<inner.y ) return yoff?yoff-1:0;

  if( point.y>=inner.y+inner.dy ) return yoff+page;

  FontSize fs=cfg.font.get()->getSize();

  return yoff+ulen((point.y-inner.y)/fs.dy);
 }

void SimpleTextListShape::draw(const DrawBuf &buf) const
 {
  if( !pane ) return;

  SmoothDrawArt art(buf);

  art.block(pane,+cfg.back);

  VColor text=enable?+cfg.text:+cfg.inactive;

  Point space=+cfg.space;

  // decor

  {
   MPane p(pane);

   MCoord width=+cfg.width;
   MCoord dx=Fraction(space.x)-width;
   MCoord dy=Fraction(space.y)-width;

   FigureTopBorder fig_top(p,width);

   fig_top.solid(art,+cfg.top);

   FigureBottomBorder fig_bottom(p,width);

   fig_bottom.solid(art,+cfg.bottom);

   if( focus )
     {
      FigureBox fig(p.shrink(Fraction(space.x)/2,Fraction(space.y)/2));

      fig.loop(art,width,+cfg.focus);
     }

   if( xoff>0 )
     {
      FigureLeftMark fig(p,dx);

      fig.solid(art,text);
     }

   if( xoff<xoffMax )
     {
      FigureRightMark fig(p,dx);

      fig.solid(art,text);
     }

   if( yoff>0 )
     {
      FigureUpMark fig(p,dy);

      fig.solid(art,text);
     }

   if( yoff<yoffMax )
     {
      FigureDownMark fig(p,dy);

      fig.solid(art,text);
     }
  }

  // text

  {
   Pane inner=pane.shrink(space);

   if( !inner ) return;

   ulen count=info->getLineCount();
   ulen index=yoff;

   Font font=cfg.font.get();

   FontSize fs=font->getSize();

   DrawBuf tbuf=buf.cutRebase(inner);

   Pane row(-xoff,0,IntAdd(xoff,inner.dx),fs.dy);

   for(; index<count && row.y+row.dy<=inner.dy ;index++,row.y+=row.dy)
     {
      if( enable && index==select ) tbuf.block_safe(row,+cfg.select);

      font->text(tbuf,row,TextPlace(AlignX_Left,AlignY_Top),info->getLine(index),text);
     }
  }
 }

} // namespace Video
} // namespace CCore

