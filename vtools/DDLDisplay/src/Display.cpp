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

void ClientWindow::menu_off()
 {
  menu.unselect();

  if( cascade_menu.isAlive() ) cascade_menu.destroy();
 }

void ClientWindow::menu_selected(int id,Point point)
 {
  Printf(Con,"id = #;\n",id);

  if( cascade_menu.isAlive() ) cascade_menu.destroy();

  switch( id )
    {
     case 1 :
      {
       cascade_menu.create(getFrame(),menu_file_data,point);
      }
     break;

     case 2 :
      {
       cascade_menu.create(getFrame(),menu_option_data,point);
      }
     break;
    }
 }

void ClientWindow::cascade_menu_selected(int id,Point point)
 {
  Used(point);

  Printf(Con,"id = #;\n",id);

  cascade_menu.destroy();

  display.setFocus();

  menu.unselect();
 }

void ClientWindow::cascade_menu_pressed(VKey vkey,KeyMod kmod)
 {
  menu.put_Key(vkey,kmod);
 }

ClientWindow::ClientWindow(SubWindowHost &host,const Config &cfg_)
 : ComboWindow(host),
   cfg(cfg_),
   menu(wlist,cfg.menu_cfg,menu_data),
   cascade_menu(host.getFrame()->getDesktop(),cfg.cascade_menu_cfg),
   display(wlist,cfg.display_cfg),
   connector_menu_selected(this,&ClientWindow::menu_selected,menu.selected),
   connector_cascade_menu_selected(this,&ClientWindow::cascade_menu_selected,cascade_menu.takeSelected()),
   connector_cascade_menu_pressed(this,&ClientWindow::cascade_menu_pressed,cascade_menu.takePressed())
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
                (MenuDisabled,"@Save",103)
                (MenuDisabled,"Save @as",103)
                (MenuSeparator)
                ("E@xit",104)
                (MenuHidden,"Hidden",1000)
                ("extra 1",1001)
                ("extra 2",1002)
                ("extra 3",1003)
                ("extra 4",1004)
                ("extra 5",1005)
                ("extra 6",1006)
                ("extra 7",1007)
                ("extra 8",1008)
                ("extra 9",1009);

  menu_option_data("@DDL",201)
                  ("@Colors",202)
                  ("@Folding",203);
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
     menu.unselect();

     if( wlist.getFocus()==&menu ) display.setFocus();
    }
  else
    {
     wlist.put_Key(vkey,kmod);
    }
 }

void ClientWindow::react_LeftClick(Point point,MouseKey mkey)
 {
  menu_off();

  wlist.put_LeftClick(point,mkey);
 }

void ClientWindow::react_RightClick(Point point,MouseKey mkey)
 {
  menu_off();

  wlist.put_RightClick(point,mkey);
 }

void ClientWindow::react_other(UserAction action)
 {
  wlist.react(action);
 }

} // namespace App



