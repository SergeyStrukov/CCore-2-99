/* Editor.h */
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

#ifndef Editor_h
#define Editor_h

#include <inc/Contour.h>

#include <inc/EditAngleWindow.h>
#include <inc/EditLengthWindow.h>
#include <inc/GeometryWindow.h>

namespace App {

/* classes */

class ItemListWindow;

class EditRatioWindow;

class EditorWindow;

/* class ItemListWindow */

class ItemListWindow : public ComboWindow // TODO
 {
  public:

   struct Config
    {
     Config() noexcept {}

     Config(const UserPreference &pref) noexcept
      {
       bind(pref.get(),pref.getSmartConfig());
      }

     template <class Bag,class Proxy>
     void bind(const Bag &bag,Proxy proxy)
      {
       Used(bag);
       Used(proxy);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

  public:

   ItemListWindow(SubWindowHost &host,const Config &cfg);

   virtual ~ItemListWindow();

   // methods

   Point getMinSize() const { return Point(10,10); }

   // drawing

   virtual void layout();

   virtual void drawBack(DrawBuf buf,bool drag_active) const;
 };

/* class EditRatioWindow */

class EditRatioWindow : public SubWindow // TODO
 {
  public:

   struct Config
    {
     Config() noexcept {}

     Config(const UserPreference &pref) noexcept
      {
       bind(pref.get(),pref.getSmartConfig());
      }

     template <class Bag,class Proxy>
     void bind(const Bag &bag,Proxy proxy)
      {
       Used(bag);
       Used(proxy);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

  public:

   EditRatioWindow(SubWindowHost &host,const Config &cfg);

   virtual ~EditRatioWindow();

   // methods

   Point getMinSize() const { return Point(10,10); }

   // drawing

   virtual void layout();

   virtual void draw(DrawBuf buf,bool drag_active) const;
 };

/* class EditorWindow */

class EditorWindow : public ComboWindow
 {
  public:

   struct Config
    {
     RefVal<VColor> back;

     RefVal<Coord> space_dxy;

     CtorRefVal<XSplitWindow::ConfigType> split_cfg;

     // app

     CtorRefVal<EditAngleWindow::ConfigType> edit_angle_cfg;
     CtorRefVal<EditLengthWindow::ConfigType> edit_length_cfg;
     CtorRefVal<GeometryWindow::ConfigType> geom_cfg;

     Config() noexcept {}

     template <class AppPref>
     Config(const UserPreference &pref,const AppPref &app_pref) noexcept
      : edit_angle_cfg(pref,app_pref),
        edit_length_cfg(pref,app_pref),
        geom_cfg(pref,app_pref)
      {
       bind(pref.get(),pref.getSmartConfig());
      }

     template <class Bag,class Proxy>
     void bind(const Bag &bag,Proxy proxy)
      {
       back.bind(bag.back);
       space_dxy.bind(bag.space_dxy);

       split_cfg.bind(proxy);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   bool modified = false ;

   XSplitWindow split1;

   YSplitWindow split2;

   EditAngleWindow edit_angle;

   EditLengthWindow edit_length;

   GeometryWindow geom;

   // layout

   bool layout_first = true ;
   Coord left_dx = 0 ;
   Coord top_dy = 0 ;

  private:

   Coord getMinDXY() const;

   Coord getMaxLeftDX() const;

   Coord getMaxTopDY() const;

   bool adjustSplitX(Coord dx);

   bool adjustSplitY(Coord dy);

   void adjustSplit(Point point);

  private:

   void split1_dragged(Point point);

   void split2_dragged(Point point);

   SignalConnector<EditorWindow,Point> connector_split1_dragged;
   SignalConnector<EditorWindow,Point> connector_split2_dragged;

  public:

   EditorWindow(SubWindowHost &host,const Config &cfg);

   virtual ~EditorWindow();

   // methods

   Point getMinSize() const { return Point(10,10); }

   bool isModified() const { return modified; }

   void load();

   void load(StrLen file_name);

   bool save();

   void save(StrLen file_name);

   // drawing

   virtual void layout();

   virtual void drawBack(DrawBuf buf,bool drag_active) const;
 };

} // namespace App

#endif
