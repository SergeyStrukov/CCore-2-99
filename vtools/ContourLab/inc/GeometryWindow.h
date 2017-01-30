/* GeometryWindow.h */
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

#ifndef GeometryWindow_h
#define GeometryWindow_h

#include <inc/Contour.h>

namespace App {

/* classes */

class GeometryWindow;

/* class GeometryWindow */

class GeometryWindow : public SubWindow
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(6,2) ;

     RefVal<Font> font;

     // app

     RefVal<Coord> shade_dxy = 8 ;

     RefVal<VColor> back    =     White ;
     RefVal<VColor> border  =     Black ;
     RefVal<VColor> face    =     Black ;
     RefVal<VColor> gray    =      Gray ;
     RefVal<VColor> text    =      Blue ;
     RefVal<VColor> hilight =     Green ;
     RefVal<VColor> focus   = OrangeRed ;

     RefVal<Coord> dxy = 300 ;

     Config() noexcept {}

     template <class AppPref>
     Config(const UserPreference &pref,const AppPref &app_pref) noexcept
      {
       bind(pref.get());
       bindApp(app_pref.get());
      }

     template <class Bag>
     void bind(const Bag &bag)
      {
       width.bind(bag.width);
       font.bind(bag.line_edit_font.font);
      }

     template <class Bag>
     void bindApp(const Bag &bag)
      {
       shade_dxy.bind(bag.shade_dxy);
       back.bind(bag.back);
       border.bind(bag.border);
       face.bind(bag.face);
       gray.bind(bag.gray);
       text.bind(bag.text);
       hilight.bind(bag.hilight);
       focus.bind(bag.focus);

       dxy.bind(bag.geometry_dxy);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   bool hilight = false ;
   bool focus = false ;

   Pane pane;
   Pane shade1;
   Pane shade2;

  private:

   void pin(Point point);

   void shift_x(Coord delta);

   void shift_y(Coord delta);

   struct DrawItem;

  public:

   Contour contour;

   GeometryWindow(SubWindowHost &host,const Config &cfg);

   virtual ~GeometryWindow();

   // methods

   Point getMinSize() const;

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
 };

} // namespace App

#endif

