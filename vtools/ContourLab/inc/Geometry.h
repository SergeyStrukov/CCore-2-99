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

  enum RealException
   {
    RealOk = 0,

    RealBlank,

    RealTooBig,
    RealTooSmall,
    RealOutOfDomain
   };

  friend const char * GetTextDesc(RealException rex);

  class Real
   {
     double val;

    private:

     static double Guard(double x);

     static void GuardDiv(double y);

    public:

     Real(double val_=0) : val(val_) {}

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

     static Real Pi() { return 3.1415926535897932384626433832; }

     static Real Abs(Real x) { return (x<0)?-x:x; }

     static Real Norm(Real x,Real y);

     static Real Arg(Real x,Real y);

     static Real Sin(Real x);

     static Real Cos(Real x);

     static Real ArcCos(Real x);

     static Real Sqrt(Real x);

     static Real Mod(Real x,Real y);

     // map

     int map(int prec);

     // print object

     void print(PrinterType &out) const
      {
       Printf(out,"#;",int(val));
      }
   };

  // AssertValid

  template <class S>
  static void AssertValid(S s)
   {
    if( s.rex ) throw s.rex;
   }

  template <class ... SS>
  static void AssertValid(SS ... ss)
   {
    ( ... , AssertValid(ss) );
   }

  // Ratio

  struct Ratio
   {
    static const int TypeId = 1 ;

    static StrLen TypeName() { return "Ratio"_c ; }

    Real val;
    RealException rex;

    Ratio(RealException rex_=RealBlank) : rex(rex_) {}

    Ratio(Real val_) : val(val_),rex(RealOk) {}

    // print object

    void print(PrinterType &out) const
     {
      Printf(out,"#;",val);
     }
   };

  static Ratio Inv(Ratio a) { return 1/a.val; }

  static Ratio Neg(Ratio a) { return -a.val; }

  static Ratio Add(Ratio a,Ratio b) { return a.val+b.val; }

  static Ratio Sub(Ratio a,Ratio b) { return a.val-b.val; }

  static Ratio Mul(Ratio a,Ratio b) { return a.val*b.val; }

  static Ratio Div(Ratio a,Ratio b) { return a.val/b.val; }

  // Length

  struct Length
   {
    static const int TypeId = 2 ;

    static StrLen TypeName() { return "Length"_c ; }

    Real val;
    RealException rex;

    Length(RealException rex_=RealBlank) : rex(rex_) {}

    Length(Real val_) : val(val_),rex(RealOk) {}

    // print object

    void print(PrinterType &out) const
     {
      Printf(out,"#;",val);
     }
   };

  static Length Add(Length a,Length b) { return a.val+b.val; }

  static Length Sub(Length a,Length b) { return a.val-b.val; }

  static Length Mul(Ratio mul,Length a) { return mul.val*a.val; }

  static Length Div(Length a,Ratio div) { return a.val/div.val; }

  static Ratio Div(Length a,Length b) { return a.val/b.val; }

  // Angle

  struct Angle
   {
    static const int TypeId = 3 ;

    static StrLen TypeName() { return "Angle"_c ; }

    Real val;
    RealException rex;

    Angle(RealException rex_=RealBlank) : rex(rex_) {}

    Angle(Real val_,Real Func(Real)=Mod) : val( Func(val_) ),rex(RealOk) {}

    Angle operator - () const { return {-val,Pos}; }

    friend Angle operator + (Angle a,Angle b) { return a.val+b.val; }

    friend Angle operator - (Angle a,Angle b) { return a.val-b.val; }

    friend Angle operator * (Ratio mul,Angle a) { return mul.val*a.val; }

    friend Angle operator / (Angle a,Ratio div) { return a.val/div.val; }

    static Angle Pi() { return {Real::Pi(),Scoped}; }

    static Angle ArcCos(Real t) { return {Real::ArcCos(t),Scoped}; }

    static Angle Arg(Real x,Real y) { return {Real::Arg(x,y),Pos}; }

    // mods

    static Real Scoped(Real a) { return a; }

    static Real Pos(Real a)
     {
      if( a <= -Real::Pi() ) return a+2*Real::Pi();

      return a;
     }

    static Real Mod(Real a) { return Pos( Real::Mod( a , 2*Real::Pi() ) ); }

    // print object

    void print(PrinterType &out) const
     {
      Printf(out,"#;",val*(180/Real::Pi()));
     }
   };

  static Angle Pi() { return Angle::Pi(); }

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
    RealException rex;

    Point(RealException rex_=RealBlank) : rex(rex_) {}

    Point(Real x_,Real y_) : x(x_),y(y_),rex(RealOk) {}

    friend Point operator + (Point a,Point b) { return {a.x+b.x,a.y+b.y}; }

    friend Point operator - (Point a,Point b) { return {a.x-b.x,a.y-b.y}; }

    friend Point operator * (Real mul,Point a) { return {mul*a.x,mul*a.y}; }

    static Real Norm(Point p) { return Real::Norm(p.x,p.y); }

    static Point Ort(Point p) { return (1/Norm(p))*p; }

    static Point Orthogonal(Point p) { return {p.y,-p.x}; }

    static Real Prod(Point a,Point b) { return a.x*b.x+a.y*b.y; }

    static Angle Arg(Point p) { return Angle::Arg(p.x,-p.y); }

    static Angle Arg(Point p,Length sign)
     {
      Angle a=Arg(p);

      if( sign.val<0 ) return -a;

      return a;
     }

    static Point Rotate(Angle a,Point p)
     {
      Real Sin=Real::Sin(a.val);
      Real Cos=Real::Cos(a.val);

      return {Cos*p.x+Sin*p.y,Cos*p.y-Sin*p.x};
     }

    static Point Polar(Length r,Angle a)
     {
      Real Sin=Real::Sin(a.val);
      Real Cos=Real::Cos(a.val);

      return {Cos*r.val,-Sin*r.val};
     }

    // print object

    void print(PrinterType &out) const
     {
      Printf(out,"(#;,#;)",x,y);
     }
   };

  // Line

  struct Line
   {
    static const int TypeId = 5 ;

    static StrLen TypeName() { return "Line"_c ; }

    Point p;
    Point ort;
    RealException rex;

    Line(RealException rex_=RealBlank) : rex(rex_) {}

    Line(Point p_,Point dir) : p(p_),ort(Point::Ort(dir)),rex(RealOk) {}
   };

  // Circle

  struct Circle
   {
    static const int TypeId = 6 ;

    static StrLen TypeName() { return "Circle"_c ; }

    Point center;
    Length radius;
    RealException rex;

    Circle(RealException rex_=RealBlank) : rex(rex_) {}

    Circle(Point center_,Length radius_) : center(center_),radius(radius_),rex(RealOk) {}
   };

  // Couple

  struct Couple
   {
    static const int TypeId = 7 ;

    static StrLen TypeName() { return "Couple"_c ; }

    Point a;
    Point b;
    RealException rex;

    Couple(RealException rex_=RealBlank) : rex(rex_) {}

    Couple(Point a_,Point b_) : a(a_),b(b_),rex(RealOk) {}
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

  static Length LengthOf(Point a,Point b) { return Point::Norm(a-b); }

  static Angle AngleOf(Point a,Point b,Point c) { return Point::Arg(c-b)-Point::Arg(a-b); }

  static Line LineOf(Point a,Point b) { return {a,b-a}; }

  static Circle CircleOf(Point center,Length radius) { return {center,radius}; }

  static Point Proj(Line a,Point p);

  static Angle AngleC(Length a,Length b,Length c);

  static Point Meet(Line a,Line b);

  static Couple Meet(Line a,Circle C); // Couple in a direction

  static Couple Meet(Circle C,Circle D); // Couple in C direction

  static Point Rotate(Point o,Angle a,Point p) { return o+Point::Rotate(a,p-o); }

  static Point Move(Point a,Point b,Point p) { return (b-a)+p; }

  static Point Move(Line a,Length len,Point p) { return len.val*a.ort+p; }

  static Point Mirror(Line a,Point p);

  static Point First(Couple c) { return c.a; }

  static Point Second(Couple c) { return c.b; }

  static Point Up(Length len,Point p) { return {p.x,p.y-len.val}; }

  static Point Down(Length len,Point p) { return {p.x,p.y+len.val}; }

  static Point Left(Length len,Point p) { return {p.x-len.val,p.y}; }

  static Point Right(Length len,Point p) { return {p.x+len.val,p.y}; }
 };

/* Map() */

inline Geometry::Real GradToRadian(double val) { return (val/180)*Geometry::Real::Pi(); }

inline MCoord Map(Geometry::Real x) { return (MCoord)x.map(MPoint::Precision); }

inline MPoint Map(Geometry::Point point) { return MPoint(Map(point.x),Map(point.y)); }

inline Geometry::Point Map(Point point) { return Geometry::Point(point.x,point.y); }

} // namespace App

#endif

