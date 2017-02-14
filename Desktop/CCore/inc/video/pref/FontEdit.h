/* FontEdit.h */
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

#ifndef CCore_inc_video_pref_FontEdit_h
#define CCore_inc_video_pref_FontEdit_h

#include <CCore/inc/video/pref/SpinEdit.h>

#include <CCore/inc/video/FontParam.h>
#include <CCore/inc/video/FontDatabase.h>

#include <CCore/inc/video/ConfigBinder.h>

namespace CCore {
namespace Video {

/* classes */

class FontEditWindow;

/* class FontEditWindow */

class FontEditWindow : public ComboWindow
 {
  public:

   struct Config
    {
     RefVal<Coord> space_dxy = 10 ;
     RefVal<Coord> progress_dy = 24 ;
     RefVal<Coord> check_dxy = 20 ;
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

     template <class Bag,class Proxy>
     void bind(const Bag &bag,Proxy proxy)
      {
       space_dxy.bind(bag.space_dxy);
       progress_dy.bind(bag.progress_dy);
       check_dxy.bind(bag.check_dxy);
       light_dxy.bind(bag.light_dxy);

       progress_cfg.bind(proxy);
       text_list_cfg.bind(proxy);
       text_cfg.bind(proxy);
       light_cfg.bind(proxy);
       label_cfg.bind(proxy);
       dline_cfg.bind(proxy);
       check_cfg.bind(proxy);
       radio_cfg.bind(proxy);
       text_contour_cfg.bind(proxy);
       contour_cfg.bind(proxy);
       spin_cfg.bind(proxy);
      }
    };

   using ConfigType = Config ;

  private:

   static DefString TestText;

   const Config &cfg;

   Coord def_dy = 20 ;
   FontCouple font;

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

      DefString getFamily(ulen index) const;

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

   void setCouple();

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

   Point getMinSize() const;

   const FontParam & getParam() const { return font.param; }

   const Font & getFont() const { return font.font; }

   const FontCouple getCouple() const { return font; }

   void setCouple(const FontCouple &font);

   // drawing

   virtual void layout();

   // base

   virtual void open();

   // signals

   Signal<> changed;
 };

} // namespace Video
} // namespace CCore

#endif
