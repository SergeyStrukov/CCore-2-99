/* Application.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: DDLDisplay 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/Client.h>

#include <CCore/inc/video/ApplicationBase.h>
#include <CCore/inc/video/WindowReport.h>
#include <CCore/inc/video/FreeTypeFont.h>
#include <CCore/inc/video/Picture.h>

#include <CCore/inc/TaskMemStack.h>
#include <CCore/inc/Path.h>

namespace App {

/* classes */

class Preference;

struct Param;

class Application;

/* struct Param */

struct Param
 {
  Desktop *desktop = DefaultDesktop ;
  MSec tick_period = DeferCallQueue::DefaultTickPeriod ;

  UserPreference pref;

  const WindowReportConfig &report_cfg;
  const DragFrame::ConfigType &frame_cfg;
  const ExceptionWindow::ConfigType &exception_cfg;

  ClientWindow::ConfigType client_cfg;

  Param()
   : report_cfg(pref.getSmartConfig()),
     frame_cfg(pref.getSmartConfig()),
     exception_cfg(pref.getSmartConfig()),
     client_cfg(pref)
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
     main_frame.createMain(cmd_display,"DDLDisplay"_def);
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

  private:

   void pref_update()
    {
     client.updateCfg();

     main_frame.input.redrawAll(true);
    }

   SignalConnector<Application> connector_pref_update;

   void client_opened(StrLen file_name,bool ok)
    {
     if( ok )
       {
        SplitPath path(file_name);
        SplitName name(path.path);

        main_frame.setTitle(String("DDL file: ")+name.name);
       }
     else
       {
        main_frame.setTitle("DDLDisplay"_def);
       }
    }

   SignalConnector<Application,StrLen,bool> connector_client_opened;

  public:

   explicit Application(WindowReportBase &report,Param &param,CmdDisplay cmd_display_)
    : ApplicationBase(param.desktop,param.tick_period),
      cmd_display(cmd_display_),
      main_frame(param.desktop,param.frame_cfg),
      exception_client(main_frame,param.exception_cfg,report),
      client(main_frame,param.client_cfg),
      connector_pref_update(this,&Application::pref_update,param.pref.updated),
      connector_client_opened(this,&Application::client_opened,client.getOpened())
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


