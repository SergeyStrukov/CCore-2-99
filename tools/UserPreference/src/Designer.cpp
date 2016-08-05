/* Designer.cpp */
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

#include <inc/Designer.h>

#include <CCore/inc/video/Layout.h>

namespace App {

/* class Preference */

Preference::Preference() noexcept
 {
 }

Preference::~Preference()
 {
 }

/* class DesignerWindow::PrefInfo::Base */

class DesignerWindow::PrefInfo::Base : public InfoBase
 {
   enum VarType
    {
     Var_Coord,
     Var_MCoord,
     Var_VColor,
     Var_unsigned,
     Var_String,
     Var_Point,
     Var_Font,

     VarLim
    };

   template <class T>
   struct RefOf
    {
     T &var;
    };

   struct RefFont
    {
     FontParam &var;
     Font &font;
    };

   struct Rec
    {
     String name;
     VarType type;

     union Ref
      {
       RefOf<Coord>     of_Coord;
       RefOf<MCoord>    of_MCoord;
       RefOf<VColor>    of_VColor;
       RefOf<unsigned>  of_unsigned;
       RefOf<DefString> of_String;
       RefOf<Point>     of_Point;
       RefFont          of_Font;

       Ref(Coord &var) : of_Coord{var} {}

       Ref(MCoord &var) : of_MCoord{var} {}

       Ref(VColor &var) : of_VColor{var} {}

       Ref(unsigned &var) : of_unsigned{var} {}

       Ref(DefString &var) : of_String{var} {}

       Ref(Point &var) : of_Point{var} {}

       Ref(FontParam &var,Font &font) : of_Font{var,font} {}
      };

     Ref ref;

     Rec(StrLen name_,Coord &var)
      : name(name_),
        type(Var_Coord),
        ref(var)
      {
      }

     Rec(StrLen name_,MCoord &var)
      : name(name_),
        type(Var_MCoord),
        ref(var)
      {
      }

     Rec(StrLen name_,VColor &var)
      : name(name_),
        type(Var_VColor),
        ref(var)
      {
      }

     Rec(StrLen name_,unsigned &var)
      : name(name_),
        type(Var_unsigned),
        ref(var)
      {
      }

     Rec(StrLen name_,DefString &var)
      : name(name_),
        type(Var_String),
        ref(var)
      {
      }

     Rec(StrLen name_,Point &var)
      : name(name_),
        type(Var_Point),
        ref(var)
      {
      }

     Rec(StrLen name_,FontParam &var,Font &font)
      : name(name_),
        type(Var_Font),
        ref(var,font)
      {
      }

     template <class Func>
     void operator () (Func func)
      {
       switch( type )
         {
          case Var_Coord : func(ref.of_Coord.var); break;

          case Var_MCoord : func(ref.of_MCoord.var); break;

          case Var_VColor : func(ref.of_VColor.var); break;

          case Var_unsigned : func(ref.of_unsigned.var); break;

          case Var_String : func(ref.of_String.var); break;

          case Var_Point : func(ref.of_Point.var); break;

          case Var_Font : func(ref.of_Font.var,ref.of_Font.font); break;
         }
      }
    };

   DynArray<Rec> list;

   bool en_all = true ;

   bool en[VarLim] = {} ;

   DynArray<Rec> sublist;

  private:

   void buildSubList()
    {
     sublist.erase();

     for(const Rec &rec : list ) if( en[rec.type] ) sublist.append_copy(rec);
    }

   template <class Func>
   static void Select(DynArray<Rec> &list,ulen index,Func func)
    {
     if( index<list.getLen() )
       {
        list[index](func);
       }
     else
       {
        func();
       }
    }

  public:

   Base() noexcept {}

   ~Base() {}

   // select

   template <class Func>
   void select(ulen index,Func func)
    {
     if( en_all )
       {
        Select(list,index,func);
       }
     else
       {
        Select(sublist,index,func);
       }
    }

   // add...()

   void add(StrLen name,Coord &var) { list.append_fill(name,var); }

   void add(StrLen name,MCoord &var) { list.append_fill(name,var); }

   void add(StrLen name,VColor &var) { list.append_fill(name,var); }

   void add(StrLen name,unsigned &var) { list.append_fill(name,var); }

