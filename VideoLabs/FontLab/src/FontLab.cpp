/* FontLab.cpp */
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

#include <inc/FontLab.h>

#include <CCore/inc/video/UserPreference.h>
#include <CCore/inc/video/CommonDrawArt.h>
#include <CCore/inc/video/Layout.h>

#include <CCore/inc/CharProp.h>
#include <CCore/inc/Exception.h>
#include <CCore/inc/Path.h>
#include <CCore/inc/Sort.h>
#include <CCore/inc/Cmp.h>
#include <CCore/inc/Print.h>

#include <CCore/inc/algon/BinarySearch.h>

namespace App {

/* class FontSelector::ExtInfo::Base */

class FontSelector::ExtInfo::Base : public ComboInfoBase
 {
   struct NameExt
    {
     StrLen name;
     StrLen ext;
     bool no_ext;

     explicit NameExt(const String &path)
      {
       SplitPath split1(Range(path));

       SplitName split2(split1.path);

       SplitExt split3(split2.name);

       name=split3.name;
       ext=split3.ext;
       no_ext=!split3;
      }
    };

   struct Rec
    {
     ComboInfoType type;
     String path;
     String name;
     String ext;

     Rec(const String &path_,NameExt name_ext)
      : type(ComboInfoText),
        path(path_),
        name(name_ext.name),
        ext(name_ext.ext)
      {
      }

     explicit Rec(StrLen ext_)
      : type(ComboInfoTitle),
        ext(ext_)
      {
      }

     operator ComboInfoItem() const
      {
       if( type==ComboInfoText ) return ComboInfoItem(type,Range(name));

       return ComboInfoItem(ComboInfoTitle,Range(ext));
      }
    };

   DynArray<Rec> list;

  private:

  public:

   explicit Base(const FontDatabase &fdb)
    {
     FontIndex index;

     index.build(fdb, [] (const FontInfo &obj) { return obj.scalable && !NameExt(obj.file_name).no_ext ; } ,
                      [] (const FontInfo &a,const FontInfo &b) { return AlphaCmp(b.monospace,a.monospace,
                                                                                 CmpAsStr(NameExt(a.file_name).ext),CmpAsStr(NameExt(b.file_name).ext),
                                                                                 CmpAsStr(a.family),CmpAsStr(b.family),
                                                                                 a.italic,b.italic,
                                                                                 a.bold,b.bold); } );
     bool first=true;
     bool monospace=true;
     StrLen ext;

     for(const FontInfo *obj : index.getList() )
       {
        String path=obj->file_name;
        NameExt name_ext(path);

        if( Change(first,false) )
          {
           monospace=obj->monospace;
          }

        if( obj->monospace==monospace )
          {
           if( monospace )
             list.append_fill("Monospace"_c);
           else
             list.append_fill("Proportional"_c);

           monospace=!monospace;
           ext=Null;
          }

        if( StrCmp(name_ext.ext,ext) )
          {
           ext=name_ext.ext;

           list.append_fill(ext);
          }

        list.append_fill(path,name_ext);
       }
    }

   ~Base()
    {
    }

   String getPath(ulen index) const
    {
     return list.at(index).path;
    }

   // AbstractComboInfo

   virtual ulen getLineCount() const
    {
     return list.getLen();
    }

   virtual ComboInfoItem getLine(ulen index) const
    {
     return list.at(index);
    }
 };

/* class FontSelector::FontInfo */

FontSelector::ExtInfo::ExtInfo() noexcept
 {
 }

FontSelector::ExtInfo::ExtInfo(const FontDatabase &fdb)
 : ComboInfo(new Base(fdb))
 {
 }

FontSelector::ExtInfo::~ExtInfo()
 {
 }

String FontSelector::ExtInfo::getPath(ulen index) const
 {
  if( Base *base=castPtr<Base>() ) return base->getPath(index);

  return Null;
 }

/* class FontSelector */

void FontSelector::setFont(ulen index)
 {
  font_selected.assert(info.getPath(index));
 }

FontSelector::FontSelector(SubWindowHost &host,const ConfigType &cfg)
 : ScrollListWindow(host,cfg),

