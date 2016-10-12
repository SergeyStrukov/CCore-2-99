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

#include <CCore/inc/Print.h>

namespace App {

/* class DisplayWindow */

DisplayWindow::DisplayWindow(SubWindowHost &host,const Config &cfg_)
 : SubWindow(host),
   cfg(cfg_)
 {
 }

DisplayWindow::~DisplayWindow()
 {
 }

/* class ClientWindow */

void ClientWindow::menu_selected(int id,Point point)
 {
  Printf(Con,"id = #;\n",id);

  menu.unselect();

  switch( id )
    {
     case 1 :
      {
       if( menu_file.isDead() ) menu_file.create(getFrame(),point);
      }
     break;
    }
 }

void ClientWindow::menu_file_selected(int id,Point point)
 {
  Printf(Con,"id = #;\n",id);

  menu_file.destroy();
 }

ClientWindow::ClientWindow(SubWindowHost &host,const Config &cfg_)
 : ComboWindow(host),
   cfg(cfg_),
   menu(wlist,cfg.menu_cfg,menu_data),
   menu_file(host.getFrame()->getDesktop(),cfg.cascade_menu_cfg,menu_file_data),
   display(wlist,cfg.display_cfg),
   connector_menu_selected(this,&ClientWindow::menu_selected,menu.selected),
   connector_menu_file_selected(this,&ClientWindow::menu_file_selected,menu_file.takeSelected())
 {
  wlist.insTop(menu,display);

  wlist.enableTabFocus(false);

  menu_data("@File",1)
           ("@Options",2)
           (MenuSeparator)
           ("E@xit",3)
           (MenuSeparator)
           ("Ex@tra",4)
           ("W@indow",5)
           ("Long line",6)
           (MenuHidden,"Long long line",7)
           ("DDL options",8);

  menu_file_data("@New",101)
                ("@Open",102)
                ("@Save",103)
                ("Save @as",103)
                (MenuSeparator)
                ("E@xit",104);
 }

ClientWindow::~ClientWindow()
 {
 }

void ClientWindow::open()
 {
  wlist.open();

  display.setFocus();
 }

void ClientWindow::layout()
 {
  Coord dy=menu.getMinSize().y;

  Pane pane(Null,getSize());

  menu.setPlace(SplitY(dy,pane));
  display.setPlace(pane);
 }

 // user input

void ClientWindow::react(UserAction action)
 {
  action.dispatch(*this);
 }

void ClientWindow::react_Key(VKey vkey,KeyMod kmod)
 {
  if( vkey==VKey_F10 )
    {
     menu.setFocus();
    }
  else if( vkey==VKey_Esc )
    {
     if( menu.getState()==MenuSelect ) menu.unselect();

     if( wlist.getFocus()==&menu ) display.setFocus();
    }
  else
    {
     wlist.put_Key(vkey,kmod);
    }
 }

void ClientWindow::react_other(UserAction action)
 {
  //Printf(Con,"#;\n",action);

  wlist.react(action);
 }

} // namespace App



