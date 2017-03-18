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
#include <CCore/inc/video/FigureLib.h>

#include <CCore/inc/Exception.h>

namespace App {

/* functions */

MPoint MCenter(Pane pane)
 {
  MPane p(pane);

  return p.getCenter();
 }

/* class HideControl */

void HideControl::check_changed(bool)
 {
  changed.assert();
 }

void HideControl::btn_pressed()
 {
  check_New.check(false);
  check_Ignore.check(false);
  check_Red.check(false);
  check_Yellow.check(false);
  check_Green.check(false);

  changed.assert();
 }

Pane HideControl::Inner(Pane pane,Coord dxy)
 {
  Point size=Point::Diag(dxy);
  Point base=pane.getBase()+(pane.getSize()-size)/2;

  return Pane(base,size);
 }

HideControl::HideControl(SubWindowHost &host,const Config &cfg_)
 : ComboWindow(host),
   cfg(cfg_),

   label_Hide(wlist,cfg.label_cfg,cfg.text_Hide),

   check_New(wlist,cfg.check_cfg),
   check_Ignore(wlist,cfg.check_cfg),
   check_Red(wlist,cfg.check_cfg),
   check_Yellow(wlist,cfg.check_cfg),
   check_Green(wlist,cfg.check_cfg),

   btn_ShowAll(wlist,cfg.btn_cfg,cfg.text_ShowAll),

   connector_check_New_changed(this,&HideControl::check_changed,check_New.changed),
   connector_check_Ignore_changed(this,&HideControl::check_changed,check_Ignore.changed),
   connector_check_Red_changed(this,&HideControl::check_changed,check_Red.changed),
   connector_check_Yellow_changed(this,&HideControl::check_changed,check_Yellow.changed),
   connector_check_Green_changed(this,&HideControl::check_changed,check_Green.changed),

   connector_btn_pressed(this,&HideControl::btn_pressed,btn_ShowAll.pressed)
 {
  wlist.insTop(label_Hide,check_New,check_Ignore,check_Red,check_Yellow,check_Green,btn_ShowAll);
 }

HideControl::~HideControl()
 {
 }

 // methods

Point HideControl::getMinSize() const
 {
  Coordinate space_dxy=+cfg.space_dxy;
  Coordinate status_dxy=+cfg.status_dxy;

  Point s1=label_Hide.getMinSize();
  Point s2=btn_ShowAll.getMinSize();

  return Point( 6*space_dxy+5*status_dxy+s1.x+s2.x , Sup(s1.y,s2.y,+status_dxy) );
 }

bool HideControl::operator [] (ItemStatus status) const
 {
  switch( status )
    {
     case Item_New    : return check_New.isChecked();
     case Item_Ignore : return check_Ignore.isChecked();
     case Item_Red    : return check_Red.isChecked();
     case Item_Yellow : return check_Yellow.isChecked();
     case Item_Green  : return check_Green.isChecked();

     default: return false;
    }
 }

 // drawing

void HideControl::layout()
 {
  Coord status_dxy=+cfg.status_dxy;
  Point s=Point::Diag(status_dxy);

  PaneCut pane(getSize(),+cfg.space_dxy);

  pane.place_cutLeft(label_Hide);

  place_New=pane.cutLeftCenter(s);
  place_Ignore=pane.cutLeftCenter(s);
  place_Red=pane.cutLeftCenter(s);
  place_Yellow=pane.cutLeftCenter(s);
  place_Green=pane.cutLeftCenter(s);

  pane.place_cutLeftCenter(btn_ShowAll);

  Coord dxy=check_New.getMinSize().dxy;

  check_New.setPlace(Inner(place_New,dxy));
  check_Ignore.setPlace(Inner(place_Ignore,dxy));
  check_Red.setPlace(Inner(place_Red,dxy));
  check_Yellow.setPlace(Inner(place_Yellow,dxy));
  check_Green.setPlace(Inner(place_Green,dxy));
 }

void HideControl::drawBack(DrawBuf buf,bool) const
 {
  SmoothDrawArt art(buf);

  Coord radius=Fraction(place_New.dx/2);

  art.ball(MCenter(place_New),radius,+cfg.status_New);
  art.ball(MCenter(place_Ignore),radius,+cfg.status_Ignore);
  art.ball(MCenter(place_Red),radius,+cfg.status_Red);
  art.ball(MCenter(place_Yellow),radius,+cfg.status_Yellow);
  art.ball(MCenter(place_Green),radius,+cfg.status_Green);
 }

/* class CountControl */

CountControl::CountControl(SubWindowHost &host,const Config &cfg_,VColor color_)
 : ComboWindow(host),
   cfg(cfg_),

