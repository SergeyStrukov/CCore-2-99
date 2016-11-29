/* Display.cpp */
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

#include <inc/Display.h>

#include <CCore/inc/video/Layout.h>

namespace App {

/* class DDLFile */

void DDLFile::erase()
 {
  result=Null;

  engine.reset();

  updated.assert();
 }

DDLFile::DDLFile() noexcept
 : eout(64_KByte),
   engine(eout)
 {
 }

DDLFile::~DDLFile()
 {
 }

auto DDLFile::open(StrLen file_name) -> OpenResult
 {
  try
    {
     eout.cleanup();

     result=engine.process(file_name,Mutate<const char>(Range(pretext)));

     if( !result )
       {
        String str=eout.close();

        eout.cleanup();

        erase();

        return {str,false};
       }
     else
       {
        eout.cleanup();

        updated.assert();

        return {Null,true};
       }
    }
  catch(CatchType)
    {
     eout.cleanup();

     erase();

     return {"Exception"_def,false};
    }
 }

auto DDLFile::openPretext(StrLen file_name) -> OpenResult
 {
  try
    {
     FileToMem map(file_name);

     eout.cleanup();

     DDL::TextEngine engine(eout,Mutate<const char>(Range(map)));

     auto result=engine.process();

     if( !result )
       {
        String str=eout.close();

        eout.cleanup();

        updated.assert();

        return {str,false};
       }
     else
       {
        eout.cleanup();

        Swap(pretext,map);

        erase();

        return {Null,true};
       }
    }
  catch(CatchType)
    {
     eout.cleanup();

     updated.assert();

     return {"Exception"_def,false};
    }
 }

void DDLFile::noPretext()
 {
  FileToMem map;

  Swap(pretext,map);

  erase();
 }

/* class DDLInnerWindow */

void DDLInnerWindow::posX(ulen pos)
 {
  x_pos=pos;

  redraw();
 }

void DDLInnerWindow::posY(ulen pos)
 {
  y_pos=pos;

  redraw();
 }

DDLInnerWindow::DDLInnerWindow(SubWindowHost &host,const Config &cfg_,const DDLFile &file_)
 : SubWindow(host),
   cfg(cfg_),
   file(file_),

   connector_posX(this,&DDLInnerWindow::posX),
   connector_posY(this,&DDLInnerWindow::posY)
 {
 }

DDLInnerWindow::~DDLInnerWindow()
 {
 }

 // methods

void DDLInnerWindow::update()
 {
  x_total=600;
  y_total=600;

  x_pos=0;
  y_pos=0;
 }

 // drawing

void DDLInnerWindow::layout() // TODO
 {
  Point size=getSize();

  x_page=size.x;

  if( x_page>=x_total ) x_pos=0;

  y_page=size.y;

  if( y_page>=y_total ) y_pos=0;
 }

void DDLInnerWindow::draw(DrawBuf buf,bool) const // TODO
 {
  buf.erase(Black);
 }

 // base

void DDLInnerWindow::open() // TODO
 {
 }

/* class DDLWindow */

void DDLWindow::file_updated()
 {
  inner.update();

  layout();

  redraw();
 }

DDLWindow::DDLWindow(SubWindowHost &host,const Config &cfg_,DDLFile &file)
 : ComboWindow(host),
   cfg(cfg_),

   inner(wlist,cfg,file),
   scroll_x(wlist,cfg.x_cfg),
   scroll_y(wlist,cfg.y_cfg),

   connector_posx(&scroll_x,&XScrollWindow::setPos,inner.scroll_x),
   connector_posy(&scroll_y,&YScrollWindow::setPos,inner.scroll_y),

