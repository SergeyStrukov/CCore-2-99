/* TestFrame.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: UserPreference 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/TestFrame.h>

#include <CCore/inc/video/Layout.h>

namespace App {

/* class TestWindow */

const char *const TestWindow::InfoText=
"This is a test control panel.\n"
"You can see user preferences here.\n"
"Use main window to adjust sizes, colors etc...";

const char *const TestWindow::ListText=
"button\n"
"check box\n"
"radio button\n"
"text line\n"
"light\n"
"scroll\n"
"line edit";

void TestWindow::changeColor(int new_id,int)
 {
  switch( new_id )
    {
     case 1 : light.setFace(Red); break;

     case 2 : light.setFace(Green); break;

     case 3 : light.setFace(Blue); break;
    }
 }

void TestWindow::lightOnOff(bool check)
 {
  light.turn(check);

  edit.alert(check);
 }

void TestWindow::knobPressed()
 {
  text.setText(String(edit.getText()));
 }

void TestWindow::enableAll(bool en)
 {
  btn.enable(en);
  alt.enable(en);
  knob.enable(en);
  check.enable(en);
  rad1.enable(en);
  rad2.enable(en);
  rad3.enable(en);
  xscroll.enable(en);
  info.enable(en);
  edit.enable(en);
  text_list.enable(en);

  label1.enable(en);
  label2.enable(en);
  label3.enable(en);
  label.enable(en);
  text.enable(en);
 }

void TestWindow::setFace(ulen index)
 {
  knob.setFace(KnobShape::FaceType(index));
 }

void TestWindow::push()
 {
  auto pos=progress.getPos();

  if( pos<progress.getTotal() )
    {
     progress.setPosPing(pos+1);
    }
  else
    {
     progress.setPos(0);
     progress.stopPing();
    }
 }

void TestWindow::shade()
 {
  disableFrameReact();

  if( enable_window.isDead() ) enable_window.create(getFrame(),"TestFrame enable"_def);
 }

void TestWindow::unshade()
 {
  enableFrameReact();
 }

TestWindow::TestWindow(SubWindowHost &host,const UserPreference &pref_)
 : ComboWindow(host),
   pref(pref_),

   btn(wlist,pref.getButtonConfig(),"Push"_def),
   knob(wlist,pref.getKnobConfig(),KnobShape::FaceOk),
   check(wlist,pref.getCheckConfig()),
   swtch(wlist,pref.getSwitchConfig(),true),
   alt(wlist,pref.getFileAltConfig()),
   rad1(wlist,1,pref.getRadioConfig()),
   rad2(wlist,2,pref.getRadioConfig()),
   rad3(wlist,3,pref.getRadioConfig()),
   xscroll(wlist,pref.getXScrollConfig()),
   info(wlist,pref.getInfoConfig(),InfoFromString(InfoText)),
   edit(wlist,pref.getLineEditConfig()),
   text_list(wlist,pref.getSimpleTextListConfig(),InfoFromString(ListText)),

   label1(wlist,pref.getLabelConfig(),"Red"_def,AlignX_Left),
   label2(wlist,pref.getLabelConfig(),"Green"_def,AlignX_Left),
   label3(wlist,pref.getLabelConfig(),"Blue"_def,AlignX_Left),
   label(wlist,pref.getLabelConfig(),"On/Off"_def,AlignX_Left),
   text(wlist,pref.getTextConfig(),"<none>"_def),
   xsingle(wlist,pref.getXSingleLineConfig()),
   ysingle(wlist,pref.getYSingleLineConfig()),
   xdouble(wlist,pref.getXDoubleLineConfig()),
   ydouble(wlist,pref.getYDoubleLineConfig()),
   contour(wlist,pref.getContourConfig()),
   text_contour(wlist,pref.getTextContourConfig(),"Select color"_def),
   light(wlist,pref.getLightConfig(),Red),
   progress(wlist,pref.getProgressConfig()),
   shade_btn(wlist,pref.getButtonConfig(),"Shade"_def),

   enable_window(host.getFrame()->getDesktop(),pref.getMessageWindowConfig()),

   connector_group_changed(this,&TestWindow::changeColor,group.changed),
   connector_check_changed(this,&TestWindow::lightOnOff,check.changed),
   connector_knob_pressed(this,&TestWindow::knobPressed,knob.pressed),
   connector_swtch_changed(this,&TestWindow::enableAll,swtch.changed),
   connector_xscroll_changed(this,&TestWindow::setFace,xscroll.changed),
   connector_btn_pressed(this,&TestWindow::push,btn.pressed),
   connector_shade_btn_pressed(this,&TestWindow::shade,shade_btn.pressed),
   connector_enable_window_destoyed(this,&TestWindow::unshade,enable_window.destroyed)
 {
  wlist.insTop(swtch,btn,alt,rad1,rad2,rad3,check,edit,knob,xscroll,info,text_list,
               label1,label2,label3,label,text,xsingle,ysingle,xdouble,ydouble,
               contour,text_contour,light,progress,shade_btn);

  group.add(rad1,rad2,rad3);

  xscroll.setRange(KnobShape::FaceLim,1);

  edit.setText("To find our long-forgotten gold.");

  enable_window.setInfo(InfoFromString("Press Ok to enable"));

  enable_window.add("Ok"_def,Button_Ok);
 }