   text(wlist,cfg.text_cfg),

   color(color_)
 {
  wlist.insTop(text);

  setCount(0);
 }

CountControl::~CountControl()
 {
 }

 // methods

Point CountControl::getMinSize() const
 {
  Coordinate dxy=+cfg.status_dxy;

  Point s=text.getMinSize("10000000000"_c);

  return Point( 2*dxy+s.x , Sup(2*dxy,s.y) );
 }

void CountControl::setCount(ulen count)
 {
  text.printf("#;",count);
 }

 // drawing

void CountControl::layout()
 {
  Coord dxy=+cfg.status_dxy;

  PaneCut pane(getSize(),0);

  Pane left=pane.cutLeftCenter(2*dxy,2*dxy);
  Point s=left.getSize();

  place_status=Pane(left.getBase()+s/4,s/2);

  pane.place(AlignCenterY(text));
 }

void CountControl::drawBack(DrawBuf buf,bool) const
 {
  SmoothDrawArt art(buf);

  Coord radius=Fraction(place_status.dx/2);

  art.ball(MCenter(place_status),radius,color);
 }

/* class InnerDataWindow */

void InnerDataWindow::setMax()
 {
  Point s=getSize();

  Coord dxy=+cfg.dxy;

  page_x=s.x/dxy;

  page_y=s.y/dxy;

  if( page_x>=total_x ) off_x=0; else Replace_min(off_x,total_x-page_x);

  if( page_y>=total_y ) off_y=0; else Replace_min(off_y,total_y-page_y);
 }

class InnerDataWindow::DrawItem : NoCopy
 {
   Coord dxy;
   VColor text;
   Font font;

  public:

   explicit DrawItem(const Config &cfg)
    : dxy(+cfg.dxy),
      text(+cfg.text),
      font(+cfg.font)
    {
    }

   void operator () (const DrawBuf &buf,Point point,const ItemData &item) const // TODO
    {
     Pane pane(point,2000,dxy);

     font->text(buf,pane,TextPlace(AlignX_Left,AlignY_Center),Range(item.ptr->name),text);
    }

   ulen operator () (const ItemData &item) const // TODO
    {
     TextSize ts=font->text(Range(item.ptr->name));

     ulen len=ts.full_dx/dxy+1;

     return item.depth+len;
    }
 };

void InnerDataWindow::posX(ulen pos)
 {
  off_x=pos;

  redraw();
 }

void InnerDataWindow::posY(ulen pos)
 {
  off_y=pos;

  redraw();
 }

InnerDataWindow::InnerDataWindow(SubWindowHost &host,const Config &cfg_,AspectData &data_)
 : SubWindow(host),
   cfg(cfg_),
   data(data_),

   connector_posX(this,&InnerDataWindow::posX),
   connector_posY(this,&InnerDataWindow::posY)
 {
 }

InnerDataWindow::~InnerDataWindow()
 {
 }

 // special methods

bool InnerDataWindow::shortDX() const
 {
  return page_x<total_x;
 }

bool InnerDataWindow::shortDY() const
 {
  return page_y<total_y;
 }

 // methods

Point InnerDataWindow::getMinSize(Point cap) const // TODO
 {
  Used(cap);

  return Point(100,100);
 }

void InnerDataWindow::update(bool new_data)
 {
  if( new_data )
    {
     DrawItem draw(cfg);

     auto items=data.getItems();

     total_x=0;

     for(const ItemData &item : items ) Replace_max(total_x,draw(item));

     total_y=items.len;

     off_x=0;
     off_y=0;

     scroll_x.assert(0);
     scroll_y.assert(0);
    }
 }

