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

  DefString text_Ok     = "Ok"_def ;
  DefString text_Cancel = "Cancel"_def ;

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

  // Frames

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

  // ExceptionWindow

  VColor exception_back    = Black ;
  VColor exception_text    = Green ;
  VColor exception_divider =   Red ;

  // MessageWindow

  Coord message_knob_dxy = 50 ;

  Ratio message_pos_ry = Div(5,12) ; // +

  // FileWindow

  Coord file_alt_dxy = 40 ;

  VColor file_accent      = Black ;
  VColor file_filter_text = Black ;

  VColor file_faceRight = Green ;
  VColor file_faceDown  =   Red ;

  FontCouple file_filter_font;

  DefString text_New_file = "New file"_def ;

  DefString hint_FileHitList = "Open/close the hit directory list"_def ;
  DefString hint_FileAddHit  = "Add the current directory to the hit list"_def ;
  DefString hint_FileUpdir   = "Goto the parent directory"_def ;
  DefString hint_FileCurdir  = "Current directory"_def ;
  DefString hint_FileDirList = "Subdirectory list"_def ;
  DefString hint_FileList    = "File list"_def ;
  DefString hint_FileAlt     = "Choose between a new file or an existing file"_def ;

  DefString hint_FileEnableFilter = "Enable/disable this filter"_def ;
  DefString hint_FileDelFilter    = "Delete this filter"_def ;
  DefString hint_FileFilter       = "Filename filter, use * or ?"_def ;

  DefString hint_FileAddFilter = "Add a filter"_def ;

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

   ToolFrame::ConfigType cfg_ToolFrame;
   MessageWindow::ConfigType cfg_MessageWindow;
   MessageFrame::ConfigType cfg_MessageFrame;
   MessageWindow::ConfigType cfg_AlertMessageWindow;
   MessageFrame::ConfigType cfg_AlertMessageFrame;
   ExceptionWindow::ConfigType cfg_ExceptionWindow;
   SimpleCascadeMenu::ConfigType cfg_SimpleCascadeMenu;

   DragFrame::ConfigType cfg_DragFrame;
   FixedFrame::ConfigType cfg_FixedFrame;

   WindowReportConfig cfg_WindowReport;

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
   TextLineWindow::ConfigType cfg_TextLineWindow;
   XScrollWindow::ConfigType cfg_XScrollWindow;
   YScrollWindow::ConfigType cfg_YScrollWindow;
   ProgressWindow::ConfigType cfg_ProgressWindow;
   InfoWindow::ConfigType cfg_InfoWindow;
   LineEditWindow::ConfigType cfg_LineEditWindow;
   SimpleTextListWindow::ConfigType cfg_SimpleTextListWindow;
   SimpleTopMenuWindow::ConfigType cfg_SimpleTopMenuWindow;
   SimpleCascadeMenuWindow::ConfigType cfg_SimpleCascadeMenuWindow;
   ScrollListWindow::ConfigType cfg_ScrollListWindow;

   // FileWindow

   DirEditWindow::ConfigType cfg_DirEditWindow;
   FileFilterListWindow::ConfigType cfg_FileFilterListWindow;
   FileCheckShape::Config cfg_FileCheckShape;
   FileWindow::ConfigType cfg_FileWindow;
   FileFrame::ConfigType cfg_FileFrame;

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

   const ToolFrame::ConfigType & getToolWindowConfig() const { return cfg_ToolFrame; }

   const MessageWindow::ConfigType & getMessageWindowConfig() const { return cfg_MessageWindow; }

   const MessageFrame::ConfigType & getMessageFrameConfig() const { return cfg_MessageFrame; }

   const MessageWindow::ConfigType & getAlertMessageWindowConfig() const { return cfg_AlertMessageWindow; }

   const MessageFrame::ConfigType & getAlertMessageFrameConfig() const { return cfg_AlertMessageFrame; }

   const ExceptionWindow::ConfigType & getExceptionWindowConfig() const { return cfg_ExceptionWindow; }

   //const SimpleCascadeMenu::ConfigType & getSimpleCascadeMenuConfig() const { return cfg_SimpleCascadeMenu; }

   const DragFrame::ConfigType & getDragFrameConfig() const { return cfg_DragFrame; }

   const FixedFrame::ConfigType & getFixedFrameConfig() const { return cfg_FixedFrame; }

   const WindowReportConfig & getWindowReportConfig() const { return cfg_WindowReport; }

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

   const TextLineWindow::ConfigType & getTextLineConfig() const { return cfg_TextLineWindow; }

   const XScrollWindow::ConfigType & getXScrollConfig() const { return cfg_XScrollWindow; }

   const YScrollWindow::ConfigType & getYScrollConfig() const { return cfg_YScrollWindow; }

   const ProgressWindow::ConfigType & getProgressConfig() const { return cfg_ProgressWindow; }

   const InfoWindow::ConfigType & getInfoConfig() const { return cfg_InfoWindow; }

   const LineEditWindow::ConfigType & getLineEditConfig() const { return cfg_LineEditWindow; }

   const SimpleTextListWindow::ConfigType & getSimpleTextListConfig() const { return cfg_SimpleTextListWindow; }

   const SimpleTopMenuWindow::ConfigType & getSimpleTopMenuConfig() const { return cfg_SimpleTopMenuWindow; }

   const SimpleCascadeMenuWindow::ConfigType & getSimpleCascadeMenuConfig() const { return cfg_SimpleCascadeMenuWindow; }

   const ScrollListWindow::ConfigType & getScrollListConfig() const { return cfg_ScrollListWindow; }

   // FileWindow

   const DirEditWindow::ConfigType & getDirEditConfig() const { return cfg_DirEditWindow; }

   const FileFilterListWindow::ConfigType & getFileFilterListConfig() const { return cfg_FileFilterListWindow; }

   const FileCheckShape::Config & getFileAltConfig() const { return cfg_FileCheckShape; }

   const FileWindow::ConfigType & getFileSubConfig() const { return cfg_FileWindow; }

   const FileFrame::ConfigType & getFileFrameConfig() const { return cfg_FileFrame; }

   // getSmartConfig()

   class Proxy
    {
      const UserPreference *obj;

     public:

      Proxy(const UserPreference *obj_) : obj(obj_) {}

      // main windows

      operator const auto & () const { return obj->cfg_ToolFrame; }

      operator const auto & () const { return obj->cfg_MessageWindow; }

      operator const auto & () const { return obj->cfg_MessageFrame; }

      operator const auto & () const { return obj->cfg_ExceptionWindow; }

      operator const auto & () const { return obj->cfg_SimpleCascadeMenu; }

      operator const auto & () const { return obj->cfg_DragFrame; }

      operator const auto & () const { return obj->cfg_FixedFrame; }

      operator const auto & () const { return obj->cfg_WindowReport; }

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

      operator const auto & () const { return obj->cfg_TextLineWindow; }

      operator const auto & () const { return obj->cfg_XScrollWindow; }

      operator const auto & () const { return obj->cfg_YScrollWindow; }

      operator const auto & () const { return obj->cfg_ProgressWindow; }

      operator const auto & () const { return obj->cfg_InfoWindow; }

      operator const auto & () const { return obj->cfg_LineEditWindow; }

      operator const auto & () const { return obj->cfg_SimpleTextListWindow; }

      operator const auto & () const { return obj->cfg_SimpleTopMenuWindow; }

      operator const auto & () const { return obj->cfg_SimpleCascadeMenuWindow; }

      operator const auto & () const { return obj->cfg_ScrollListWindow; }

      // FileFrame

      operator const auto & () const { return obj->cfg_DirEditWindow; }

      operator const auto & () const { return obj->cfg_FileFilterListWindow; }

      operator const auto & () const { return obj->cfg_FileCheckShape; }

      operator const auto & () const { return obj->cfg_FileWindow; }

      operator const auto & () const { return obj->cfg_FileFrame; }
    };

   Proxy getSmartConfig() const { return this; }

   // signals

   Signal<> updated;
 };

} // namespace Video
} // namespace CCore

#endif