   connector_selected(this,&FontSelector::setFont,selected)
 {
 }

FontSelector::~FontSelector()
 {
 }

void FontSelector::createList(const FontDatabase &fdb)
 {
  info=ExtInfo(fdb);

  setInfo(info);
 }

/* class FontSelectorFrame */

FontSelectorFrame::FontSelectorFrame(FrameWindow *parent_,const Config &cfg_)
 : parent(parent_),
   cfg(cfg_),
   frame(parent_->getDesktop(),cfg.drag_cfg),
   client(frame,cfg.selector_cfg),

   selected(client.font_selected)
 {
  frame.bindClient(client);
 }

FontSelectorFrame::~FontSelectorFrame()
 {
 }

void FontSelectorFrame::create(const FontDatabase &fdb)
 {
  if( frame.isDead() )
    {
     client.createList(fdb);

     Point screen_size=frame.getScreenSize();

     Point size(screen_size.x/2,screen_size.y/2);

     frame.create(parent,FreeCenter(screen_size,size),+cfg.title);
    }
 }

/* class Progress::Client */

Progress::Client::Client(SubWindowHost &host,const Config &cfg_)
 : ComboWindow(host),
   cfg(cfg_),

   progress(wlist,cfg.progress_cfg)
 {
  wlist.insTop(progress);
 }

Progress::Client::~Client()
 {
 }

