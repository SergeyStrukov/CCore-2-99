/* ItemListWindow.h */
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

#ifndef ItemListWindow_h
#define ItemListWindow_h

#include <inc/Contour.h>

namespace App {

/* classes */

class EditFormulaShape;

class EditFormulaWindow;

class ItemListWindow;

/* class EditFormulaShape */

class EditFormulaShape : public LineEditShape
 {
  public:

   struct Config : LineEditShape::Config
    {
     // app

     RefVal<VColor> text        =     Black ;
     RefVal<VColor> error_text  =       Red ;
     RefVal<VColor> number_text = DarkGreen ;
     RefVal<VColor> length_text = DarkGreen ;
     RefVal<VColor> angle_text  = DarkGreen ;
     RefVal<VColor> name_text   =      Blue ;
     RefVal<VColor> punct_text  =     Black ;

     RefVal<unsigned> delay = 1_sectick ;

     Config() noexcept {}

     template <class AppPref>
     Config(const UserPreference &pref,const AppPref &app_pref) noexcept
      {
       bind(pref.get());

       font.bind(pref.get().code_font.font);

       bindApp(app_pref.get());
      }

     template <class Bag>
     void bindApp(const Bag &bag)
      {
       text.bind(bag.edit_text);
       error_text.bind(bag.edit_error_text);
       number_text.bind(bag.edit_number_text);
       length_text.bind(bag.edit_length_text);
       angle_text.bind(bag.edit_angle_text);
       name_text.bind(bag.edit_name_text);
       punct_text.bind(bag.edit_punct_text);
       delay.bind(bag.edit_delay);
      }
    };

   CharAccent *accent = 0 ;

   const Config & getCfg() const { return static_cast<const Config &>(cfg); }

   virtual void drawText(Font font,const DrawBuf &buf,Pane pane,TextPlace place,StrLen text,ulen off,VColor vc) const;

   EditFormulaShape(PtrLen<char> text_buf,const Config &cfg) : LineEditShape(text_buf,cfg) {}
 };

/* class EditFormulaWindow */

class EditFormulaWindow : public LineEditWindowOf<EditFormulaShape>
 {
   SimpleArray<CharAccent> storage;

   DeferTick defer_tick;

   unsigned time = 0 ;

   bool flag = false ;

  private:

   void tick();

   void start();

   SignalConnector<EditFormulaWindow> connector_changed;

  public:

   EditFormulaWindow(SubWindowHost &host,const ConfigType &cfg);

   ~EditFormulaWindow();

   // methods

   CharAccent * getAccentBuf() { return shape.accent; }

   void clearAccentFlag() { flag=false; }

   void normalAccent();

   // signals

   Signal<> paused;
 };

/* class ItemListWindow */

class ItemListWindow : public ComboWindow
 {
  public:

   struct Config
    {
     RefVal<Coord> space_dxy = 10 ;

     CtorRefVal<ScrollListWindow::ConfigType> list_cfg;
     CtorRefVal<KnobWindow::ConfigType> knob_cfg;

     CtorRefVal<RefLabelWindow::ConfigType> label_cfg;
     CtorRefVal<CheckWindow::ConfigType> check_cfg;

     // app

     CtorRefVal<EditFormulaWindow::ConfigType> edit_cfg;

     RefVal<DefString> text_show = "show"_def ;
     RefVal<DefString> text_gray = "gray"_def ;
     RefVal<DefString> text_name = "name"_def ;

     Config() noexcept {}

     template <class AppPref>
     Config(const UserPreference &pref,const AppPref &app_pref) noexcept
      : edit_cfg(pref,app_pref)
      {
       bind(pref.get(),pref.getSmartConfig());
       bindApp(app_pref.get());
      }

     template <class Bag,class Proxy>
     void bind(const Bag &bag,Proxy proxy)
      {
       space_dxy.bind(bag.space_dxy);

       list_cfg.bind(proxy);
       knob_cfg.bind(proxy);

       label_cfg.bind(proxy);
       check_cfg.bind(proxy);
      }

     template <class Bag>
     void bindApp(const Bag &bag)
      {
       text_show.bind(bag.text_show);
       text_gray.bind(bag.text_gray);
       text_name.bind(bag.text_name);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   ScrollListWindow list;

   EditFormulaWindow edit;

   KnobWindow knob_down;
   KnobWindow knob_up;
   KnobWindow knob_del;
   KnobWindow knob_add;

   RefLabelWindow label_show;
   RefLabelWindow label_gray;
   RefLabelWindow label_name;

   CheckWindow check_show;
   CheckWindow check_gray;
   CheckWindow check_name;

  private:

   void list_selected(ulen index);

   SignalConnector<ItemListWindow,ulen> connector_list_selected;

   void knob_down_pressed();

   void knob_up_pressed();

   void knob_del_pressed();

   void knob_add_pressed();

   SignalConnector<ItemListWindow> connector_edit_entered;

   SignalConnector<ItemListWindow> connector_knob_down_pressed;
   SignalConnector<ItemListWindow> connector_knob_up_pressed;
   SignalConnector<ItemListWindow> connector_knob_del_pressed;
   SignalConnector<ItemListWindow> connector_knob_add_pressed;

   void check_show_changed(bool check);

   void check_gray_changed(bool check);

   void check_name_changed(bool check);

   SignalConnector<ItemListWindow,bool> connector_check_show_changed;
   SignalConnector<ItemListWindow,bool> connector_check_gray_changed;
   SignalConnector<ItemListWindow,bool> connector_check_name_changed;

  public:

   ItemListWindow(SubWindowHost &host,const Config &cfg);

   virtual ~ItemListWindow();

   // methods

   Point getMinSize(Point cap=Point::Max()) const;

   void setInfo(const ComboInfo &info) { list.setInfo(info); }

   bool select(ulen index) { return list.select(index); }

   bool reselect() { return list.reselect(); }

   void ping() { list.ping(); }

   void updateList()
    {
     list.layout();

     list.redraw();
    }

   void setCheck(bool show,bool gray,bool name);

   void setCheck(const Label &label) { setCheck(label.show,label.gray,label.show_name); }

   void noItem();

   StrLen getText() const { return edit.getText(); }

   CharAccent * getAccentBuf() { return edit.getAccentBuf(); }

   void clearAccentFlag() { edit.clearAccentFlag(); }

   void normalAccent() { edit.normalAccent(); }

   void enableAdd(bool ok)
    {
     edit.alert(!ok);

     knob_add.enable(ok);
    }

   // drawing

   virtual void layout();

   // signals

   Signal<ulen> command_up;
   Signal<ulen> command_down;
   Signal<ulen> command_del;
   Signal<ulen> command_add;

   Signal<ulen> selected;

   Signal<ulen,bool> show_changed;
   Signal<ulen,bool> gray_changed;
   Signal<ulen,bool> name_changed;

   Signal<> &text_changed;
   Signal<> &text_paused;
 };

} // namespace App

#endif
