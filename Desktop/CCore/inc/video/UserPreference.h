/* UserPreference.h */
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
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_video_UserPreference_h
#define CCore_inc_video_UserPreference_h

#include <CCore/inc/video/ConfigStore.h>

#include <CCore/inc/video/WindowLib.h>
#include <CCore/inc/video/ToolFrame.h>
#include <CCore/inc/video/WindowReport.h>
#include <CCore/inc/video/MessageFrame.h>
#include <CCore/inc/video/Menu.h>
#include <CCore/inc/video/FileFrame.h>

#include <CCore/inc/video/pref/SpinEdit.h>
#include <CCore/inc/video/pref/CoordEdit.h>

namespace CCore {
namespace Video {

/* classes */

struct FontCouple;

struct HomeSyncBase;

template <class Bag,class ... TT> class ConfigBinder;

struct ConfigItemBind;

struct UserPreferenceBag;

class UserPreference;

/* struct FontCouple */

struct FontCouple
 {
  Font font;
  FontParam param;

  FontCouple() noexcept {}

  void create() { font=param.create(); }
 };

/* struct HomeSyncBase */

struct HomeSyncBase
 {
  virtual void syncMap(ConfigMap &map)=0;

  virtual void updateMap(ConfigMap &map) const = 0;

  void syncHome(StrLen home_dir,StrLen cfg_file) noexcept; // "/dir" "/file"

  void updateHome(StrLen home_dir,StrLen cfg_file) noexcept; // "/dir" "/file"
 };

/* class ConfigBinder<Bag,TT> */

template <class Bag,class ... TT>
class ConfigBinder : NoCopyBase<Bag> , public HomeSyncBase
 {
   template <class T>
   struct Item
    {
     T obj;
    };

   struct Pack : Item<TT>...
    {
    };

   Pack pack;

  private:

   template <class T>
   auto bind(T &obj) -> decltype( obj.bind(0) )
    {
     return obj.bind(get());
    }

   template <class T>
   auto bind(T &obj) -> decltype( obj.bind(0,0) )
    {
     return obj.bind(get(),getSmartConfig());
    }

   template <class T>
   void bind()
    {
     bind( static_cast<Item<T> &>(pack).obj );
    }

  public:

   // constructors

   ConfigBinder() noexcept
    {
     ( bind<TT>() , ... );
    }

   ~ConfigBinder() {}

   // methods

   const Bag & get() const { return *this; }

   Bag & ref() { return *this; }

   template <OneOfTypes<TT...> T>
   const T & getConfig() const
    {
     return static_cast<const Item<T> &>(pack).obj;
    }

   virtual void syncMap(ConfigMap &map)
    {
     Bag::Members(this, [&map] (const char *name,AnyType &obj) { map.sync(name,obj); } );

     Bag::createFonts();
    }

   virtual void updateMap(ConfigMap &map) const
    {
     Bag::Members(this, [&map] (const char *name,AnyType &obj) { map.update(name,obj); } );
    }

   // getSmartConfig()

   class Proxy
    {
      const ConfigBinder *obj;

     public:

      Proxy(const ConfigBinder *obj_) : obj(obj_) {}

      template <OneOfTypes<TT...> T>
      operator const T & () const { return obj->template getConfig<T>(); }
    };

   Proxy getSmartConfig() const { return this; }
 };

/* struct ConfigItemBind */

struct ConfigItemBind
 {
  virtual void group(DefString name)=0;

  virtual void space()=0;

  virtual void item(DefString name,unsigned &var)=0;

  virtual void item(DefString name,Coord &var)=0;

  virtual void item(DefString name,MCoord &var)=0;

  virtual void item(DefString name,VColor &var)=0;

  virtual void item(DefString name,Clr &var)=0;

  virtual void item(DefString name,Point &var)=0;

  virtual void item(DefString name,DefString &var)=0;

  virtual void item(DefString name,FontCouple &var)=0;

  virtual void item(DefString name,bool &var)=0;

  virtual void item(DefString name,Ratio &var)=0;
 };

/* struct UserPreferenceBag */

struct UserPreferenceBag
 {
  // common

  MCoord width = Fraction(6,2) ;

  Coord space_dxy = 10 ;

  Coord check_dxy   = 20 ;
  Coord knob_dxy    = 30 ;
  Coord radio_dxy   = 20 ;
  Coord switch_dxy  = 30 ;
  Coord light_dxy   = 20 ;
  Coord scroll_dxy  = 20 ;
  Coord progress_dy = 24 ;

  VColor gray    =       Gray ;
  VColor grayUp  =      Green ;
  VColor snow    =       Snow ;
  VColor snowUp  =  PaleGreen ;

