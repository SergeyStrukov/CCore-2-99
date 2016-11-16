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

#include <CCore/inc/video/DragWindow.h>
#include <CCore/inc/video/FixedWindow.h>
#include <CCore/inc/video/ToolWindow.h>
#include <CCore/inc/video/MessageWindow.h>
#include <CCore/inc/video/WindowReport.h>
#include <CCore/inc/video/WindowLib.h>
#include <CCore/inc/video/Menu.h>

namespace CCore {
namespace Video {

/* classes */

struct FontCouple;

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

/* struct UserPreferenceBag */

struct UserPreferenceBag
 {
  // common

  Coord space_dxy      = 10 ;
  Coord text_cursor_dx =  3 ;

  Coord check_dxy   = 20 ;
  Coord knob_dxy    = 30 ;
  Coord radio_dxy   = 20 ;
  Coord switch_dxy  = 30 ;
  Coord light_dxy   = 20 ;
  Coord scroll_dxy  = 20 ;
  Coord progress_dy = 24 ;

  MCoord width = Fraction(6,2) ;

  VColor ground   =    Silver ;
  VColor back     =    Silver ;
  VColor line     =      Gray ;
  VColor inactive =      Gray ;

  VColor bottom   =      Gray ;
  VColor bottomUp =     Green ;
  VColor top      =      Snow ;
  VColor topUp    =     Green ;

  VColor border   =      Blue ;
  VColor focus    = OrangeRed ;

  VColor face     =     Black ;
  VColor faceUp   =      Blue ;
  VColor mark     =     Black ;

  // text

  VColor text_select    =    Yellow ;
  VColor text_cursor    =      Blue ;

  VColor label_text     =     Black ;
  VColor contour_text   =     Black ;

  VColor button_text    =     Black ;
  VColor message_text   =     Black ;
  VColor info_text      =     Black ;
  VColor line_edit_text =     Black ;
  VColor list_text      =     Black ;

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

  // menu

  VColor menu_back      =    Silver ;
  VColor menu_text      =     Black ;
  VColor menu_hilight   =      Blue ;
  VColor menu_select    = OrangeRed ;
  VColor menu_hot       =       Red ;

  bool use_hotcolor = true ;

  // scroll list

  VColor scroll_list_title        = Navy ;
  VColor scroll_list_title_top    = Aqua ;
  VColor scroll_list_title_bottom = Gray ;

  // other

  unsigned line_edit_period     =   10_tick ;
  unsigned scroll_speedUpPeriod =   12_tick ;
  unsigned progress_time        = 3_sectick ;
  unsigned progress_period      =    2_tick ;

  Coord line_edit_ex        = 3 ;

  VColor switch_on        =  Green ;
  VColor switch_off       =    Red ;

  VColor progress_border        =     Black ;
  VColor progress_ping_top      =      Snow ;
  VColor progress_ping_bottom   =     Black ;
  VColor progress_active_top    =     Green ;
  VColor progress_active_bottom = DarkGreen ;

  // TopWindow

  unsigned blink_time   = 3_sectick ;
  unsigned blink_period =    3_tick ;

  DefString fatal_error = "Fatal error"_def ;

  Coord frame_dxy = 12 ;
  Coord title_dy  = 32 ;
  Coord btn_dx    = 26 ;
  Coord btn_dy    = 24 ;

  VColor frame             =      Snow ;
  VColor active_frame      = RoyalBlue ;
  VColor inactive_frame    =    Silver ;
  VColor frameHilight      = PaleGreen ;
  VColor frameDrag         =      Pink ;
  VColor title             =     Black ;

  VColor drag              =    Silver ;
  VColor dragHilight       =     Green ;
  VColor dragActive        =       Red ;

  VColor btnFace           = SteelBlue ;
  VColor btnFaceHilight    =     Green ;
  VColor btnPict           =     White ;
  VColor btnPictClose      =       Red ;
  VColor btnPictAlert      =       Red ;
  VColor btnPictNoAlert    =      Gray ;
  VColor btnPictCloseAlert =    Orange ;

  FontCouple title_font;

  VColor shade_color = Violet ;
  Clr    shade_alpha =     64 ;

  // ExceptionWindow

  VColor exw_back    = Black ;
  VColor exw_text    = Green ;
  VColor exw_divider =   Red ;

  // MessageWindow

  Coord msgw_knob_dxy = 50 ;

  // constructors

  UserPreferenceBag() noexcept {}

  // methods

  template <class Ptr,class Func>
  static void Members(Ptr ptr,Func func);

