/* MessageWindow.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 3.00
//
//  Tag: Desktop
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <CCore/inc/video/MessageWindow.h>

#include <CCore/inc/video/Layout.h>

namespace CCore {
namespace Video {

/* class MessageSubWindow::Btn */

void MessageSubWindow::Btn::finish()
 {
  owner->finish.assert(btn_id);
 }

MessageSubWindow::Btn::Btn(SubWindowHost &host,const ButtonWindow::ConfigType &cfg,const DefString &name,int btn_id_,MessageSubWindow *owner_)
 : ButtonWindow(host,cfg,name),
   btn_id(btn_id_),
   owner(owner_),
   connector_pressed(this,&Btn::finish,pressed)
 {
 }

MessageSubWindow::Btn::~Btn()
 {
 }

/* class MessageSubWindow */

void MessageSubWindow::knob_pressed()
 {
  finish.assert(Button_Ok);
 }

MessageSubWindow::MessageSubWindow(SubWindowHost &host,const Config &cfg_)
 : ComboWindow(host),
   cfg(cfg_),
   btn_cfg(cfg.btn_cfg),

   info(wlist,cfg.info_cfg),
   dline(wlist,cfg.dline_cfg),
   knob(wlist,cfg.knob_cfg,KnobShape::FaceOk),

   connector_knob_pressed(this,&MessageSubWindow::knob_pressed,knob.pressed)
 {
 }

MessageSubWindow::~MessageSubWindow()
 {
 }

 // methods

Point MessageSubWindow::getMinSize() const
 {
  Coordinate space_dxy=+cfg.space_dxy;

  Coordinate space2=2*space_dxy;

  Point top=info.getMinSize().addXY(+space2);

  Coordinate line_dy=dline.getMinSize().dy;

  Point btn;

  if( ulen count_=btn_list.getLen() )
    {
     auto list=Range(btn_list.getPtr(),count_);

     Point bs=BtnSize(list);

     auto count=CountToCoordinate(count_);

     auto total=count*bs.x+(count-1)*space_dxy;

     btn.x=+total;
     btn.y=bs.y;
    }
  else
    {
     btn=Point::Diag(+cfg.knob_dxy);
    }

  Point bottom=btn.addXY(+space2);

  return Point( Max(bottom.x,top.x) , bottom.y+line_dy+top.y );
 }

MessageSubWindow & MessageSubWindow::setInfo(const Info &info_)
 {
  info.setInfo(info_);

  return *this;
 }

MessageSubWindow & MessageSubWindow::add(const DefString &name,int btn_id)
 {
  btn_list.append_fill(OwnPtr<Btn>( new Btn(wlist,btn_cfg,name,btn_id,this) ));

  return *this;
 }

 // drawing

bool MessageSubWindow::isGoodSize(Point size) const
 {
  return size>=getMinSize();
 }

void MessageSubWindow::layout()
 {
  Coord space_dxy=+cfg.space_dxy;

  PaneCut pane(getSize(),0);

  pane.cutBottom(space_dxy);

  if( ulen count=btn_count )
    {
     auto list=Range(btn_list.getPtr(),count);

     Point bs=BtnSize(list);

     PlaceRow row(pane.cutBottom(bs.y),bs,space_dxy,count);

     for(OwnPtr<Btn> &obj : list )
       {
        obj->setPlace(*row);

        ++row;
       }
    }
  else
    {
     Coord knob_dxy=+cfg.knob_dxy;

     knob.setPlace( pane.cutBottomCenter(knob_dxy,knob_dxy) );
    }

  pane.cutBottom(space_dxy);

  Pane top=pane.place_cutBottom(dline);

  info.setPlace( top.shrink(space_dxy) );
 }

void MessageSubWindow::draw(DrawBuf buf,bool drag_active) const
 {
  buf.erase(+cfg.back);

  wlist.draw(buf,drag_active);
 }

void MessageSubWindow::draw(DrawBuf buf,Pane pane,bool drag_active) const
 {
  buf.erase(pane,+cfg.back);

  wlist.draw(buf,pane,drag_active);
 }

 // base

void MessageSubWindow::open()
 {
  wlist.delAll();

  btn_count=btn_list.getLen();

  if( btn_count )
    {
     btn_list.apply( [this] (OwnPtr<Btn> &obj) { wlist.insBottom(obj.getPtr()); } );

     wlist.insBottom(info);
    }
  else
    {
     wlist.insTop(knob,info);
    }

  wlist.insBottom(dline);

  wlist.open();

  wlist.focusTop();
 }

/* class MessageWindow */

void MessageWindow::finish(int btn_id_)
 {
  btn_id=btn_id_;

  destroy();
 }

MessageWindow::MessageWindow(Desktop *desktop,const Config &cfg_)
 : FixedFrame(desktop,cfg_.frame_cfg),
   cfg(cfg_),
   sub_win(*this,cfg.msg_cfg),

   connector_finish(this,&MessageWindow::finish,sub_win.finish)
 {
  bindClient(sub_win);
 }

MessageWindow::MessageWindow(Desktop *desktop,const Config &cfg,Signal<> &update)
 : MessageWindow(desktop,cfg)
 {
  connectUpdate(update);
 }

MessageWindow::~MessageWindow()
 {
 }

void MessageWindow::alive()
 {
  FixedFrame::alive();

  btn_id=Button_Cancel;
 }

Pane MessageWindow::getPane(bool is_main,StrLen title) const
 {
  return GetWindowPlace(desktop,+cfg.pos_ry,getMinSize(is_main,title,sub_win.getMinSize()));
 }

} // namespace Video
} // namespace CCore