  VColor back     =    Silver ;
  VColor line     =      Gray ;
  VColor inactive =      Gray ;

  VColor border   =      Blue ;
  VColor focus    = OrangeRed ;

  VColor face     =     Black ;
  VColor faceUp   =      Blue ;
  VColor mark     =     Black ;
  VColor alert    =      Pink ;

  DefString text_Yes    = "Yes"_def ;
  DefString text_No     = "No"_def ;

  DefString text_Ok     = "Ok"_def ;
  DefString text_Cancel = "Cancel"_def ;

  DefString text_LoadFile = "Select a file to load from"_def ;
  DefString text_SaveFile = "Select a file to save to"_def ;

  DefString text_Alert   = "Alert"_def ;
  DefString text_AskSave = "Save modifications?"_def ;

  DefString text_Error   = "Error"_def ;
  DefString text_Warning = "Warning"_def ;

  DefString text_Close   = "Close"_def ;
  DefString text_Insert  = "Insert"_def ;

  // text

  Coord text_cursor_dx = 3 ;

  VColor text_select    = Yellow ;
  VColor text_cursor    =   Blue ;

  VColor label_text     =  Black ;
  VColor contour_text   =  Black ;

  VColor button_text    =  Black ;
  VColor message_text   =  Black ;
  VColor info_text      =  Black ;
  VColor line_edit_text =  Black ;
  VColor list_text      =  Black ;

  Point button_space    = Point(6,4) ;
  Point message_space   = Point(6,4) ;
  Point line_edit_space = Point(6,4) ;
  Point info_space      = Point(8,8) ;
  Point list_space      = Point(8,8) ;
  Point menu_space      = Point(4,4) ;

  FontCouple label_font;
  FontCouple contour_font;

  FontCouple button_font;
  FontCouple message_font;
  FontCouple info_font;
  FontCouple line_edit_font;
  FontCouple list_font;
  FontCouple menu_font;

  FontCouple code_font;

  // switch

  VColor switch_on  = Green ;
  VColor switch_off =   Red ;

  // progress

  unsigned progress_time   = 3_sectick ;
  unsigned progress_period =    2_tick ;

  VColor progress_border   =     Black ;
  VColor progress_grayUp   = DarkGreen ;
  VColor progress_snowUp   =     Green ;
  VColor progress_grayPing =     Black ;
  VColor progress_snowPing =      Snow ;

  // scroll

  unsigned scroll_speedUpPeriod = 12_tick ;

  // line_edit

  unsigned line_edit_period = 10_tick ;

  Coord line_edit_ex = 3 ;

  // scroll_list

  VColor scroll_list_title       = Navy ;
  VColor scroll_list_titleTop    = Aqua ;
  VColor scroll_list_titleBottom = Gray ;

  // menu

  VColor menu_back      =    Silver ;
  VColor menu_text      =     Black ;
  VColor menu_hilight   =      Blue ;
  VColor menu_select    = OrangeRed ;
  VColor menu_hot       =       Red ;

  bool menu_hotcolor = true ;

  // Frame

  unsigned blinkTime   = 3_sectick ;
  unsigned blinkPeriod =    3_tick ;

  Coord frame_dxy = 12 ;
  Coord title_dy  = 32 ;
  Coord btn_dx    = 26 ;
  Coord btn_dy    = 24 ;

  VColor frame             =      Snow ;
  VColor frameHilight      = PaleGreen ;
  VColor frameDrag         =      Pink ;

  VColor title             =     Black ;
  VColor titleActive       = RoyalBlue ;
  VColor titleInactive     =    Silver ;

  VColor drag              =    Silver ;
  VColor dragHilight       =     Green ;
  VColor dragActive        =       Red ;
  VColor dragSmall         =     Wheat ;

  VColor btnFace           = SteelBlue ;
  VColor btnFaceHilight    =     Green ;
  VColor btnPict           =     White ;
  VColor btnPictClose      =       Red ;
  VColor btnPictAlert      =       Red ;
  VColor btnPictNoAlert    =      Gray ;
  VColor btnPictCloseAlert =    Orange ;

  VColor hintBack   = Wheat ;
  VColor hintText   =  Blue ;
  VColor hintBorder = Green ;

  MCoord hintWidth = Fraction(3) ;

  FontCouple title_font;
  FontCouple hint_font;

  VColor shadeColor = Violet ;
  Clr    shadeAlpha =     64 ;

  DefString text_Fatal_error = "Fatal error"_def ;
  DefString text_No_hint     = "<No hint available>"_def ;