  void sync(ConfigMap &map);

  void update(ConfigMap &map) const;

  struct Bind
   {
    virtual void group(DefString name)=0;

    virtual void space()=0;

    virtual void item(DefString name,Coord &var)=0;

    virtual void item(DefString name,MCoord &var)=0;

    virtual void item(DefString name,VColor &var)=0;

    virtual void item(DefString name,Clr &var)=0;

    virtual void item(DefString name,unsigned &var)=0;

    virtual void item(DefString name,DefString &var)=0;

    virtual void item(DefString name,Point &var)=0;

    virtual void item(DefString name,FontCouple &var)=0;

    virtual void item(DefString name,bool &var)=0;
   };

  void bind(Bind &binder);

  void createFonts();
 };

/* class UserPreference */

class UserPreference : NoCopyBase<UserPreferenceBag>
 {
   static const char *const PrefFile;

   // main windows

   DragWindow::ConfigType cfg_DragWindow;
   FixedWindow::ConfigType cfg_FixedWindow;
   ToolWindow::ConfigType cfg_ToolWindow;
   MessageSubWindow::ConfigType cfg_MessageSubWindow;
   MessageWindow::ConfigType cfg_MessageWindow;
   ExceptionWindow::ConfigType cfg_ExceptionWindow;
   SimpleCascadeMenu::ConfigType cfg_SimpleCascadeMenu;

   // sub windows

   ButtonWindow::ConfigType cfg_ButtonWindow;
   KnobWindow::ConfigType cfg_KnobWindow;
   CheckWindow::ConfigType cfg_CheckWindow;
   SwitchWindow::ConfigType cfg_SwitchWindow;
   RadioWindow::ConfigType cfg_RadioWindow;
   TextWindow::ConfigType cfg_TextWindow;
   LabelWindow::ConfigType cfg_LabelWindow;
   XSingleLineWindow::ConfigType cfg_XSingleLineWindow;
   YSingleLineWindow::ConfigType cfg_YSingleLineWindow;
   XDoubleLineWindow::ConfigType cfg_XDoubleLineWindow;
   YDoubleLineWindow::ConfigType cfg_YDoubleLineWindow;
   ContourWindow::ConfigType cfg_ContourWindow;
   TextContourWindow::ConfigType cfg_TextContourWindow;
   LightWindow::ConfigType cfg_LightWindow;
   XScrollWindow::ConfigType cfg_XScrollWindow;
   YScrollWindow::ConfigType cfg_YScrollWindow;
   ProgressWindow::ConfigType cfg_ProgressWindow;
   InfoWindow::ConfigType cfg_InfoWindow;
   LineEditWindow::ConfigType cfg_LineEditWindow;
   SimpleTextListWindow::ConfigType cfg_SimpleTextListWindow;
   SimpleTopMenuWindow::ConfigType cfg_SimpleTopMenuWindow;
   SimpleCascadeMenuWindow::ConfigType cfg_SimpleCascadeMenuWindow;
   ScrollListWindow::ConfigType cfg_ScrollListWindow;

  public:

   // constructors

   UserPreference() noexcept;

   ~UserPreference();

   // methods

   const UserPreferenceBag & get() const { return *this; }

   UserPreferenceBag & take() { return *this; }

   void sync() noexcept;

   void update() noexcept;

   // main windows

   const DragWindow::ConfigType & getDragWindowConfig() const { return cfg_DragWindow; }

   const FixedWindow::ConfigType & getFixedWindowConfig() const { return cfg_FixedWindow; }

   const ToolWindow::ConfigType & getToolWindowConfig() const { return cfg_ToolWindow; }

   const MessageWindow::ConfigType & getMessageWindowConfig() const { return cfg_MessageWindow; }

   const ExceptionWindow::ConfigType & getExceptionWindowConfig() const { return cfg_ExceptionWindow; }

   const SimpleCascadeMenu::ConfigType & getSimpleCascadeMenu() const { return cfg_SimpleCascadeMenu; }

   // sub windows

   const ButtonWindow::ConfigType & getButtonConfig() const { return cfg_ButtonWindow; }

   const KnobWindow::ConfigType & getKnobConfig() const { return cfg_KnobWindow; }

   const CheckWindow::ConfigType & getCheckConfig() const { return cfg_CheckWindow; }

   const SwitchWindow::ConfigType & getSwitchConfig() const { return cfg_SwitchWindow; }

   const RadioWindow::ConfigType & getRadioConfig() const { return cfg_RadioWindow; }

