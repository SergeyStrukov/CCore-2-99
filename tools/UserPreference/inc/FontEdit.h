/* FontEdit.h */
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

#ifndef FontEdit_h
#define FontEdit_h

#include <inc/SpinEdit.h>

#include <CCore/inc/video/WindowLib.h>
#include <CCore/inc/video/FontParam.h>
#include <CCore/inc/video/FontDatabase.h>

namespace App {

/* classes */

class FontEditWindow;

/* class FontEditWindow */

class FontEditWindow : public ComboWindow
 {
  public:

   struct Config
    {
     RefVal<Coord> progress_dy = 20 ;
     RefVal<Coord> space_dxy = 10 ;
     RefVal<Coord> check_dxy = 18 ;
     RefVal<Coord> light_dxy = 20 ;

     CtorRefVal<ProgressWindow::ConfigType> progress_cfg;
     CtorRefVal<SimpleTextListWindow::ConfigType> text_list_cfg;
     CtorRefVal<TextWindow::ConfigType> text_cfg;
     CtorRefVal<LightWindow::ConfigType> light_cfg;
     CtorRefVal<LabelWindow::ConfigType> label_cfg;
     CtorRefVal<XDoubleLineWindow::ConfigType> dline_cfg;
     CtorRefVal<CheckWindow::ConfigType> check_cfg;
     CtorRefVal<RadioWindow::ConfigType> radio_cfg;
     CtorRefVal<TextContourWindow::ConfigType> text_contour_cfg;
     CtorRefVal<ContourWindow::ConfigType> contour_cfg;

     CtorRefVal<SpinEditWindow::ConfigType> spin_cfg;

     Config() noexcept {}

     explicit Config(const UserPreference &pref)
      : spin_cfg(pref)
      {
       progress_dy.bind(pref.get().progress_dy);
       space_dxy.bind(pref.get().space_dxy);
       check_dxy.bind(pref.get().check_dxy);
       light_dxy.bind(pref.get().light_dxy);

       progress_cfg.bind(pref.getProgressConfig());
       text_list_cfg.bind(pref.getSimpleTextListConfig());
       text_cfg.bind(pref.getTextConfig());
       light_cfg.bind(pref.getLightConfig());
       label_cfg.bind(pref.getLabelConfig());
       dline_cfg.bind(pref.getXDoubleLineConfig());
       check_cfg.bind(pref.getCheckConfig());
       radio_cfg.bind(pref.getRadioConfig());
       text_contour_cfg.bind(pref.getTextContourConfig());
       contour_cfg.bind(pref.getContourConfig());
      }
    };

   using ConfigType = Config ;

  private:

   static const char *const TestText;

   const Config &cfg;

   Coord def_dy = 20 ;
   FontParam param;
   Font font;

   ProgressWindow progress;

   class ProgressControl : public IncrementalProgress
    {
      WindowList &wlist;
      ProgressWindow &window;

     public:

      ProgressControl(WindowList &wlist_,ProgressWindow &window_) : wlist(wlist_),window(window_) {}

      ~ProgressControl() {}

      // IncrementalProgress

      virtual void start() { wlist.insTop(window); }

      virtual void setTotal(unsigned total) { window.setTotal(total); }

      virtual bool setPos(unsigned pos) { window.setPosPing(pos); return true; }

      virtual void stop() noexcept { wlist.del(window); }
    };

   ProgressControl progress_control;

   FontDatabase fdb;
   FontDatabase::Incremental fdb_inc;
   bool fdb_flag = true ;

   class FDBInfo : public Info
    {
      class Base;

     private:

      Base * getBase() const;

     public:

      FDBInfo();

      ~FDBInfo();

      void build(FontDatabase &fdb);

      const FontInfo * get(ulen index) const;

      String getFamily(ulen index) const;

      ulen getIndex(StrLen file_name) const;
    };

   FDBInfo info;

   SimpleTextListWindow text_list;
   TextWindow file_name_text;
   TextWindow family_text;

   LightWindow scalable_light;
   LightWindow monospace_light;
   LightWindow bold_light;
   LightWindow italic_light;

   LabelWindow scalable_label;
   LabelWindow monospace_label;
   LabelWindow bold_label;
   LabelWindow italic_label;

   XDoubleLineWindow line1;

   SpinEditWindow fdy_spin;
   CheckWindow fdx_check;
   SpinEditWindow fdx_spin;

   XDoubleLineWindow line2;

   RadioGroup hint_group;

   RadioWindow no_hint_radio;
   RadioWindow native_hint_radio;
   RadioWindow auto_hint_radio;

   LabelWindow no_hint_label;
   LabelWindow native_hint_label;
   LabelWindow auto_hint_label;

   TextContourWindow hint_contour;

   RadioGroup smooth_group;

   RadioWindow no_smooth_radio;
   RadioWindow smooth_radio;
   RadioWindow RGB_radio;
   RadioWindow BGR_radio;

   LabelWindow no_smooth_label;
   LabelWindow smooth_label;
   LabelWindow RGB_label;
   LabelWindow BGR_label;

   TextContourWindow smooth_contour;

   CheckWindow kerning_check;

   LabelWindow kerning_label;

   SpinEditWindow strength_spin;

   LabelWindow strength_label;

   InfoWindow::ConfigType info_cfg;

   InfoWindow font_test;

   ContourWindow test_contour;

  private:

   class MaxIndexFunc;

   static int GetMaxIndex(Font font);

   void updateFont();

   void showFont(ulen select);

   void noSize();

   void setSize();

   void setConfig();

   void setParam();

   void fdbComplete(bool ok);

   SignalConnector<FontEditWindow,bool> connector_fdb_complete;

   void selectFont(ulen select);

   SignalConnector<FontEditWindow,ulen> connector_text_list_selected;

   void fdxEnable(bool enable);

   SignalConnector<FontEditWindow,bool> connector_fdx_check_changed;

   void fdxyChanged(int);

   SignalConnector<FontEditWindow,int> connector_fdy_spin_changed;
   SignalConnector<FontEditWindow,int> connector_fdx_spin_changed;

   void hintChanged(int new_id,int old_id);

   SignalConnector<FontEditWindow,int,int> connector_hint_group_changed;

   void smoothChanged(int new_id,int old_id);

   SignalConnector<FontEditWindow,int,int> connector_smooth_group_changed;

   void kerningChanged(bool check);

   SignalConnector<FontEditWindow,bool> connector_kerning_check_changed;

   void strengthChanged(int strength);

   SignalConnector<FontEditWindow,int> connector_strength_spin_changed;

  public:

   FontEditWindow(SubWindowHost &host,const ConfigType &cfg);

   virtual ~FontEditWindow();

   // methods

   const FontParam & getParam() const { return param; }

   const Font & getFont() const { return font; }

   void setParam(const FontParam &param,const Font &font);

   // drawing

   virtual void layout();

   // base

   virtual void open();

   // signals

   Signal<> changed;
 };

} // namespace App

#endif
