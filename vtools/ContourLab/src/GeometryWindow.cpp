/* GeometryWindow.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: ContourLab 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2017 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/GeometryWindow.h>

#include <CCore/inc/video/FigureLib.h>

namespace App {

/* class GeometryWindow */

void GeometryWindow::pin(Point point) // TODO
 {
  Used(point);
 }

void GeometryWindow::shift_x(Coord delta) // TODO
 {
  Used(delta);
 }

void GeometryWindow::shift_y(Coord delta) // TODO
 {
  Used(delta);
 }

struct GeometryWindow::DrawItem
 {
  SmoothDrawArt art;

  Coord dx;
  Coord dy;

  MCoord width;

  VColor face;
  VColor text;

  Font font;

  Coord shift;

  DrawItem(const SmoothDrawArt &art_,Coord dx_,Coord dy_,const Config &cfg)
   : art(art_),
     dx(dx_),
     dy(dy_),
     width(+cfg.width),
     face(+cfg.face),
     text(+cfg.text),
     font(+cfg.font)
   {
    shift=RoundUpLen(6*width);
   }

  static Geometry::Point SafeMeet(Geometry::Line a,Geometry::Point b,Geometry::Point c) // TODO
   {
    try
      {
       return Geometry::MeetIn(a,b,c);
      }
    catch(Geometry::RealException rex)
      {
       return rex;
      }
   }

  void drawName(MPoint p,const String &name)
   {
    Point base=p.toPoint();

    if( base.x<dx/2 )
      {
       if( base.y<dy/2 )
         {
          base.addXY(shift);

          Pane pane(base.x,base.y,dx-base.x,dy-base.y);

          font->text(art.getBuf(),pane,TextPlace(AlignX_Left,AlignY_Top),Range(name),text);
         }
       else
         {
          base.addXsubY(shift);

          Pane pane(base.x,0,dx-base.x,base.y);

          font->text(art.getBuf(),pane,TextPlace(AlignX_Left,AlignY_Bottom),Range(name),text);
         }
      }
    else
      {
       if( base.y<dy/2 )
         {
          base.subXaddY(shift);

          Pane pane(0,base.y,base.x,dy-base.y);

          font->text(art.getBuf(),pane,TextPlace(AlignX_Right,AlignY_Top),Range(name),text);
         }
       else
         {
          base.subXY(shift);

          Pane pane(0,0,base.x,base.y);

          font->text(art.getBuf(),pane,TextPlace(AlignX_Right,AlignY_Bottom),Range(name),text);
         }
      }
   }

  void drawLine(Geometry::Point a,Geometry::Point b,const String &name)
   {
    Used(name);

    MPoint beg=Map(a);
    MPoint end=Map(b);

    art.path(width,face,beg,end);

    drawName(end,name);
   }

  void operator () (const String &name,Geometry::Ratio s)
   {
    Used(name);
    Used(s);

    // do nothing
   }

  void operator () (const String &name,Geometry::Length s)
   {
    Used(name);
    Used(s);

    // do nothing
   }

  void operator () (const String &name,Geometry::Angle s)
   {
    Used(name);
    Used(s);

    // do nothing
   }

  void operator () (const String &name,Geometry::Point s)
   {
    if( s.x>=0 && s.y>=0 && s.x<=dx && s.y<=dy )
      {
       MPoint p=Map(s);

       art.ball(p,2*width,face);

       drawName(p,name);
      }
   }

  void operator () (const String &name,Geometry::Line s)
   {
    Geometry::Point a(0,0),
                    b(0,dy),
                    c(dx,dy),
                    d(dx,0);

    Geometry::Point p1=SafeMeet(s,a,b);
    Geometry::Point q1=SafeMeet(s,b,c);
    Geometry::Point p2=SafeMeet(s,c,d);
    Geometry::Point q2=SafeMeet(s,d,a);

    if( p1.rex )
      {
       if( q1.rex )
         {
          if( !p2.rex && !q2.rex ) // p2 , q2
            {
             drawLine(p2,q2,name);
            }
         }
       else // q1
         {
          if( q2.rex )
            {
             if( !p2.rex ) // p2 , q1
               {
                drawLine(p2,q1,name);
               }
            }
          else // q1 , q2
            {
             drawLine(q1,q2,name);
            }
         }
      }
    else // p1
      {
       if( p2.rex )
         {
          if( q1.rex )
            {
             if( !q2.rex ) // p1 , q2
               {
                drawLine(p1,q2,name);
               }
            }
          else // p1 , q1
            {
             if( q2.rex )
               {
                drawLine(p1,q1,name);
               }
             else // p1 , q1 , q2
               {
                drawLine(q1,q2,name);
               }
            }
         }
       else // p1 , p2
         {
          drawLine(p1,p2,name);
         }
      }
   }

  void operator () (const String &name,Geometry::Circle s) // TODO
   {
    Used(name);

    MPoint p=Map(s.center);
    MCoord r=Map(s.radius.val);

    art.circle(p,r,width,face);
   }

  void operator () (const String &name,Geometry::Couple s)
   {
    Used(name);
    Used(s);

    // do nothing
   }
 };

