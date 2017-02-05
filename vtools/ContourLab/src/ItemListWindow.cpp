/* ItemListWindow.cpp */
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

#include <inc/ItemListWindow.h>

#include <CCore/inc/video/Layout.h>

namespace App {

/* class EditFormulaShape */

void EditFormulaShape::drawText(Font font,const DrawBuf &buf,Pane pane,TextPlace place,StrLen text,ulen off,VColor) const
 {
  const Config &cfg=getCfg();

  VColor normal_text=+cfg.text;
  VColor error_text=+cfg.error_text;
  VColor number_text=+cfg.number_text;
  VColor length_text=+cfg.length_text;
  VColor angle_text=+cfg.angle_text;
  VColor name_text=+cfg.name_text;
  VColor punct_text=+cfg.punct_text;

  auto map = [=] (CharAccent accent) -> VColor
                 {
                  switch( accent )
                    {
                     case CharNormal : return normal_text;

                     case CharNumber : return number_text;

                     case CharLength : return length_text;

                     case CharAngle : return angle_text;

                     case CharName : return name_text;

                     case CharPunct : return punct_text;

                     default: return error_text;
                    }
                 } ;

  CharAccent *accent=this->accent;

  auto func = [=] (ulen index,char,Point,Point) -> VColor { return map(accent[off+index]); } ;

  auto proxy=ToFunction<VColor (ulen index,char ch,Point base,Point delta)>(func);

  font->text(buf,pane,place,text,proxy.function());
 }

/* class EditFormulaWindow */

void EditFormulaWindow::tick()
 {
  if( time )
    {
     time--;
    }
  else
    {
     defer_tick.stop();

     paused.assert();
    }
 }

void EditFormulaWindow::start()
 {
  const ConfigType &cfg=shape.getCfg();

  time=+cfg.delay;

  defer_tick.start();
 }

EditFormulaWindow::EditFormulaWindow(SubWindowHost &host,const ConfigType &cfg)
 : LineEditWindowOf<EditFormulaShape>(host,cfg),
   storage(DefBufLen),

   connector_changed(this,&EditFormulaWindow::start,changed)
 {
  shape.accent=storage.getPtr();

  defer_tick=input.create(static_cast<void (LineEditWindowOf<EditFormulaShape>::*)()>(&EditFormulaWindow::tick));
 }

EditFormulaWindow::~EditFormulaWindow()
 {
 }

void EditFormulaWindow::normalAccent()
 {
  if( Change(flag,true) ) Range(shape.accent,shape.len).set(CharNormal);
 }

/* class ItemListWindow */

void ItemListWindow::list_selected(ulen index)
 {
  ulen count=list.getInfo()->getLineCount();

  knob_down.enable( index<count-1 );

  knob_up.enable( index>0 );

  knob_del.enable();

  check_show.enable();
  check_gray.enable();
  check_name.enable();

  selected.assert(index);
 }

void ItemListWindow::knob_down_pressed()
 {
  command_down.assert(list.getSelect());
 }

void ItemListWindow::knob_up_pressed()
 {
  command_up.assert(list.getSelect());
 }

void ItemListWindow::knob_del_pressed()
 {
  command_del.assert(list.getSelect());
 }

void ItemListWindow::knob_add_pressed()
 {
  command_add.assert(list.getSelect());
 }

void ItemListWindow::check_show_changed(bool check)
 {
  show_changed.assert(list.getSelect(),check);
 }

void ItemListWindow::check_gray_changed(bool check)
 {
  gray_changed.assert(list.getSelect(),check);
 }

void ItemListWindow::check_name_changed(bool check)
 {
  name_changed.assert(list.getSelect(),check);
 }

ItemListWindow::ItemListWindow(SubWindowHost &host,const Config &cfg_)
 : ComboWindow(host),
   cfg(cfg_),

   list(wlist,cfg.list_cfg),
   edit(wlist,cfg.edit_cfg),

   knob_down(wlist,cfg.knob_cfg,KnobShape::FaceDown),
   knob_up(wlist,cfg.knob_cfg,KnobShape::FaceUp),
   knob_del(wlist,cfg.knob_cfg,KnobShape::FaceCross),
   knob_add(wlist,cfg.knob_cfg,KnobShape::FacePlus),