TestWindow::~TestWindow()
 {
 }

 // drawing

void TestWindow::layout()
 {
  Coord space_dxy=pref.get().space_dxy;

  PaneCut pane(getSize(),space_dxy);

  pane.shrink();

  // swtch , btn , progress

  {
   auto temp_swtch=CutBox(swtch);
   auto temp_btn=CutPoint(btn);
   auto temp_alt=CutPoint(alt);
   auto temp_progress=CutPoint(progress);

   temp_progress.size.x=10*temp_progress.size.y;

   Coord dy=SupDY(temp_swtch,temp_btn,temp_alt,temp_progress);

   PaneCut p=pane.cutTop(dy);

   p.place_cutLeft(temp_swtch)
    .place_cutLeftCenter(temp_btn)
    .place_cutLeftCenter(alt)
    .place_cutLeftCenter(temp_progress);
  }

  // text_contour , label , rad

  {
   auto temp_rad1=CutBox(rad1);
   auto temp_rad2=CutBox(rad2);
   auto temp_rad3=CutBox(rad3);

   auto temp_label1=CutPoint(label1);
   auto temp_label2=CutPoint(label2);
   auto temp_label3=CutPoint(label3);

   Coord line_dy=SupDY(temp_rad1,temp_label1);

   Coord label_dx=SupDX(temp_label1,temp_label2,temp_label3);

   Point inner_size( temp_rad1.getExt()+label_dx+3*space_dxy , 3*line_dy+4*space_dxy );

   Point size=text_contour.getMinSize(inner_size);

   pane.place_cutTopLeft(text_contour,size);

   {
    PaneCut pane(text_contour.getInner(),space_dxy);

    pane.shrink();

    pane.cutTop(line_dy).place_cutLeft(temp_rad1).place_cutLeft(temp_label1);
    pane.cutTop(line_dy).place_cutLeft(temp_rad2).place_cutLeft(temp_label2);
    pane.cutTop(line_dy).place_cutLeft(temp_rad3).place_cutLeft(temp_label3);
   }
  }

  {
   auto temp_check=CutBox(check);
   auto temp_label=CutPoint(label);
   auto temp_light=CutBox(light);

   Coord dy=SupDY(temp_check,temp_label,temp_light);

   PaneCut p=pane.cutTop(dy);

   p.place_cutLeft(temp_check)
    .place_cutLeft(temp_label)
    .place_cutLeft(temp_light);
  }

  // xsingle , edit , text , xdouble

  {
   pane.place_cutTop(xsingle);

   Point size=edit.getMinSize();

   size.x*=3;

   pane.place_cutTopLeft(edit,size);

   pane.place_cutTopLeft(text,size);

   pane.place_cutTop(xdouble);
  }

  // ysingle , knob , ydouble , xscroll

  {
   auto temp_knob=CutBox(knob);
   auto temp_xscroll=CutPoint(xscroll);

   temp_xscroll.size.x=10*temp_xscroll.size.y;

   Coord dy=SupDY(temp_knob,temp_xscroll);

   PaneCut p=pane.cutTop(dy);

   p.place_cutLeft(ysingle)
    .place_cutLeft(temp_knob)
    .place_cutLeft(ydouble)
    .place_cutLeftCenter(temp_xscroll);
  }

  // info

  {
   Point size=contour.getMinSize(info.getMinSize());

   pane.place_cutTopLeft(contour,size);

   info.setPlace(contour.getInner());
  }

  // text_list

  pane.place_cutTop(text_list);

  // shade_btn

  pane.place_cutTopLeft(shade_btn);
 }

void TestWindow::draw(DrawBuf buf,bool drag_active) const
 {
  VColor ground=pref.get().ground;

  buf.erase(ground);

  wlist.draw(buf,drag_active);
 }

