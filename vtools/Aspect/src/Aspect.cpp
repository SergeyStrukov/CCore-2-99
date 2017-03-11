/* Aspect.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: Aspect 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2017 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/Aspect.h>

#include <CCore/inc/video/Layout.h>

#include <CCore/inc/Exception.h>

namespace App {

/* class AspectWindow */

void AspectWindow::setAspect(StrLen file_name)
 {
  StrLen path=text_path.getText().str();

  if( !path )
    {
     errorMsg(+cfg.text_Nothing);

     return;
    }

  aspect_file_name=String(file_name);

  RelPath rel(path,file_name);

  if( +rel )
    text_aspect.setText(String(rel.getPath()));
  else
    text_aspect.setText(aspect_file_name);
 }

void AspectWindow::errorMsg(StrLen text)
 {
  msg_frame.setInfo(String(text));

  msg_frame.create(getFrame(),+cfg.text_Error);

  disableFrameReact();
 }

void AspectWindow::msg_destroyed()
 {
  enableFrameReact();
 }

AspectWindow::AspectWindow(SubWindowHost &host,const Config &cfg_)
 : ComboWindow(host),
   cfg(cfg_),

   label_path(wlist,cfg.label_cfg,cfg.text_Path),
   label_aspect(wlist,cfg.label_cfg,cfg.text_Aspect),

   text_path(wlist,cfg.text_cfg),
   text_aspect(wlist,cfg.text_cfg),

   msg_frame(host.getFrameDesktop(),cfg.msg_cfg),

   connector_msg_destroyed(this,&AspectWindow::msg_destroyed,msg_frame.destroyed)
 {
  wlist.insTop(label_path,label_aspect,text_path,text_aspect);
 }

AspectWindow::~AspectWindow()
 {
 }

 // methods

Point AspectWindow::getMinSize() const // TODO
 {
  return Point(100,100);
 }

void AspectWindow::blank() // TODO
 {
  data.blank(".");

  text_path.setText(data.getPath());

  redraw();
 }

void AspectWindow::load(StrLen file_name) // TODO
 {
  Used(file_name);
 }

bool AspectWindow::save()
 {
  if( !has_file ) return false;

  ErrorText etext;

  data.save(Range(aspect_file_name),etext);

  if( !etext )
    {
     errorMsg(etext.getText());
    }
  else
    {
     clearModified();
    }

  return true;
 }

void AspectWindow::save(StrLen file_name)
 {
  setAspect(file_name);
  setModified();
  has_file=true;

  save();
 }

 // drawing

void AspectWindow::layout() // TODO
 {
  PaneCut pane(getSize(),+cfg.space_dxy);

  pane.shrink();

  // label_path , label_aspect , text_path , text_aspect

  {
   auto label__path=CutPoint(label_path);
   auto label__aspect=CutPoint(label_aspect);
   auto text__path=CutPoint(text_path);
   auto text__aspect=CutPoint(text_aspect);

   Coord dy=SupDY(label__path,text__path);

   pane.cutTop(dy).place_cutLeft(label__path).place(text__path);

   pane.cutTop(dy).place_cutLeft(label__aspect).place(text__aspect);
  }
 }

void AspectWindow::drawBack(DrawBuf buf,bool) const
 {
  buf.erase(+cfg.back);
 }

} // namespace App
