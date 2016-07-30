/* Designer.h */
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

#ifndef Designer_h
#define Designer_h

#include <inc/TestFrame.h>
#include <inc/CoordEdit.h>
#include <inc/MCoordEdit.h>
#include <inc/FontEdit.h>
#include <inc/PointEdit.h>
#include <inc/ColorEdit.h>

namespace App {

/* classes */

class Preference;

class PlaceFrame;

class DesignerWindow;

/* class Preference */

class Preference : public UserPreference
 {
  public:

   Preference();

   ~Preference();

   // signals

   Signal<> update;
 };

/* class PlaceFrame */

class PlaceFrame
 {
   Point max_size;
   Pane max_pane;

  public:

   explicit PlaceFrame(Desktop *desktop)
    {
     max_size=desktop->getScreenSize();
     max_pane=desktop->getMaxPane();
    }

   Point getMaxSize() const { return max_size; }

   Pane getPane(Ratio x,Ratio dx,Ratio y,Ratio dy) const
    {
     return Pane(x*max_pane.dx,y*max_pane.dy,dx*max_pane.dx,dy*max_pane.dy)+max_pane.getBase();
    }
 };

/* class DesignerWindow */

class DesignerWindow : public ComboWindow
 {
  public:

   struct Config
    {
     RefVal<Coord> space_dxy = 10 ;

     RefVal<VColor> back = Silver ;

     CtorRefVal<SimpleTextListWindow::ConfigType> text_list_cfg;
     CtorRefVal<CheckWindow::ConfigType> check_cfg;
     CtorRefVal<LabelWindow::ConfigType> label_cfg;

     CtorRefVal<ButtonWindow::ConfigType> btn_cfg;

     CtorRefVal<LineEditWindow::ConfigType> edit_cfg;
     CtorRefVal<CoordEditWindow::ConfigType> coord_cfg;
     CtorRefVal<MCoordEditWindow::ConfigType> mcoord_cfg;
     CtorRefVal<FontEditWindow::ConfigType> font_cfg;
     CtorRefVal<SpinEditWindow::ConfigType> unsigned_cfg;
     CtorRefVal<PointEditWindow::ConfigType> point_cfg;
     CtorRefVal<ColorEditWindow::ConfigType> color_cfg;

     Config()
      {
      }

