/* Test.h */
//----------------------------------------------------------------------------------------
//
//  Project: Test 1.00
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

namespace App {

/* classes */

class Test;

/* class Test */

class Test : public SubWindow
 {
  public:

   struct Config
    {
     VColor text        = DarkGreen ;
     VColor back        =    Silver ;

     Font font;

     Config() {}
    };

  public:

   const Config &cfg;

   String text;
   MouseShape mshape = Mouse_Null ;

  private:

   void setShape(MouseShape mshape);

  public:

   Test(SubWindowHost &host,const Config &cfg);

   virtual ~Test();

   // drawing

   virtual void layout();

   virtual void draw(DrawBuf buf,bool drag_active) const;

   // mouse

   virtual MouseShape getMouseShape(Point point,KeyMod kmod) const;

   // user input

   virtual void react(UserAction action);

   void react_Key(VKey vkey,KeyMod kmod);

   void react_Char(char ch);

   void react_AltChar(char ch);
 };

} // namespace App

#endif

