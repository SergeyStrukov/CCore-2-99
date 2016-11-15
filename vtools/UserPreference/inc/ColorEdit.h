/* ColorEdit.h */
//----------------------------------------------------------------------------------------
//
//  Project: UserPreference 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef ColorEdit_h
#define ColorEdit_h

#include <inc/Application.h>

namespace App {

/* classes */

class ColorEditWindow;

/* class ColorEditWindow */

class ColorEditWindow : public SubWindow
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(8,2) ;

     RefVal<Coord> space_dxy =  10 ;
     RefVal<Coord> radius    =  15 ;

     RefVal<Coord> mix_len   = 350 ;
     RefVal<Coord> mix_width =  30 ;

     RefVal<Coord> white_len = 250 ;

     RefVal<unsigned> len = 10 ;

     RefVal<VColor> top    =      Gray ;
     RefVal<VColor> bottom =      Snow ;
     RefVal<VColor> focus  = OrangeRed ;

     RefVal<Font> font;

     Config() noexcept {}

     explicit Config(const UserPreference &pref)
      {
       space_dxy.bind(pref.get().space_dxy);
       top.bind(pref.get().bottom);
       bottom.bind(pref.get().top);
       focus.bind(pref.get().focus);
       font.bind(pref.get().label_font.font);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   bool focus = false ;

   // colors

   VColor value = Black ;

   VColor back = Black ;
   VColor fore = Green ;

   VColor left = Black ;
   VColor right = White ;

   VColor top = Black ;

   DynArray<VColor> palette;

   enum ItemIndex : unsigned
    {
     Item_none = 0,

     Item_value,

     Item_back,
     Item_fore,

     Item_left,
     Item_right,
     Item_mix,

     Item_top,
     Item_white,

     Item_palette_base
    };

   ItemIndex item = Item_value ;

   Coord mix_pos = 0 ;
   Coord white_pos = 0 ;

   // positions

   Point value_center;
   Point back_center;
   Point fore_center;
   Point left_center;
   Point right_center;
   Point top_center;

   Point palette_base_center;

   Pane palette_contour;
   Pane mix_place;
   Pane mixUp_place;
   Pane text_place;
   Pane white_place;
   Pane whiteUp_place;

   unsigned len = 10 ;

  private:

   void preparePalette();

   VColor pal(ulen ind) const { return (ind<palette.getLen())?palette[ind]:Black; }

   class Art;

   void draw(Art &art,MPoint center,VColor color,bool select) const;

   static Coord Dist(Point p) { return Max(IntAbs(p.x),IntAbs(p.y)); }

   static Coord Dist(Point a,Point b) { return Dist(a-b); }

   class WhiteField;

   ItemIndex getItem(Point point) const;

   VColor getColor(ItemIndex item) const;

   void setColor(ItemIndex item,VColor vc);

   void copy(VColor vc);

   class TextToColor;

   bool past(VColor &ret);

  public:

   ColorEditWindow(SubWindowHost &host,const Config &cfg);

   virtual ~ColorEditWindow();

   // methods

   Point getMinSize() const { return Null; }

   VColor getColor() const { return value; }

   void setColor(VColor value);

   // drawing

   virtual void layout();

   virtual void draw(DrawBuf buf,bool drag_active) const;

   // base

   virtual void open();

   virtual void close();

   // keyboard

   virtual FocusType askFocus() const;

   virtual void gainFocus();

   virtual void looseFocus();

   // tab focus

   virtual void topTabFocus();

   virtual bool nextTabFocus();

   virtual void bottomTabFocus();

   virtual bool prevTabFocus();

   // mouse

   virtual void looseCapture();

   virtual MouseShape getMouseShape(Point point,KeyMod kmod) const;

   // user input

   virtual void react(UserAction action);

   void react_Key(VKey vkey,KeyMod kmod);

   void react_LeftClick(Point point,MouseKey mkey);

   void react_RightClick(Point point,MouseKey mkey);

   void react_Move(Point point,MouseKey mkey);

   void react_Wheel(Point point,MouseKey mkey,Coord delta);

   // signals

   Signal<VColor> changed; // value
 };

} // namespace App

#endif


