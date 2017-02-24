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

#include <CCore/inc/video/WindowLib.h>

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
     CtorRefVal<ScrollListWindow::ConfigType> list_cfg;
     CtorRefVal<TextWindow::ConfigType> text_cfg;
     CtorRefVal<LightWindow::ConfigType> light_cfg;
     CtorRefVal<LabelWindow::ConfigType> label_cfg;
     CtorRefVal<XDoubleLineWindow::ConfigType> dline_cfg;
     CtorRefVal<CheckWindow::ConfigType> check_cfg;
     CtorRefVal<RadioWindow::ConfigType> radio_cfg;
     CtorRefVal<TextContourWindow::ConfigType> text_contour_cfg;
     CtorRefVal<ContourWindow::ConfigType> contour_cfg;
     CtorRefVal<SpinorWindow::ConfigType> spin_cfg;
     CtorRefVal<InfoWindow::ConfigType> info_cfg;

     Config() noexcept {}

     template <class Bag,class Proxy>
     void bind(const Bag &bag,Proxy proxy)
      {
       space_dxy.bind(bag.space_dxy);
       progress_dy.bind(bag.progress_dy);
       check_dxy.bind(bag.check_dxy);
       light_dxy.bind(bag.light_dxy);

       progress_cfg.bind(proxy);
       list_cfg.bind(proxy);
       text_cfg.bind(proxy);
       light_cfg.bind(proxy);
       label_cfg.bind(proxy);
       dline_cfg.bind(proxy);
       check_cfg.bind(proxy);
       radio_cfg.bind(proxy);
       text_contour_cfg.bind(proxy);
       contour_cfg.bind(proxy);
       spin_cfg.bind(proxy);
       info_cfg.bind(proxy);
      }
    };

   using ConfigType = Config ;

  private:

   static DefString TestText();

   const Config &cfg;

   Coord def_dy = 20 ;
   FontCouple font;

   ProgressWindow progress;

   class ProgressControl : public IncrementalProgress
    {
      WindowList &wlist;
      ProgressWindow &window;

     public:

      ProgressControl(WindowList &wlist,ProgressWindow &window);

      ~ProgressControl();

      // IncrementalProgress

      virtual void start();

      virtual void setTotal(unsigned total);

      virtual bool setPos(unsigned pos);

      virtual void stop() noexcept;
    };

   ProgressControl progress_control;

   FontDatabase fdb;
   FontDatabase::Incremental fdb_inc;
   bool fdb_flag = true ;

   class FDBInfo : public ComboInfo
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

   ScrollListWindow list;
   TextWindow text_file_name;
   TextWindow text_family;

   LightWindow light_scalable;
   LightWindow light_monospace;
   LightWindow light_bold;
   LightWindow light_italic;

   LabelWindow label_scalable;
   LabelWindow label_monospace;
   LabelWindow label_bold;
   LabelWindow label_italic;

   XDoubleLineWindow line1;

   SpinorWindow spin_fdy;
   CheckWindow check_fdx;
   SpinorWindow spin_fdx;

   XDoubleLineWindow line2;

   RadioGroup group_hint;

   RadioWindow radio_no_hint;
   RadioWindow radio_native_hint;
   RadioWindow radio_auto_hint;

   LabelWindow label_no_hint;
   LabelWindow label_native_hint;
   LabelWindow label_auto_hint;

   TextContourWindow contour_hint;

   RadioGroup group_smooth;

   RadioWindow radio_no_smooth;
   RadioWindow radio_smooth;
   RadioWindow radio_RGB;
   RadioWindow radio_BGR;

   LabelWindow label_no_smooth;
   LabelWindow label_smooth;
   LabelWindow label_RGB;
   LabelWindow label_BGR;

   TextContourWindow contour_smooth;

   CheckWindow check_kerning;

   LabelWindow label_kerning;

   SpinorWindow spin_strength;

   LabelWindow label_strength;

   InfoWindow::ConfigType info_cfg;

   InfoWindow info_test;

   ContourWindow contour_test;

  private:

   class MaxIndexFunc;

   static int GetMaxIndex(Font font);

   void updateFont();

   void showFont(ulen select);

   void showFont();

   void noSize();

   void setSize();

   void setConfig();

   void setCouple();

  private:

   void fdb_completed(bool ok);

   SignalConnector<FontEditWindow,bool> connector_fdb_completed;

   void list_selected(ulen select);

   SignalConnector<FontEditWindow,ulen> connector_list_selected;

   void check_fdx_changed(bool enable);

   SignalConnector<FontEditWindow,bool> connector_check_fdx_changed;

   void spin_fdxy_changed(int);

   SignalConnector<FontEditWindow,int> connector_spin_fdy_changed;
   SignalConnector<FontEditWindow,int> connector_spin_fdx_changed;

   void group_hint_changed(int new_id,int old_id);

   SignalConnector<FontEditWindow,int,int> connector_group_hint_changed;

   void group_smooth_changed(int new_id,int old_id);

   SignalConnector<FontEditWindow,int,int> connector_group_smooth_changed;

   void check_kerning_changed(bool check);

   SignalConnector<FontEditWindow,bool> connector_check_kerning_changed;

   void spin_strength_changed(int strength);

   SignalConnector<FontEditWindow,int> connector_spin_strength_changed;

  public:

   FontEditWindow(SubWindowHost &host,const ConfigType &cfg);

   virtual ~FontEditWindow();

   // methods

   Point getMinSize(Point cap=Point::Max()) const;

   const FontParam & getParam() const { return font.param; }

   const Font & getFont() const { return font.font; }

   const FontCouple & getCouple() const { return font; }

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