void TestWindow::draw(DrawBuf buf,Pane pane,bool drag_active) const
 {
  VColor ground=pref.get().ground;

  buf.erase(pane,ground);

  wlist.draw(buf,pane,drag_active);
 }

/* class TestClient */

void TestClient::menu_off()
 {
  if( cascade_menu.isAlive() ) cascade_menu.destroy();

  menu.unselect();
 }

void TestClient::menu_selected(int id,Point point)
 {
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
       cascade_menu.create(getFrame(),menu_edit_data,point);
      }
     break;

     case 3 :
      {
       cascade_menu.create(getFrame(),menu_options_data,point);
      }
     break;

     case 5 :
      {
       cascade_menu.create(getFrame(),menu_window_data,point);
      }
     break;
    }
 }

void TestClient::cascade_menu_selected(int id,Point point)
 {
  switch( id )
    {
     case 101 :
      {
       if( !file_window.isAlive() ) file_window.create(point,"Select file"_def);
      }
     break;
    }

  cascade_menu.destroy();

  test.setFocus();

  menu.unselect();
 }

void TestClient::cascade_menu_pressed(VKey vkey,KeyMod kmod)
 {
  menu.put_Key(vkey,kmod);
 }

void TestClient::update()
 {
  cascade_menu.update();
 }

TestClient::TestClient(SubWindowHost &host,const UserPreference &pref,Signal<> &update)
 : ComboWindow(host),
   menu(wlist,pref.getSmartConfig(),menu_data),
   cascade_menu(host.getFrame()->getDesktop(),pref.getSmartConfig()),
   test(wlist,pref),
   file_window(host.getFrame()->getDesktop(),pref.getSmartConfig(),{true},update),
   connector_menu_selected(this,&TestClient::menu_selected,menu.selected),
   connector_cascade_menu_selected(this,&TestClient::cascade_menu_selected,cascade_menu.takeSelected()),
   connector_cascade_menu_pressed(this,&TestClient::cascade_menu_pressed,cascade_menu.takePressed()),
   connector_update(this,&TestClient::update,update)
 {
  wlist.insTop(menu,test);

  wlist.enableTabFocus(false);

  menu_data("@File",1)
           ("@Edit",2)
           (MenuSeparator)
           ("@Options",3)
           (MenuDisabled,"@Modules",4)
           ("@Window",5);

  menu_file_data("@New",101)
                ("@Open",102)
                (MenuDisabled,"@Save",103)
                (MenuDisabled,"Save @as",104)
                (MenuSeparator)
                ("E@xit",105);

  menu_edit_data("@Undo",201)
                ("@Check",202)
                (MenuSeparator)
                ("Cut",203)
                ("Copy",204)
                ("Paste",205)
                (MenuSeparator)
                ("@Run",206);

  menu_options_data("@Colors",301)
                   ("@Fonts",302)
                   ("@Targets",303);

  menu_window_data("@Split",501)
                  ("@Close all",502)
                  ("S@tack",503);

  file_window.addFilter("*.h");
  file_window.addFilter("*.cpp");
  file_window.addFilter("*");
 }

TestClient::~TestClient()
 {
 }

 // base

void TestClient::open()
 {
  wlist.open();

  test.setFocus();
 }

 // drawing

void TestClient::layout()
 {
  Coord dy=menu.getMinSize().y;

  Pane pane(Null,getSize());

  menu.setPlace(SplitY(dy,pane));
  test.setPlace(pane);
 }

 // user input

void TestClient::react(UserAction action)
 {
  action.dispatch(*this);
 }

void TestClient::react_Key(VKey vkey,KeyMod kmod)
 {
  if( vkey==VKey_F10 )
    {
     menu.setFocus();
    }
  else if( vkey==VKey_Esc )
    {
     menu.unselect();

     if( wlist.getFocus()==&menu ) test.setFocus();
    }
  else
    {
     wlist.put_Key(vkey,kmod);
    }
 }

void TestClient::react_LeftClick(Point point,MouseKey mkey)
 {
  menu_off();

  wlist.put_LeftClick(point,mkey);
 }

void TestClient::react_RightClick(Point point,MouseKey mkey)
 {
  menu_off();

  wlist.put_RightClick(point,mkey);
 }

void TestClient::react_other(UserAction action)
 {
  wlist.react(action);
 }

/* class TestFrame */

TestFrame::TestFrame(Desktop *desktop,const UserPreference &pref,Signal<> &update)
 : DragWindow(desktop,pref.getDragWindowConfig(),update),
   client(*this,pref,update)
 {
  bindClient(client);
 }

TestFrame::~TestFrame()
 {
 }

} // namespace App

