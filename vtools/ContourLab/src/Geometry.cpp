/* Geometry.cpp */
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

#include <inc/Geometry.h>

#include <CCore/inc/Exception.h>

#include <math.h>

namespace App {

/* struct Geometry::Real */

static const double Cap = 1.e+100 ;

static double Abs(double x) { return (x<0)?-x:x; }

double Geometry::Real::Guard(double x)
 {
  if( Abs(x)>Cap )
    {
     Printf(Exception,"App::Geometry::Real::Guard(...) : too big");
    }

  return x;
 }

void Geometry::Real::GuardDiv(double y)
 {
  if( Abs(y)<1/Cap )
    {
     Printf(Exception,"App::Geometry::Real::GuardDiv(...) : too small");
    }
 }

Geometry::Real Geometry::Real::Norm(Real x,Real y)
 {
  return Guard( hypot(x.val,y.val) );
 }

Geometry::Real Geometry::Real::Arg(Real x,Real y)
 {
  GuardDiv( Abs(x.val)+Abs(y.val) );

  return atan2(y.val,x.val);
 }

Geometry::Real Geometry::Real::Sin(Real x)
 {
  return sin(x.val);
 }

Geometry::Real Geometry::Real::Cos(Real x)
 {
  return cos(x.val);
 }

Geometry::Real Geometry::Real::ArcCos(Real x)
 {
  if( x.val<-1 || x.val>1 )
    {
     Printf(Exception,"App::Geometry::Real::ArcCos(...) : out of domain");
    }

  return acos(x.val);
 }

Geometry::Real Geometry::Real::Sqrt(Real x)
 {
  if( x.val<0 )
    {
     Printf(Exception,"App::Geometry::Real::Sqrt(...) : negative");
    }

  return sqrt(x.val);
 }

Geometry::Real Geometry::Real::Mod(Real x,Real y)
 {
  GuardDiv(y.val);

  return remainder(x.val,y.val);
 }

/* struct Geometry */

Geometry::Point Geometry::Proj(Line a,Point A)
 {
  Real t=Point::Prod(A-a.A,a.Ort);

  return a.A+t*a.Ort;
 }

Geometry::Angle Geometry::TAngle(Length a,Length b,Length c)
 {
  Real t=(Sq(a.val)+Sq(b.val)-Sq(c.val))/(2*a.val*b.val);

  return {Real::ArcCos(t)};
 }

Geometry::Point Geometry::MeetOf(Line a,Line b)
 {
  Point E=Point::Orthogonal(b.Ort);

  Real t=Point::Prod(a.A-b.A,E)/Point::Prod(a.Ort,E);

  return a.A-t*a.Ort;
 }

Geometry::Couple Geometry::MeetOf(Line a,Circle C)
 {
  Point P=Proj(a,C.O);

  Real x=Point::Norm(P-C.O);

  Real t=Real::Sqrt( Sq(C.radius.val)-Sq(x) );

  Point delta=t*a.Ort;

  return {P-delta,P+delta};
 }

Geometry::Couple Geometry::MeetOf(Circle C,Circle D)
 {
  Angle a=TAngle(C.radius,LengthOf(C.O,D.O),D.radius);

  Angle d=Point::Arg(D.O-C.O,C.radius);

  return {C.O+Point::Polar(C.radius,d-a),C.O+Point::Polar(C.radius,d+a)};
 }

} // namespace App

