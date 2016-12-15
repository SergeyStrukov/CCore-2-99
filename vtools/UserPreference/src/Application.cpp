/* Application.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: UserPreference 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/Designer.h>

#include <CCore/inc/video/ApplicationBase.h>
#include <CCore/inc/video/WindowReport.h>
#include <CCore/inc/video/FreeTypeFont.h>
#include <CCore/inc/video/Picture.h>

#include <CCore/inc/TaskMemStack.h>

namespace App {

/* classes */

struct Param;

class Application;

/* struct Param */

struct Param
 {
  Desktop *desktop = DefaultDesktop ;
  MSec tick_period = DeferCallQueue::DefaultTickPeriod ;

  Preference pref;

  const DragWindow::ConfigType &drag_cfg;
  const ExceptionWindow::ConfigType &exception_cfg;

  DesignerWindow::ConfigType designer_cfg;

  Param()
   : drag_cfg(pref.getDragWindowConfig()),
     exception_cfg(pref.getExceptionWindowConfig()),
     designer_cfg(pref)
   {
   }
 };

/* class Application */

class Application : public ApplicationBase
 {
   const CmdDisplay cmd_display;

   DragWindow main_win;

   ExceptionClient exception_client;
   DesignerWindow client;

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
     PlaceFrame place(desktop);

     main_win.createMain(cmd_display,
                         place.getPane(Div(1,8),Div(1,2),Div(1,8),Div(3,4)),
                         place.getMaxSize(),
                         "User Preference"_def);
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

   void prefUpdate()
    {
     main_win.redrawAll(true);
    }

   SignalConnector<Application> connector_pref_update;

  public:

   explicit Application(WindowReportBase &report,Param &param,CmdDisplay cmd_display_)
    : ApplicationBase(param.desktop,param.tick_period),
      cmd_display(cmd_display_),
      main_win(param.desktop,param.drag_cfg),
      exception_client(main_win,param.exception_cfg,report),
      client(main_win,param.designer_cfg,param.pref),
      connector_pref_update(this,&Application::prefUpdate,param.pref.update)
    {
     main_win.bindAlertClient(exception_client);
     main_win.bindClient(client);
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

     Application app(report,param,cmd_display);

     return app.run();
    }
  catch(CatchType)
    {
     ErrorMsgBox("Cannot start application: no memory","Fatal error");

     return 1;
    }
 }

} // namespace App