   const TextWindow::ConfigType & getTextConfig() const { return cfg_TextWindow; }

   const LabelWindow::ConfigType & getLabelConfig() const { return cfg_LabelWindow; }

   const XSingleLineWindow::ConfigType & getXSingleLineConfig() const { return cfg_XSingleLineWindow; }

   const YSingleLineWindow::ConfigType & getYSingleLineConfig() const { return cfg_YSingleLineWindow; }

   const XDoubleLineWindow::ConfigType & getXDoubleLineConfig() const { return cfg_XDoubleLineWindow; }

   const YDoubleLineWindow::ConfigType & getYDoubleLineConfig() const { return cfg_YDoubleLineWindow; }

   const ContourWindow::ConfigType & getContourConfig() const { return cfg_ContourWindow; }

   const TextContourWindow::ConfigType & getTextContourConfig() const { return cfg_TextContourWindow; }

   const LightWindow::ConfigType & getLightConfig() const { return cfg_LightWindow; }

   const XScrollWindow::ConfigType & getXScrollConfig() const { return cfg_XScrollWindow; }

   const YScrollWindow::ConfigType & getYScrollConfig() const { return cfg_YScrollWindow; }

   const ProgressWindow::ConfigType & getProgressConfig() const { return cfg_ProgressWindow; }

   const InfoWindow::ConfigType & getInfoConfig() const { return cfg_InfoWindow; }

   const LineEditWindow::ConfigType & getLineEditConfig() const { return cfg_LineEditWindow; }

   const SimpleTextListWindow::ConfigType & getSimpleTextListConfig() const { return cfg_SimpleTextListWindow; }

   const SimpleTopMenuWindow::ConfigType & getSimpleTopMenuConfig() const { return cfg_SimpleTopMenuWindow; }

   const SimpleCascadeMenuWindow::ConfigType & getSimpleCascadeMenuConfig() const { return cfg_SimpleCascadeMenuWindow; }

   const ScrollListWindow::ConfigType & getScrollListWindowConfig() const { return cfg_ScrollListWindow; }

   // getSmartConfig()

   class Proxy
    {
      const UserPreference *obj;

     public:

      Proxy(const UserPreference *obj_) : obj(obj_) {}

      // main windows

      operator const auto & () const { return obj->cfg_DragWindow; }

      operator const auto & () const { return obj->cfg_FixedWindow; }

      operator const auto & () const { return obj->cfg_ToolWindow; }

      operator const auto & () const { return obj->cfg_MessageWindow; }

      operator const auto & () const { return obj->cfg_ExceptionWindow; }

      operator const auto & () const { return obj->cfg_SimpleCascadeMenu; }

      // sub windows

      operator const auto & () const { return obj->cfg_ButtonWindow; }

      operator const auto & () const { return obj->cfg_KnobWindow; }

      operator const auto & () const { return obj->cfg_CheckWindow; }

      operator const auto & () const { return obj->cfg_SwitchWindow; }

      operator const auto & () const { return obj->cfg_RadioWindow; }

      operator const auto & () const { return obj->cfg_TextWindow; }

      operator const auto & () const { return obj->cfg_LabelWindow; }

      operator const auto & () const { return obj->cfg_XSingleLineWindow; }

      operator const auto & () const { return obj->cfg_YSingleLineWindow; }

      operator const auto & () const { return obj->cfg_XDoubleLineWindow; }

      operator const auto & () const { return obj->cfg_YDoubleLineWindow; }

      operator const auto & () const { return obj->cfg_ContourWindow; }

      operator const auto & () const { return obj->cfg_TextContourWindow; }

      operator const auto & () const { return obj->cfg_LightWindow; }

      operator const auto & () const { return obj->cfg_XScrollWindow; }

      operator const auto & () const { return obj->cfg_YScrollWindow; }

      operator const auto & () const { return obj->cfg_ProgressWindow; }

      operator const auto & () const { return obj->cfg_InfoWindow; }

      operator const auto & () const { return obj->cfg_LineEditWindow; }

      operator const auto & () const { return obj->cfg_SimpleTextListWindow; }

      operator const auto & () const { return obj->cfg_SimpleTopMenuWindow; }

      operator const auto & () const { return obj->cfg_SimpleCascadeMenuWindow; }

      operator const auto & () const { return obj->cfg_ScrollListWindow; }
    };

   Proxy getSmartConfig() const { return this; }
 };

} // namespace Video
} // namespace CCore

#endif

