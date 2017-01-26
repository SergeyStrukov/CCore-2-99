/* Client.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: ContourLab 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2017 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/Client.h>

namespace App {

/* class ClientWindow */

void ClientWindow::menuOff()
 {
  if( cascade_menu.isAlive() ) cascade_menu.destroy();

  menu.unselect();
 }

void ClientWindow::fileOff()
 {
  if( file_frame.isAlive() )
    {
     cont=ContinueNone;

     file_frame.destroy();
    }
 }

void ClientWindow::msgOff()
 {
  if( msg_frame.isAlive() )
    {
     cont=ContinueNone;

     msg_frame.destroy();
    }
 }

void ClientWindow::askSave(Continue cont_)
 {
  msgOff();

  cont=cont_;

  msg_frame.create(getFrame(),+cfg.text_Alert);

  disableFrameReact();
 }

void ClientWindow::startOpen(Point point)
 {
  fileOff();

  file_frame.setNewFile(false);

  cont=ContinueOpen;

  file_frame.create(getFrame(),point,+cfg.text_LoadFile);

  disableFrameReact();
 }

void ClientWindow::startSave(Point point)
 {
  fileOff();

  file_frame.setNewFile(true);

  cont=ContinueSaveAs;

  file_frame.create(getFrame(),point,+cfg.text_SaveFile);

  disableFrameReact();
 }

void ClientWindow::menu_selected(int id,Point point)
 {
  if( cascade_menu.isAlive() ) cascade_menu.destroy();

  switch( id )
    {
     case MenuFile :
      {
       cascade_menu.create(getFrame(),menu_file_data,point);
      }
     break;

     case MenuOptions :
      {
       cascade_menu.create(getFrame(),menu_opt_data,point);
      }
     break;
    }
 }

void ClientWindow::cascade_menu_selected(int id,Point point) // TODO
 {
  switch( id )
    {
     case MenuFileNew :
      {
       if( editor.isModified() )
         {
          askSave(ContinueNew);
         }
       else
         {
          editor.load();
         }
      }
     break;

     case MenuFileOpen :
      {
       if( editor.isModified() )
         {
          file_point=point;

          askSave(ContinueStartOpen);
         }
       else
         {
          startOpen(point);
         }
      }
     break;

     case MenuFileSave :
      {
       if( editor.save() ) break;
      }
     // falldown;

     case MenuFileSaveAs :
      {
       startSave(point);
      }
     break;

     case MenuFileClose :
      {
       if( editor.isModified() )
         {
          askSave(ContinueClose);
         }
       else
         {
          editor.close();
         }
      }
     break;

     case MenuFileExit :
      {
       askFrameClose();
      }
     break;

     case MenuOptionsUserPref :
      {
       // TODO
      }
     break;

     case MenuOptionsAppPref :
      {
       // TODO
      }
     break;
    }

  cascade_menu.destroy();

  editor.setFocus();

  menu.unselect();
 }

void ClientWindow::file_destroyed()
 {
  enableFrameReact();

  switch( Replace(cont,ContinueNone) )
    {
     case ContinueOpen :
      {
       StrLen file_name=file_frame.getFilePath();

       if( +file_name )
         {
          editor.load(file_name);
         }
      }
     break;

     case ContinueSaveAs :
      {
       StrLen file_name=file_frame.getFilePath();

       if( +file_name )
         {
          editor.save(file_name);
         }
      }
     break;
    }
 }

void ClientWindow::msg_destroyed()
 {
  enableFrameReact();

  switch( msg_frame.getButtonId() )
    {
     case Button_Yes :
      {
       editor.save();
      }
     break;

     case Button_Cancel : return;
    }

  switch( Replace(cont,ContinueNone) )
    {
     case ContinueNew :
      {
       editor.load();
      }
     break;

     case ContinueStartOpen :
      {
       startOpen(file_point);
      }
     break;

     case ContinueClose :
      {
       editor.close();
      }
     break;

     case ContinueExit :
      {
       getFrame()->destroy();
      }
     break;
    }
 }

void ClientWindow::cascade_menu_pressed(VKey vkey,KeyMod kmod)
 {
  menu.put_Key(vkey,kmod);
 }

ClientWindow::ClientWindow(SubWindowHost &host,const Config &cfg_)
 : ComboWindow(host),
   cfg(cfg_),

   menu(wlist,cfg.menu_cfg,menu_data),
   cascade_menu(host.getFrameDesktop(),cfg.cascade_menu_cfg),
   editor(wlist,cfg.editor_cfg),
   file_frame(host.getFrameDesktop(),cfg.file_cfg,{true}),
   msg_frame(host.getFrameDesktop(),cfg.msg_cfg),

   connector_menu_selected(this,&ClientWindow::menu_selected,menu.selected),
   connector_cascade_menu_selected(this,&ClientWindow::cascade_menu_selected,cascade_menu.selected),
   connector_cascade_menu_pressed(this,&ClientWindow::cascade_menu_pressed,cascade_menu.pressed),
   connector_file_destroyed(this,&ClientWindow::file_destroyed,file_frame.destroyed),
   connector_msg_destroyed(this,&ClientWindow::msg_destroyed,msg_frame.destroyed)
 {
  wlist.insTop(menu,editor);

  wlist.enableTabFocus(false);

  menu_data("@File",MenuFile)
           ("@Options",MenuOptions);

  menu_file_data("@New",MenuFileNew)
                ("@Open",MenuFileOpen)
                (MenuSeparator)
                ("@Save",MenuFileSave)
                ("Save @as",MenuFileSaveAs)
                (MenuSeparator)
                ("@Close",MenuFileClose)
                (MenuSeparator)
                ("E@xit",MenuFileExit);

  menu_opt_data("@Global",MenuOptionsUserPref)
               ("@Application",MenuOptionsAppPref);

  file_frame.addFilter("*.cont.ddl"_c);
  file_frame.addFilter("*"_c,false);

  msg_frame.setInfo(cfg.text_AskSave.get().str())
           .add(+cfg.text_Yes,Button_Yes)
           .add(+cfg.text_No,Button_No)
           .add(+cfg.text_Cancel,Button_Cancel);
 }

ClientWindow::~ClientWindow()
 {
 }

 // base

void ClientWindow::open()
 {
  wlist.open();

  editor.setFocus();
 }

 // drawing

void ClientWindow::layout()
 {
  Coord dy=menu.getMinSize().dy;

  Pane pane(Null,getSize());

  menu.setPlace(SplitY(dy,pane));
  editor.setPlace(pane);
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

     if( wlist.getFocus()==&menu ) editor.setFocus();
    }
  else
    {
     wlist.put_Key(vkey,kmod);
    }
 }

void ClientWindow::react_LeftClick(Point point,MouseKey mkey)
 {
  menuOff();

  wlist.put_LeftClick(point,mkey);
 }

void ClientWindow::react_RightClick(Point point,MouseKey mkey)
 {
  menuOff();

  wlist.put_RightClick(point,mkey);
 }

void ClientWindow::react_other(UserAction action)
 {
  wlist.react(action);
 }

 // AliveControl

bool ClientWindow::askDestroy()
 {
  if( editor.isModified() )
    {
     askSave(ContinueExit);

     return false;
    }

  return true;
 }

} // namespace App
