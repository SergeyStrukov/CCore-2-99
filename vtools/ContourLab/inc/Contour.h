/* Contour.h */
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

#ifndef Contour_h
#define Contour_h

#include <inc/Application.h>

#include <CCore/inc/MemBase.h>
#include <CCore/inc/RefPtr.h>
#include <CCore/inc/String.h>

namespace App {

/* functions */

void GuardTypeMismatch();

inline void GuardType(int t1,int t2) { if( t1!=t2 ) GuardTypeMismatch(); }

/* classes */

struct Geometry;

class Contour;

/* struct Geometry */

struct Geometry
 {
  enum RealConst
   {
    Nul = 0,
    One = 1
   };

  struct Real
   {
    double x;

    Real(RealConst c=Nul) : x(c) {}

    Real operator - () const;

    Real inv() const;

    friend Real operator + (Real a,Real b);

    friend Real operator - (Real a,Real b);

    friend Real operator * (Real a,Real b);

    friend Real operator / (Real a,Real b);
   };

  struct Ratio
   {
    Real tau;
   };

  static Ratio Inv(Ratio a);

  static Ratio Neg(Ratio a);

  static Ratio Add(Ratio a,Ratio b);

  static Ratio Sub(Ratio a,Ratio b);

  static Ratio Mul(Ratio a,Ratio b);

  static Ratio Div(Ratio a,Ratio b);

  struct Length
   {
    Real len;
   };

  static Length Add(Length a,Length b);

  static Length Sub(Length a,Length b);

  static Length Mul(Ratio mul,Length a);

  static Length Div(Length a,Ratio div);

  static Ratio Div(Length a,Length b);

  struct Angle
   {
    Real alpha;
   };

  static Angle Pi();

  static Angle Add(Angle a,Angle b);

  static Angle Sub(Angle a,Angle b);

  static Angle Mul(Ratio mul,Angle a);

  static Angle Div(Angle a,Ratio div);

  struct Point
   {
    Real x;
    Real y;
   };

  struct Line
   {
    Point A;
    Point B;
   };

  struct Circle
   {
    Point O;
    Length r;
   };

  struct Couple
   {
    Point P;
    Point Q;
   };

  static Length LengthOf(Point A,Point B);

  static Angle AngleOf(Point A,Point B,Point C);

  static Point MeetOf(Line a,Line b);

  static Couple MeetOf(Line a,Circle C);

  static Couple MeetOf(Circle C,Circle D);

  static Point Rotate(Point O,Angle a,Point X);

  static Point Move(Point P,Point Q,Point X);

  static Point Move(Point P,Point Q,Length l,Point X);

  static Point Mirror(Line a,Point X);

  static Point Up(Length l,Point X);

  static Point Down(Length l,Point X);

  static Point Left(Length l,Point X);

  static Point Right(Length l,Point X);

  // Object

  class Object
   {
     class Base : MemBase_nocopy
      {
        ulen ref_count = 1 ;

       public:

        Base() {}

        virtual ~Base() {}

        void incRef() { ref_count++; }

        bool decRef() { return !--ref_count; }

        void destroy() noexcept { delete this; }
      };

     template <class T>
     struct Node : Base
      {
       T obj;

       template <class ... SS>
       explicit Node(SS && ... ss) : obj( std::forward<SS>(ss)... ) {}
      };

     RefPtr<Base> ptr;
     int type_id;

    public:

     template <class T> struct Tag {};

     template <class T,class ... SS>
     Object(Tag<T>,SS && ... ss) : ptr(new Node<T>( std::forward<SS>(ss)... )),type_id(T::TypeId) {}

     ~Object() {}

     int getTypeId() const { return type_id; }

     template <class T>
     T & cast() const
      {
       GuardType(type_id,T::TypeId);

       return static_cast<Node<T> *>(ptr.getPtr())->obj;
      }
   };

  template <class T,class ... SS>
  static Object Create(SS && ... ss) { return Object(Object::Tag<T>(), std::forward<SS>(ss)... ); }

  // Common

  template <int Id>
  struct Common
   {
    static const int TypeId = Id ;

    String name;
   };
 };

/* class Contour */

class Contour : public Geometry
 {
 };

} // namespace App

#endif
