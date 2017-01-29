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

#include <math.h>

namespace App {

/* struct Geometry::Real */

const char * GetTextDesc(Geometry::RealException rex)
 {
  switch( rex )
    {
     case Geometry::RealOk : return "ok";

     case Geometry::RealBlank : return "uninitialized";

     case Geometry::RealTooBig : return "too big";

     case Geometry::RealTooSmall : return "too small";

     case Geometry::RealOutOfDomain : return "out of domain";

     default: return "???";
    }
 }

static const double Cap = 1.e+100 ;

double Geometry::Real::Guard(double x)
 {
  if( fabs(x)>Cap ) throw RealTooBig;

  return x;
 }

void Geometry::Real::GuardDiv(double y)
 {
  if( fabs(y)<1/Cap ) throw RealTooSmall;
 }

Geometry::Real Geometry::Real::Norm(Real x,Real y)
 {
  return Guard( hypot(x.val,y.val) );
 }

Geometry::Real Geometry::Real::Arg(Real x,Real y)
 {
  GuardDiv( fabs(x.val)+fabs(y.val) );

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
  if( x<-1 || x>1 ) throw RealOutOfDomain;

  return acos(x.val);
 }

Geometry::Real Geometry::Real::Sqrt(Real x)
 {
  if( x<0 ) throw RealOutOfDomain;

  return sqrt(x.val);
 }

Geometry::Real Geometry::Real::Mod(Real x,Real y)
 {
  GuardDiv(y.val);

  return remainder(x.val,y.val);
 }

int Geometry::Real::map(int prec)
 {
  return (int)ldexp(val,prec);
 }

/* struct Geometry */

Geometry::Point Geometry::Proj(Line a,Point p)
 {
  Real t=Point::Prod(p-a.p,a.ort);

  return a.p+t*a.ort;
 }

Geometry::Angle Geometry::AngleC(Length a,Length b,Length c)
 {
  Real t=(Sq(a.val)+Sq(b.val)-Sq(c.val))/Real::Abs(2*a.val*b.val);

  return Angle::ArcCos(t);
 }

Geometry::Point Geometry::Meet(Line a,Line b)
 {
  Point e=Point::Orthogonal(b.ort);

  Real t=Point::Prod(a.p-b.p,e)/Point::Prod(a.ort,e);

  return a.p-t*a.ort;
 }

Geometry::Couple Geometry::Meet(Line a,Circle C)
 {
  Point p=Proj(a,C.center);

  Real x=Point::Norm(p-C.center);

  Real t=Real::Sqrt( Sq(C.radius.val)-Sq(x) );

  Point delta=t*a.ort;

  return {p-delta,p+delta};
 }

Geometry::Couple Geometry::Meet(Circle C,Circle D)
 {
  Angle a=AngleC(C.radius,LengthOf(C.center,D.center),D.radius);

  Angle d=Point::Arg(D.center-C.center,C.radius);

  return {C.center+Point::Polar(C.radius,d-a),C.center+Point::Polar(C.radius,d+a)};
 }

Geometry::Point Geometry::Mirror(Line a,Point p)
 {
  Point e=Point::Orthogonal(a.ort);

  return p-2*Point::Prod(p-a.p,e)*e;
 }

} // namespace App