GeometryWindow::GeometryWindow(SubWindowHost &host,const Config &cfg_)
 : SubWindow(host),
   cfg(cfg_)
 {
 }

GeometryWindow::~GeometryWindow()
 {
 }

 // methods

Point GeometryWindow::getMinSize() const
 {
  return Point::Diag(+cfg.dxy);
 }

 // drawing

bool GeometryWindow::isGoodSize(Point size) const
 {
  return size>=getMinSize();
 }

void GeometryWindow::layout()
 {
  Coord s=+cfg.shade_dxy;

  Point size=getSize().subXY(s);

  pane=Pane(Null,size.x,size.y);
  shade1=Pane(s,size.y,size.x,s);
  shade2=Pane(size.x,s,s,size.y-s);
 }

void GeometryWindow::draw(DrawBuf buf,bool) const
 {
  if( pane.dx<10 || pane.dy<10 ) return;

  MCoord w=+cfg.width;

  VColor gray=+cfg.gray;

  SmoothDrawArt art(buf.cut(pane));

  MPane p(pane);
  FigureBox fig(p);

  // body

  {
   fig.solid(art,+cfg.back);

   buf.erase(shade1,gray);
   buf.erase(shade2,gray);
  }

  contour.apply(DrawItem(art,pane.dx,pane.dy,cfg));

  // border

  {
   VColor vc = focus? +cfg.focus : ( hilight? +cfg.hilight : +cfg.border ) ;

   fig.loop(art,HalfPos,w,vc);
  }
 }

 // base

void GeometryWindow::open()
 {
  focus=false;
  hilight=false;
 }

 // keyboard

FocusType GeometryWindow::askFocus() const
 {
  return FocusOk;
 }

void GeometryWindow::gainFocus()
 {
  if( Change(focus,true) ) redraw();
 }

void GeometryWindow::looseFocus()
 {
  if( Change(focus,false) ) redraw();
 }

 // mouse

MouseShape GeometryWindow::getMouseShape(Point,KeyMod) const
 {
  return Mouse_Arrow;
 }

 // user input

void GeometryWindow::react(UserAction action)
 {
  action.dispatch(*this);
 }

void GeometryWindow::react_Key(VKey vkey,KeyMod kmod,unsigned repeat)
 {
  switch( vkey )
    {
     case VKey_Left :
      {
       if( kmod&KeyMod_Shift )
         shift_x(-10*Coord(repeat));
       else
         shift_x(-Coord(repeat));
      }
     break;

     case VKey_Right :
      {
       if( kmod&KeyMod_Shift )
         shift_x(10*Coord(repeat));
       else
         shift_x(Coord(repeat));
      }
     break;

     case VKey_Up :
      {
       if( kmod&KeyMod_Shift )
         shift_y(-10*Coord(repeat));
       else
         shift_y(-Coord(repeat));
      }
     break;

     case VKey_Down :
      {
       if( kmod&KeyMod_Shift )
         shift_y(10*Coord(repeat));
       else
         shift_y(Coord(repeat));
      }
     break;
    }
 }

void GeometryWindow::react_LeftClick(Point point,MouseKey)
 {
  if( pane.contains(point) ) pin(point);
 }

void GeometryWindow::react_Move(Point point,MouseKey mkey)
 {
  if( pane.contains(point) )
    {
     if( Change(hilight,true) ) redraw();

     if( mkey&MouseKey_Left ) pin(point);
    }
  else
    {
     if( Change(hilight,false) ) redraw();
    }
 }

void GeometryWindow::react_Leave()
 {
  if( Change(hilight,false) ) redraw();
 }

void GeometryWindow::react_Wheel(Point,MouseKey mkey,Coord delta)
 {
  if( mkey&MouseKey_Shift )
    {
     shift_y(delta);
    }
  else
    {
     shift_x(delta);
    }
 }

} // namespace App
