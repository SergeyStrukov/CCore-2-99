/* Shape.ScrollList.cpp */
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

#include <CCore/inc/video/lib/Shape.ScrollList.h>

#include <CCore/inc/video/FigureLib.h>

namespace CCore {
namespace Video {

/* class ScrollListShape */

Point ScrollListShape::getMinSize() const
 {
  Font font=cfg.font.get();

  Coord dx=0;
  Coord dy=0;

  for(ulen index=0,count=info->getLineCount(); index<count ;index++)
    {
     TextSize ts=font->text(GetText(info->getLine(index)));

     IntGuard( !ts.overflow );

     Replace_max(dx,ts.full_dx);

     dy=IntAdd(dy,ts.dy);
    }

  return 2*(+cfg.space)+Point(dx,dy);
 }

void ScrollListShape::setMax()
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
        TextSize ts=font->text(GetText(info->getLine(index)));

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

void ScrollListShape::initSelect() // 0 -> down , MaxULen
 {
  select=MaxULen;

  setSelectDown(0);
 }

bool ScrollListShape::setSelectDown(ulen pos) // pos -> down , -> up , unchanged
 {
  ulen count=info->getLineCount();

  Replace_min(pos,count);

  for(ulen i=pos; i<count ;i++)
    {
     if( info->getLine(i).type==ComboInfoText )
       {
        return Change(select,i);
       }
    }

  for(ulen i=pos; i-- ;)
    {
     if( info->getLine(i).type==ComboInfoText )
       {
        return Change(select,i);
       }
    }

  return false;
 }

bool ScrollListShape::setSelectUp(ulen pos) // pos -> up , -> down , unchanged
 {
  ulen count=info->getLineCount();

  if( !count ) return false;

  Replace_min(pos,count-1);

  pos++;

  for(ulen i=pos; i-- ;)
    {
     if( info->getLine(i).type==ComboInfoText )
       {
        return Change(select,i);
       }
    }

  for(ulen i=pos; i<count ;i++)
    {
     if( info->getLine(i).type==ComboInfoText )
       {
        return Change(select,i);
       }
    }

  return false;
 }

bool ScrollListShape::showSelect()
 {
  if( select<yoff )
    {
     yoff=select;

     return true;
    }
  else
    {
     ulen i=select-yoff;

     if( i>=page && page>0 )
       {
        yoff=Min<ulen>(select-page+1,yoffMax);

        return true;
       }
    }

  return false;
 }

ulen ScrollListShape::getPosition(Point point) const
 {
  Pane inner=pane.shrink(+cfg.space);

  if( !inner ) return 0;

  if( point.y<inner.y ) return yoff?yoff-1:0;

  if( point.y>=inner.y+inner.dy ) return yoff+page;

  FontSize fs=cfg.font.get()->getSize();

  return yoff+ulen((point.y-inner.y)/fs.dy);
 }

void ScrollListShape::draw(const DrawBuf &buf) const
 {
  if( !pane ) return;

  SmoothDrawArt art(buf);

  art.block(pane,+cfg.back);

  VColor text=enable?+cfg.text:+cfg.inactive;
  VColor title=enable?+cfg.title:+cfg.inactive;
  VColor top=+cfg.top;
  VColor bottom=+cfg.bottom;

  Point space=+cfg.space;

  // decor

  {
   MPane p(pane);

   MCoord width=+cfg.width;
   MCoord dx=Fraction(space.x)-width;
   MCoord dy=Fraction(space.y)-width;

   FigureTopBorder fig_top(p,width);

   fig_top.solid(art,top);

   FigureBottomBorder fig_bottom(p,width);

   fig_bottom.solid(art,bottom);

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
      if( enable && index==select ) tbuf.erase(row,+cfg.select);

      ComboInfoItem item=info->getLine(index);

      switch( item.type )
        {
         case ComboInfoText :
          {
           font->text(tbuf,row,TextPlace(AlignX_Left,AlignY_Top),item.text,text);
          }
         break;

         case ComboInfoSeparator :
          {
           SmoothDrawArt tart(tbuf);

           MPane p(row);
           MCoord delta=2*(p.dy/5);

           p=p.shrinkY(delta);

           TwoField field(p.getTopLeft(),bottom,p.getBottomLeft(),top);

           FigureBox(p).solid(tart,field);
          }
         break;

         case ComboInfoTitle :
          {
           SmoothDrawArt tart(tbuf);

           TextSize ts=font->text(item.text);
           Coord off=ts.dy;
           Coord len=ts.full_dx;

#if 0

           MPane p(row);
           MCoord delta=(p.dy/3);

           p=p.shrinkY(delta);

           TwoField field(p.getTopLeft(),+cfg.title_top,p.getBottomLeft(),+cfg.title_bottom);

           MCoord x1=p.x+MPoint::LShift(off);
           MCoord x2=x1+MPoint::LShift(len);

           MPane p1(p.x,x1,p.y,p.ey);
           MPane p2(x2,p.ex,p.y,p.ey);

           FigureBox(p1).solid(tart,field);

           if( +p2 ) FigureBox(p2).solid(tart,field);

#else

           MPane p(row);

           TwoField field(p.getTopLeft(),+cfg.title_top,p.getBottomLeft(),+cfg.title_bottom);

           FigureBox(p).solid(tart,field);

#endif

           Pane t(row.x+off,row.y,len,row.dy);

           font->text(tbuf,t,TextPlace(AlignX_Center,AlignY_Top),item.text,title);
          }
         break;
        }
     }
  }
 }

} // namespace Video
} // namespace CCore

