/* Test.h */
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

#ifndef Test_h
#define Test_h

#include <inc/Application.h>

#include <CCore/inc/Array.h>
#include <CCore/inc/PlatformRandom.h>

#include <CCore/inc/video/Picture.h>

namespace App {

/* classes */

struct ShapeParam;

struct Shape1;

struct Shape2;

struct Shape3;

struct Shape4;

class Test;

/* struct ShapeParam */

struct ShapeParam
 {
  MCoord *param;
  StrLen name;

  ShapeParam() : param(0) {}

  ShapeParam(MCoord &param_,StrLen name_) : param(&param_),name(name_) {}
 };

/* struct Shape1 */

struct Shape1
 {
  MCoord y1 =  99328 ;
  MCoord y2 =      0 ;
  MCoord r1 = 129024 ;
  MCoord r2 =  50000 ;

  void draw(DrawBuf buf) const;

  ShapeParam getParam(VKey vkey);
 };

/* struct Shape2 */

struct Shape2
 {
  MCoord dx     = Fraction(600) ;
  MCoord dy     =  Fraction(400) ;
  MCoord radius =  Fraction(150) ;

  void draw(DrawBuf buf) const;

  ShapeParam getParam(VKey vkey);
 };

/* struct Shape3 */

struct Shape3
 {
  MCoord dx     = Fraction(20) ;
  MCoord dy     = Fraction(20) ;

  void draw(DrawBuf buf) const;

  ShapeParam getParam(VKey vkey);
 };

/* struct Shape4 */

struct Shape4
 {
  MCoord dx     = Fraction(32) ;
  MCoord dy     = Fraction(32) ;

  Picture pict;

  Shape4();

  void draw(DrawBuf buf) const;

  ShapeParam getParam(VKey vkey);
 };

/* class Test */

class Test : public SubWindow
 {
  public:

   struct Config
    {
     VColor back = Silver ;
     VColor text =  Black ;

     Font font;

     Config() {}
    };

  public:

   const Config &cfg;

   String info;

   Shape2 shape;

   ShapeParam param;

  private:

   void text(StrLen str)
    {
     info=str;

     redraw();
    }

   template <class ... TT>
   void printf(const char *format,const TT & ... tt)
    {
     char temp[TextBufLen];
     PrintBuf out(Range(temp));

     Printf(out,format,tt...);

     text(out.close());
    }

   void printParam();

  public:

   Test(SubWindowHost &host,const Config &cfg);

   virtual ~Test();

   // drawing

   virtual void layout();

   virtual void draw(DrawBuf buf,bool drag_active) const;

   // user input

   virtual void react(UserAction action);

   void react_Key(VKey vkey,KeyMod kmod);

   void react_Wheel(Point point,MouseKey mkey,Coord delta);
 };

} // namespace App

#endif