   void add(StrLen name,DefString &var) { list.append_fill(name,var); }

   void add(StrLen name,Point &var) { list.append_fill(name,var); }

   void add(StrLen name,FontParam &var,Font &font) { list.append_fill(name,var,font); }

   // enable...()

   bool enable_all(bool on)
    {
     if( Change(en_all,on) )
       {
        if( !on ) buildSubList();

        return true;
       }
     else
       {
        return false;
       }
    }

   bool enable(VarType type,bool on)
    {
     if( Change(en[type],on) )
       {
        if( en_all ) return false;

        buildSubList();

        return true;
       }
     else
       {
        return false;
       }
    }

   bool enable_Coord(bool on) { return enable(Var_Coord,on); }

   bool enable_MCoord(bool on) { return enable(Var_MCoord,on); }

   bool enable_VColor(bool on) { return enable(Var_VColor,on); }

   bool enable_unsigned(bool on) { return enable(Var_unsigned,on); }

   bool enable_String(bool on) { return enable(Var_String,on); }

   bool enable_Point(bool on) { return enable(Var_Point,on); }

   bool enable_Font(bool on) { return enable(Var_Font,on); }

   // AbstractInfo

   virtual ulen getLineCount() const
    {
     if( en_all )
       {
        return list.getLen();
       }
     else
       {
        return sublist.getLen();
       }
    }

   virtual StrLen getLine(ulen index) const
    {
     if( en_all )
       {
        return Range(list.at(index).name);
       }
     else
       {
        return Range(sublist.at(index).name);
       }
    }
 };

/* class DesignerWindow::PrefInfo::Binder */

class DesignerWindow::PrefInfo::Binder : public UserPreferenceBag::Bind
 {
   Base *base;

  public:

   Binder(Base *base_) : base(base_) {}

   // UserPreferenceBag::Bind

   virtual void group(StrLen) {}

   virtual void space() {}

   virtual void item(StrLen name,Coord &var) { base->add(name,var); }

   virtual void item(StrLen name,MCoord &var) { base->add(name,var); }

   virtual void item(StrLen name,VColor &var) { base->add(name,var); }

   virtual void item(StrLen name,unsigned &var) { base->add(name,var); }

   virtual void item(StrLen name,DefString &var) { base->add(name,var); }

   virtual void item(StrLen name,Point &var) { base->add(name,var); }

   virtual void item(StrLen name,FontParam &var,Font &font) { base->add(name,var,font); }
 };

/* class DesignerWindow::PrefInfo */

auto DesignerWindow::PrefInfo::getBase() -> Base *
 {
  return castPtr<Base>();
 }

DesignerWindow::PrefInfo::PrefInfo()
 : Info(new Base)
 {
 }

DesignerWindow::PrefInfo::~PrefInfo()
 {
 }

auto DesignerWindow::PrefInfo::getBinder() -> Binder
 {
  return getBase();
 }

bool DesignerWindow::PrefInfo::enable_all(bool on)
 {
  return getBase()->enable_all(on);
 }

bool DesignerWindow::PrefInfo::enable_Coord(bool on)
 {
  return getBase()->enable_Coord(on);
 }

bool DesignerWindow::PrefInfo::enable_MCoord(bool on)
 {
  return getBase()->enable_MCoord(on);
 }

bool DesignerWindow::PrefInfo::enable_VColor(bool on)
 {
  return getBase()->enable_VColor(on);
 }

bool DesignerWindow::PrefInfo::enable_unsigned(bool on)
 {
  return getBase()->enable_unsigned(on);
 }

bool DesignerWindow::PrefInfo::enable_String(bool on)
 {
  return getBase()->enable_String(on);
 }

bool DesignerWindow::PrefInfo::enable_Point(bool on)
 {
  return getBase()->enable_Point(on);
 }

bool DesignerWindow::PrefInfo::enable_Font(bool on)
 {
  return getBase()->enable_Font(on);
 }

template <class Func>
void DesignerWindow::PrefInfo::select(ulen index,Func func)
 {
  getBase()->select(index,func);
 }

/* class DesignerWindow */

void DesignerWindow::switchTo(SubWindow *editor)
 {
  if( active_editor!=editor )
    {
     if( active_editor ) wlist.del(active_editor);

     SubWindow *focus=wlist.getFocus();

     active_editor=editor;

     if( editor ) wlist.insBottom(editor);

     if( !focus ) focus=editor;

     if( focus ) focus->setFocus();
    }
 }

void DesignerWindow::testDestroyed()
 {
  getFrame()->askClose();
 }

void DesignerWindow::newList()
 {
  text_list.setInfo(info);

  selectVar(text_list.getSelect());

  layout();

  redraw();
 }

