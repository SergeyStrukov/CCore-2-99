/* Geometry.h */
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

#ifndef Geometry_h
#define Geometry_h

#include <inc/Application.h>

namespace App {

/* classes */

struct Geometry;

/* struct Geometry */

struct Geometry
 {
  // Real

  struct Real
   {
    double val;

    Real(double val_) : val(val_) {}

    Real operator - () const { return -val; }

    friend Real operator + (Real a,Real b) { return Guard(a.val+b.val); }

    friend Real operator - (Real a,Real b) { return Guard(a.val-b.val); }

    friend Real operator * (Real a,Real b) { return Guard(a.val*b.val); }

    friend Real operator / (Real a,Real b) { GuardDiv(b.val); return Guard(a.val/b.val); }

    friend bool operator == (Real a,Real b) { return a.val==b.val; }

    friend bool operator != (Real a,Real b) { return a.val!=b.val; }

    friend bool operator < (Real a,Real b) { return a.val<b.val; }

    friend bool operator <= (Real a,Real b) { return a.val<=b.val; }

    friend bool operator > (Real a,Real b) { return a.val>b.val; }

    friend bool operator >= (Real a,Real b) { return a.val>=b.val; }

    static double Guard(double x);

    static void GuardDiv(double y);

    static Real Pi() { return 3.1415926535897932384626433832; }

    static Real Abs(Real x) { return (x<0)?-x:x; }

    static Real Norm(Real x,Real y);

    static Real Arg(Real x,Real y);

    static Real Sin(Real x);

    static Real Cos(Real x);

    static Real ArcCos(Real x);

    static Real Sqrt(Real x);

    static Real Mod(Real x,Real y);
   };

  // Ratio

  struct Ratio
   {
    static const int TypeId = 1 ;

    static StrLen TypeName() { return "Ratio"_c ; }

    Real val;
   };

  static Ratio Inv(Ratio a) { return {1/a.val}; }

  static Ratio Neg(Ratio a) { return {-a.val}; }

  static Ratio Add(Ratio a,Ratio b) { return {a.val+b.val}; }

  static Ratio Sub(Ratio a,Ratio b) { return {a.val-b.val}; }

  static Ratio Mul(Ratio a,Ratio b) { return {a.val*b.val}; }

  static Ratio Div(Ratio a,Ratio b) { return {a.val/b.val}; }

  // Length

  struct Length
   {
    static const int TypeId = 2 ;

    static StrLen TypeName() { return "Length"_c ; }

    Real val;
   };

  static Length Add(Length a,Length b) { return {a.val+b.val}; }

  static Length Sub(Length a,Length b) { return {a.val-b.val}; }

  static Length Mul(Ratio mul,Length a) { return {mul.val*a.val}; }

  static Length Div(Length a,Ratio div) { return {a.val/div.val}; }

  static Ratio Div(Length a,Length b) { return {a.val/b.val}; }

  // Angle

  struct Angle
   {
    static const int TypeId = 3 ;

    static StrLen TypeName() { return "Angle"_c ; }

    Real val;

    Angle operator - () const { return Pos({-val}); }

    friend Angle operator + (Angle a,Angle b) { return Mod( {a.val+b.val} ); }

    friend Angle operator - (Angle a,Angle b) { return Mod( {a.val-b.val} ); }

    friend Angle operator * (Ratio mul,Angle a) { return Mod( {mul.val*a.val} ); }

    friend Angle operator / (Angle a,Ratio div) { return Mod( {a.val/div.val} ); }

    static Angle Mod(Angle a) { return Pos( {Real::Mod(a.val,2*Real::Pi())} ); }

    static Angle Pos(Angle a)
     {
      if( a.val <= -Real::Pi() ) return {a.val+2*Real::Pi()};

      return a;
     }
   };

  static Angle Pi() { return {Real::Pi()}; }

  static Angle Neg(Angle a) { return -a; }

  static Angle Add(Angle a,Angle b) { return a+b; }

  static Angle Sub(Angle a,Angle b) { return a-b; }

  static Angle Mul(Ratio mul,Angle a) { return mul*a; }

  static Angle Div(Angle a,Ratio div) { return a/div; }

  // Point

  struct Point
   {
    static const int TypeId = 4 ;

    static StrLen TypeName() { return "Point"_c ; }

    Real x;
    Real y;