 // drawing

void Progress::Client::layout()
 {
  Point size=getSize();
  Pane pane(Null,size);

  progress.setPlace(pane.shrink(+cfg.space_dxy));
 }

void Progress::Client::drawBack(DrawBuf buf,bool) const
 {
  buf.erase(+cfg.back);
 }

/* class Progress */

void Progress::start()
 {
  Point screen_size=frame.getScreenSize();

  DefString title=+cfg.title;
  Coord space=+cfg.space_dxy;

  Point client_size(15*space,3*space);

  Point size=frame.getMinSize(false,title.str(),client_size);

  frame.create(parent,FreeCenter(screen_size,size),title);
 }

void Progress::setTotal(unsigned total)
 {
  client.setTotal(total);
 }

bool Progress::setPos(unsigned pos)
 {
  client.setPos(pos);

  return frame.isAlive();
 }

void Progress::stop() noexcept
 {
  try { frame.destroy(); } catch(...) {}
 }

Progress::Progress(FrameWindow *parent_,const Config &cfg_)
 : parent(parent_),
   cfg(cfg_),
   frame(parent_->getDesktop(),cfg.fixed_cfg),
   client(frame,cfg)
 {
  frame.bindClient(client);
 }

Progress::~Progress()
 {
 }

/* class FontLab */

FontSmoothType FontLab::Next(FontSmoothType fsm)
 {
  switch( fsm )
    {
     default: return FontSmooth;

     case FontSmooth : return FontSmoothLCD_RGB;

     case FontSmoothLCD_RGB : return FontSmoothLCD_BGR;

     case FontSmoothLCD_BGR : return FontSmoothNone;
    }
 }

FontHintType FontLab::Next(FontHintType fht)
 {
  switch( fht )
    {
     default: return FontHintNative;

     case FontHintNative : return FontHintAuto;

     case FontHintAuto : return FontHintNone;
    }
 }

bool FontLab::set(StrLen file_name)
 {
  try
    {
     font=FreeTypeFont(file_name);

     font.setSize(font_size);

     font.setConfig(font_config);

     return true;
    }
  catch(CatchType)
    {
     return false;
    }
 }

bool FontLab::set(StrLen dir,StrLen file_name)
 {
  try
    {
     font=FreeTypeFont(dir,file_name);

     font.setSize(font_size);

     font.setConfig(font_config);

     return true;
    }
  catch(CatchType)
    {
     return false;
    }
 }

void FontLab::set(int choice)
 {
  switch( choice )
    {
     case 1 : font=fb.build("Anonymous Pro",font_size,font_config); break;
     case 2 : font=fb.build("Anonymous Pro"|Bold,font_size,font_config); break;
     case 3 : font=fb.build("Anonymous Pro"|Italic,font_size,font_config); break;

     case 4 : font=fb.build("Arial",font_size,font_config); break;
     case 5 : font=fb.build("Arial"|Bold,font_size,font_config); break;
     case 6 : font=fb.build("Arial"|Italic,font_size,font_config); break;

     case 7 : font=fb.build("Times New Roman",font_size,font_config); break;
     case 8 : font=fb.build("Tahoma",font_size,font_config); break;
     case 9 : font=fb.build("Courier New",font_size,font_config); break;
    }
 }

void FontLab::show(DrawBuf buf,StrLen text) const
 {
  cfg.font->text(buf,info_pane,TextPlace(AlignX_Left,AlignY_Top),text,+cfg.info);
 }

void FontLab::drawTable(DrawBuf buf) const
 {
  CommonDrawArt art(buf);

  FontSize fs=font->getSize();

  Point delta=Point(font_size,fs.dy).addXY(1);
  Coord by=fs.by;

  VColor net=+cfg.net;

  // net
  {
   Point a=base;
   Coord len=Cols*delta.x;

   art.path(net,a,a.addX(len));

   for(auto cnt=Rows; cnt ;cnt--)
     {
      a=a.addY(delta.y);

      art.path(net,a,a.addX(len));
     }
  }

  {
   Point a=base;
   Coord len=Rows*delta.y;

   art.path(net,a,a.addY(len));

   for(auto cnt=Cols; cnt ;cnt--)
     {
      a=a.addX(delta.x);

      art.path(net,a,a.addY(len));
     }
  }

  // cells
  {
   Point a=base;
   unsigned char ch=0;
   VColor vc = alt? +cfg.text_alt : +cfg.text ;

   for(auto cnt=Rows; cnt ;cnt--)
     {
      Point b=a.addX(1).addY(by);

      for(auto cnt=Cols; cnt ;cnt--)
        {
         char str[1]={(char)ch++};

         TextSize ts=font->text(Range(str,1));

         art.path(+cfg.base,b,b.addX(ts.dx));

         font->text(buf,all_pane,TextPlace(b.x,b.y),Range(str,1),vc);

         b=b.addX(delta.x);
        }

      a=a.addY(delta.y);
     }
  }
 }

static const char * SampleText=
"Copyright 2000-2015 by\r\n"
"David Turner, Robert Wilhelm, and Werner Lemberg.\r\n"
"\r\n"
"This file is part of the FreeType project, and may only be used,\r\n"
"modified, and distributed under the terms of the FreeType project\r\n"
"license, LICENSE.TXT. By continuing to use, modify, or distribute\r\n"
"this file you indicate that you have read the license and understand\r\n"
"and accept it fully.\r\n"
"\r\n"
"void FontLab::react_Wheel(Point point,MouseKey mkey,Coord delta)\r\n"
" {\r\n"
"  Used(point);\r\n"
"  Used(mkey);\r\n"
"\r\n"
"  font_size=Cap<Coord>(1,font_size+delta,100);\r\n"
"\r\n"
"  imp->setSize(font_size);\r\n"
"\r\n"
"  redraw();\r\n"
" }";

void FontLab::drawSampleText(DrawBuf buf) const
 {
  Point a=base;

  FontSize fs=font->getSize();

  Coord dy=fs.dy;

  VColor vc = alt? +cfg.text_alt : +cfg.text ;

  for(StrLen text=SampleText; +text ;)
    {
     StrLen line=CutLine(text);

     a=a.addY(dy);

     font->text(buf,all_pane,TextPlace(a),line,vc);
    }
 }

void FontLab::complete_fb(bool ok)
 {
  enableFrameReact();

  if( ok )
    {
     font=fb.build("Anonymous Pro",font_size,font_config);

     fb_flag=false;

     redraw();
    }
  else
    {
     askFrameClose();
    }
 }

void FontLab::setFont(String file_name)
 {
  if( set(Range(file_name)) ) redraw();
 }

FontLab::FontLab(SubWindowHost &host,const Config &cfg_)
 : SubWindow(host),
   cfg(cfg_),

