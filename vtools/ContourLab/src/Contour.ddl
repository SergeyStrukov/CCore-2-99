/* Contour.ddl */
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

type Bool = uint8 ;

Bool True = 1 ;

Bool False = 0 ;

struct Label
 {
  text name;

  Bool show;
  Bool gray;
  Bool show_name;
 };

type Exception = uint8 ;

struct Real
 {
 };
 
struct Ratio
 {
  Real val;
  Exception rex;
 };
  
struct Length
 {
  Real val;
  Exception rex;
 }; 
 
struct Angle
 {
  Real val;
  Exception rex;
 };
  
struct Point
 {
  Real x;
  Real y;
  Exception rex;
 }; 
 
struct Line
 {
  Point a;
  Point ort;
  Exception rex;
 };
 
struct Circle 
 {
  Point center;
  Length radius;
  Exception rex;
 };
 
struct Couple
 {
  Point a;
  Point b;
  Exception rex;
 };
 
struct Dot 
 {
  Point point;
  Bool break_flag;
 };
 
struct Step 
 {
  Point[] points;
  Exception rex;
 };
 
struct Path
 {
  Dot[] dots;
  Exception rex;
 };
 
struct Loop
 {
  Dot[] dots;
  Exception rex;
 };
  
struct Solid
 {
  Dot[] dots;
  Exception rex;
 };

struct Pad
 {
  Label label;
  {Ratio,Length,Angle,Point} *object;
 };
 
struct Formula
 {
  Label label;
 };
 
struct Contour
 {
  Pad[] pads;
  Formula[] formulas;
 };
   