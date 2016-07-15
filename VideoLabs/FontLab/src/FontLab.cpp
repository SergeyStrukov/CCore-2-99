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

#include <CCore/inc/video/CommonDrawArt.h>

#include <CCore/inc/CharProp.h>
#include <CCore/inc/Exception.h>
#include <CCore/inc/Path.h>
#include <CCore/inc/Sort.h>
#include <CCore/inc/Cmp.h>
#include <CCore/inc/Print.h>
#include <CCore/inc/video/UserPreference.h>

#include <CCore/inc/algon/BinarySearch.h>

namespace App {

/* testPref() */

static void testPref()
 {
  UserPreference pref;

  pref.update();
 }

/* class FontSelector */

FontSelector::Rec::Rec(const String &path_,StrLen name_,StrLen ext_)
 : path(path_),
   name(name_),
   ext(ext_)
 {
 }

void FontSelector::setSelect(ulen select_)
 {
  if( Change(select,select_) )
    {
     if( select<list.getLen() )
       {
        Rec &rec=list[select];

        select_font.assert(rec.path);
       }
    }
 }

void FontSelector::listMove()
 {
  ulen len=list.getLen();

  count=Min<ulen>(len-first,max_count);

  if( select<first )
    {
     setSelect(first);
    }
  else
    {
     ulen lim=first+count;

     if( select>=lim )
       {
        if( count )
          setSelect(lim-1);
        else
          setSelect(first);
       }
    }

  redraw();
 }

void FontSelector::listUp(ulen delta)
 {
  if( !delta ) return;

  if( first>delta )
    {
     first-=delta;

     listMove();
    }
  else if( first )
    {
     first=0;

     listMove();
    }
 }

void FontSelector::listDown(ulen delta)
 {
  if( !delta ) return;

  ulen len=list.getLen();
  ulen cap=(len>max_count)?len-max_count:0;

  if( first>=cap ) return;

  Replace_min<ulen>(delta,cap-first);

  first+=delta;

  listMove();
 }

FontSelector::FontSelector(SubWindowHost &host,const Config &cfg_)
 : SubWindow(host),
   cfg(cfg_)
 {
 }

FontSelector::~FontSelector()
 {
 }

void FontSelector::createList(const FontDatabase &fdb)
 {
  list.erase();

  FontIndex index;

  index.build(fdb, [] (const FontInfo &obj) { return obj.scalable; } ,
                   [] (const FontInfo &a,const FontInfo &b) { return AlphaCmp(b.monospace,a.monospace,
                                                                              CmpAsStr(a.family),CmpAsStr(b.family),
                                                                              a.italic,b.italic,
                                                                              a.bold,b.bold); } );

  for(const FontInfo *obj : index.getList() )
    {
     String path=obj->file_name;

     SplitPath split1(Range(path));

     SplitName split2(split1.path);

     SplitExt split3(split2.name);

     if( !split3.no_ext )
       {
        list.append_fill(path,split3.name,split3.ext);
       }
    }

  first=0;
  count=0;
  select=0;
 }

 // drawing

void FontSelector::layout()
 {
  Point size=getSize();

  FontSize fs=cfg.font.get()->getSize();

  begX=10;
  dX=size.x-20;
  dY=fs.dy;
  bY=fs.by;

  if( dY<=0 ) dY=1;

  max_count=size.y/dY;

  count=Min<ulen>(list.getLen()-first,max_count);
 }

void FontSelector::draw(DrawBuf buf,bool drag_active) const
 {
  Used(drag_active);

  buf.erase(+cfg.back);

  VColor text=+cfg.text;

  Pane pane(begX,0,dX,dY);

  for(ulen ind=first,cnt=count; cnt ;cnt--,ind++,pane.y+=dY)
    {
     const Rec &rec=list[ind];

     if( ind==select )
       {
        CommonDrawArt art(buf);

        art.block(pane,+cfg.select);
       }

     cfg.font.get()->text(buf,pane,TextPlace(AlignX_Left,bY),Range(rec.name),text);
     cfg.font.get()->text(buf,pane,TextPlace(AlignX_Right,bY),Range(rec.ext),text);
    }
 }

