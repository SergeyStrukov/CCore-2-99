/* ShapeLib.InfoShape.cpp */
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

/* class InfoShape */

Point InfoShape::getMinSize() const
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

void InfoShape::setMax()
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

     ulen h=ulen(inner.dy/fs.dy);

     if( count>h )
       yoffMax=count-h;
     else
       yoffMax=0;

     dxoff=fs.medDx();
    }
  else
    {
     xoffMax=0;
     yoffMax=0;
     dxoff=0;
    }
 }

void InfoShape::draw(const DrawBuf &buf) const
 {
  if( !pane ) return;

  SmoothDrawArt art(buf);

  VColor text=enable?+cfg.text:+cfg.inactive;

  Point space=+cfg.space;

  // decor

  {
   MPane p(pane);

   MCoord width=+cfg.width;
   MCoord dx=Fraction(space.x)-width;
   MCoord dy=Fraction(space.y)-width;

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
      font->text(tbuf,row,TextPlace(AlignX_Left,AlignY_Top),info->getLine(index),text);
     }
  }
 }

} // namespace Video
} // namespace CCore


