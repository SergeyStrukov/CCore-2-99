/* FontLab.h */
//----------------------------------------------------------------------------------------
//
//  Project: FontLab 1.00
//
//  Tag: VideoLabs
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef FontLab_h
#define FontLab_h

#include <inc/Application.h>

#include <CCore/inc/Array.h>
#include <CCore/inc/Cmp.h>

#include <CCore/inc/video/FontDatabase.h>

namespace App {

/* classes */

class FontSelector;

class FontSelectorFrame;

class Progress;

class FontLab;

/* class FontSelector */

class FontSelector : public ScrollListWindow
 {
   class ExtInfo : public ComboInfo
    {
      class Base;

     public:

      ExtInfo() noexcept;

      explicit ExtInfo(const FontDatabase &fdb);

      ~ExtInfo();

      String getPath(ulen index) const;
    };

   ExtInfo info;

  private:

   void setFont(ulen index);

   SignalConnector<FontSelector,ulen> connector_selected;

  public:

   FontSelector(SubWindowHost &host,const ConfigType &cfg);

   virtual ~FontSelector();

   // methods

   void createList(const FontDatabase &fdb);

   // signals

   Signal<String> font_selected;
 };

/* class FontSelectorFrame */

class FontSelectorFrame : NoCopy
 {
  public:

   struct Config
    {
     CtorRefVal<DragFrame::ConfigType> drag_cfg;
     CtorRefVal<FontSelector::ConfigType> selector_cfg;

     RefVal<DefString> title = "Select Font"_def ;

     Config() noexcept {}
    };

   using ConfigType = Config ;

  private:

   FrameWindow *parent;

   const Config &cfg;

   DragFrame frame;

   FontSelector client;

  public:

   FontSelectorFrame(FrameWindow *parent,const Config &cfg);

   ~FontSelectorFrame();

   void create(const FontDatabase &fdb);

   Signal<String> &selected;
 };

/* class Progress */

class Progress : public IncrementalProgress
 {
  public:

   struct Config
    {
     RefVal<Coord> space_dxy = 20 ;

     RefVal<VColor> back = Silver ;

     RefVal<DefString> title = "Building font cache"_def ;

     CtorRefVal<ProgressWindow::ConfigType> progress_cfg;

     CtorRefVal<FixedFrame::ConfigType> fixed_cfg;

     Config() noexcept {}
    };

   using ConfigType = Config ;

  private:

   class Client : public ComboWindow
    {
      const Config &cfg;

      ProgressWindow progress;

     public:

      Client(SubWindowHost &host,const Config &cfg);

      virtual ~Client();

      // progress

      void setTotal(unsigned total) { progress.setTotal(total); }

      void setPos(unsigned pos) { progress.setPosPing(pos); }

      // drawing

      virtual void layout();

      virtual void drawBack(DrawBuf buf,bool drag_active) const;
    };

   FrameWindow *parent;

   const Config &cfg;

   FixedFrame frame;

   Client client;

  private:

   // IncrementalProgress

   virtual void start();

   virtual void setTotal(unsigned total);

   virtual bool setPos(unsigned pos);

   virtual void stop() noexcept;

  public:

   Progress(FrameWindow *parent,const Config &cfg);

   ~Progress();
 };

/* class FontLab */

class FontLab : public SubWindow
 {
  public:

   struct Config
    {
     RefVal<Coord> space  = 10 ;

     RefVal<VColor> back =    Silver ;
     RefVal<VColor> text =     Black ;

     RefVal<VColor> back_alt =    Black ;
     RefVal<VColor> text_alt =   Orange ;

     RefVal<VColor> info = DarkGreen ;
     RefVal<VColor> net  =      Blue ;
     RefVal<VColor> base =       Red ;
     RefVal<VColor> clip =       Red ;

     RefVal<Font> font;

     CtorRefVal<Progress::ConfigType> progress_cfg;
     CtorRefVal<FontSelectorFrame::ConfigType> selector_cfg;

     Config() {}
    };

   using ConfigType = Config ;

  private:

   static const Coord Rows = 16 ;
   static const Coord Cols = 16 ;

   const Config &cfg;

   Progress progress;
   FontBuilder::Incremental fbinc;
   FontBuilder fb;
   bool fb_flag = true ;

   Coord font_size = 16 ;
   FreeTypeFont::Config font_config;

   FreeTypeFont font;

   Point base;
   Pane all_pane;
   Pane info_pane;
   Pane clip_pane;

   bool sample = true ;
   bool clip = false ;
   bool alt = false ;

   FontSelectorFrame selector_frame;

  private:

   static FontSmoothType Next(FontSmoothType fsm);

   static FontHintType Next(FontHintType fht);

   bool set(StrLen file_name);

   bool set(StrLen dir,StrLen file_name);

   void set(int choice);

   void show(DrawBuf buf,StrLen text) const;

   template <class ... TT>
   void show(DrawBuf buf,const char *format,const TT & ... tt) const
    {
     char temp[TextBufLen];
     PrintBuf out(Range(temp));

     Printf(out,format,tt...);

     show(buf,out.close());
    }

   void drawTable(DrawBuf buf) const;

   void drawSampleText(DrawBuf buf) const;

  private:

   void complete_fb(bool ok);

   void setFont(String file_name);

   SignalConnector<FontLab,bool> connector_fbinc_completed;
   SignalConnector<FontLab,String> connector_selector_selected;

  public:

   FontLab(SubWindowHost &host,const Config &cfg);

   virtual ~FontLab();

   // drawing

   virtual void layout();

   virtual void draw(DrawBuf buf,bool drag_active) const;

   // base

   virtual void open();

   // user input

   virtual void react(UserAction action);

   void react_Key(VKey vkey,KeyMod kmod);

   void react_Wheel(Point point,MouseKey mkey,Coord delta);
 };

} // namespace App

#endif