 // user input

void FontSelector::react(UserAction action)
 {
  action.dispatch(*this);
 }

void FontSelector::react_Key(VKey vkey,KeyMod kmod)
 {
  switch( vkey )
    {
     case VKey_Up :
      {
       if( kmod&KeyMod_Shift )
         {
          listUp(1);
         }
       else
         {
          if( select )
            {
             setSelect(select-1);

             if( select<first ) listUp(1); else redraw();
            }
         }
      }
     break;

     case VKey_Down :
      {
       if( kmod&KeyMod_Shift )
         {
          listDown(1);
         }
       else
         {
          ulen len=list.getLen();
          ulen cap=len?len-1:0;

          if( select<cap )
            {
             setSelect(select+1);

             if( select>=first+count ) listDown(1); else redraw();
            }
         }
      }
     break;

     case VKey_PageUp :
      {
       listUp(max_count);
      }
     break;

     case VKey_PageDown :
      {
       listDown(max_count);
      }
     break;
    }
 }

/* class FontSelectorWindow */

FontSelectorWindow::FontSelectorWindow(Desktop *desktop,const DragWindow::ConfigType &drag_cfg,const FontSelector::Config &cfg)
 : frame(desktop,drag_cfg),
   win(frame,cfg)
 {
  frame.bindClient(win);
 }

FontSelectorWindow::~FontSelectorWindow()
 {
 }

void FontSelectorWindow::create(FrameWindow *parent,const FontDatabase &fdb,const String &title)
 {
  if( frame.isDead() )
    {
     win.createList(fdb);

     Point max_size=frame.getDesktop()->getScreenSize();

     Pane pane(max_size/4,max_size/2);

     frame.create(parent,pane,max_size,title);
    }
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
  cfg.font.get()->text(buf,info_pane,TextPlace(AlignX_Left,AlignY_Top),text,+cfg.info);
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
   VColor vc=alt?+cfg.text_alt:+cfg.text;

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

  VColor vc=alt?+cfg.text_alt:+cfg.text;

  for(StrLen text=SampleText; +text ;)
    {
     StrLen line=CutLine(text);

     a=a.addY(dy);

     font->text(buf,all_pane,TextPlace(a.x,a.y),line,vc);
    }
 }

void FontLab::setFont(String file_name)
 {
  if( set(Range(file_name)) ) redraw();
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
     getFrame()->askClose();
    }
 }

FontLab::FontLab(SubWindowHost &host,const Config &cfg_,FontSelectorWindow &selector_)
 : SubWindow(host),
   cfg(cfg_),
   progress(this),
   fbinc(progress),
   selector(selector_),
   connector_fbinc(this,&FontLab::complete_fb,fbinc.complete),
   connector_font(this,&FontLab::setFont,selector.takeSignal())
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

  FontSize font_size=cfg.font.get()->getSize();

  Coord delta=10;

  info_pane={delta,delta,Coord(size.x-2*delta),Coord(font_size.dy+delta)};

  base={delta,Coord(font_size.dy+2*delta)};

  clip_pane=Pane(base+Point(10,10),{50,30});
 }

void FontLab::draw(DrawBuf buf,bool drag_active) const
 {
  Used(drag_active);

  buf.erase(alt?+cfg.back_alt:+cfg.back);

  FontSize fs=font->getSize();

  show(buf,"#; #; #; : #; #; dy = #; by = #; gamma = #;/10",font.getFamily(),font.getStyle(),font_size,
                                                            font_config.fht,font_config.fsm,
                                                            fs.dy,fs.by,int(10*font_config.gamma_order));

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
       selector.create(getFrame(),fb.getDatabase(),cfg.select_title.get().makeString());
      }
     return;

     case VKey_F11 :
      {
       testPref();
      }
     return;

     case VKey_F1 :
      {
       font_config.fsm=Next(font_config.fsm);

       font.setConfig(font_config);
      }
     break;

     case VKey_F2 :
      {
       font_config.fht=Next(font_config.fht);

       font.setConfig(font_config);
      }
     break;

     case VKey_F3 : sample=!sample; break;

     case VKey_F4 : clip=!clip; break;

     case VKey_F5 : alt=!alt; break;

     case VKey_F6 :
      {
       font_config.strength=20-font_config.strength;

       font.setConfig(font_config);
      }
     break;

     case VKey_F7 :
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