 // enable

void DesignerWindow::enable_all(bool on)
 {
  if( info.enable_all(on) ) newList();
 }

void DesignerWindow::enable_Coord(bool on)
 {
  if( info.enable_Coord(on) ) newList();
 }

void DesignerWindow::enable_MCoord(bool on)
 {
  if( info.enable_MCoord(on) ) newList();
 }

void DesignerWindow::enable_VColor(bool on)
 {
  if( info.enable_VColor(on) ) newList();
 }

void DesignerWindow::enable_unsigned(bool on)
 {
  if( info.enable_unsigned(on) ) newList();
 }

void DesignerWindow::enable_String(bool on)
 {
  if( info.enable_String(on) ) newList();
 }

void DesignerWindow::enable_Point(bool on)
 {
  if( info.enable_Point(on) ) newList();
 }

void DesignerWindow::enable_Font(bool on)
 {
  if( info.enable_Font(on) ) newList();
 }

 // buttons

void DesignerWindow::savePref()
 {
  pref.update();

  btn_Save.disable();
 }

void DesignerWindow::selfPref()
 {
  self_pref.take()=pref.get();

  self_pref.update.assert();
 }

 // select

void DesignerWindow::select(Coord &var)
 {
  coord_var=&var;

  coord_edit.setCoord(var);

  switchTo(coord_edit);
 }

void DesignerWindow::select(MCoord &var)
 {
  mcoord_var=&var;

  mcoord_edit.setMCoord(var);

  switchTo(mcoord_edit);
 }

void DesignerWindow::select(VColor &var)
 {
  color_var=&var;

  color_edit.setColor(var);

  switchTo(color_edit);
 }

void DesignerWindow::select(unsigned &var)
 {
  unsigned_var=&var;

  unsigned_edit.setValue(var);

  switchTo(unsigned_edit);
 }

void DesignerWindow::select(DefString &var)
 {
  string_var=&var;

  string_edit.setText(var.str());

  switchTo(string_edit);
 }

void DesignerWindow::select(Point &var)
 {
  point_var=&var;

  point_edit.setPoint(var);

  switchTo(point_edit);
 }

void DesignerWindow::select(FontParam &var,Font &font)
 {
  font_param_var=&var;
  font_var=&font;

  font_edit.setParam(var,font);

  switchTo(font_edit);
 }

void DesignerWindow::select()
 {
  switchTo(0);
 }

struct DesignerWindow::FuncSelect
 {
  DesignerWindow *obj;

  template <class ... TT>
  void operator () (TT & ... var) { obj->select(var...); }
 };

void DesignerWindow::selectVar(ulen index)
 {
  info.select(index,FuncSelect{this});
 }

 // changed

void DesignerWindow::changed()
 {
  btn_Save.enable();

  update.assert();
 }

void DesignerWindow::string_edit_changed()
 {
  if( string_var )
    {
     *string_var=String(string_edit.getText());

     changed();
    }
 }

void DesignerWindow::coord_edit_changed(Coord value)
 {
  if( coord_var )
    {
     *coord_var=value;

     changed();
    }
 }

void DesignerWindow::mcoord_edit_changed(MCoord value)
 {
  if( mcoord_var )
    {
     *mcoord_var=value;

     changed();
    }
 }

void DesignerWindow::font_edit_changed()
 {
  if( font_var && font_param_var )
    {
     *font_param_var=font_edit.getParam();
     *font_var=font_edit.getFont();

     changed();
    }
 }

void DesignerWindow::unsigned_edit_changed(int value)
 {
  if( unsigned_var )
    {
     *unsigned_var=value;

     changed();
    }
 }

void DesignerWindow::point_edit_changed(Point value)
 {
  if( point_var )
    {
     *point_var=value;

     changed();
    }
 }

void DesignerWindow::color_edit_changed(VColor value)
 {
  if( color_var )
    {
     *color_var=value;

     changed();
    }
 }

