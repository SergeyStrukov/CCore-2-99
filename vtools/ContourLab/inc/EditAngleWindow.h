/* EditAngleWindow.h */
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

#ifndef EditAngleWindow_h
#define EditAngleWindow_h

#include <inc/Geometry.h>

namespace App {

/* classes */

class EditAngleWindow;

/* class EditAngleWindow */

class EditAngleWindow : public SubWindow
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<Font> font;

     // special

     RefVal<VColor> back    =     White ;
     RefVal<VColor> border  =     Black ;
     RefVal<VColor> face    =     Black ;
     RefVal<VColor> gray    =      Gray ;
     RefVal<VColor> text    =      Blue ;
     RefVal<VColor> hilight =     Green ;
     RefVal<VColor> focus   = OrangeRed ;

     RefVal<Coord> dxy = 100 ;

     Config() noexcept {}

     Config(const UserPreference &pref) noexcept
      {
       bind(pref.get(),pref.getSmartConfig());
      }

     template <class Bag,class Proxy>
     void bind(const Bag &bag,Proxy proxy)
      {
       Used(proxy);

       width.bind(bag.width);

       font.bind(bag.line_edit_font.font);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   Geometry::Angle value;

   bool hilight = false ;
   bool focus = false ;

   Coord len = 0 ;
   Pane pane;
   Point base;
   Pane text_plus;
   Pane text_minus;

  private:

   void newValue(Geometry::Angle value);

   void pin(Point point);

   void rotate(Coord delta);

  public:

   EditAngleWindow(SubWindowHost &host,const Config &cfg);

   virtual ~EditAngleWindow();

   // methods

   SizeBox getMinSize() const;

   Geometry::Angle getValue() const { return value; }

   void setValue(Geometry::Angle value_)
    {
     value=value_;

     redraw();
    }

   // drawing

   virtual bool isGoodSize(Point size) const;

   virtual void layout();

   virtual void draw(DrawBuf buf,bool drag_active) const;

   // base

   virtual void open();

   // keyboard

   virtual FocusType askFocus() const;

   virtual void gainFocus();

   virtual void looseFocus();

   // mouse

   virtual MouseShape getMouseShape(Point point,KeyMod kmod) const;

   // user input

   virtual void react(UserAction action);

   void react_Key(VKey vkey,KeyMod kmod,unsigned repeat);

   void react_LeftClick(Point point,MouseKey mkey);

   void react_Move(Point point,MouseKey mkey);

   void react_Leave();

   void react_Wheel(Point,MouseKey mkey,Coord delta);

   // signals

   Signal<Geometry::Angle> changed;
 };

} // namespace App

#endif


