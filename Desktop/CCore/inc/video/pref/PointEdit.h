/* PointEdit.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 3.00
//
//  Tag: Desktop
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2017 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_video_pref_PointEdit_h
#define CCore_inc_video_pref_PointEdit_h

#include <CCore/inc/video/pref/SpinEdit.h>

namespace CCore {
namespace Video {

/* classes */

class PointEditWindow;

/* class PointEditWindow */

class PointEditWindow : public ComboWindow
 {
  public:

   struct Config
    {
     RefVal<Coord> space_dxy = 10 ;

     RefVal<MCoord> width = Fraction(1) ;

     RefVal<VColor> line = Black ;

     CtorRefVal<SpinEditWindow::ConfigType> spin_cfg;

     Config() noexcept {}

     template <class Bag,class Proxy>
     void bind(const Bag &bag,Proxy proxy)
      {
       space_dxy.bind(bag.space_dxy);
       width.bind(bag.coord_edit_width);
       line.bind(bag.coord_edit_line);

       spin_cfg.bind(proxy);
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

} // namespace Video
} // namespace CCore

#endif