 // constructors

DesignerWindow::DesignerWindow(SubWindowHost &host,const Config &cfg_,Preference &self_pref_)
 : ComboWindow(host),
   cfg(cfg_),
   self_pref(self_pref_),
   test_frame(getDesktop(),pref,update),

   text_list(wlist,cfg.text_list_cfg),

   check_all(wlist,cfg.check_cfg,true),
   check_Coord(wlist,cfg.check_cfg),
   check_MCoord(wlist,cfg.check_cfg),
   check_VColor(wlist,cfg.check_cfg),
   check_unsigned(wlist,cfg.check_cfg),
   check_String(wlist,cfg.check_cfg),
   check_Point(wlist,cfg.check_cfg),
   check_Font(wlist,cfg.check_cfg),

   label_all(wlist,cfg.label_cfg,String("All"),AlignX_Left),
   label_Coord(wlist,cfg.label_cfg,String("size"),AlignX_Left),
   label_MCoord(wlist,cfg.label_cfg,String("milli-size"),AlignX_Left),
   label_VColor(wlist,cfg.label_cfg,String("color"),AlignX_Left),
   label_unsigned(wlist,cfg.label_cfg,String("count"),AlignX_Left),
   label_String(wlist,cfg.label_cfg,String("text"),AlignX_Left),
   label_Point(wlist,cfg.label_cfg,String("point"),AlignX_Left),
   label_Font(wlist,cfg.label_cfg,String("font"),AlignX_Left),

   btn_Save(wlist,cfg.btn_cfg,String("Save")),
   btn_Self(wlist,cfg.btn_cfg,String("Apply to self")),

   string_edit(wlist,cfg.edit_cfg),
   coord_edit(wlist,cfg.coord_cfg),
   mcoord_edit(wlist,cfg.mcoord_cfg),
   font_edit(wlist,cfg.font_cfg),
   unsigned_edit(wlist,cfg.unsigned_cfg),
   point_edit(wlist,cfg.point_cfg),
   color_edit(wlist,cfg.color_cfg),

   connector_test_frame_destroyed(this,&DesignerWindow::testDestroyed,test_frame.destroyed),

   connector_check_all_changed(this,&DesignerWindow::enable_all,check_all.changed),
   connector_check_Coord_changed(this,&DesignerWindow::enable_Coord,check_Coord.changed),
   connector_check_MCoord_changed(this,&DesignerWindow::enable_MCoord,check_MCoord.changed),
   connector_check_VColor_changed(this,&DesignerWindow::enable_VColor,check_VColor.changed),
   connector_check_unsigned_changed(this,&DesignerWindow::enable_unsigned,check_unsigned.changed),
   connector_check_String_changed(this,&DesignerWindow::enable_String,check_String.changed),
   connector_check_Point_changed(this,&DesignerWindow::enable_Point,check_Point.changed),
   connector_check_Font_changed(this,&DesignerWindow::enable_Font,check_Font.changed),

   connector_btnSave_pressed(this,&DesignerWindow::savePref,btn_Save.pressed),
   connector_btnSelf_pressed(this,&DesignerWindow::selfPref,btn_Self.pressed),

   connector_text_list_selected(this,&DesignerWindow::selectVar,text_list.selected),

   connector_string_edit_changed(this,&DesignerWindow::string_edit_changed,string_edit.changed),
   connector_coord_edit_changed(this,&DesignerWindow::coord_edit_changed,coord_edit.changed),
   connector_mcoord_edit_changed(this,&DesignerWindow::mcoord_edit_changed,mcoord_edit.changed),
   connector_font_edit_changed(this,&DesignerWindow::font_edit_changed,font_edit.changed),
   connector_unsigned_edit_changed(this,&DesignerWindow::unsigned_edit_changed,unsigned_edit.changed),
   connector_point_edit_changed(this,&DesignerWindow::point_edit_changed,point_edit.changed),
   connector_color_edit_changed(this,&DesignerWindow::color_edit_changed,color_edit.changed)
 {
  pref.sync();

  wlist.insTop(text_list,check_all,check_Coord,check_MCoord,check_VColor,
                         check_unsigned,check_String,check_Point,check_Font,
                         label_all,label_Coord,label_MCoord,label_VColor,
                         label_unsigned,label_String,label_Point,label_Font,
                         btn_Save,btn_Self);

  btn_Save.disable();

  unsigned_edit.setValue(0,0,10000);

  // fill text_list

  auto binder=info.getBinder();

  pref.take().bind(binder);

  newList();
 }

DesignerWindow::~DesignerWindow()
 {
 }

