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

class DDLWindow;

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

/* class DDLWindow */

class DDLWindow : public SubWindow
 {
  public:

   struct Config
    {
     Config() noexcept {}

     explicit Config(const UserPreference &pref) // TODO
      {
       Used(pref);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   const DDLFile &file;

  public:

   DDLWindow(SubWindowHost &host,const Config &cfg,const DDLFile &file);

   virtual ~DDLWindow();

   // methods

   Point getMinSize() const { return Null; }
 };

/* class DisplayWindow */

class DisplayWindow : public ComboWindow
 {
  public:

   struct Config
    {
     RefVal<Coord> space_dxy = 10 ;

     RefVal<VColor> back = Silver ;

     CtorRefVal<MessageWindow::ConfigType> msg_cfg;
     CtorRefVal<LabelWindow::ConfigType> label_cfg;
     CtorRefVal<TextWindow::ConfigType> text_cfg;
     CtorRefVal<XDoubleLineWindow::ConfigType> dline_cfg;

     DDLWindow::Config ddl_cfg;

     Config() noexcept {}

     explicit Config(const UserPreference &pref)
      : msg_cfg(pref.getAlertMessageWindowConfig()),
        label_cfg(SmartBind,pref),
        text_cfg(SmartBind,pref),
        dline_cfg(SmartBind,pref),
        ddl_cfg(pref)
      {
       space_dxy.bind(pref.get().space_dxy);
       back.bind(pref.get().back);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   DDLFile file;

   // subwins

   LabelWindow label_pretext;
   LabelWindow label_file;

   TextWindow text_pretext;
   TextWindow text_file;

   XDoubleLineWindow dline;

   DDLWindow ddl;

   // msg

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

   // drawing

   virtual void layout();

   virtual void draw(DrawBuf buf,bool drag_active) const;

   virtual void draw(DrawBuf buf,Pane pane,bool drag_active) const;

   // signals

   Signal<StrLen,bool> opened; // file_name ,  ok
 };

} // namespace App

