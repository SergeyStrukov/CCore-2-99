/* Shape.FixedFrame.cpp */
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
//  Copyright (c) 2017 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <CCore/inc/video/lib/Shape.FixedFrame.h>

#include <CCore/inc/video/FigureLib.h>

namespace CCore {
namespace Video {

/* class FixedFrameShape */

class FixedFrameShape::DrawArt : public SmoothDrawArt
 {
  public:

   DrawArt(const DrawBuf &buf) : SmoothDrawArt(buf) {}
 };

void FixedFrameShape::draw_Frame(DrawArt &art,Pane part) const
 {
  VColor gray=+cfg.gray;
  VColor frame=+cfg.frame;
  VColor small=+cfg.dragSmall;
  VColor frameSmall = has_good_size? frame : small ;

  if( hilight==DragType_Bar ) frameSmall=frame=+cfg.frameHilight;

  if( drag_type==DragType_Bar ) frameSmall=frame=+cfg.frameDrag;

  if( !client )
    {
     art.block(Pane(Null,size),frame);

     return;
    }

  Pane all(Null,size);

  PaneSub sub(all,client);

  MPane outer(all);
  MPane inner(client);

  // top

  Pane top=Inf(sub.top,part);

  if( +top )
    {
     MCoord y0=outer.y;
     MCoord y1=inner.y;

     FigureBox fig{MPane(top)};

     fig.solid(art,YField(y0,frame,y1,gray));
    }

  // bottom

  Pane bottom=Inf(sub.bottom,part);

  if( +bottom )
    {
     MCoord y0=inner.ey;
     MCoord y1=outer.ey;

     FigureBox fig{MPane(bottom)};

     fig.solid(art,YField(y0,frameSmall,y1,gray));
    }

  // left

  Pane left=Inf(sub.left,part);

  if( +left )
    {
     MCoord x0=outer.x;
     MCoord x1=inner.x;

     FigureBox fig{MPane(left)};

     fig.solid(art,XField(x0,frame,x1,gray));
    }

  {
   MCoord x0=outer.x;
   MCoord x1=inner.x;

   MCoord y0=outer.y;
   MCoord y1=inner.y;
   MCoord y2=inner.ey;
   MCoord y3=outer.ey;

   FigureBottomLeftArrow fig1(x0,x1,y0,y1);
   FigureTopLeftArrow fig2(x0,x1,y2,y3);

   XField field(x0,frame,x1,gray);

   fig1.solid(art,field);
   fig2.solid(art,field);
  }

  // right

  Pane right=Inf(sub.right,part);

  if( +right )
    {
     MCoord x0=inner.ex;
     MCoord x1=outer.ex;

     FigureBox fig{MPane(right)};

     fig.solid(art,XField(x0,frameSmall,x1,gray));
    }

  {
   MCoord x0=inner.ex;
   MCoord x1=outer.ex;

   MCoord y0=outer.y;
   MCoord y1=inner.y;
   MCoord y2=inner.ey;
   MCoord y3=outer.ey;

   FigureBottomRightArrow fig1(x0,x1,y0,y1);
   FigureTopRightArrow fig2(x0,x1,y2,y3);

   XField field(x0,frameSmall,x1,gray);

   fig1.solid(art,field);
   fig2.solid(art,field);
  }
 }

void FixedFrameShape::draw_Frame(const DrawBuf &buf,DragType drag_type) const
 {
  Pane part=getPane(drag_type);

  DrawArt art(buf.cut(part));

  draw_Frame(art,part);
 }

void FixedFrameShape::draw_Bar(DrawArt &art) const
 {
  if( +titleBar )
    {
     MPane p(titleBar);

     MCoord width=+cfg.width;

     MCoord ex=p.dy/4;

     VColor gray=+cfg.gray;
     VColor snow=+cfg.snow;

     FigureButton fig(p,ex);

     fig.curveSolid(art, has_focus? +cfg.active : +cfg.inactive );

     fig.getTop().curvePath(art,HalfPos,width,gray);
     fig.getBottom().curvePath(art,HalfPos,width,snow);

     Pane pane=titleBar.shrink(RoundUpLen(ex),RoundUpLen(width));

     cfg.font->text(art.getBuf(),pane,TextPlace(AlignX_Left,AlignY_Center),title.str(),+cfg.title);
    }
 }

auto FixedFrameShape::draw_Btn(DrawArt &art,Pane btn,DragType zone) const
 {
  MPane p(btn);

  MCoord width=+cfg.width;

  MCoord ex=p.dx/8;

  FigureButton fig(p,ex);

  MPane ret=p.shrink(p.dx/5,p.dy/5);

  VColor gray=+cfg.gray;

  if( btn_type==zone )
    {
     fig.curveSolid(art,gray);

     ret+=MPoint::Diag(width);
    }
  else
    {
     VColor face = (hilight==zone)? +cfg.btnFaceHilight : +cfg.btnFace ;

     VColor snow=+cfg.snow;

     fig.curveSolid(art,face);

     fig.getTop().curvePath(art,HalfPos,width,gray);
     fig.getBottom().curvePath(art,HalfPos,width,snow);
    }

  return ret;
 }

void FixedFrameShape::draw_Alert(DrawArt &art) const
 {
  if( +btnAlert )
    {
     MPane p(btnAlert);

     MCoord width=+cfg.width;

     MCoord ex=p.dx/8;

     FigureButton fig(p,ex);

     MPane q=p.shrink(p.dx/5,p.dy/5);

     VColor gray=+cfg.gray;

     VColor alert;

     if( alert_type==AlertType_No )
       {
        fig.curveLoop(art,HalfPos,width,gray);

        alert=+cfg.btnPictNoAlert;
       }
     else
       {
        if( btn_type==DragType_Alert )
          {
           fig.curveSolid(art,gray);

           q+=MPoint::Diag(width);
          }
        else
          {
           VColor face = (hilight==DragType_Alert)? +cfg.btnFaceHilight : +cfg.btnFace ;

           VColor snow=+cfg.snow;

           fig.curveSolid(art,face);

           fig.getTop().curvePath(art,HalfPos,width,gray);
           fig.getBottom().curvePath(art,HalfPos,width,snow);
          }

        alert = (alert_type==AlertType_Closed)? +cfg.btnPictAlert : +cfg.btnPictCloseAlert ;
       }

     if( !alert_blink )
       {
        PolyFigureExclamation fig(q.x+q.dx/2,q.y,q.dy);

        fig.solid(art,alert);
       }
    }
 }

void FixedFrameShape::draw_Help(DrawArt &art) const
 {
  if( +btnHelp )
    {
     MPane p=draw_Btn(art,btnHelp,DragType_Help);

     PolyFigureQuestion fig(p.x+p.dx/2,p.y,p.dy);

     if( help )
       {
        fig.solid(art,+cfg.btnPictAlert);
       }
     else
       {
        fig.solid(art,+cfg.btnPict);
       }
    }
 }

void FixedFrameShape::draw_Min(DrawArt &art) const
 {
  if( +btnMin )
    {
     MPane p=draw_Btn(art,btnMin,DragType_Min);

     FigureBox fig_pict(p.x,p.ex,p.ey-p.dy/4,p.ey);

     fig_pict.solid(art,+cfg.btnPict);
    }
 }

void FixedFrameShape::draw_Close(DrawArt &art) const
 {
  if( +btnClose )
    {
     MPane p=draw_Btn(art,btnClose,DragType_Close);

     MCoord w=p.dx/6;

     VColor pict=+cfg.btnPictClose;

     art.path(w,pict,p.getTopLeft(),p.getBottomRight());
     art.path(w,pict,p.getTopRight(),p.getBottomLeft());
    }
 }

void FixedFrameShape::reset(const DefString &title_,bool is_main_)
 {
  has_focus=false;
  is_main=is_main_;

  drag_type=DragType_None;
  hilight=DragType_None;
  btn_type=DragType_None;
  alert_type=AlertType_No;
  alert_blink=false;
  help=false;
  has_good_size=true;

  title=title_;

  time=0;
 }

void FixedFrameShape::layout(Point size_)
 {
  size=size_;

  Coord dxy=+cfg.frame_dxy;
  Coord tdy=+cfg.title_dy;
  Coord bdx=+cfg.btn_dx;
  Coord bdy=+cfg.btn_dy;

  Coord btn_len = is_main? 5*bdx : 3*bdx ;

  if( size>Point( 2*dxy+btn_len+bdx/2+Max(tdy,dxy) , dxy+Max(tdy,dxy) ) )
    {
     Pane pane=Pane(Null,size);

     SplitX(dxy,pane);
     SplitX(pane,dxy);

     Pane top=SplitY(tdy,pane);

     SplitY(pane,dxy);

     client=pane;

     Coord yb=(tdy-bdy)/2;

     Coord tx=top.dx-btn_len;

     if( is_main )
       {
        Coord xb0=top.x+tx;
        Coord xb1=xb0+bdx+bdx/8;
        Coord xb2=xb1+bdx+bdx/8;
        Coord xb3=xb2+bdx+bdx/2;

        btnAlert=Pane(xb0,yb,bdx,bdy);
        btnHelp=Pane(xb1,yb,bdx,bdy);
        btnMin=Pane(xb2,yb,bdx,bdy);
        btnClose=Pane(xb3,yb,bdx,bdy);
       }
     else
       {
        Coord xb0=top.x+tx;
        Coord xb1=xb0+bdx+bdx/2;

        btnAlert=Empty;
        btnMin=Empty;
        btnHelp=Pane(xb0,yb,bdx,bdy);
        btnClose=Pane(xb1,yb,bdx,bdy);
       }

     Coord w=RoundUpLen(+cfg.width);

     titleBar=Pane(top.x+bdx/4,w,tx-bdx/2,tdy-2*w);
    }
  else
    {
     client=Empty;

     btnAlert=Empty;
     btnHelp=Empty;
     btnMin=Empty;
     btnClose=Pane(Null,bdx,bdy);

     titleBar=Empty;
    }
 }

Point FixedFrameShape::getDeltaSize() const
 {
  Coord dxy=+cfg.frame_dxy;
  Coord tdy=+cfg.title_dy;

  return Point(dxy,tdy)+Point(dxy,dxy);
 }

Coord FixedFrameShape::getMinDx(bool is_main,StrLen title) const
 {
  Coord width=RoundUpLen(+cfg.width);
  Coord tdy=+cfg.title_dy;

  MCoord ex=Fraction(tdy-2*width)/4;

  Coord dxy=+cfg.frame_dxy;
  Coord bdx=+cfg.btn_dx;

  TextSize ts=cfg.font->text_guarded(title);

  Coord btn_len = is_main? 5*bdx : 3*bdx ;

  Coordinate dx=ts.full_dx;

  dx+=2*RoundUpLen(ex);

  Replace_max(dx,Max(tdy,dxy));

  dx+=2*dxy+btn_len+bdx/2;

  return +dx;
 }

DragType FixedFrameShape::dragTest(Point point) const
 {
  if( btnAlert.contains(point) ) return DragType_Alert;

  if( btnHelp.contains(point) ) return DragType_Help;

  if( btnMin.contains(point) ) return DragType_Min;

  if( btnClose.contains(point) ) return DragType_Close;

  return client.contains(point)?DragType_None:DragType_Bar;
 }

Pane FixedFrameShape::getPane(DragType drag_type) const
 {
  switch( drag_type )
    {
     case DragType_Bar : return Pane(Null,size);

     case DragType_Alert       : return btnAlert;
     case DragType_Help        : return btnHelp;
     case DragType_Min         : return btnMin;
     case DragType_Close       : return btnClose;

     default: return Empty;
    }
 }

Hint FixedFrameShape::getHint(Point point) const
 {
  switch( dragTest(point) )
    {
     case DragType_Alert       : return {btnAlert,+cfg.hint_Alert};
     case DragType_Help        : return {btnHelp,+cfg.hint_Help};
     case DragType_Min         : return {btnMin,+cfg.hint_Minimize};
     case DragType_Close       : return {btnClose,+cfg.hint_Close};

     default: return Null;
    }
 }

void FixedFrameShape::draw(const DrawBuf &buf) const
 {
  DrawArt art(buf);

  draw_Frame(art);

  draw_Bar(art);

  draw_Alert(art);
  draw_Help(art);
  draw_Min(art);
  draw_Close(art);
 }

void FixedFrameShape::draw(const DrawBuf &buf,DragType drag_type) const
 {
  DrawArt art(buf);

  if( drag_type==DragType_Bar )
    {
     draw_Frame(art);

     draw_Bar(art);

     draw_Alert(art);
     draw_Help(art);
     draw_Min(art);
     draw_Close(art);
    }
  else
    {
     draw_Frame(buf,drag_type);

     switch( drag_type )
       {
        case DragType_Alert       : draw_Alert(art); break;
        case DragType_Help        : draw_Help(art); break;
        case DragType_Min         : draw_Min(art); break;
        case DragType_Close       : draw_Close(art); break;
       }
    }
 }

void FixedFrameShape::drawHint(const DrawBuf &buf,Hint hint) const
 {
  if( +titleBar )
    {
     // 1

     MPane bar(titleBar);

     MCoord width=+cfg.width;

     Coord dxy=RoundUpLen(width);

     MCoord ex=bar.dy/4;

     Pane pane=titleBar.shrink(RoundUpLen(ex)+2*dxy,2*dxy);

     // 2

     SmoothDrawArt art(buf);

     MCoord topY;
     MCoord minX;
     MCoord maxX;

     {
      MPane p(pane);

      topY=p.ey;
      minX=p.x;
      maxX=p.ex;

      FigureBox fig(p);

      VColor snow=+cfg.snow;
      VColor gray=+cfg.gray;
      VColor hintBack=+cfg.hintBack;
      VColor hintText=+cfg.hintText;

      fig.solid(art,hintBack);

      FigureTopBorder fig_top(p,width);
      FigureBottomBorder fig_bottom(p,width);

      fig_top.solid(art,snow);
      fig_bottom.solid(art,gray);

      StrLen text;

      if( +hint )
        {
         text=hint.text.str();
        }
      else
        {
         text=cfg.text_No_hint.get().str();
        }

      cfg.fontHint->text(buf,pane,TextPlace(AlignX_Left,AlignY_Center),text,hintText);
     }

     // 3

     if( +hint )
       {
        MCoord width=+cfg.hintWidth;

        VColor border=+cfg.hintBorder;

        Coord dxy=RoundUpLen(width);

        MPane p(hint.pane.expand(dxy));

        FigureBox fig(p);

        fig.loop(art,HalfNeg,width,border);

        MPoint a=p.getTopMid().subY(width/2);
        MPoint b(a.x,topY);

        if( a.y>b.y )
          {
           art.path(width,border,a,b);

           if( b.x<minX )
             {
              MPoint c(minX,b.y);

              art.path(width,border,b,c);

              art.ball(c,2*width,border);
             }
           else if( b.x>maxX )
             {
              MPoint c(maxX,b.y);

              art.path(width,border,b,c);

              art.ball(c,2*width,border);
             }
           else
             {
              art.ball(b,2*width,border);
             }
          }
        else
          {
           if( p.x-width/2>maxX )
             {
              MPoint a=p.getLeftMid().subX(width/2);
              MPoint b(maxX,a.y);

              art.path(width,border,a,b);

              if( b.y<=topY )
                {
                 art.ball(b,2*width,border);
                }
              else
                {
                 MPoint c(b.x,topY);

                 art.path(width,border,b,c);

                 art.ball(c,2*width,border);
                }
             }
           else if( p.ex+width/2<minX )
             {
              MPoint a=p.getRightMid().addX(width/2);
              MPoint b(minX,a.y);

              art.path(width,border,a,b);

              if( b.y<=topY )
                {
                 art.ball(b,2*width,border);
                }
              else
                {
                 MPoint c(b.x,topY);

                 art.path(width,border,b,c);

                 art.ball(c,2*width,border);
                }
             }
          }
       }
    }
 }

} // namespace Video
} // namespace CCore

