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
#include <CCore/inc/Printf.h>
#include <CCore/inc/PlatformRandom.h>
#include <CCore/inc/Cmp.h>

#include <CCore/inc/video/FixedWindow.h>
#include <CCore/inc/video/WindowLib.h>
#include <CCore/inc/video/Layout.h>
#include <CCore/inc/video/FontDatabase.h>

namespace App {

/* type unicode */

using unicode = uint32 ;

/* classes */

class FontSelector;

class FontSelectorWindow;

class Progress;

class FontLab;

/* class FontSelector */

class FontSelector : public SubWindow
 {
  public:

   struct Config
    {
     RefVal<VColor> text   =  Black ;
     RefVal<VColor> select = Yellow ;
     RefVal<VColor> back   = Silver ;

     RefVal<Font> font;

     Config() {}
    };

  private:

   const Config &cfg;

   struct Rec : CmpComparable<Rec>
    {
     String path;
     String name;
     String ext;

     Rec(const String &path,StrLen name,StrLen ext);
    };

   DynArray<Rec> list;
   ulen first = 0 ;
   ulen count = 0 ;
   ulen select = 0 ;

   Coord begX = 0 ;
   Coord dX = 0 ;
   Coord dY = 0 ;
   Coord bY = 0 ;
   ulen max_count = 0 ;

  private:

   void setSelect(ulen select);

   void listMove();

   void listUp(ulen delta);

   void listDown(ulen delta);

  public:

   FontSelector(SubWindowHost &host,const Config &cfg);

   virtual ~FontSelector();

   void createList(const FontDatabase &fdb);

   // drawing

   virtual void layout();

   virtual void draw(DrawBuf buf,bool drag_active) const;

   // user input

   virtual void react(UserAction action);

   void react_Key(VKey vkey,KeyMod kmod);

   // signals

   Signal<String> select_font;
 };

/* class Progress */

class Progress : public IncrementalProgress
 {
   class Client : public SubWindow
    {
      ProgressWindow::ConfigType cfg;

      VColor back = Silver ;

      Coord space_dxy = 20 ;

      ProgressWindow progress;

     public:

      explicit Client(SubWindowHost &host)
       : SubWindow(host),
         progress(host,cfg)
       {
       }

      virtual ~Client()
       {
       }

      // progress

      void setTotal(unsigned total) { progress.setTotal(total); }

      void setPos(unsigned pos) { progress.setPosPing(pos); }

      // drawing

      virtual void layout()
       {
        Point size=getSize();
        Pane pane(Null,size);

        progress.setPlace(pane.shrink(space_dxy));
       }

      virtual void draw(DrawBuf buf,bool drag_active) const
       {
        buf.erase(back);

        progress.forward_draw(buf,drag_active);
       }
    };

   class Frame : public FixedWindow
    {
      Client sub_win;

     public:

      Frame(Desktop *desktop,const FixedWindow::ConfigType &cfg)
       : FixedWindow(desktop,cfg),
         sub_win(*this)
       {
        bindClient(sub_win);
       }

      virtual ~Frame()
       {
       }

      // progress

      void setTotal(unsigned total) { sub_win.setTotal(total); }

      bool setPos(unsigned pos) { sub_win.setPos(pos); return isAlive(); }

      // create

      void create(FrameWindow *parent)
       {
        Point screen_size=getDesktop()->getScreenSize();

        String title("Building font cache");

        Point client_size(300,60);

        Point size=getMinSize(Range(title),client_size);

        FixedWindow::create(parent,FreeCenter({Null,screen_size},size),title);
       }
    };

   FrameWindow *parent;

   FixedWindow::ConfigType cfg;

   Frame frame;

  private:

   // IncrementalProgress

   virtual void start()
    {
     frame.create(parent);
    }

   virtual void setTotal(unsigned total)
    {
     frame.setTotal(total);
    }

   virtual bool setPos(unsigned pos)
    {
     return frame.setPos(pos);
    }

   virtual void stop() noexcept
    {
     try { frame.destroy(); } catch(...) {}
    }

  public:

   Progress(Desktop *desktop,FrameWindow *parent_)
    : parent(parent_),
      frame(desktop,cfg)
    {
    }

   Progress(SubWindow *sub_win) : Progress(sub_win->getDesktop(),sub_win->getFrame()) {}

   ~Progress()
    {
    }
 };

/* class FontSelectorWindow */

class FontSelectorWindow : NoCopy
 {
   DragWindow frame;

   FontSelector win;

  public:

   FontSelectorWindow(Desktop *desktop,const DragWindow::ConfigType &drag_cfg,const FontSelector::Config &cfg);

   ~FontSelectorWindow();

   void create(FrameWindow *parent,const FontDatabase &fdb,const String &title);

   Signal<String> & takeSignal() { return win.select_font; }
 };

/* class FontLab */

class FontLab : public SubWindow
 {
  public:

   struct Config
    {
     RefVal<VColor> back =    Silver ;
     RefVal<VColor> text =     Black ;

     RefVal<VColor> back_alt =    Black ;
     RefVal<VColor> text_alt =   Orange ;

     RefVal<VColor> info = DarkGreen ;
     RefVal<VColor> net  =      Blue ;
     RefVal<VColor> base =       Red ;
     RefVal<VColor> clip =       Red ;

     RefVal<DefString> select_title = "Select Font"_def ;

     RefVal<Font> font;

     Config() {}
    };

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

   FontSelectorWindow &selector;

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

   void setFont(String file_name);

   void complete_fb(bool ok);

  private:

   SignalConnector<FontLab,bool> connector_fbinc;
   SignalConnector<FontLab,String> connector_font;

  public:

   FontLab(SubWindowHost &host,const Config &cfg,FontSelectorWindow &selector);

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