 // drawing

void DesignerWindow::layout()
 {
  Point size=getSize();

  Coord space_dxy=+cfg.space_dxy;

  Pointsor psor(space_dxy);

  Coord max_dy=psor.maxDY(size.y);

  // text_list

  {
   psor.placeMinX(text_list,MinSize,max_dy);
  }

  // check box

  {
   Point s=Sup(label_all.getMinSize(),label_Coord.getMinSize(),label_MCoord.getMinSize(),label_VColor.getMinSize(),
               label_unsigned.getMinSize(),label_String.getMinSize(),label_Point.getMinSize(),label_Font.getMinSize());

   Point btn1_size=btn_Save.getMinSize();
   Point btn2_size=btn_Self.getMinSize();

   Pointsor psor1=psor.cutX(s.y);

   psor1.placeY(check_all,s.y);
   psor1.placeY(check_Coord,s.y);
   psor1.placeY(check_MCoord,s.y);
   psor1.placeY(check_VColor,s.y);
   psor1.placeY(check_unsigned,s.y);
   psor1.placeY(check_String,s.y);
   psor1.placeY(check_Point,s.y);
   psor1.placeY(check_Font,s.y);

   psor1.placeY(btn_Save,btn1_size);

   {
    Point pos=psor1;

    Coord max_dy=psor1.maxDY(size.y);

    Coord delta=0;

    if( max_dy>btn2_size.y ) delta=max_dy-btn2_size.y;

    btn_Self.setPlace(Pane(pos.addY(delta),btn2_size));
   }

   Pointsor psor2=psor.cutX(Max<Coord>(s.x,Max(btn1_size.x,btn2_size.x)-s.y-space_dxy));

   psor2.placeY(label_all,s);
   psor2.placeY(label_Coord,s);
   psor2.placeY(label_MCoord,s);
   psor2.placeY(label_VColor,s);
   psor2.placeY(label_unsigned,s);
   psor2.placeY(label_String,s);
   psor2.placeY(label_Point,s);
   psor2.placeY(label_Font,s);
  }

  Coord max_dx=psor.maxDX(size.x);

  // font_edit color_edit

  {
   Pane pane(psor,{max_dx,max_dy});

   font_edit.setPlace(pane);
   color_edit.setPlace(pane);
  }

  // string_edit

  {
   Point s=string_edit.getMinSize();

   Pane pane(psor,{Max(s.x,max_dx),s.y});

   string_edit.setPlace(pane);
  }

  // coord_edit

  {
   Point s=coord_edit.getMinSize();

   Pane pane(psor,s);

   coord_edit.setPlace(pane);
  }

  // mcoord_edit

  {
   Point s=mcoord_edit.getMinSize();

   Pane pane(psor,s);

   mcoord_edit.setPlace(pane);
  }

  // unsigned_edit

  {
   Point s=unsigned_edit.getMinSize();

   Pane pane(psor,s);

   unsigned_edit.setPlace(pane);
  }

  // point_edit

  {
   Point s=point_edit.getMinSize();

   Pane pane(psor,s);

   point_edit.setPlace(pane);
  }
 }

void DesignerWindow::draw(DrawBuf buf,bool drag_active) const
 {
  buf.erase(+cfg.back);

  wlist.draw(buf,drag_active);
 }

void DesignerWindow::draw(DrawBuf buf,Pane pane,bool drag_active) const
 {
  buf.block_safe(pane,+cfg.back);

  wlist.draw(buf,pane,drag_active);
 }

 // base

void DesignerWindow::open()
 {
  if( test_frame.isDead() )
    {
     PlaceFrame place(getDesktop());

     test_frame.create(getFrame(),
                       place.getPane(Rational(2,3)-Rational(1,48),Rational(1,3),Rational(1,8),Rational(3,4)),
                       place.getMaxSize(),
                       String("Test Frame"));
    }

  wlist.open();

  wlist.focusTop();
 }

} // namespace App


