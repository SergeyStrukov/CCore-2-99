/* Application.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: ShapeLab 1.00
//
//  Tag: VideoLabs
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2015 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/ShapeLab.h>

#include <CCore/inc/video/ApplicationBase.h>
#include <CCore/inc/video/WindowReport.h>

#include <CCore/inc/TaskMemStack.h>

#include <CCore/inc/Print.h>

namespace App {

/* classes */

class Client;

struct Param;

class Application;

/* class Client */

class Client : ShapeLab::Config , public ShapeLab , public AliveControl
 {
  public:

   Client(SubWindowHost &host,FontBuilder &fb)
    : ShapeLab::Config(fb),
      ShapeLab(host,*this)
    {
    }

   ~Client() {}

   // AliveControl

   virtual void alive()
    {
     Printf(Con,"alive()\n");
    }

   virtual void dead()
    {
     Printf(Con,"dead()\n");
    }

   virtual bool askDestroy()
    {
     Printf(Con,"askDestroy()\n");

     return true;
    }
 };

/* struct Param */

struct Param : WindowReportParam
 {
  Param()
   {
   }
 };

/* class Application */

class Application : public ApplicationBase
 {
   const CmdDisplay cmd_display;

   FontBuilder fb;

   DragFrame main_win;

   ExceptionClient exception_client;
   Client client;

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
     Point max_size=desktop->getScreenSize();

     main_win.createMain(cmd_display,max_size,String("ShapeLab"));
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
      main_win(param.desktop,param.report_cfg),
      exception_client(main_win,param.exception_cfg,report),
      client(main_win,fb)
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

     Application app(report,param,cmd_display);

     return app.run();
    }
  catch(CatchType)
    {
     return 1;
    }
 }

} // namespace App