   progress(host.getFrame(),cfg.progress_cfg),
   fbinc(progress),
   selector_frame(host.getFrame(),cfg.selector_cfg),

   connector_fbinc_completed(this,&FontLab::complete_fb,fbinc.completed),
   connector_selector_selected(this,&FontLab::setFont,selector_frame.selected)
 {
 }

FontLab::~FontLab()
 {
  fbinc.cancel();
 }

 // drawing

void FontLab::layout()
 {
  Point size=getSize();

  all_pane=Pane(Null,size);

  FontSize fs=cfg.font.get()->getSize();

  Coord delta=+cfg.space;

  info_pane=Pane(delta,delta,size.x-2*delta,fs.dy+delta);

  base=Point(delta,fs.dy+2*delta);

  clip_pane=Pane(base+Point(10,10),{50,30});
 }

void FontLab::draw(DrawBuf buf,bool) const
 {
  buf.erase( alt? +cfg.back_alt : +cfg.back );

  FontSize fs=font->getSize();

  show(buf,"#; #; #; : #; #; dy = #; by = #; gamma = #;/10",font.getFamily(),font.getStyle(),font_size,
                                                            font_config.fht,font_config.fsm,
                                                            fs.dy,fs.by,
                                                            int(10*font_config.gamma_order));

  if( clip )
    {
     PaneBorder border(clip_pane.expand(1));

     CommonDrawArt(buf).loop(border.get(),+cfg.clip);

     buf=buf.cut(clip_pane);
    }

  if( sample )
    drawSampleText(buf);
  else
    drawTable(buf);
 }

 // base

void FontLab::open()
 {
  if( fb_flag )
    {
     disableFrameReact();

     fb.prepare(fbinc);
    }
 }

 // user input

void FontLab::react(UserAction action)
 {
  action.dispatch(*this);
 }

void FontLab::react_Key(VKey vkey,KeyMod kmod)
 {
  Used(vkey);
  Used(kmod);

  switch( vkey )
    {
     case VKey_F10 :
      {
       selector_frame.create(fb.getDatabase());
      }
     return;

     case VKey_F11 :
      {
       UserPreference pref;

       pref.update();
      }
     return;

     case VKey_F2 :
      {
       font_config.fsm=Next(font_config.fsm);

       font.setConfig(font_config);
      }
     break;

     case VKey_F3 :
      {
       font_config.fht=Next(font_config.fht);

       font.setConfig(font_config);
      }
     break;

     case VKey_F4 : sample=!sample; break;

     case VKey_F5 : clip=!clip; break;

     case VKey_F6 : alt=!alt; break;

     case VKey_F7 :
      {
       font_config.strength=20-font_config.strength;

       font.setConfig(font_config);
      }
     break;

     case VKey_F8 :
      {
       font_config.use_kerning=!font_config.use_kerning;

       font.setConfig(font_config);
      }
     break;

     case VKey_NumMinus :
      {
       if( !font_config.gamma_order ) font_config.gamma_order=1.8;

       font_config.gamma_order=Cap(0.1,font_config.gamma_order-0.1,10.);

       font.setConfig(font_config);
      }
     break;

     case VKey_NumPlus :
      {
       if( !font_config.gamma_order ) font_config.gamma_order=1.8;

       font_config.gamma_order=Cap(0.1,font_config.gamma_order+0.1,10.);

       font.setConfig(font_config);
      }
     break;

     case VKey_1 : set(1); break;
     case VKey_2 : set(2); break;
     case VKey_3 : set(3); break;
     case VKey_4 : set(4); break;
     case VKey_5 : set(5); break;
     case VKey_6 : set(6); break;
     case VKey_7 : set(7); break;
     case VKey_8 : set(8); break;
     case VKey_9 : set(9); break;

     default: return;
    }

  redraw();
 }

void FontLab::react_Wheel(Point point,MouseKey mkey,Coord delta)
 {
  Used(point);
  Used(mkey);

  font_size=Cap<Coord>(1,font_size+delta,100);

  font.setSize(font_size);

  redraw();
 }

} // namespace App