 // drawing

bool InnerDataWindow::isGoodSize(Point size) const
 {
  return size>=getMinSize();
 }

void InnerDataWindow::layout()
 {
  setMax();
 }

void InnerDataWindow::draw(DrawBuf buf,bool) const
 {
  DrawItem draw(cfg);

  auto items=data.getItems();

  Point point=Null;
  Coord dxy=+cfg.dxy;
  ulen off=off_x;

  for(ulen i=off_y,j=0; i<items.len && j<page_y ;point=point.addY(dxy),j++)
    {
     const ItemData &item=items[i];
     ulen depth=item.depth;

     Point p = ( off<=depth )? point.addX((depth-off)*dxy) : point.subX((off-depth)*dxy) ;

     draw(buf,p,item);

     if( item.is_dir && !item.is_open )
       i=item.next_index;
     else
       i++;
    }
 }

 // base

void InnerDataWindow::open()
 {
  focus=false;
 }

 // keyboard

FocusType InnerDataWindow::askFocus() const
 {
  return FocusOk;
 }

void InnerDataWindow::gainFocus()
 {
  if( Change(focus,true) ) redraw();
 }

void InnerDataWindow::looseFocus()
 {
  if( Change(focus,false) ) redraw();
 }

 // mouse

void InnerDataWindow::looseCapture()
 {
  // do nothing
 }

MouseShape InnerDataWindow::getMouseShape(Point,KeyMod) const
 {
  return Mouse_Arrow;
 }

 // user input

void InnerDataWindow::react(UserAction action)
 {
  action.dispatch(*this);
 }

/* class DataWindow */

void DataWindow::setScroll()
 {
  if( scroll_x.isListed() ) inner.setScrollXRange(scroll_x);

  if( scroll_y.isListed() ) inner.setScrollYRange(scroll_y);
 }

DataWindow::DataWindow(SubWindowHost &host,const Config &cfg_,AspectData &data)
 : ComboWindow(host),
   cfg(cfg_),

   inner(wlist,cfg_,data),
   scroll_x(wlist,cfg_.x_cfg),
   scroll_y(wlist,cfg_.y_cfg),

   connector_posx(&scroll_x,&XScrollWindow::setPos,inner.scroll_x),
   connector_posy(&scroll_y,&YScrollWindow::setPos,inner.scroll_y)
 {
  wlist.insTop(inner);

  inner.connect(scroll_x.changed,scroll_y.changed);
 }

DataWindow::~DataWindow()
 {
 }

 // methods

Point DataWindow::getMinSize(Point cap) const
 {
  Point delta(scroll_y.getMinSize().dx,0);

  return inner.getMinSize(cap-delta)+delta;
 }

void DataWindow::update(bool new_data)
 {
  inner.update(new_data);
 }

