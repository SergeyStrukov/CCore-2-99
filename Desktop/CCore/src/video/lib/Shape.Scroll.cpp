/* Shape.Scroll.cpp */
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

#include <CCore/inc/video/lib/Shape.Scroll.h>

#include <CCore/inc/video/FigureLib.h>

namespace CCore {
namespace Video {

/* class ScrollShape */

unsigned ScrollShape::Accelerate(unsigned count,unsigned period)
 {
  if( count==0 ) return 1;

  if( count<period ) return 0;

  count-=period;

  count/=period;

  if( count<10 ) return count+1;

  return 10;
 }

ulen ScrollShape::Delta(ulen m,uCoord p,uCoord q)
 {
  if( p>=q ) return m;

  UIntFunc<ulen>::Mul mul(m,p);

  return UIntFunc<ulen>::Div(mul.hi,mul.lo,q);
 }

ulen ScrollShape::delta(uCoord delta,uCoord len,uCoord dlen) const
 {
  if( total<=page ) return 0;

  uCoord ext=2*dlen;

  if( len<=ext ) return 0;

  len-=ext;

  if( len<=dlen ) return 0;

  if( Delta(total,dlen,len)<page ) return Delta(total,delta,len);

  return Delta(total-page,delta,len-dlen);
 }

void ScrollShape::dragPos(Coord from,Coord to,uCoord len,uCoord dlen)
 {
  if( from<to )
    {
     pos=AddToCap(drag_pos, delta(IntDist(from,to),len,dlen) ,getMaxPos());
    }
  else if( from>to )
    {
     pos=PosSub(drag_pos, delta(IntDist(to,from),len,dlen) );
    }
 }

/* class XScrollShape */

SizeY XScrollShape::getMinSize() const
 {
  return +cfg.dxy;
 }

ScrollType XScrollShape::getType(MPoint point) const
 {
  MPane p(pane);

  if( !p || page>=total || !enable ) return ScrollType_None;

  MPoint a=pane.getBase();
  MPoint s=p.getSize();

  MCoord y0=a.y;
  MCoord y1=y0+s.y;

  if( point.y<y0 || point.y>y1 ) return ScrollType_None;

  MCoord x0=a.x;
  MCoord x1=x0+s.y;
  MCoord x5=x0+s.x;
  MCoord x4=x5-s.y;

  if( point.x<x0 || point.x>x5 ) return ScrollType_None;

  if( point.x<x1 ) return ScrollType_Down;

  if( point.x>x4 ) return ScrollType_Up;

  ulen cap=total-page;
  ulen pos_=Min(pos,cap);

  MCoord len=Max(Position(page,total,0,x4-x1),s.y);

  MCoord x2=Position(pos_,cap,x1,x4-len);
  MCoord x3=x2+len;

  if( point.x<x2 ) return ScrollType_DownPage;

  if( point.x>x3 ) return ScrollType_UpPage;

  return ScrollType_Drag;
 }

void XScrollShape::draw(const DrawBuf &buf) const
 {
  MPane p(pane);

  if( !p ) return;

  SmoothDrawArt art(buf);

  MPoint a=p.getBase();
  MPoint s=p.getSize();

  art.block(pane,+cfg.back);

  if( s.x<5*s.y )
    {
     art.block(pane,+cfg.bottom);
     art.block(pane.shrink(0,pane.dy/3),+cfg.face);

     return;
    }

  MCoord y0=a.y;
  MCoord y1=y0+s.y;

  MCoord x0=a.x;
  MCoord x1=x0+s.y;
  MCoord x5=x0+s.x;
  MCoord x4=x5-s.y;

  bool enable_=enable;

  MCoord x2;
  MCoord x3;

  if( page>=total )
    {
     enable_=false;

     x2=x1;
     x3=x4;
    }
  else
    {
     ulen cap=total-page;
     ulen pos_=Min(pos,cap);

     MCoord len=Max(Position(page,total,0,x4-x1),s.y);

     x2=Position(pos_,cap,x1,x4-len);
     x3=x2+len;
    }

  MCoord width=+cfg.width;
  MCoord d=Max<MCoord>(s.y/6,width);

  VColor top=+cfg.top;
  VColor bottom=+cfg.bottom;
  VColor face=+cfg.face;
  VColor faceUp=+cfg.faceUp;
  VColor topf=focus?+cfg.focus:top;
  VColor eface=enable_?face:bottom;

  // down

  if( down==ScrollType_Down )
    {
     FigureBox fig_box(x0,x1,y0,y1);

     fig_box.solid(art,bottom);

     FigureLeftArrow fig(x0+d,x1-d,y0+d,y1-d);

     fig.shift(MPoint::Diag(width));

     fig.curveSolid(art,eface);
    }
  else
    {
     FigureTopBorder fig_top(x0,x1,y0,y1,width);

     fig_top.solid(art,topf);

     FigureBottomBorder fig_bottom(x0,x1,y0,y1,width);

     fig_bottom.solid(art,bottom);

     FigureLeftArrow fig(x0+d,x1-d,y0+d,y1-d);

     VColor fc;

     if( enable_ )
       {
        if( mover==ScrollType_Down )
          fc=faceUp;
        else
          fc=face;
       }
     else
       {
        fc=bottom;
       }

     fig.curveSolid(art,fc);
    }

  // up

  if( down==ScrollType_Up )
    {
     FigureBox fig_box(x4,x5,y0,y1);

     fig_box.solid(art,bottom);

     FigureRightArrow fig(x4+d,x5-d,y0+d,y1-d);

     fig.shift(MPoint::Diag(width));

     fig.curveSolid(art,eface);
    }
  else
    {
     FigureTopBorder fig_top(x4,x5,y0,y1,width);

     fig_top.solid(art,topf);

     FigureBottomBorder fig_bottom(x4,x5,y0,y1,width);

     fig_bottom.solid(art,bottom);

     FigureRightArrow fig(x4+d,x5-d,y0+d,y1-d);

     VColor fc;

     if( enable_ )
       {
        if( mover==ScrollType_Up )
          fc=faceUp;
        else
          fc=face;
       }
     else
       {
        fc=bottom;
       }

     fig.curveSolid(art,fc);
    }

  // down page

  {
   FigureBox fig_top(x1,x2,y0,y0+width);

   fig_top.solid(art,topf);

   FigureBox fig_bottom(x1,x2,y1-width,y1);

   fig_bottom.solid(art,bottom);
  }

  if( down==ScrollType_DownPage )
    {
     MCoord e=Ratio(427,10)*s.y;

     FigureBox fig(x1,x2,y0+e,y1-e);

     fig.solid(art,face);
    }
  else
    {
     MCoord e=Ratio(3,3)*s.y;

     FigureBox fig(x1,x2,y0+e,y1-e);

     VColor fc;

     if( enable_ )
       {
        if( mover==ScrollType_DownPage )
          fc=faceUp;
        else
          fc=face;
       }
     else
       {
        fc=bottom;
       }

     fig.solid(art,fc);
    }

  // up page

  {
   FigureBox fig_top(x3,x4,y0,y0+width);

   fig_top.solid(art,topf);

   FigureBox fig_bottom(x3,x4,y1-width,y1);

   fig_bottom.solid(art,bottom);
  }

  if( down==ScrollType_UpPage )
    {
     MCoord e=Ratio(427,10)*s.y;

     FigureBox fig(x3,x4,y0+e,y1-e);

     fig.solid(art,face);
    }
  else
    {
     MCoord e=Ratio(3,3)*s.y;

     FigureBox fig(x3,x4,y0+e,y1-e);

     VColor fc;

     if( enable_ )
       {
        if( mover==ScrollType_UpPage )
          fc=faceUp;
        else
          fc=face;
       }
     else
       {
        fc=bottom;
       }

     fig.solid(art,fc);
    }

  // drag

  {
   FigureBox fig(x2,x3,y0,y1);

   fig.solid(art,TwoField({x0,y0},top,{x0,y1},bottom));
  }

  {
   MCoord f=(x3-x2)/4;

   MCoord a0=x2+f;
   MCoord a1=x2+(x3-x2)/2;
   MCoord a2=x3-f;

   MCoord e=s.y/3;

   MCoord b0=y0+e;
   MCoord b1=y0+s.y/2;
   MCoord b2=y1-e;

   VColor fc;

   if( enable_ )
     {
      if( mover==ScrollType_Drag )
        fc=faceUp;
      else
        fc=face;
     }
   else
     {
      fc=bottom;
     }

   if( a2-a0>2*(b2-b0) )
     {
      MCoord c=(a2-a0)/2-(b2-b0);

      FigurePoints<6> fig;

      fig[0]={a0,b1};
      fig[1]={a1-c,b0};
      fig[2]={a1+c,b0};
      fig[3]={a2,b1};
      fig[4]={a1+c,b2};
      fig[5]={a1-c,b2};

      fig.solid(art,fc);
     }
   else
     {
      FigurePoints<4> fig;

      fig[0]={a0,b1};
      fig[1]={a1,b0};
      fig[2]={a2,b1};
      fig[3]={a1,b2};

      fig.solid(art,fc);
     }
  }
 }

/* class YScrollShape */

SizeX YScrollShape::getMinSize() const
 {
  return +cfg.dxy;
 }

ScrollType YScrollShape::getType(MPoint point) const
 {
  MPane p(pane);

  if( !p || page>=total || !enable ) return ScrollType_None;

  MPoint a=pane.getBase();
  MPoint s=p.getSize();

  MCoord x0=a.x;
  MCoord x1=x0+s.x;

  if( point.x<x0 || point.x>x1 ) return ScrollType_None;

  MCoord y0=a.y;
  MCoord y1=y0+s.x;
  MCoord y5=y0+s.y;
  MCoord y4=y5-s.x;

  if( point.y<y0 || point.y>y5 ) return ScrollType_None;

  if( point.y<y1 ) return ScrollType_Down;

  if( point.y>y4 ) return ScrollType_Up;

  ulen cap=total-page;
  ulen pos_=Min(pos,cap);

  MCoord len=Max(Position(page,total,0,y4-y1),s.x);

  MCoord y2=Position(pos_,cap,y1,y4-len);
  MCoord y3=y2+len;

  if( point.y<y2 ) return ScrollType_DownPage;

  if( point.y>y3 ) return ScrollType_UpPage;

  return ScrollType_Drag;
 }

void YScrollShape::draw(const DrawBuf &buf) const
 {
  MPane p(pane);

  if( !p ) return;

  SmoothDrawArt art(buf);

  MPoint a=p.getBase();
  MPoint s=p.getSize();

  art.block(pane,+cfg.back);

  if( s.y<5*s.x )
    {
     art.block(pane,+cfg.bottom);
     art.block(pane.shrink(pane.dx/3,0),+cfg.face);

     return;
    }

  MCoord x0=a.x;
  MCoord x1=x0+s.x;

  MCoord y0=a.y;
  MCoord y1=y0+s.x;
  MCoord y5=y0+s.y;
  MCoord y4=y5-s.x;

  bool enable_=enable;

  MCoord y2;
  MCoord y3;

  if( page>=total )
    {
     enable_=false;

     y2=y1;
     y3=y4;
    }
  else
    {
     ulen cap=total-page;
     ulen pos_=Min(pos,cap);

     MCoord len=Max(Position(page,total,0,y4-y1),s.x);

     y2=Position(pos_,cap,y1,y4-len);
     y3=y2+len;
    }

  MCoord width=+cfg.width;
  MCoord d=Max<MCoord>(s.x/6,width);

  VColor top=+cfg.top;
  VColor bottom=+cfg.bottom;
  VColor face=+cfg.face;
  VColor faceUp=+cfg.faceUp;
  VColor topf=focus?+cfg.focus:top;
  VColor eface=enable_?face:bottom;

  // down

  if( down==ScrollType_Down )
    {
     FigureBox fig_box(x0,x1,y0,y1);

     fig_box.solid(art,bottom);

     FigureUpArrow fig(x0+d,x1-d,y0+d,y1-d);

     fig.shift(MPoint::Diag(width));

     fig.curveSolid(art,eface);
    }
  else
    {
     FigureTopBorder fig_top(x0,x1,y0,y1,width);

     fig_top.solid(art,topf);

     FigureBottomBorder fig_bottom(x0,x1,y0,y1,width);

     fig_bottom.solid(art,bottom);

     FigureUpArrow fig(x0+d,x1-d,y0+d,y1-d);

     VColor fc;

     if( enable_ )
       {
        if( mover==ScrollType_Down )
          fc=faceUp;
        else
          fc=face;
       }
     else
       {
        fc=bottom;
       }

     fig.curveSolid(art,fc);
    }

  // up

  if( down==ScrollType_Up )
    {
     FigureBox fig_box(x0,x1,y4,y5);

     fig_box.solid(art,bottom);

     FigureDownArrow fig(x0+d,x1-d,y4+d,y5-d);

     fig.shift(MPoint::Diag(width));

     fig.curveSolid(art,eface);
    }
  else
    {
     FigureTopBorder fig_top(x0,x1,y4,y5,width);

     fig_top.solid(art,topf);

     FigureBottomBorder fig_bottom(x0,x1,y4,y5,width);

     fig_bottom.solid(art,bottom);

     FigureDownArrow fig(x0+d,x1-d,y4+d,y5-d);

     VColor fc;

     if( enable_ )
       {
        if( mover==ScrollType_Up )
          fc=faceUp;
        else
          fc=face;
       }
     else
       {
        fc=bottom;
       }

     fig.curveSolid(art,fc);
    }

  // down page

  {
   FigureBox fig_top(x0,x0+width,y1,y2);

   fig_top.solid(art,topf);

   FigureBox fig_bottom(x1-width,x1,y1,y2);

   fig_bottom.solid(art,bottom);
  }

  if( down==ScrollType_DownPage )
    {
     MCoord e=Ratio(427,10)*s.x;

     FigureBox fig(x0+e,x1-e,y1,y2);

     fig.solid(art,face);
    }
  else
    {
     MCoord e=Ratio(3,3)*s.x;

     FigureBox fig(x0+e,x1-e,y1,y2);

     VColor fc;

     if( enable_ )
       {
        if( mover==ScrollType_DownPage )
          fc=faceUp;
        else
          fc=face;
       }
     else
       {
        fc=bottom;
       }

     fig.solid(art,fc);
    }

  // up page

  {
   FigureBox fig_top(x0,x0+width,y3,y4);

   fig_top.solid(art,topf);

   FigureBox fig_bottom(x1-width,x1,y3,y4);

   fig_bottom.solid(art,bottom);
  }

  if( down==ScrollType_UpPage )
    {
     MCoord e=Ratio(427,10)*s.x;

     FigureBox fig(x0+e,x1-e,y3,y4);

     fig.solid(art,face);
    }
  else
    {
     MCoord e=Ratio(3,3)*s.x;

     FigureBox fig(x0+e,x1-e,y3,y4);

     VColor fc;

     if( enable_ )
       {
        if( mover==ScrollType_UpPage )
          fc=faceUp;
        else
          fc=face;
       }
     else
       {
        fc=bottom;
       }

     fig.solid(art,fc);
    }

  // drag

  {
   FigureBox fig(x0,x1,y2,y3);

   fig.solid(art,TwoField({x0,y0},top,{x1,y0},bottom));
  }

  {
   MCoord e=s.x/3;

   MCoord a0=x0+e;
   MCoord a1=x0+s.x/2;
   MCoord a2=x1-e;

   MCoord f=(y3-y2)/4;

   MCoord b0=y2+f;
   MCoord b1=y2+(y3-y2)/2;
   MCoord b2=y3-f;

   VColor fc;

   if( enable_ )
     {
      if( mover==ScrollType_Drag )
        fc=faceUp;
      else
        fc=face;
     }
   else
     {
      fc=bottom;
     }

   if( b2-b0>2*(a2-a0) )
     {
      MCoord c=(b2-b0)/2-(a2-a0);

      FigurePoints<6> fig;

      fig[0]={a0,b1-c};
      fig[1]={a1,b0};
      fig[2]={a2,b1-c};
      fig[3]={a2,b1+c};
      fig[4]={a1,b2};
      fig[5]={a0,b1+c};

      fig.solid(art,fc);
     }
   else
     {
      FigurePoints<4> fig;

      fig[0]={a0,b1};
      fig[1]={a1,b0};
      fig[2]={a2,b1};
      fig[3]={a1,b2};

      fig.solid(art,fc);
     }
  }
 }

} // namespace Video
} // namespace CCore

