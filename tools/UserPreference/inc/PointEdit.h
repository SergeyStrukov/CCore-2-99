/* PointEdit.h */
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

#ifndef PointEdit_h
#define PointEdit_h

#include <inc/SpinEdit.h>

namespace App {

/* classes */

class PointEditWindow;

/* class PointEditWindow */

class PointEditWindow : public ComboWindow
 {
  public:

   struct Config
    {
     RefVal<MCoord> width = Fraction(1) ;

     RefVal<Coord> space_dxy = 10 ;

     RefVal<VColor> line = Black ;

     CtorRefVal<SpinEditWindow::ConfigType> spin_cfg;

     Config()
      {
      }

     explicit Config(const UserPreference &pref)
      : spin_cfg(pref)
      {
       space_dxy.bind(pref.get().space_dxy);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   SpinEditWindow x_spin;
   SpinEditWindow y_spin;

   Point pos;

  private:

   void drawBox(const DrawBuf &buf) const;

   void changePoint(int);

   SignalConnector<PointEditWindow,int> connector_x_spin_changed;
   SignalConnector<PointEditWindow,int> connector_y_spin_changed;

  public:

   PointEditWindow(SubWindowHost &host,const ConfigType &cfg);

   virtual ~PointEditWindow();

   // methods

   Point getMinSize() const;

   Point getPoint() const;

   void setPoint(Point value);

   // drawing

   virtual void layout();

   virtual void draw(DrawBuf buf,bool drag_active) const;

   virtual void draw(DrawBuf buf,Pane pane,bool drag_active) const;

   // signals

   Signal<Point> changed; // value
 };

} // namespace App

#endif

