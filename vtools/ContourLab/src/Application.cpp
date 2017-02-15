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
#include <CCore/inc/video/ConfigEditor.h>
#include <CCore/inc/video/Picture.h>

#include <CCore/inc/TaskMemStack.h>

namespace App {

/* classes */

struct AppPreferenceBag;

class AppPreference;

struct Param;

class Application;

/* struct AppPreferenceBag */

struct AppPreferenceBag : ConfigItemHost
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

  // menu

  DefString menu_File    = "@File"_def ;
  DefString menu_Options = "@Options"_def ;
  DefString menu_New     = "@New"_def ;
  DefString menu_Open    = "@Open"_def ;
  DefString menu_Save    = "@Save"_def ;
  DefString menu_SaveAs  = "Save @as"_def ;
  DefString menu_Exit    = "E@xit"_def ;
  DefString menu_Global  = "@Global"_def ;
  DefString menu_App     = "@Application"_def ;

  // hints

  DefString hint_File   = "The current file"_def ;
  DefString hint_Angle  = "Set the selected angle"_def ;
  DefString hint_Length = "Set the selected length"_def ;
  DefString hint_Ratio  = "Set the selected ratio"_def ;
  DefString hint_Geom   = "Draw of elements, use to set the selected point"_def ;

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

  // methods

  template <class Ptr,class Func>
  static void Members(Ptr ptr,Func func);

  virtual void bind(ConfigItemBind &binder);

  void createFonts();
 };

template <class Ptr,class Func>
void AppPreferenceBag::Members(Ptr ptr,Func func) // TODO
 {
  func("arrow_size",ptr->arrow_size);

  func("shade_dxy",ptr->shade_dxy);

  func("back",ptr->back);
  func("border",ptr->border);
  func("face",ptr->face);
  func("gray",ptr->gray);
  func("vtext",ptr->text);
  func("hilight",ptr->hilight);
  func("focus",ptr->focus);

  func("edit_angle_dxy",ptr->edit_angle_dxy);

  func("edit_length_dx",ptr->edit_length_dx);
  func("edit_length_dy",ptr->edit_length_dy);

  func("edit_ratio_dx",ptr->edit_ratio_dx);
  func("edit_ratio_dy",ptr->edit_ratio_dy);

  func("edit_text",ptr->edit_text);
  func("edit_error_text",ptr->edit_error_text);
  func("edit_number_text",ptr->edit_number_text);
  func("edit_length_text",ptr->edit_length_text);
  func("edit_angle_text",ptr->edit_angle_text);
  func("edit_name_text",ptr->edit_name_text);
  func("edit_punct_text",ptr->edit_punct_text);

  func("edit_delay",ptr->edit_delay);

  func("geometry_dxy",ptr->geometry_dxy);
  func("text_show",ptr->text_show);
  func("text_gray",ptr->text_gray);
  func("text_name",ptr->text_name);
  func("title_Ins",ptr->title_Ins);

  func("hint_File",ptr->hint_File);
  func("hint_Angle",ptr->hint_Angle);
  func("hint_Length",ptr->hint_Length);
  func("hint_Ratio",ptr->hint_Ratio);
  func("hint_Geom",ptr->hint_Geom);

  func("hint_function_List",ptr->hint_function_List);

  func("hint_item_List",ptr->hint_item_List);
  func("hint_item_Edit",ptr->hint_item_Edit);

  func("hint_item_Down",ptr->hint_item_Down);
  func("hint_item_Up",ptr->hint_item_Up);
  func("hint_item_Del",ptr->hint_item_Del);
  func("hint_item_Add",ptr->hint_item_Add);
  func("hint_item_Ins",ptr->hint_item_Ins);

  func("hint_item_Show",ptr->hint_item_Show);
  func("hint_item_Gray",ptr->hint_item_Gray);
  func("hint_item_Name",ptr->hint_item_Name);
 }

void AppPreferenceBag::bind(ConfigItemBind &binder) // TODO
 {
  binder.group("Common"_def);

    binder.item("Arrow size"_def,arrow_size);
    binder.item("Shade length"_def,shade_dxy);
    binder.space();
    binder.item("Back"_def,back);
 }

void AppPreferenceBag::createFonts()
 {
 }

/* class AppPreference */

class AppPreference : public ConfigBinder<AppPreferenceBag>
 {
   static StrLen Key() { return "/5C73F691B5ED963C3580A06131FD2D6EC57ECE3349028B3986554636EC1A70B8"_c;  }

   static StrLen File() { return "/AppPreference.ddl"_c;  }

  public:

   // constructors

   AppPreference() noexcept
    {
    }

   ~AppPreference()
    {
    }

   // methods

   void sync() noexcept
    {
     syncHome(Key(),File());
    }

   void update() noexcept
    {
     updateHome(Key(),File());
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
   Param &param;

   const CmdDisplay cmd_display;

   DragFrame main_frame;

   ExceptionClient exception_client;

   ClientWindow client;

   ConfigEditorFrame app_frame;

  private:

   void userPref(Point base) // TODO
    {
     Used(base);
    }

   void appPref(Point base)
    {
     if( app_frame.isDead() )
       {
        app_frame.create(main_frame.getFrame(),base,param.pref.get().title_AppPref);
       }
    }

   SignalConnector<Application,Point> connector_userPref;
   SignalConnector<Application,Point> connector_appPref;

   void appUpdated()
    {
     param.pref.updated.assert();
    }

   void appSave()
    {
     param.app_pref.update();
    }

   SignalConnector<Application> connector_app_updated;
   SignalConnector<Application> connector_app_save;

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

   explicit Application(WindowReportBase &report,Param &param_,CmdDisplay cmd_display_)
    : ApplicationBase(param_.desktop,param_.tick_period),
      param(param_),
      cmd_display(cmd_display_),
      main_frame(param.desktop,param.frame_cfg,param.pref.updated),
      exception_client(main_frame,param.exception_cfg,report),
      client(main_frame,param.client_cfg),
      app_frame(param.desktop,param.pref.getSmartConfig(),false),

      connector_userPref(this,&Application::userPref,client.doUserPref),
      connector_appPref(this,&Application::appPref,client.doAppPref),
      connector_app_updated(this,&Application::appUpdated,app_frame.updated),
      connector_app_save(this,&Application::appSave,app_frame.doSave)
    {
     main_frame.bindAlertClient(exception_client);
     main_frame.bindClient(client);

     app_frame.bindConfig(param.app_pref.ref());
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
     param.app_pref.sync();

     Application app(report,param,cmd_display);

     return app.run();
    }
  catch(CatchType)
    {
     return 1;
    }
 }

} // namespace App


