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
#include <CCore/inc/video/MessageWindow.h>
#include <CCore/inc/video/WindowReport.h>
#include <CCore/inc/video/WindowLib.h>

namespace CCore {
namespace Video {

/* classes */

struct UserPreferenceBag;

class UserPreference;

/* struct UserPreferenceBag */

struct UserPreferenceBag
 {
  // common

  Coord space_dxy      = 10 ;
  Coord text_cursor_dx =  3 ;

  Coord check_dxy   = 20 ;
  Coord knob_dxy    = 30 ;
  Coord swtch_dxy   = 30 ;
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

  Font label_font;
  Font contour_font;

  Font button_font;
  Font message_font;
  Font info_font;
  Font line_edit_font;
  Font list_font;

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

  Font title_font;

  // ExceptionWindow

  VColor exw_back    = Black ;
  VColor exw_text    = Green ;
  VColor exw_divider =   Red ;

  // font params

  FontParam label_font_param;
  FontParam contour_font_param;
  FontParam message_font_param;
  FontParam info_font_param;
  FontParam line_edit_font_param;
  FontParam list_font_param;
  FontParam button_font_param;
  FontParam title_font_param;

  // constructors

  UserPreferenceBag() noexcept {}

  // methods

  void sync(ConfigMap &map);

  void update(ConfigMap &map) const;

  struct Bind
   {
    virtual void group(StrLen name)=0;

    virtual void space()=0;

    virtual void item(StrLen name,Coord &var)=0;

    virtual void item(StrLen name,MCoord &var)=0;

    virtual void item(StrLen name,VColor &var)=0;

    virtual void item(StrLen name,unsigned &var)=0;

    virtual void item(StrLen name,DefString &var)=0;

    virtual void item(StrLen name,Point &var)=0;

    virtual void item(StrLen name,FontParam &var,Font &font)=0;
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
   MessageSubWindow::ConfigType cfg_MessageSubWindow;
   MessageWindow::ConfigType cfg_MessageWindow;
   ExceptionWindow::ConfigType cfg_ExceptionWindow;

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

   const MessageWindow::ConfigType & getMessageWindowConfig() const { return cfg_MessageWindow; }

   const ExceptionWindow::ConfigType & getExceptionWindowConfig() const { return cfg_ExceptionWindow; }

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
 };

} // namespace Video
} // namespace CCore

#endif