   label_show(wlist,cfg.label_cfg,cfg.text_show),
   label_gray(wlist,cfg.label_cfg,cfg.text_gray),
   label_name(wlist,cfg.label_cfg,cfg.text_name),

   check_show(wlist,cfg.check_cfg),
   check_gray(wlist,cfg.check_cfg),
   check_name(wlist,cfg.check_cfg),

   connector_list_selected(this,&ItemListWindow::list_selected,list.selected),
   connector_edit_entered(this,&ItemListWindow::knob_add_pressed,edit.entered),
   connector_knob_down_pressed(this,&ItemListWindow::knob_down_pressed,knob_down.pressed),
   connector_knob_up_pressed(this,&ItemListWindow::knob_up_pressed,knob_up.pressed),
   connector_knob_del_pressed(this,&ItemListWindow::knob_del_pressed,knob_del.pressed),
   connector_knob_add_pressed(this,&ItemListWindow::knob_add_pressed,knob_add.pressed),

   connector_check_show_changed(this,&ItemListWindow::check_show_changed,check_show.changed),
   connector_check_gray_changed(this,&ItemListWindow::check_gray_changed,check_gray.changed),
   connector_check_name_changed(this,&ItemListWindow::check_name_changed,check_name.changed),

   text_changed(edit.changed),
   text_paused(edit.paused)
 {
  wlist.insTop(knob_down,knob_up,knob_del,list,label_show,label_gray,label_name,check_show,check_gray,check_name,edit,knob_add);

  noItem();

  enableAdd(false);
 }

ItemListWindow::~ItemListWindow()
 {
 }

 // methods

Point ItemListWindow::getMinSize(Point cap) const
 {
  Coord space=+cfg.space_dxy;

  Coord knob_dxy=knob_down.getMinSize().dxy;
  Coord kspace=BoxSpace(knob_dxy);

  Coord check_dxy=check_show.getMinSize().dxy;
  Coord label_dy=label_show.getMinSize().y;

  Point edit_size=edit.getMinSize();

  Coord dy=3*space+knob_dxy+Max(edit_size.y,knob_dxy)+Max(check_dxy,label_dy);

  Point list_size=list.getMinSize(Point(cap.x,cap.y-dy));

  return Point( Max_cast( list_size.x , edit_size.x+knob_dxy+kspace , 3*knob_dxy+2*kspace ) , list_size.y+dy );
 }

void ItemListWindow::setCheck(bool show,bool gray,bool name)
 {
  check_show.check(show);
  check_gray.check(gray);
  check_name.check(name);
 }

void ItemListWindow::noItem()
 {
  knob_down.disable();
  knob_up.disable();
  knob_del.disable();

  setCheck(false,false,false);

  check_show.disable();
  check_gray.disable();
  check_name.disable();
 }

 // drawing

void ItemListWindow::layout()
 {
  Coord space=+cfg.space_dxy;

  PaneCut pane(getSize(),space);

  // knob_down , knob_up , knob_del

  {
   Coord dy=knob_down.getMinSize().dxy;

   PaneCut p=pane.cutTop(dy);

   p.place_cutLeft(knob_down).place_cutLeft(knob_up).place_cutRight(knob_del);
  }

  // edit , knob_add

  {
   auto knob__add=CutBox(knob_add);
   auto edit_=CutPoint(edit);

   Coord dy=SupDY(knob__add,edit_);

   PaneCut p=pane.cutBottom(dy);

   p.place_cutRight(knob__add).place(edit_);
  }

  // label... , check...

  {
   auto check__show=CutBox(check_show);
   auto label__show=CutPoint(label_show);

   Coord dy=SupDY(check__show,label__show);

   PaneCut p=pane.cutBottom(dy);

   p.place_cutLeft(check__show).place_cutLeft(label__show)
    .place_cutLeft(check_gray).place_cutLeft(label_gray)
    .place_cutLeft(check_name).place_cutLeft(label_name);
  }

  // list

  {
   pane.place(list);
  }
 }

} // namespace App

