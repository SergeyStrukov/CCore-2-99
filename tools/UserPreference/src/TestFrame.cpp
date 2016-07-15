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
 }

void TestWindow::knobPressed()
 {
  text.setText(edit.getText());
 }

void TestWindow::enableAll(bool en)
 {
  btn.enable(en);
  knob.enable(en);
  check.enable(en);
  rad1.enable(en);
  rad2.enable(en);
  rad3.enable(en);
  xscroll.enable(en);
  info.enable(en);
  edit.enable(en);

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

TestWindow::TestWindow(SubWindowHost &host,const UserPreference &pref_)
 : ComboWindow(host),
   pref(pref_),

   btn(wlist,pref.getButtonShapeConfig(),String("Push")),
   knob(wlist,pref.getKnobWindowConfig(),KnobShape::FaceOk),
   check(wlist,pref.getCheckWindowConfig()),
   swtch(wlist,pref.getSwitchWindowConfig(),true),
   rad1(wlist,1,pref.getRadioWindowConfig()),
   rad2(wlist,2,pref.getRadioWindowConfig()),
   rad3(wlist,3,pref.getRadioWindowConfig()),
   xscroll(wlist,pref.getXScrollConfig()),
   info(wlist,pref.getInfoConfig(),InfoFromString(InfoText)),
   edit(wlist,pref.getLineEditConfig()),
   text_list(wlist,pref.getSimpleTextListConfig(),InfoFromString(ListText)),

   label1(wlist,pref.getLabelWindowConfig(),String("Red"),AlignX_Left),
   label2(wlist,pref.getLabelWindowConfig(),String("Green"),AlignX_Left),
   label3(wlist,pref.getLabelWindowConfig(),String("Blue"),AlignX_Left),
   label(wlist,pref.getLabelWindowConfig(),String("On/Off"),AlignX_Left),
   text(wlist,pref.getTextWindowConfig(),String("<none>")),
   xsingle(wlist,pref.getXSingleLineConfig()),
   ysingle(wlist,pref.getYSingleLineConfig()),
   xdouble(wlist,pref.getXDoubleLineConfig()),
   ydouble(wlist,pref.getYDoubleLineConfig()),
   contour(wlist,pref.getContourConfig()),
   text_contour(wlist,pref.getTextContourConfig(),String("Select color")),
   light(wlist,pref.getLightConfig()),
   progress(wlist,pref.getProgressConfig()),

   connector_group_changed(this,&TestWindow::changeColor,group.changed),
   connector_check_changed(this,&TestWindow::lightOnOff,check.changed),
   connector_knob_pressed(this,&TestWindow::knobPressed,knob.pressed),
   connector_swtch_changed(this,&TestWindow::enableAll,swtch.changed),
   connector_xscroll_changed(this,&TestWindow::setFace,xscroll.changed),
   connector_btn_pressed(this,&TestWindow::push,btn.pressed)
 {
  wlist.insTop(swtch,btn,rad1,rad2,rad3,check,edit,knob,xscroll,info,text_list,
               label1,label2,label3,label,text,xsingle,ysingle,xdouble,ydouble,contour,text_contour,light,progress);

  group.add(rad1,rad2,rad3);

  light.setFace(Red);

  xscroll.setRange(KnobShape::FaceLim,1);

  edit.setText("To find our long-forgotten gold.");
 }

TestWindow::~TestWindow()
 {
 }

 // drawing

void TestWindow::layout()
 {
  Coord space_dxy=pref.get().space_dxy;

  Pointsor psor(space_dxy);

  // swtch , btn , progress

  {
   Point size=btn.getMinSize();

   Pointsor psor1=psor.cutY(size.y);

   psor1.placeX(swtch,size.y,CenterFunc(pref.get().swtch_dxy));

   psor1.placeX(btn,size);

   Coord pdy=pref.get().progress_dy;
   Coord pdx=10*pdy;

   psor1.placeX(progress,pdx,size.y,CenterYFunc(pdy));
  }

  Point lsize=Sup(label1.getMinSize(),label2.getMinSize(),label3.getMinSize());

  Point inner_size(lsize.x+3*space_dxy+lsize.y,3*lsize.y+4*space_dxy);

  // text_contour

  {
   Point size=text_contour.getMinSize(inner_size);

   psor.placeY(text_contour,size);
  }

  // label , rad

  {
   Panesor psor(text_contour.getInner(),space_dxy);

   psor.shrink();

   {
    Panesor psor1=psor.cutY(lsize.y);

    psor1.placeX(rad1,lsize.y,CenterFunc(pref.get().check_dxy));

    psor1.placeX(label1,lsize.x);
   }

   {
    Panesor psor1=psor.cutY(lsize.y);

    psor1.placeX(rad2,lsize.y,CenterFunc(pref.get().check_dxy));

    psor1.placeX(label2,lsize.x);
   }

   {
    Panesor psor1=psor.cutY(lsize.y);

    psor1.placeX(rad3,lsize.y,CenterFunc(pref.get().check_dxy));

    psor1.placeX(label3,lsize.x);
   }
  }

  // label , check , light

  {
   Point size=label.getMinSize();

   Pointsor psor1=psor.cutY(size.y);

   psor1.placeX(check,size.y,CenterFunc(pref.get().check_dxy));

   psor1.placeX(label,size);

   psor1.placeX(light,size.y,CenterFunc(pref.get().light_dxy));
  }

  // xsingle , text , edit , xdouble

  {
   Point size=edit.getMinSize();

   size.x*=3;
   size.y+=space_dxy;

   psor.placeMinY(xsingle,size.x,MinSize);

   psor.placeY(edit,size);

   psor.placeY(text,size);

   psor.placeMinY(xdouble,size.x,MinSize);
  }

  // ysingle , knob , ydouble , xscroll

  {
   Coord dy=btn.getSize().y;

   Pointsor psor1=psor.cutY(dy);

   psor1.placeMinX(ysingle,MinSize,dy);

   psor1.placeX(knob,dy,CenterFunc(pref.get().knob_dxy));

   psor1.placeMinX(ydouble,MinSize,dy);

   Coord sdy=pref.get().scroll_dxy;
   Coord sdx=10*sdy;

   psor1.placeX(xscroll,sdx,dy,CenterYFunc(sdy));
  }

  // info

  {
   Point size=contour.getMinSize(info.getMinSize());

   psor.placeY(contour,size);

   info.setPlace(contour.getInner());
  }

  // text_list

  {
   psor.placeMinY(text_list);
  }
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

  buf.block_safe(pane,ground);

  wlist.draw(buf,pane,drag_active);
 }

/* class TestFrame */

TestFrame::TestFrame(Desktop *desktop,const UserPreference &pref,Signal<> &update)
 : DragWindow(desktop,pref.getDragWindowConfig(),update),
   test(*this,pref)
 {
  bindClient(test);
 }

TestFrame::~TestFrame()
 {
 }

} // namespace App