   connector_file_updated(this,&DDLWindow::file_updated,file.updated)
 {
  wlist.insTop(inner);

  inner.bind(scroll_x.changed,scroll_y.changed);
 }

DDLWindow::~DDLWindow()
 {
 }

void DDLWindow::layout()
 {
  Pane all(Null,getSize());
  Pane pane(all);
  Coord delta_x=scroll_y.getMinSize().dx;
  Coord delta_y=scroll_x.getMinSize().dy;

  inner.setPlace(pane);

  if( inner.shortDY() )
    {
     Pane py=SplitX(pane,delta_x);

     inner.setPlace(pane);
     scroll_y.setPlace(py);

     wlist.insBottom(scroll_y);

     if( inner.shortDX() )
       {
        Pane px=SplitY(pane,delta_y);

        inner.setPlace(pane);
        scroll_x.setPlace(px);

        wlist.insBottom(scroll_x);
       }
     else
       {
        wlist.del(scroll_x);
       }
    }
  else
    {
     if( inner.shortDX() )
       {
        Pane px=SplitY(pane,delta_y);

        inner.setPlace(pane);

        if( inner.shortDY() )
          {
           pane=all;
           Pane py=SplitX(pane,delta_x);
           Pane px=SplitY(pane,delta_y);

           inner.setPlace(pane);
           scroll_x.setPlace(px);
           scroll_y.setPlace(py);

           wlist.insBottom(scroll_x);

           wlist.insBottom(scroll_y);
          }
        else
          {
           scroll_x.setPlace(px);

           wlist.insBottom(scroll_x);

           wlist.del(scroll_y);
          }
       }
     else
       {
        wlist.del(scroll_x);

        wlist.del(scroll_y);
       }
    }

  setScroll();
 }

/* class DisplayWindow */

void DisplayWindow::error(StrLen str)
 {
  msg.setInfo(str);

  if( msg.isDead() ) msg.create(getFrame(),"Error"_def);
 }

void DisplayWindow::setPretextFileName(StrLen file_name)
 {
  text_pretext.setText(SafeString(file_name));
 }

void DisplayWindow::setFileName(StrLen file_name)
 {
  text_file.setText(SafeString(file_name));
 }

DisplayWindow::DisplayWindow(SubWindowHost &host,const Config &cfg_)
 : ComboWindow(host),
   cfg(cfg_),

   label_pretext(wlist,cfg.label_cfg,"Pretext"_def),
   label_file(wlist,cfg.label_cfg,"File"_def),
   text_pretext(wlist,cfg.textline_cfg),
   text_file(wlist,cfg.textline_cfg),
   dline(wlist,cfg.dline_cfg),
   ddl(wlist,cfg.ddl_cfg,file),

   msg(getFrame()->getDesktop(),cfg.msg_cfg)
 {
  wlist.insTop(label_pretext,label_file,text_pretext,text_file,dline,ddl);
 }

DisplayWindow::~DisplayWindow()
 {
 }

 // methods

void DisplayWindow::open(StrLen file_name)
 {
  auto result=file.open(file_name);

  if( result.ok )
    {
     opened.assert(file_name,true);

     setFileName(file_name);

     redraw();
    }
  else
    {
     opened.assert(file_name,false);

     setFileName(Empty);

     redraw();

     error(result.etext.str());
    }
 }

void DisplayWindow::openPretext(StrLen file_name)
 {
  auto result=file.openPretext(file_name);

  if( result.ok )
    {
     setPretextFileName(file_name);

     redraw();
    }
  else
    {
     error(result.etext.str());
    }
 }

void DisplayWindow::noPretext()
 {
  file.noPretext();

  setPretextFileName(Empty);

  redraw();
 }

 // drawing

void DisplayWindow::layout()
 {
  PaneCut pane(getSize(),+cfg.space_dxy);

  pane.shrink();

  // label_pretext , text_pretext

  {
   auto label__pretext=CutPoint(label_pretext);
   auto text__pretext=CutPoint(text_pretext);

   Coord dy=SupDY(label__pretext,text__pretext);

   pane.cutTop(dy).place_cutLeftCenter(label__pretext).place(AlignCenterY(text__pretext));
  }

  // label_file , text_file

  {
   auto label__file=CutPoint(label_file);
   auto text__file=CutPoint(text_file);

   Coord dy=SupDY(label__file,text__file);

   pane.cutTop(dy).place_cutLeftCenter(label__file).place(AlignCenterY(text__file));
  }

  // dline

  pane.place_cutTop(dline);

  // ddl

  pane.place(ddl);
 }

void DisplayWindow::draw(DrawBuf buf,bool drag_active) const
 {
  buf.erase(+cfg.back);

  wlist.draw(buf,drag_active);
 }

void DisplayWindow::draw(DrawBuf buf,Pane pane,bool drag_active) const
 {
  buf.block_safe(pane,+cfg.back);

  wlist.draw(buf,pane,drag_active);
 }

} // namespace App



