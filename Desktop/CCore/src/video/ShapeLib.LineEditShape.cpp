/* ShapeLib.LineEditShape.cpp */
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

/* class LineEditShape */

Point LineEditShape::getMinSize() const
 {
  return getMinSize("Sample12345");
 }

Point LineEditShape::getMinSize(StrLen sample_text) const
 {
  Font font=cfg.font.get();

  MCoord width=+cfg.width;

  FontSize fs=font->getSize();

  MCoord ex=(Fraction(fs.dy)+2*width)/4+Fraction(+cfg.ex);

  Coord dx=RoundUpLen(ex+width);
  Coord dy=RoundUpLen(width)+(+cfg.cursor_dx);

  TextSize ts=font->text(sample_text);

  IntGuard( !ts.overflow );

  return Point(ts.full_dx,ts.dy)+Point(2*dx+dy,2*dy)+(+cfg.space);
 }

void LineEditShape::setMax()
 {
  Font font=cfg.font.get();

  MCoord width=+cfg.width;

  FontSize fs=font->getSize();

  MCoord ex=(Fraction(fs.dy)+2*width)/4+Fraction(+cfg.ex);

  Coord dx=RoundUpLen(ex+width);
  Coord dy=RoundUpLen(width);

  Pane inner=pane.shrink(dx,dy);

  TextSize ts=font->text(text_buf.prefix(len));

  IntGuard( !ts.overflow );

  Coord extra=2*(+cfg.cursor_dx)+IntAbs(ts.skew);

  Coord tx=IntAdd(ts.full_dx,extra);

  if( tx>inner.dx )
    xoffMax=tx-inner.dx;
  else
    xoffMax=0;

  dxoff=fs.medDx();
 }

void LineEditShape::showCursor()
 {
  MCoord width=+cfg.width;

  Font font=cfg.font.get();

  FontSize fs=font->getSize();

  MCoord ex=(Fraction(fs.dy)+2*width)/4+Fraction(+cfg.ex);

  Coord dx=RoundUpLen(ex+width);
  Coord inner_dx=pane.dx-2*dx;

  if( inner_dx<=0 ) return;

  TextSize ts=font->text(text_buf.prefix(pos));

  Coord cursor_dx=+cfg.cursor_dx;
  Coord x=ts.dx0+ts.dx-xoff;

  inner_dx-=2*cursor_dx;

  if( x<cursor_dx )
    {
     x=cursor_dx-x;

     if( xoff>x )
       xoff-=x;
     else
       xoff=0;
    }
  else if( x>=inner_dx )
    {
     x=x-inner_dx;

     if( xoff<xoffMax-x )
       xoff+=x;
     else
       xoff=xoffMax;
    }
 }

ulen LineEditShape::getPosition(Point point) const
 {
  Font font=cfg.font.get();

  MCoord width=+cfg.width;

  FontSize fs=font->getSize();

  MCoord ex=(Fraction(fs.dy)+2*width)/4+Fraction(+cfg.ex);

  Coord dx=RoundUpLen(ex+width);
  Coord dy=RoundUpLen(width);

  Pane inner=pane.shrink(dx,dy);

  if( !inner ) return 0;

  point-=inner.getBase();

  point+=Point(xoff,0);

  TextSize ts=font->text(text_buf.prefix(pos));

  Coord cursor_dx=+cfg.cursor_dx;

  Coord x1=ts.dx0;
  Coord x2=x1+ts.dx;
  Coord x3=x2+cursor_dx;

  Coord ytop=(inner.dy-ts.dy)/2;
  Coord ybase=ytop+ts.by;

  ulen pos1=font->position(text_buf.prefix(pos),point-Point(x1,ybase));

  if( pos1==0 ) return 0;

  if( pos1<=pos ) return pos1-1;

  ulen pos2=font->position(text_buf.part(pos,len-pos),point-Point(x3,ybase));

  if( pos2==0 ) return pos;

  if( pos2<=len-pos ) return pos2-1+pos;

  return len;
 }

void LineEditShape::drawText(Font font,const DrawBuf &buf,Pane pane,TextPlace place,StrLen text,VColor vc) const
 {
  font->text(buf,pane,place,text,vc);
 }