 // drawing

void DataWindow::layout()
 {
  Pane all(Null,getSize());
  Pane pane(all);

  Coord delta_x=scroll_y.getMinSize().dx;
  Coord delta_y=scroll_x.getMinSize().dy;

  inner.setPlace(pane);

  if( inner.shortDY() )
    {
     Pane py=SplitX(pane,delta_x);

     inner.setPlace(pane);
     scroll_y.setPlace(py);

     wlist.insBottom(scroll_y);

     if( inner.shortDX() )
       {
        Pane px=SplitY(pane,delta_y);

        inner.setPlace(pane);
        scroll_x.setPlace(px);

        wlist.insBottom(scroll_x);
       }
     else
       {
        wlist.del(scroll_x);
       }
    }
  else
    {
     if( inner.shortDX() )
       {
        Pane px=SplitY(pane,delta_y);

        inner.setPlace(pane);

        if( inner.shortDY() )
          {
           pane=all;
           Pane py=SplitX(pane,delta_x);
           Pane px=SplitY(pane,delta_y);

           inner.setPlace(pane);
           scroll_x.setPlace(px);
           scroll_y.setPlace(py);

           wlist.insBottom(scroll_x);

           wlist.insBottom(scroll_y);
          }
        else
          {
           scroll_x.setPlace(px);

           wlist.insBottom(scroll_x);

           wlist.del(scroll_y);
          }
       }
     else
       {
        wlist.del(scroll_x);

        wlist.del(scroll_y);
       }
    }

  setScroll();
 }

/* class AspectWindow */

void AspectWindow::setAspect(StrLen file_name)
 {
  StrLen path=Range(data.getPath());

  aspect_file_name=String(file_name);

  RelPath rel(path,file_name);

  if( +rel )
    text_aspect.setText(String(rel.get()));
  else
    text_aspect.setText(aspect_file_name);

  has_file=true;
 }

void AspectWindow::clearAspect()
 {
  aspect_file_name=Null;

  text_aspect.setText(""_def);

  has_file=false;
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

   line1(wlist,cfg.line_cfg),

   hide(wlist,cfg.hide_cfg),

   count_red(wlist,cfg.count_cfg,+cfg.hide_cfg.status_Red),
   count_yellow(wlist,cfg.count_cfg,+cfg.hide_cfg.status_Yellow),
   count_green(wlist,cfg.count_cfg,+cfg.hide_cfg.status_Green),

   line2(wlist,cfg.line_cfg),

   data_window(wlist,cfg.data_cfg,data),

   msg_frame(host.getFrameDesktop(),cfg.msg_cfg),

   connector_msg_destroyed(this,&AspectWindow::msg_destroyed,msg_frame.destroyed)
 {
  wlist.insTop(label_path,label_aspect,text_path,text_aspect,line1,hide,count_red,count_yellow,count_green,line2,data_window);
 }

AspectWindow::~AspectWindow()
 {
 }

 // methods

Point AspectWindow::getMinSize() const // TODO
 {
  return Point(100,100);
 }

void AspectWindow::blank(StrLen path)
 {
  data.blank(path);

  text_path.setText(data.getPath());

  clearAspect();

  setModified();

  update(true);
 }

void AspectWindow::load(StrLen file_name)
 {
  ErrorText etext;

  data.load(file_name,etext);

  if( !etext )
    {
     text_path.setText(data.getPath());

     setAspect(file_name);
    }
  else
    {
     text_path.setText(""_def);

     clearAspect();

     errorMsg(etext.getText());
    }

  clearModified();

  update(true);
 }

bool AspectWindow::save()
 {
  if( !has_file ) return false;

  ErrorText etext;

  data.save(Range(aspect_file_name),etext);

  if( !etext )
    {
     clearModified();
    }
  else
    {
     errorMsg(etext.getText());
    }

  return true;
 }

void AspectWindow::save(StrLen file_name)
 {
  if( !data )
    {
     errorMsg(+cfg.text_Nothing);

     return;
    }

  setAspect(file_name);

  setModified();

  save();
 }

void AspectWindow::updateCount()
 {
  count_red.setCount(data.getCount(Item_Red));
  count_yellow.setCount(data.getCount(Item_Yellow));
  count_green.setCount(data.getCount(Item_Green));
 }

void AspectWindow::update(bool new_data)
 {
  if( new_data ) updateCount();

  data_window.update(new_data);

  layout();

  redraw();
 }

 // drawing

void AspectWindow::layout()
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

  // line1

  pane.place_cutTop(line1);

  // hide

  pane.place_cutTop(hide);

  // count_red , count_yellow , count_green

  {
   auto count__red=CutPoint(count_red);

   Coord dy=count__red.getMinSize().y;

   PaneCut p=pane.cutTop(dy);

   p.place_cutLeft(count__red).place_cutLeft(count_yellow).place_cutLeft(count_green);
  }

  // line2

  pane.place_cutTop(line2);

  // data_window

  pane.place(data_window);
 }

void AspectWindow::drawBack(DrawBuf buf,bool) const
 {
  buf.erase(+cfg.back);
 }

} // namespace App

