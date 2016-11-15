/* MessageWindow.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
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

void MessageSubWindow::Btn::pressed_id()
 {
  owner->finish.assert(btn_id);
 }

MessageSubWindow::Btn::Btn(SubWindowHost &host,const ButtonShape::Config &cfg,const DefString &name,int btn_id_,MessageSubWindow *owner_)
 : ButtonWindow(host,cfg,name),
   btn_id(btn_id_),
   owner(owner_),
   connector_pressed(this,&MessageSubWindow::Btn::pressed_id,pressed)
 {
 }

MessageSubWindow::Btn::~Btn()
 {
 }

/* class MessageSubWindow */

void MessageSubWindow::knobOk_pressed()
 {
  finish.assert(Button_Ok);
 }

MessageSubWindow::MessageSubWindow(SubWindowHost &host,const Config &cfg_)
 : SubWindow(host),
   cfg(cfg_),
   wlist(*this),
   dlist(*this),

   showInfo(wlist,cfg.info_cfg),
   dline(dlist,cfg.dline_cfg),
   knobOk(wlist,cfg.knob_cfg,KnobShape::FaceOk),

   btn_cfg(cfg.btn_cfg),

   connector_knobOk_pressed(this,&MessageSubWindow::knobOk_pressed,knobOk.pressed)
 {
  dlist.insTop(dline);

  wlist.enableTabFocus();
  wlist.enableClickFocus();
 }

MessageSubWindow::~MessageSubWindow()
 {
 }

 // methods

Point MessageSubWindow::getMinSize() const
 {
  Coord space_dxy=+cfg.space_dxy;

  Point size=showInfo.getMinSize().addXY(2*space_dxy);

  if( ulen count_=btn_list.getLen() )
    {
     Point bs;

     for(ulen ind=0; ind<count_ ;ind++) bs=Sup(bs,btn_list[ind]->getMinSize());

     Coord delta=IntAdd(bs.y,2*space_dxy);

     auto count=ToCoordinate(count_);

     auto total=count*bs.x+(count+1)*space_dxy;

     return Point(Max(+total,size.x),size.y+delta);
    }
  else
    {
     Coord knob_dxy=+cfg.knob_dxy;

     Coord delta=knob_dxy+2*space_dxy;

     return Point(Max(delta,size.x),size.y+delta);
    }
 }

MessageSubWindow & MessageSubWindow::setInfo(const Info &info)
 {
  showInfo.setInfo(info);

  return *this;
 }

MessageSubWindow & MessageSubWindow::add(const DefString &name,int btn_id)
 {
  btn_list.append_fill(OwnPtr<Btn>( new Btn(wlist,btn_cfg,name,btn_id,this) ));

  return *this;
 }

 // drawing

void MessageSubWindow::layout()
 {
  Point size=getSize();

  Coord space_dxy=+cfg.space_dxy;

  if( ulen count=btn_count )
    {
     Point bs;

     for(ulen ind=0; ind<count ;ind++) bs=Sup(bs,btn_list[ind]->getMinSize());

     Coord delta=IntAdd(bs.y,2*space_dxy);

     Pane top(Null,size);

     Pane bottom=SplitY(top,delta);

     showInfo.setPlace(top.shrink(space_dxy));

     dline.setPlace(EnvelopeY(bottom.getBase(),bottom.dx,space_dxy));

     PlaceRow row(bottom,bs,space_dxy,count);

     for(ulen ind=0; ind<count ;ind++)
       {
        btn_list[ind]->setPlace(*row);

        ++row;
       }
    }
  else
    {
     Coord knob_dxy=+cfg.knob_dxy;
     Coord delta=knob_dxy+2*space_dxy;

     Pane top(Null,size);

     Pane bottom=SplitY(top,delta);

     showInfo.setPlace(top.shrink(space_dxy));

     dline.setPlace(EnvelopeY(bottom.getBase(),bottom.dx,space_dxy));

     knobOk.setPlace(FreeCenter(bottom,knob_dxy));
    }
 }

void MessageSubWindow::draw(DrawBuf buf,bool drag_active) const
 {
  buf.erase(+cfg.back);

  dlist.draw(buf,drag_active);
  wlist.draw(buf,drag_active);
 }

void MessageSubWindow::draw(DrawBuf buf,Pane pane,bool drag_active) const
 {
  buf.block_safe(pane,+cfg.back);

  dlist.draw(buf,pane,drag_active);
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

     wlist.insBottom(showInfo);
    }
  else
    {
     wlist.insTop(knobOk,showInfo);
    }

  wlist.open();

  wlist.focusTop();

  dlist.open();
 }

void MessageSubWindow::close()
 {
  wlist.close();
  dlist.close();
 }

 // keyboard

FocusType MessageSubWindow::askFocus() const
 {
  return FocusTab;
 }

void MessageSubWindow::gainFocus()
 {
  wlist.gainFocus();
 }

void MessageSubWindow::looseFocus()
 {
  wlist.looseFocus();
 }

 // tab focus

void MessageSubWindow::topTabFocus()
 {
  wlist.topTabFocus();
 }

bool MessageSubWindow::nextTabFocus()
 {
  return wlist.nextTabFocus();
 }

void MessageSubWindow::bottomTabFocus()
 {
  wlist.bottomTabFocus();
 }

bool MessageSubWindow::prevTabFocus()
 {
  return wlist.prevTabFocus();
 }

 // mouse

void MessageSubWindow::looseCapture()
 {
  wlist.looseCapture();
 }

MouseShape MessageSubWindow::getMouseShape(Point point,KeyMod kmod) const
 {
  return wlist.getMouseShape(point,kmod,Mouse_Arrow);
 }

 // user input

void MessageSubWindow::react(UserAction action)
 {
  wlist.react(action);
 }

/* class MessageWindow */

void MessageWindow::finish(int btn_id_)
 {
  btn_id=btn_id_;

  destroy();
 }

MessageWindow::MessageWindow(Desktop *desktop,const Config &cfg_)
 : FixedWindow(desktop,cfg_.frame_cfg),
   cfg(cfg_),
   sub_win(*this,cfg.msg_cfg),

   connector_finish(this,&MessageWindow::finish,sub_win.finish)
 {
  bindClient(sub_win);
 }

MessageWindow::MessageWindow(Desktop *desktop,const Config &cfg,Signal<> &update)
 : MessageWindow(desktop,cfg)
 {
  bindUpdate(update);
 }

MessageWindow::~MessageWindow()
 {
 }

void MessageWindow::alive()
 {
  FixedWindow::alive();

  btn_id=Button_Cancel;
 }

Pane MessageWindow::getPane(StrLen title) const
 {
  return GetWindowPlace(desktop,+cfg.ypos_ratio,getMinSize(title,sub_win.getMinSize()));
 }

} // namespace Video
} // namespace CCore


