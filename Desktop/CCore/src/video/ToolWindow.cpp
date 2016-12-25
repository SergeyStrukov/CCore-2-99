/* ToolWindow.cpp */
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

#include <CCore/inc/video/ToolWindow.h>

namespace CCore {
namespace Video {

/* class ToolWindow */

void ToolWindow::guardClient()
 {
  if( !client ) GuardNoClient();
 }

void ToolWindow::guardDead()
 {
  if( isAlive() ) GuardNotDead();
 }

void ToolWindow::reset()
 {
  size=Null;
  client_enter=false;
  client_capture=false;
  delay_draw=false;
  enable_react=true;
 }

SubWindow & ToolWindow::getClient()
 {
  guardClient();

  return *client;
 }

void ToolWindow::shade(FrameBuf<DesktopColor> &buf)
 {
  if( !enable_react ) buf.erase(+cfg.shade_color,+cfg.shade_alpha);
 }

void ToolWindow::redraw(FuncArgType<FrameBuf<DesktopColor> &> func)
 {
  if( host->isDead() ) return;

  if( host->getToken() )
    {
     delay_draw=true;

     return;
    }

  FrameBuf<DesktopColor> buf(host->getDrawPlane());

  if( size<=buf.getSize() )
    {
     func(buf);
    }
  else
    {
     buf.erase(Black);

     host->invalidate(1);
    }
 }

ToolWindow::ToolWindow(Desktop *desktop,const Config &cfg_)
 : FrameWindow(desktop),
   cfg(cfg_),
   input(this)
 {
 }

ToolWindow::~ToolWindow()
 {
 }

 // methods

void ToolWindow::bindClient(SubWindow &client_)
 {
  guardDead();

  client=&client_;
 }

void ToolWindow::createMain(Pane pane,const DefString &title)
 {
  guardClient();

  has_focus=false;

  host->createMain(pane,pane.getSize());
  host->setTitle(title.str());
  host->display(CmdDisplay_Normal);
 }

void ToolWindow::create(Pane pane)
 {
  guardClient();

  has_focus=false;

  host->create(pane,pane.getSize());
  host->show();
 }

void ToolWindow::create(FrameWindow *parent,Pane pane)
 {
  guardClient();

  has_focus=false;

  host->create(parent->getHost(),pane,pane.getSize());
  host->show();
 }

void ToolWindow::redrawAll(bool do_layout)
 {
  if( do_layout )
    {
     if( client ) client->setPlace(Pane(Null,size));
    }

  redraw( [this] (FrameBuf<DesktopColor> &buf)
                 {
                  getClient().forward_draw(buf,false);

                  shade(buf);

                  host->invalidate(1);

                 } );
 }

void ToolWindow::redrawClient()
 {
  redraw( [this] (FrameBuf<DesktopColor> &buf)
                 {
                  getClient().forward_draw(buf,false);

                  shade(buf);

                  host->invalidate(Pane(Null,size),1);

                 } );
 }

void ToolWindow::redrawClient(Pane pane)
 {
  redraw( [this,pane] (FrameBuf<DesktopColor> &buf)
                      {
                       getClient().forward_draw(buf,pane,false);

                       shade(buf);

                       host->invalidate(pane,1);

                      } );
 }

 // SubWindowHost

FrameWindow * ToolWindow::getFrame()
 {
  return this;
 }

Point ToolWindow::getScreenOrigin()
 {
  Pane pane=host->getPlace();

  return pane.getBase();
 }

void ToolWindow::redraw(Pane pane)
 {
  input.redrawClient(pane);
 }

void ToolWindow::setFocus(SubWindow *)
 {
  if( has_focus )
    {
     getClient().gainFocus();
    }
 }

void ToolWindow::captureMouse(SubWindow *)
 {
  if( !client_capture )
    {
     client_capture=true;

     host->captureMouse();
    }
 }

void ToolWindow::releaseMouse(SubWindow *)
 {
  if( client_capture )
    {
     client_capture=false;

     host->releaseMouse();
    }
 }

 // base

void ToolWindow::alive()
 {
  reset();

  host->trackMouseHover();
  host->trackMouseLeave();

  getClient().open();
 }

void ToolWindow::dead()
 {
  getClient().close();
 }

void ToolWindow::askClose()
 {
  destroy();
 }

void ToolWindow::setSize(Point size_,bool buf_dirty)
 {
  if( size!=size_ || buf_dirty )
    {
     size=size_;

     redrawAll(true);
    }
 }

void ToolWindow::paintDone(unsigned)
 {
  if( delay_draw )
    {
     delay_draw=false;

     redrawAll();
    }
 }

 // keyboard

void ToolWindow::gainFocus()
 {
  has_focus=true;

  getClient().gainFocus();
 }

void ToolWindow::looseFocus()
 {
  has_focus=false;

  getClient().looseFocus();
 }

 // mouse

void ToolWindow::looseCapture()
 {
  if( client_capture )
    {
     client_capture=false;

     getClient().looseCapture();
    }
 }

void ToolWindow::setMouseShape(Point point,KeyMod kmod)
 {
  host->setMouseShape(getClient().forward_getMouseShape(point,kmod));
 }

 // user input

void ToolWindow::disableReact()
 {
  enable_react=false;

  redrawAll();
 }

void ToolWindow::enableReact()
 {
  enable_react=true;

  redrawAll();
 }

void ToolWindow::react(UserAction action)
 {
  if( enable_react ) action.dispatch(*this);
 }

void ToolWindow::react_other(UserAction action)
 {
  if( action.fromKeyboard() )
    {
     getClient().react(action);
    }
  else
    {
     Point point=action.getPoint();

     if( client_capture || Pane(Null,size).contains(point) )
       {
        getClient().forward_react(action);
       }
    }
 }

void ToolWindow::react_Move(Point point,MouseKey mkey)
 {
  if( Pane(Null,size).contains(point) )
    {
     client_enter=true;

     getClient().forward().put_Move(point,mkey);
    }
  else
    {
     if( client_capture ) getClient().forward().put_Move(point,mkey);

     if( client_enter )
       {
        client_enter=false;

        getClient().put_Leave();
       }
    }
 }

void ToolWindow::react_Leave()
 {
  if( client_enter )
    {
     client_enter=false;

     getClient().put_Leave();
    }
 }

} // namespace Video
} // namespace CCore