  DefString hint_ResizeTopLeft     = "Resize top-left"_def ;
  DefString hint_ResizeLeft        = "Resize left"_def ;
  DefString hint_ResizeBottomLeft  = "Resize bottom-left"_def ;
  DefString hint_ResizeBottom      = "Resize bottom"_def ;
  DefString hint_ResizeBottomRight = "Resize bottom-right"_def ;
  DefString hint_ResizeRight       = "Resize right"_def ;
  DefString hint_ResizeTopRight    = "Resize top-right"_def ;

  DefString hint_Alert    = "Open/close alert view"_def ;
  DefString hint_Help     = "Help on/off"_def ;
  DefString hint_Minimize = "Minimize"_def ;
  DefString hint_Maximize = "Maximize"_def ;
  DefString hint_Restore  = "Restore"_def ;
  DefString hint_Close    = "Close"_def ;

  // Exception

  VColor exception_back    = Black ;
  VColor exception_text    = Green ;
  VColor exception_divider =   Red ;

  // Message

  Coord message_knob_dxy = 50 ;

  Ratio message_pos_ry = Div(5,12) ;

  // File

  Coord file_alt_dxy = 40 ;

  VColor file_accent      = Black ;
  VColor file_filter_text = Black ;

  VColor file_faceRight = Green ;
  VColor file_faceDown  =   Red ;

  FontCouple file_filter_font;

  DefString text_New_file = "New file"_def ;

  DefString hint_FileHitList   = "Open/close the hit directory list"_def ;
  DefString hint_FileAddHit    = "Add the current directory to the hit list"_def ;
  DefString hint_FileUpdir     = "Goto the parent directory"_def ;
  DefString hint_FileCurdir    = "Current directory"_def ;
  DefString hint_FileDirList   = "Subdirectory list"_def ;
  DefString hint_FileList      = "File list"_def ;
  DefString hint_FileMakeDir   = "Create a new directory"_def ;
  DefString hint_FileRemoveDir = "Delete the selected directory"_def ;
  DefString hint_FileAlt       = "Choose between a new file or an existing file"_def ;

  DefString hint_FileEnableFilter = "Enable/disable this filter"_def ;
  DefString hint_FileDelFilter    = "Delete this filter"_def ;
  DefString hint_FileFilter       = "Filename filter, use * or ?"_def ;

  DefString hint_FileAddFilter = "Add a filter"_def ;

  // CoordEdit

  MCoord coord_edit_width = Fraction(1) ;
  Coord coord_edit_mark_dy = 3 ;

  VColor coord_edit_line = Black ;

  // constructors

  UserPreferenceBag() noexcept {}

  // methods

  template <class Ptr,class Func>
  static void Members(Ptr ptr,Func func);

  void bind(ConfigItemBind &binder);

  void createFonts();
 };

/* class UserPreference */

class UserPreference : public ConfigBinder<UserPreferenceBag, // Update here

                                           ButtonWindow::ConfigType,
                                           KnobWindow::ConfigType,
                                           CheckWindow::ConfigType,
                                           ContourWindow::ConfigType,
                                           TextContourWindow::ConfigType,
                                           InfoWindow::ConfigType,
                                           LabelWindow::ConfigType,
                                           LightWindow::ConfigType,
                                           XSingleLineWindow::ConfigType,
                                           XDoubleLineWindow::ConfigType,
                                           LineEditWindow::ConfigType,
                                           ProgressWindow::ConfigType,
                                           RadioWindow::ConfigType,
                                           XScrollWindow::ConfigType,
                                           SimpleTextListWindow::ConfigType,
                                           SwitchWindow::ConfigType,
                                           TextWindow::ConfigType,
                                           TextLineWindow::ConfigType,
                                           ScrollListWindow::ConfigType,
                                           XSplitWindow::ConfigType,

                                           DragFrame::ConfigType,
                                           FixedFrame::ConfigType,
                                           ToolFrame::ConfigType,

                                           WindowReportConfig,
                                           ExceptionWindow::ConfigType,

                                           MessageWindow::ConfigType,
                                           MessageFrame::ConfigType,
                                           MessageWindow::AlertConfigType,
                                           MessageFrame::AlertConfigType,

                                           SimpleTopMenuWindow::ConfigType,
                                           SimpleCascadeMenu::ConfigType,

                                           DirEditWindow::ConfigType,
                                           FileFilterListWindow::ConfigType,
                                           FileCheckShape::Config,
                                           FileWindow::ConfigType,
                                           FileFrame::ConfigType,

                                           SpinEditWindow::ConfigType,
                                           CoordEditWindow::ConfigType
                                          >
 {
   static StrLen PrefFile();

  public:

   // constructors

   UserPreference() noexcept;

   ~UserPreference();

   // methods

   void sync() noexcept;

   void update() noexcept;

   // signals

   mutable Signal<> updated;
 };

} // namespace Video
} // namespace CCore

#endif