     explicit Config(const UserPreference &pref)
      : coord_cfg(pref),
        mcoord_cfg(pref),
        font_cfg(pref),
        unsigned_cfg(pref),
        point_cfg(pref),
        color_cfg(pref)
      {
       text_list_cfg.bind(pref.getSimpleTextListConfig());
       check_cfg.bind(pref.getCheckConfig());
       label_cfg.bind(pref.getLabelConfig());

       btn_cfg.bind(pref.getButtonConfig());

       edit_cfg.bind(pref.getLineEditConfig());
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   Preference &self_pref;

   UserPreference pref;
   Signal<> update;

   TestFrame test_frame;

   SimpleTextListWindow text_list;

   CheckWindow check_all;
   CheckWindow check_Coord;
   CheckWindow check_MCoord;
   CheckWindow check_VColor;
   CheckWindow check_unsigned;
   CheckWindow check_String;
   CheckWindow check_Point;
   CheckWindow check_Font;

   LabelWindow label_all;
   LabelWindow label_Coord;
   LabelWindow label_MCoord;
   LabelWindow label_VColor;
   LabelWindow label_unsigned;
   LabelWindow label_String;
   LabelWindow label_Point;
   LabelWindow label_Font;

   ButtonWindow btn_Save;
   ButtonWindow btn_Self;

   class PrefInfo : public Info
    {
      class Base;

     private:

      Base * getBase();

     public:

      PrefInfo();

      ~PrefInfo();

      class Binder;

      Binder getBinder();

      bool enable_all(bool on);

      bool enable_Coord(bool on);

      bool enable_MCoord(bool on);

      bool enable_VColor(bool on);

      bool enable_unsigned(bool on);

      bool enable_String(bool on);

      bool enable_Point(bool on);

      bool enable_Font(bool on);

      template <class Func>
      void select(ulen index,Func func);
    };

   PrefInfo info;

   LineEditWindow string_edit;
   DefString *string_var = 0 ;

   CoordEditWindow coord_edit;
   Coord *coord_var = 0 ;

   MCoordEditWindow mcoord_edit;
   MCoord *mcoord_var = 0 ;

   FontEditWindow font_edit;
   FontParam *font_param_var = 0 ;
   Font *font_var = 0 ;

   SpinEditWindow unsigned_edit;
   unsigned *unsigned_var = 0 ;

   PointEditWindow point_edit;
   Point *point_var = 0 ;

   ColorEditWindow color_edit;
   VColor *color_var = 0 ;

   SubWindow *active_editor = 0 ;

  private:

   void switchTo(SubWindow *editor);

   void switchTo(SubWindow &editor) { switchTo(&editor); }

  private:

   void testDestroyed();

   SignalConnector<DesignerWindow> connector_test_frame_destroyed;

   void newList();

   void enable_all(bool on);

   void enable_Coord(bool on);

   void enable_MCoord(bool on);

   void enable_VColor(bool on);

   void enable_unsigned(bool on);

   void enable_String(bool on);

   void enable_Point(bool on);

   void enable_Font(bool on);

   SignalConnector<DesignerWindow,bool> connector_check_all_changed;
   SignalConnector<DesignerWindow,bool> connector_check_Coord_changed;
   SignalConnector<DesignerWindow,bool> connector_check_MCoord_changed;
   SignalConnector<DesignerWindow,bool> connector_check_VColor_changed;
   SignalConnector<DesignerWindow,bool> connector_check_unsigned_changed;
   SignalConnector<DesignerWindow,bool> connector_check_String_changed;
   SignalConnector<DesignerWindow,bool> connector_check_Point_changed;
   SignalConnector<DesignerWindow,bool> connector_check_Font_changed;

   void savePref();

   SignalConnector<DesignerWindow> connector_btnSave_pressed;

   void selfPref();

   SignalConnector<DesignerWindow> connector_btnSelf_pressed;

   void select(Coord &var);

   void select(MCoord &var);

   void select(VColor &var);

   void select(unsigned &var);

   void select(DefString &var);

   void select(Point &var);

   void select(FontParam &var,Font &font);

   void select();

   struct FuncSelect;

   void selectVar(ulen index);

   SignalConnector<DesignerWindow,ulen> connector_text_list_selected;

   void changed();

   void string_edit_changed();

   SignalConnector<DesignerWindow> connector_string_edit_changed;

   void coord_edit_changed(Coord value);

   SignalConnector<DesignerWindow,Coord> connector_coord_edit_changed;

   void mcoord_edit_changed(MCoord value);

   SignalConnector<DesignerWindow,MCoord> connector_mcoord_edit_changed;

   void font_edit_changed();

   SignalConnector<DesignerWindow> connector_font_edit_changed;

   void unsigned_edit_changed(int value);

   SignalConnector<DesignerWindow,int> connector_unsigned_edit_changed;

   void point_edit_changed(Point value);

   SignalConnector<DesignerWindow,Point> connector_point_edit_changed;

   void color_edit_changed(VColor value);

   SignalConnector<DesignerWindow,VColor> connector_color_edit_changed;

  public:

   DesignerWindow(SubWindowHost &host,const Config &cfg,Preference &self_pref);

   virtual ~DesignerWindow();

   // drawing

   virtual void layout();

   virtual void draw(DrawBuf buf,bool drag_active) const;

   virtual void draw(DrawBuf buf,Pane pane,bool drag_active) const;

   // base

   virtual void open();
 };

} // namespace App

#endif
