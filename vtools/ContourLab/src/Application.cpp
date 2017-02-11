/* Application.cpp */
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

#include <inc/Client.h>

#include <CCore/inc/video/ApplicationBase.h>
#include <CCore/inc/video/WindowReport.h>
#include <CCore/inc/video/Picture.h>

#include <CCore/inc/TaskMemStack.h>

namespace App {

/* classes */

struct AppPreferenceBag;

class AppPreference;

struct Param;

class Application;

/* struct AppPreferenceBag */

struct AppPreferenceBag
 {
  // common

  MCoord arrow_size = Fraction(5) ;

  Coord shade_dxy = 8 ;

  VColor back    =     White ;
  VColor border  =     Black ;
  VColor face    =     Black ;
  VColor gray    =      Gray ;
  VColor text    =      Blue ;
  VColor hilight =     Green ;
  VColor focus   = OrangeRed ;

  // edit angle

  Coord edit_angle_dxy = 100 ;

  // edit length

  Coord edit_length_dx = 500 ;
  Coord edit_length_dy = 100 ;

  // edit ratio

  Coord edit_ratio_dx = 500 ;
  Coord edit_ratio_dy = 100 ;

  // edit formula

  VColor edit_text        =     Black ;
  VColor edit_error_text  =       Red ;
  VColor edit_number_text = DarkGreen ;
  VColor edit_length_text = DarkGreen ;
  VColor edit_angle_text  = DarkGreen ;
  VColor edit_name_text   =      Blue ;
  VColor edit_punct_text  =     Black ;

  unsigned edit_delay = 1_sectick ;

  // geometry

  Coord geometry_dxy = 300 ;

  DefString text_show = "show"_def ;
  DefString text_gray = "gray"_def ;
  DefString text_name = "name"_def ;

  DefString title_Ins = "Function list"_def ;

  // hints

  DefString hint_File = "The current file"_def ;
  DefString hint_Angle = "Set the selected angle"_def ;
  DefString hint_Length = "Set the selected length"_def ;
  DefString hint_Ratio = "Set the selected ratio"_def ;
  DefString hint_Geom = "Draw of elements, use to set the selected point"_def ;

  DefString hint_function_List = "Select a function to be inserted into the cursor position"_def ;

  DefString hint_item_List = "Geometry object list"_def ;
  DefString hint_item_Edit = "Enter a formula to create a new object"_def ;

  DefString hint_item_Down = "Move the object down in the list"_def ;
  DefString hint_item_Up   = "Move the object up in the list"_def ;
  DefString hint_item_Del  = "Delete the object"_def ;
  DefString hint_item_Add  = "Add a new object"_def ;
  DefString hint_item_Ins  = "Open the Function list window"_def ;

  DefString hint_item_Show = "Show this object"_def ;
  DefString hint_item_Gray = "Gray this object if not selected"_def ;
  DefString hint_item_Name = "Show the name of this object"_def ;

  // constructors

  AppPreferenceBag() noexcept {}
 };

/* class AppPreference */

class AppPreference : public ConfigBinder<AppPreferenceBag>
 {
  public:

   // constructors

   AppPreference() noexcept
    {
    }

   ~AppPreference()
    {
    }
 };

/* struct Param */

struct Param
 {
  Desktop *desktop = DefaultDesktop ;
  MSec tick_period = DeferCallQueue::DefaultTickPeriod ;

  UserPreference pref;
  AppPreference app_pref;

  const WindowReportConfig &report_cfg;
  const ExceptionWindow::ConfigType &exception_cfg;
  const DragFrame::ConfigType &frame_cfg;

  ClientWindow::ConfigType client_cfg;

  Param() noexcept
   : report_cfg(pref.getSmartConfig()),
     exception_cfg(pref.getSmartConfig()),
     frame_cfg(pref.getSmartConfig()),
     client_cfg(pref,app_pref)
   {
   }
 };

/* class Application */

class Application : public ApplicationBase
 {
   const CmdDisplay cmd_display;

   DragFrame main_frame;

   ExceptionClient exception_client;

   ClientWindow client;

  private:

   virtual void clearException() noexcept
    {
     exception_client.clear();
    }

   virtual void guardException()
    {
     exception_client.guard();
    }

   virtual void showException() noexcept
    {
     exception_client.show();
    }

   virtual void prepare()
    {
     main_frame.createMain(cmd_display,"ContourLab"_def);
    }

   virtual void beforeLoop() noexcept
    {
     exception_client.beforeLoop();
    }

   virtual void afterLoop() noexcept
    {
     exception_client.afterLoop();
    }

   virtual void final()
    {
     // do nothing
    }

  public:

   explicit Application(WindowReportBase &report,Param &param,CmdDisplay cmd_display_)
    : ApplicationBase(param.desktop,param.tick_period),
      cmd_display(cmd_display_),
      main_frame(param.desktop,param.frame_cfg,param.pref.updated),
      exception_client(main_frame,param.exception_cfg,report),
      client(main_frame,param.client_cfg)
    {
     main_frame.bindAlertClient(exception_client);
     main_frame.bindClient(client);
    }

   ~Application()
    {
    }
 };

/* Main() */

int Main(CmdDisplay cmd_display)
 {
  try
    {
     TaskMemStack tms(64_KByte);

     Param param;
     WindowReport report(param);

     SetAppIcon(DefaultAppIcon());

     param.pref.sync();

     Application app(report,param,cmd_display);

     return app.run();
    }
  catch(CatchType)
    {
     return 1;
    }
 }

} // namespace App


