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

#include <inc/EditAngleWindow.h>
#include <inc/EditLengthWindow.h>
#include <inc/EditRatioWindow.h>
#include <inc/GeometryWindow.h>
#include <inc/ItemListWindow.h>

namespace App {

/* classes */

class EditorWindow;

/* class EditorWindow */

class EditorWindow : public ComboWindow
 {
  public:

   struct Config
    {
     RefVal<VColor> back = Silver ;

     RefVal<Coord> space_dxy = 10 ;

     CtorRefVal<XSplitWindow::ConfigType> split_cfg;

     // app

     CtorRefVal<EditAngleWindow::ConfigType> edit_angle_cfg;
     CtorRefVal<EditLengthWindow::ConfigType> edit_length_cfg;
     CtorRefVal<EditRatioWindow::ConfigType> edit_ratio_cfg;
     CtorRefVal<GeometryWindow::ConfigType> geom_cfg;
     CtorRefVal<ItemListWindow::ConfigType> ilist_cfg;

     Config() noexcept {}

     template <class AppPref>
     Config(const UserPreference &pref,const AppPref &app_pref) noexcept
      : edit_angle_cfg(pref,app_pref),
        edit_length_cfg(pref,app_pref),
        edit_ratio_cfg(pref,app_pref),
        geom_cfg(pref,app_pref),
        ilist_cfg(pref,app_pref)
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

   EditRatioWindow edit_ratio;

   GeometryWindow geom;

   ItemListWindow list_pad;
   ItemListWindow list_formula;

   // layout

   bool layout_first = true ;
   Coord left_dx = 0 ;
   Coord top_dy = 0 ;

  private:

   void deactivate();

   void activate(SubWindow &editor);

   template <class T>
   struct EditorPad
    {
     Contour::Object obj;
     T *ptr = 0 ;

     void select(const Contour::Object &obj_,T &var)
      {
       obj=obj_;
       ptr=&var;
      }

     bool set(T val)
      {
       if( ptr )
         {
          *ptr=val;

          return true;
         }

       return false;
      }
    };

   EditorPad<Geometry::Angle> angle_pad;
   EditorPad<Geometry::Length> length_pad;
   EditorPad<Geometry::Ratio> ratio_pad;

   void select(const Contour::Object &obj,Geometry::Angle &angle);

   void select(const Contour::Object &obj,Geometry::Length &length);

   void select(const Contour::Object &obj,Geometry::Ratio &ratio);

   void select(const Contour::Object &obj,Geometry::Point &point);

   struct SelectPad;

   void selectPad(ulen index);

   void unselectPad();

   void angle_changed(Geometry::Angle angle);

   void length_changed(Geometry::Length length);

   void ratio_changed(Geometry::Ratio ratio);

   SignalConnector<EditorWindow,Geometry::Angle> connector_angle_changed;
   SignalConnector<EditorWindow,Geometry::Length> connector_length_changed;
   SignalConnector<EditorWindow,Geometry::Ratio> connector_ratio_changed;

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

  private:

   void pad_up(ulen ind);

   void pad_down(ulen ind);

   void pad_del(ulen ind);

   void pad_add(ulen ind);

   void pad_selected(ulen ind);

   SignalConnector<EditorWindow,ulen> connectoir_list_pad_command_up;
   SignalConnector<EditorWindow,ulen> connectoir_list_pad_command_down;
   SignalConnector<EditorWindow,ulen> connectoir_list_pad_command_del;
   SignalConnector<EditorWindow,ulen> connectoir_list_pad_command_add;
   SignalConnector<EditorWindow,ulen> connectoir_list_pad_command_selected;

  private:

   void formula_up(ulen ind);

   void formula_down(ulen ind);

   void formula_del(ulen ind);

   void formula_add(ulen ind);

   void formula_selected(ulen ind);

   SignalConnector<EditorWindow,ulen> connectoir_list_formula_command_up;
   SignalConnector<EditorWindow,ulen> connectoir_list_formula_command_down;
   SignalConnector<EditorWindow,ulen> connectoir_list_formula_command_del;
   SignalConnector<EditorWindow,ulen> connectoir_list_formula_command_add;
   SignalConnector<EditorWindow,ulen> connectoir_list_formula_command_selected;

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