void LineEditShape::draw(const DrawBuf &buf) const
 {
  MPane p(pane);

  if( !p ) return;

  SmoothDrawArt art(buf);

  Font font=cfg.font.get();

  // figure

  MCoord width=+cfg.width;

  FontSize fs=font->getSize();

  MCoord ex=(Fraction(fs.dy)+2*width)/4+Fraction(+cfg.ex);

  VColor text=enable?+cfg.text:+cfg.inactive;

  FigureButton fig(p,ex);

  // body

  fig.curveSolid(art,+cfg.back);

  // border

  if( focus )
    {
     fig.curveLoop(art,HalfPos,width,+cfg.focus);
    }
  else
    {
     auto fig_top=fig.getTop();

     fig_top.curvePath(art,HalfPos,width,+cfg.top);

     auto fig_bottom=fig.getBottom();

     fig_bottom.curvePath(art,HalfPos,width,+cfg.bottom);
    }

  // arrows

  {
   MCoord len=ex-width;
   MCoord y=p.y+p.dy/2;

   if( xoff>0 ) // Left
     {
      MCoord x=p.x+width;

      FigureLeftMark fig(x,y,len);

      fig.solid(art,text);
     }

   if( xoff<xoffMax ) // Right
     {
      MCoord x=p.ex-width;

      FigureRightMark fig(x,y,len);

      fig.solid(art,text);
     }
  }

  // text

  {
   Coord dx=RoundUpLen(ex+width);
   Coord dy=RoundUpLen(width);

   Pane inner=pane.shrink(dx,dy);

   if( !inner ) return;

   DrawBuf tbuf=buf.cutRebase(inner);

   Pane tpane(-xoff,0,IntAdd(xoff,inner.dx),inner.dy);

   ulen pos=enable?this->pos:len;

   TextSize ts=font->text(text_buf.prefix(pos));

   Coord cursor_dx=+cfg.cursor_dx;

   Coord x1=ts.dx0;
   Coord x2=x1+ts.dx;
   Coord x3=x2+cursor_dx;

   Coord ytop=(inner.dy-ts.dy)/2;
   Coord ybase=ytop+ts.by;

   MCoord w=Fraction(cursor_dx);
   MCoord h=Fraction(ts.dy);
   MCoord skew=Fraction(ts.skew);

   MCoord y0=Fraction(ytop);

   MCoord skew1=Rational(h-Fraction(ts.by),h)*skew;

   // selection

   if( enable && select_len )
     {
      Coord xs0;

      if( select_off<pos )
        {
         xs0=x1+font->text(text_buf.prefix(pos),select_off).dx;
        }
      else
        {
         xs0=x3+font->text(text_buf.part(pos,len-pos),select_off-pos).dx;
        }

      ulen lim=select_off+select_len;

      Coord xs1;

      if( lim<=pos )
        {
         xs1=x1+font->text(text_buf.prefix(pos),lim).dx;
        }
      else
        {
         xs1=x3+font->text(text_buf.part(pos,len-pos),lim-pos).dx;
        }

      Smooth::DrawArt art(tbuf);

      FigurePoints<4> fig;

      MCoord a=Fraction(xs0-xoff)-skew1;
      MCoord b=Fraction(xs1-xoff)-skew1;

      fig[0]={a+skew,y0-w};
      fig[1]={a,y0+h+w};
      fig[2]={b,y0+h+w};
      fig[3]={b+skew,y0-w};

      fig.solid(art,+cfg.select);
     }

   drawText(font,tbuf,tpane,TextPlace(x1,ybase),text_buf.prefix(pos),text);

   drawText(font,tbuf,tpane,TextPlace(x3,ybase),text_buf.part(pos,len-pos),text);

   // cursor

   if( enable && ( !hide_cursor || focus ) )
     {
      MCoord c0=y0+Fraction(dy);
      MCoord c1=c0+h;

      MCoord b0=Fraction(x2-xoff+dx)-skew1;
      MCoord b1=b0+w;

      MCoord a0=b0+skew;
      MCoord a1=a0+w;

      Smooth::DrawArt art(buf);

      FigurePoints<12> fig;

      fig[0]={a1,c0};
      fig[1]={a1+w,c0};
      fig[2]={a1+w,c0-w};
      fig[3]={a0-w,c0-w};
      fig[4]={a0-w,c0};
      fig[5]={a0,c0};

      fig[6]={b0,c1};
      fig[7]={b0-w,c1};
      fig[8]={b0-w,c1+w};
      fig[9]={b1+w,c1+w};
      fig[10]={b1+w,c1};
      fig[11]={b1,c1};

      if( focus && cursor )
        {
         fig.solid(art,+cfg.cursor);
        }
      else
        {
         fig.loop(art,HalfPos,w/3,+cfg.cursor);
        }
     }
  }
 }

} // namespace Video
} // namespace CCore