    friend Point operator + (Point A,Point B) { return {A.x+B.x,A.y+B.y}; }

    friend Point operator - (Point A,Point B) { return {A.x-B.x,A.y-B.y}; }

    friend Point operator * (Real mul,Point A) { return {mul*A.x,mul*A.y}; }

    static Angle Arg(Point A) { return {Real::Arg(-A.x,A.y)}; }

    static Angle Arg(Point A,Length sign)
     {
      Angle a=Arg(A);

      if( sign.val<0 ) return -a;

      return a;
     }

    static Real Norm(Point A) { return Real::Norm(A.x,A.y); }

    static Point Ort(Point A) { return (1/Norm(A))*A; }

    static Point Orthogonal(Point A) { return {A.y,-A.x}; }

    static Real Prod(Point A,Point B) { return A.x*B.x+A.y*B.y; }

    static Point Rotate(Angle a,Point A)
     {
      Real Sin=Real::Sin(a.val);
      Real Cos=Real::Cos(a.val);

      return {Cos*A.x+Sin*A.y,Cos*A.y-Sin*A.x};
     }

    static Point Polar(Length R,Angle a)
     {
      Real Sin=Real::Sin(a.val);
      Real Cos=Real::Cos(a.val);

      return {Cos*R.val,-Sin*R.val};
     }
   };

  // Line

  struct Line
   {
    static const int TypeId = 5 ;

    static StrLen TypeName() { return "Line"_c ; }

    Point A;
    Point Ort;
   };

  // Circle

  struct Circle
   {
    static const int TypeId = 6 ;

    static StrLen TypeName() { return "Circle"_c ; }

    Point O;
    Length radius;
   };

  // Couple

  struct Couple
   {
    static const int TypeId = 7 ;

    static StrLen TypeName() { return "Couple"_c ; }

    Point P;
    Point Q;
   };

  // type functions

  template <class Func>
  static auto TypeSwitch(int type_id,Func func)
   {
    switch( type_id )
      {
       case Ratio::TypeId : return func.template doIt<Ratio>();

       case Length::TypeId : return func.template doIt<Length>();

       case Angle::TypeId : return func.template doIt<Angle>();

       case Point::TypeId : return func.template doIt<Point>();

       case Line::TypeId : return func.template doIt<Line>();

       case Circle::TypeId : return func.template doIt<Circle>();

       case Couple::TypeId : return func.template doIt<Couple>();

       default: return func.doOther();
      }
   }

  struct TypeNameFunc
   {
    template <class S>
    static StrLen doIt() { return S::TypeName(); }

    static StrLen doOther() { return Null; }
   };

  static StrLen TypeName(int type_id)
   {
    return TypeSwitch(type_id,TypeNameFunc());
   }

  // functions

  static Length LengthOf(Point A,Point B) { return {Point::Norm(A-B)}; }

  static Angle AngleOf(Point A,Point B,Point C) { return Point::Arg(C-B)-Point::Arg(A-B); }

  static Line LineOf(Point A,Point B) { return {A,Point::Ort(B-A)}; }

  static Circle CircleOf(Point O,Length radius) { return {O,radius}; }

  static Point Proj(Line a,Point A);

  static Angle TAngle(Length a,Length b,Length c); // against c

  static Point MeetOf(Line a,Line b);

  static Couple MeetOf(Line a,Circle C); // Couple in a direction

  static Couple MeetOf(Circle C,Circle D); // Couple in C direction

  static Point Rotate(Point O,Angle a,Point X) { return O+Point::Rotate(a,X-O); }

  static Point Move(Point P,Point Q,Point X) { return (Q-P)+X; }

  static Point Move(Line a,Length len,Point X) { return len.val*a.Ort+X; }

  static Point Mirror(Line a,Point X)
   {
    Point E=Point::Orthogonal(a.Ort);

    return X-2*Point::Prod(X-a.A,E)*E;
   }

  static Point First(Couple c) { return c.P; }

  static Point Second(Couple c) { return c.Q; }

  static Point Up(Length len,Point X) { return {X.x,X.y-len.val}; }

  static Point Down(Length len,Point X) { return {X.x,X.y+len.val}; }

  static Point Left(Length len,Point X) { return {X.x-len.val,X.y}; }

  static Point Right(Length len,Point X) { return {X.x+len.val,X.y}; }
 };

} // namespace App

#endif

