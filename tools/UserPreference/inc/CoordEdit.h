/* CoordEdit.h */
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

#ifndef CoordEdit_h
#define CoordEdit_h

#include <inc/SpinEdit.h>

namespace App {

/* classes */

class CoordEditWindow;

/* class CoordEditWindow */

class CoordEditWindow : public ComboWindow
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(1) ;
     RefVal<Coord> mark_dy = 3 ;

     RefVal<VColor> line = Black ;

     CtorRefVal<SpinEditWindow::ConfigType> spin_cfg;

     Config()
      {
      }

     explicit Config(const UserPreference &pref)
      : spin_cfg(pref)
      {
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   SpinEditWindow spin;

   Point pos;

  private:

   void drawLine(const DrawBuf &buf) const;

   void valueChanged(int value);

   SignalConnector<CoordEditWindow,int> spin_changed;

  public:

   CoordEditWindow(SubWindowHost &host,const ConfigType &cfg);

   virtual ~CoordEditWindow();

   // methods

   Point getMinSize() const;

   Coord getCoord() const { return Coord(spin.getValue()); }

   void setCoord(Coord value) { spin.setValue(value); }

   // drawing

   virtual void layout();

   virtual void draw(DrawBuf buf,bool drag_active) const;

   virtual void draw(DrawBuf buf,Pane pane,bool drag_active) const;

   // signals

   Signal<Coord> changed; // value
 };

} // namespace App

#endif



