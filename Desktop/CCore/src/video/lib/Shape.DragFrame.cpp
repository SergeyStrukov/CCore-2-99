/* Shape.DragFrame.cpp */
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

#include <CCore/inc/video/lib/Shape.DragFrame.h>

#include <CCore/inc/video/FigureLib.h>

namespace CCore {
namespace Video {

/* class DragShape */

class DragFrameShape::DrawArt : public SmoothDrawArt
 {
  public:

   DrawArt(const DrawBuf &buf) : SmoothDrawArt(buf) {}
 };

VColor DragFrameShape::dragColor(DragType zone) const
 {
  if( drag_type==zone ) return +cfg.dragActive;

  if( hilight==zone ) return +cfg.dragHilight;

  return +cfg.drag;
 }

void DragFrameShape::draw_Frame(DrawArt &art,Pane part) const
 {
  VColor frame=+cfg.frame;
  VColor gray=+cfg.gray;
  VColor small=+cfg.dragSmall;
  VColor frameSmall = has_good_size? frame : small ;

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

void DragFrameShape::draw_Frame(const DrawBuf &buf,DragType drag_type) const
 {
  Pane part=getPane(drag_type);

  DrawArt art(buf.cut(part));

  draw_Frame(art,part);
 }

void DragFrameShape::draw_TopLeft(DrawArt &art) const
 {
  if( +dragTopLeft )
    {
     MPane p(dragTopLeft);

     MCoord width=+cfg.width;

     FigureTopLeftArrow fig(p.shrink(width));

     fig.solid(art,dragColor(DragType_TopLeft));
    }
 }

void DragFrameShape::draw_Left(DrawArt &art) const
 {
  if( +dragLeft )
    {
     MPane p(dragLeft);

     MCoord d=p.dx/2;
     MCoord x0=p.x+d;

     MCoord y0=p.y+d;
     MCoord y1=p.ey-d;

     MCoord w=p.dx/4;

     art.path(w,dragColor(DragType_Left),MPoint(x0,y0),MPoint(x0,y1));
    }
 }

void DragFrameShape::draw_BottomLeft(DrawArt &art) const
 {
  if( +dragBottomLeft )
    {
     MPane p(dragBottomLeft);

     MCoord width=+cfg.width;

     FigureBottomLeftArrow fig(p.shrink(width));

     fig.solid(art,dragColor(DragType_BottomLeft));
    }
 }

void DragFrameShape::draw_Bottom(DrawArt &art) const
 {
  if( +dragBottom )
    {
     MPane p(dragBottom);

     MCoord d=p.dy/2;
     MCoord y0=p.y+d;

     MCoord x0=p.x+d;
     MCoord x1=p.ex-d;

     MCoord w=p.dy/4;

     art.path(w,dragColor(DragType_Bottom),MPoint(x0,y0),MPoint(x1,y0));
    }
 }

void DragFrameShape::draw_BottomRight(DrawArt &art) const
 {
  if( +dragBottomRight )
    {
     MPane p(dragBottomRight);

     MCoord width=+cfg.width;

     FigureBottomRightArrow fig(p.shrink(width));

     fig.solid(art,dragColor(DragType_BottomRight));
    }
 }

void DragFrameShape::draw_Right(DrawArt &art) const
 {
  if( +dragRight )
    {
     MPane p(dragRight);

     MCoord d=p.dx/2;
     MCoord x0=p.x+d;

     MCoord y0=p.y+d;
     MCoord y1=p.ey-d;

     MCoord w=p.dx/4;

     art.path(w,dragColor(DragType_Right),MPoint(x0,y0),MPoint(x0,y1));
    }
 }

void DragFrameShape::draw_TopRight(DrawArt &art) const
 {
  if( +dragTopRight )
    {
     MPane p(dragTopRight);

     MCoord width=+cfg.width;

     FigureTopRightArrow fig(p.shrink(width));

     fig.solid(art,dragColor(DragType_TopRight));
    }
 }

void DragFrameShape::draw_Bar(DrawArt &art) const
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

auto DragFrameShape::draw_Btn(DrawArt &art,Pane btn,DragType zone) const
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

void DragFrameShape::draw_Alert(DrawArt &art) const
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

void DragFrameShape::draw_Help(DrawArt &art) const
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

void DragFrameShape::draw_Min(DrawArt &art) const
 {
  if( +btnMin )
    {
     MPane p=draw_Btn(art,btnMin,DragType_Min);

     FigureBox fig_pict(p.x,p.ex,p.ey-p.dy/4,p.ey);

     fig_pict.solid(art,+cfg.btnPict);
    }
 }

void DragFrameShape::draw_Max(DrawArt &art) const
 {
  if( +btnMax )
    {
     MPane p=draw_Btn(art,btnMax,DragType_Max);

     VColor pict=+cfg.btnPict;

     if( max_button )
       {
        FigureBox fig_pict(p);

        fig_pict.solid(art,pict);
       }
     else
       {
        MCoord e=p.dy/5;

        FigureBox fig_pict(p.x,p.ex-p.dx/3,p.y+e,p.ey-e);

        fig_pict.solid(art,pict);
       }
    }
 }

void DragFrameShape::draw_Close(DrawArt &art) const
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

void DragFrameShape::reset(const DefString &title_,bool is_main_,bool max_button_)
 {
  has_focus=false;
  max_button=max_button_;
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

void DragFrameShape::layout(Point size_)
 {
  size=size_;

  Coord dxy=+cfg.frame_dxy;
  Coord tdy=+cfg.title_dy;
  Coord bdx=+cfg.btn_dx;
  Coord bdy=+cfg.btn_dy;

  Coord btn_len = is_main? 6*bdx+bdx/8 : 4*bdx ;

  if( size>Point( 2*dxy+btn_len+bdx/2+Max(tdy,dxy) , dxy+Max(tdy,dxy) ) )
    {
     Pane pane=Pane(Null,size);

     Pane left=SplitX(dxy,pane);
     Pane right=SplitX(pane,dxy);

     dragTopLeft=SplitY(dxy,left);
     dragBottomLeft=SplitY(left,dxy);
     dragLeft=left;

     dragTopRight=SplitY(dxy,right);
     dragBottomRight=SplitY(right,dxy);
     dragRight=right;

     dragBar=SplitY(tdy,pane);
     dragBottom=SplitY(pane,dxy);
     client=pane;

     Coord yb=(tdy-bdy)/2;

     Coord tx=dragBar.dx-btn_len;

     if( is_main )
       {
        Coord xb0=dragBar.x+tx;
        Coord xb1=xb0+bdx+bdx/8;
        Coord xb2=xb1+bdx+bdx/8;
        Coord xb3=xb2+bdx+bdx/8;
        Coord xb4=xb3+bdx+bdx/2;

        btnAlert=Pane(xb0,yb,bdx,bdy);
        btnHelp=Pane(xb1,yb,bdx,bdy);
        btnMin=Pane(xb2,yb,bdx,bdy);
        btnMax=Pane(xb3,yb,bdx,bdy);
        btnClose=Pane(xb4,yb,bdx,bdy);
       }
     else
       {
        Coord xb0=dragBar.x+tx;
        Coord xb1=xb0+bdx+bdx/8;
        Coord xb2=xb1+bdx+bdx/2;

        btnAlert=Empty;
        btnMin=Empty;
        btnHelp=Pane(xb0,yb,bdx,bdy);
        btnMax=Pane(xb1,yb,bdx,bdy);
        btnClose=Pane(xb2,yb,bdx,bdy);
       }

     Coord w=RoundUpLen(+cfg.width);

     titleBar=Pane(dragBar.x+bdx/4,w,tx-bdx/2,tdy-2*w);
    }
  else
    {
     dragTopLeft=Empty;
     dragBottomLeft=Empty;
     dragLeft=Empty;

     dragTopRight=Empty;
     dragBottomRight=Pane(Null,size);
     dragRight=Empty;

     dragBar=Empty;
     dragBottom=Empty;
     client=Empty;

     btnAlert=Empty;
     btnMin=Empty;
     btnMax=Empty;
     btnClose=Empty;

     titleBar=Empty;
    }
 }

Point DragFrameShape::getDeltaSize() const
 {
  Coord dxy=+cfg.frame_dxy;
  Coord tdy=+cfg.title_dy;

  return Point(dxy,tdy)+Point(dxy,dxy);
 }

Coord DragFrameShape::getMinDx(bool is_main,StrLen title) const
 {
  Coord width=RoundUpLen(+cfg.width);
  Coord tdy=+cfg.title_dy;

  MCoord ex=Fraction(tdy-2*width)/4;

  Coord dxy=+cfg.frame_dxy;
  Coord bdx=+cfg.btn_dx;

  TextSize ts=cfg.font->text_guarded(title);

  Coord btn_len = is_main? 6*bdx+bdx/8 : 4*bdx ;

  Coordinate dx=ts.full_dx;

  dx+=2*dxy+btn_len+bdx/2+2*RoundUpLen(ex);

  return +dx;
 }

DragType DragFrameShape::dragTest(Point point) const
 {
  if( dragTopLeft.contains(point) ) return DragType_TopLeft;

  if( dragLeft.contains(point) ) return DragType_Left;

  if( dragBottomLeft.contains(point) ) return DragType_BottomLeft;

  if( dragBottom.contains(point) ) return DragType_Bottom;

  if( dragBottomRight.contains(point) ) return DragType_BottomRight;

  if( dragRight.contains(point) ) return DragType_Right;

  if( dragTopRight.contains(point) ) return DragType_TopRight;

  if( dragBar.contains(point) )
    {
     if( btnAlert.contains(point) ) return DragType_Alert;

     if( btnHelp.contains(point) ) return DragType_Help;

     if( btnMin.contains(point) ) return DragType_Min;

     if( btnMax.contains(point) ) return DragType_Max;

     if( btnClose.contains(point) ) return DragType_Close;

     return DragType_Bar;
    }

  return DragType_None;
 }

Pane DragFrameShape::getPane(DragType drag_type) const
 {
  switch( drag_type )
    {
     case DragType_TopLeft     : return dragTopLeft;
     case DragType_Left        : return dragLeft;
     case DragType_BottomLeft  : return dragBottomLeft;
     case DragType_Bottom      : return dragBottom;
     case DragType_BottomRight : return dragBottomRight;
     case DragType_Right       : return dragRight;
     case DragType_TopRight    : return dragTopRight;

     case DragType_Alert       : return btnAlert;
     case DragType_Help        : return btnHelp;
     case DragType_Min         : return btnMin;
     case DragType_Max         : return btnMax;
     case DragType_Close       : return btnClose;

     default: return Empty;
    }
 }

Hint DragFrameShape::getHint(Point point) const
 {
  switch( dragTest(point) )
    {
     case DragType_TopLeft     : return {dragTopLeft,"Resize top-left"_def};
     case DragType_Left        : return {dragLeft,"Resize left"_def};
     case DragType_BottomLeft  : return {dragBottomLeft,"Resize bottom-left"_def};
     case DragType_Bottom      : return {dragBottom,"Resize bottom"_def};
     case DragType_BottomRight : return {dragBottomRight,"Resize bottom-right"_def};
     case DragType_Right       : return {dragRight,"Resize right"_def};
     case DragType_TopRight    : return {dragTopRight,"Resize top-right"_def};

     case DragType_Alert       : return {btnAlert,"Open/close alert view"_def};
     case DragType_Help        : return {btnHelp,"Help on/off"_def};
     case DragType_Min         : return {btnMin,"Minimize"_def};
     case DragType_Max         : return {btnMax,max_button?"Maximize"_def:"Restore"_def};
     case DragType_Close       : return {btnClose,"Close"_def};

     default: return Null;
    }
 }

void DragFrameShape::draw(const DrawBuf &buf) const
 {
  DrawArt art(buf);

  draw_Frame(art);

  draw_TopLeft(art);
  draw_Left(art);
  draw_BottomLeft(art);
  draw_Bottom(art);
  draw_BottomRight(art);
  draw_Right(art);
  draw_TopRight(art);

  draw_Bar(art);

  draw_Alert(art);
  draw_Help(art);
  draw_Min(art);
  draw_Max(art);
  draw_Close(art);
 }

void DragFrameShape::draw(const DrawBuf &buf,DragType drag_type) const
 {
  DrawArt art(buf);

  draw_Frame(buf,drag_type);

  switch( drag_type )
    {
     case DragType_TopLeft     : draw_TopLeft(art); break;
     case DragType_Left        : draw_Left(art); break;
     case DragType_BottomLeft  : draw_BottomLeft(art); break;
     case DragType_Bottom      : draw_Bottom(art); break;
     case DragType_BottomRight : draw_BottomRight(art); break;
     case DragType_Right       : draw_Right(art); break;
     case DragType_TopRight    : draw_TopRight(art); break;

     case DragType_Alert       : draw_Alert(art); break;
     case DragType_Help        : draw_Help(art); break;
     case DragType_Min         : draw_Min(art); break;
     case DragType_Max         : draw_Max(art); break;
     case DragType_Close       : draw_Close(art); break;
    }
 }

void DragFrameShape::drawHint(const DrawBuf &buf,Hint hint) const
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
       }
    }
 }

} // namespace Video
} // namespace CCore

