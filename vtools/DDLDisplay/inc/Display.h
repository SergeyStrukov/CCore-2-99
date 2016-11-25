/* Display.h */
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

#include <inc/Application.h>

#include <CCore/inc/video/MessageWindow.h>

#include <CCore/inc/FileName.h>
#include <CCore/inc/FileToMem.h>
#include <CCore/inc/String.h>

#include <CCore/inc/ddl/DDLEngine.h>

namespace App {

/* classes */

class DDLFile;

class DisplayWindow;

/* class DDLFile */

class DDLFile : NoCopy
 {
   PrintString eout;

   FileToMem pretext;

   DDL::FileEngine<FileName,FileToMem> engine;

   DDL::EngineResult result;

  private:

   void erase();

  public:

   DDLFile() noexcept;

   ~DDLFile();

   auto getResult() const { return result; }

   struct OpenResult
    {
     DefString etext;
     bool ok;
    };

   OpenResult open(StrLen file_name);

   OpenResult openPretext(StrLen file_name);

   void noPretext();
 };

/* class DisplayWindow */

class DisplayWindow : public SubWindow
 {
  public:

   struct Config
    {
     CtorRefVal<MessageWindow::ConfigType> msg_cfg;

     Config() noexcept {}

     explicit Config(const UserPreference &pref)
      : msg_cfg(pref.getAlertMessageWindowConfig())
      {
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   DDLFile file;

   MessageWindow msg;

  private:

   void error(StrLen str);

   void setPretextFileName(StrLen file_name);

   void setFileName(StrLen file_name);

  public:

   DisplayWindow(SubWindowHost &host,const Config &cfg);

   virtual ~DisplayWindow();

   // methods

   void open(StrLen file_name);

   void openPretext(StrLen file_name);

   void noPretext();

   // signals

   Signal<StrLen,bool> opened; // file_name ,  ok
 };

} // namespace App

