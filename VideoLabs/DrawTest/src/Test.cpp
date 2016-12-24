/* Test.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: DrawTest 1.00
//
//  Tag: VideoLabs
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/Test.h>

#include <CCore/inc/video/FigureLib.h>

#include <CCore/inc/Exception.h>

#include <math.h>

namespace App {

/* struct Shape1 */

void Shape1::draw(DrawBuf buf) const
 {
  Smooth::DrawArt art(buf);

  MCoord radius=Fraction(256);

  MCoord a=Ratio(683,10)*radius;
  MCoord c=radius/5;
  MCoord b=a-2*c;
  MCoord r=Ratio(3,2)*c;

  MPoint center=MPoint::Diag(radius);

  FigureDots<4> fig;

  fig[0]={{-c,c-a}};
  fig[1]={{0,-a}};
  fig[2]={{c,c-a}};
  fig[3]={{0,b},Smooth::DotBreak};

  fig.transform(Smooth::DotShift(center));

  fig.curveSolid(art,Blue);

  art.ball(center.addY(b+r),r,Blue);

  // ---

  art.circle(center,radius,Fraction(1),Red);

  art.circle(center.subY(y1),r1,Fraction(1),Red);

  art.circle(center.subY(y1),r1-r2,Fraction(1),Red);

  // ---

  MCoord x2=(MCoord)sqrt(Sq<double>(r1-r2/2)-Sq<double>(y2));

  art.circle(center-MPoint(x2,y1+y2),r2/2,Fraction(1),Red);
 }

ShapeParam Shape1::getParam(VKey vkey)
 {
  switch( vkey )
    {
     case VKey_F1 : return {y1,"y1"};

     case VKey_F2 : return {r1,"r1"};

     case VKey_F3 : return {r2,"r2"};

     case VKey_F4 : return {y2,"y2"};

     default: return {};
    }
 }

/* struct Shape2 */

void Shape2::draw(DrawBuf buf) const
 {
  Smooth::DrawArt art(buf);

  MCoord x=Fraction(10);
  MCoord y=Fraction(10);

  FigureRoundBox fig(x,x+dx,y,y+dy,radius);

  fig.curveSolid(art,PaleGreen);

  fig.curveLoop(art,HalfPos,Fraction(2),Blue);
  fig.curveLoop(art,HalfNeg,Fraction(2),Red);

  art.path(Fraction(1),Black,fig.getLeftTop(),fig.getRightTop());

  art.path(Fraction(1),Black,fig.getLeftBottom(),fig.getRightBottom());

  art.path(Fraction(1),Black,fig.getTopLeft(),fig.getBottomLeft());

  art.path(Fraction(1),Black,fig.getTopRight(),fig.getBottomRight());
 }

ShapeParam Shape2::getParam(VKey vkey)
 {
  switch( vkey )
    {
     case VKey_F1 : return {dx,"dx"};

     case VKey_F2 : return {dy,"dy"};

     case VKey_F3 : return {radius,"radius"};

     default: return {};
    }
 }

/* struct Shape3 */

void Shape3::draw(DrawBuf buf) const
 {
  Smooth::DrawArt art(buf);

  MCoord x=Fraction(10);
  MCoord y=Fraction(10);

  MCoord len=dx/3;
  MCoord width=Fraction(6,2);

  MCoord radius = Div(40,100)*Min(len,dy) ;

  TwoField field({x,y},Green,{x,y+dy},DarkGreen);

  for(unsigned count=3; count ;count--,x+=len-width)
    {
     FigureRoundBox fig(x,x+len,y,y+dy,radius);

     fig.curveSolid(art,field);
     fig.curveLoop(art,HalfPos,width,Black);
    }
 }

ShapeParam Shape3::getParam(VKey vkey)
 {
  switch( vkey )
    {
     case VKey_F1 : return {dx,"dx"};

     case VKey_F2 : return {dy,"dy"};

     default: return {};
    }
 }

/* struct Shape4 */

Shape4::Shape4()
 {
  pict=DefaultAppIcon();
 }

void Shape4::draw(DrawBuf buf) const
 {
  Pane pane(10,10,RoundUpLen(dx),RoundUpLen(dy));

  buf.erase(pane,pict->defaultGround());

  pict->draw(buf,pane);
 }

ShapeParam Shape4::getParam(VKey vkey)
 {
  switch( vkey )
    {
     case VKey_F1 : return {dx,"dx"};

     case VKey_F2 : return {dy,"dy"};

     default: return {};
    }
 }

/* class Test */

void Test::printParam()
 {
  if( param.param )
    {
     printf("#; = #;",param.name,*param.param);
    }
 }

Test::Test(SubWindowHost &host,const Config &cfg_)
 : SubWindow(host),
   cfg(cfg_)
 {
 }

Test::~Test()
 {
 }

 // drawing

void Test::layout()
 {
 }

void Test::draw(DrawBuf buf,bool) const
 {
  try
    {
     buf.erase(cfg.back);

     shape.draw(buf);

     cfg.font->text(buf,Pane(10,512,1024,30),{AlignX_Left,AlignY_Bottom},Range(info),cfg.text);
    }
  catch(CatchType)
    {
    }
 }

  // user input

void Test::react(UserAction action)
 {
  action.dispatch(*this);
 }

void Test::react_Key(VKey vkey,KeyMod)
 {
  param=shape.getParam(vkey);

  printParam();
 }

void Test::react_Wheel(Point,MouseKey mkey,Coord delta_)
 {
  MCoord delta;

  if( mkey&MouseKey_Shift )
    delta=Fraction(delta_,4);
  else
    delta=Fraction(delta_);

  if( param.param )
    {
     *param.param+=delta;

     printParam();
    }
 }

} // namespace App

