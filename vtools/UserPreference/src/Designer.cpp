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
     Var_Clr,
     Var_unsigned,
     Var_String,
     Var_Point,
     Var_Font,
     Var_bool,

     VarLim
    };

   struct Rec
    {
     DefString name;
     VarType type;

     union Ref
      {
       Coord      &of_Coord;
       MCoord     &of_MCoord;
       VColor     &of_VColor;
       Clr        &of_Clr;
       unsigned   &of_unsigned;
       DefString  &of_String;
       Point      &of_Point;
       FontCouple &of_Font;
       bool       &of_bool;

       Ref(Coord &var) : of_Coord{var} {}

       Ref(MCoord &var) : of_MCoord{var} {}

       Ref(VColor &var) : of_VColor{var} {}

       Ref(unsigned &var) : of_unsigned{var} {}

       Ref(Clr &var) : of_Clr{var} {}

       Ref(DefString &var) : of_String{var} {}

       Ref(Point &var) : of_Point{var} {}

       Ref(FontCouple &var) : of_Font{var} {}

       Ref(bool &var) : of_bool{var} {}
      };

     Ref ref;

     Rec(DefString name_,Coord &var)
      : name(name_),
        type(Var_Coord),
        ref(var)
      {
      }

     Rec(DefString name_,MCoord &var)
      : name(name_),
        type(Var_MCoord),
        ref(var)
      {
      }

     Rec(DefString name_,VColor &var)
      : name(name_),
        type(Var_VColor),
        ref(var)
      {
      }

     Rec(DefString name_,Clr &var)
      : name(name_),
        type(Var_Clr),
        ref(var)
      {
      }

     Rec(DefString name_,unsigned &var)
      : name(name_),
        type(Var_unsigned),
        ref(var)
      {
      }

     Rec(DefString name_,DefString &var)
      : name(name_),
        type(Var_String),
        ref(var)
      {
      }

     Rec(DefString name_,Point &var)
      : name(name_),
        type(Var_Point),
        ref(var)
      {
      }

     Rec(DefString name_,FontCouple &var)
      : name(name_),
        type(Var_Font),
        ref(var)
      {
      }

     Rec(DefString name_,bool &var)
      : name(name_),
        type(Var_bool),
        ref(var)
      {
      }

     template <class Func>
     void operator () (Func func)
      {
       switch( type )
         {
          case Var_Coord : func(ref.of_Coord); break;

          case Var_MCoord : func(ref.of_MCoord); break;

          case Var_VColor : func(ref.of_VColor); break;

          case Var_Clr : func(ref.of_Clr); break;

          case Var_unsigned : func(ref.of_unsigned); break;

          case Var_String : func(ref.of_String); break;

          case Var_Point : func(ref.of_Point); break;

          case Var_Font : func(ref.of_Font); break;

          case Var_bool : func(ref.of_bool); break;
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

   void add(DefString name,Coord &var) { list.append_fill(name,var); }

   void add(DefString name,MCoord &var) { list.append_fill(name,var); }

   void add(DefString name,VColor &var) { list.append_fill(name,var); }

   void add(DefString name,Clr &var) { list.append_fill(name,var); }

   void add(DefString name,unsigned &var) { list.append_fill(name,var); }

   void add(DefString name,DefString &var) { list.append_fill(name,var); }

   void add(DefString name,Point &var) { list.append_fill(name,var); }

   void add(DefString name,FontCouple &var) { list.append_fill(name,var); }

   void add(DefString name,bool &var) { list.append_fill(name,var); }

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

   bool enable_Clr(bool on) { return enable(Var_Clr,on); }

   bool enable_unsigned(bool on) { return enable(Var_unsigned,on); }

   bool enable_String(bool on) { return enable(Var_String,on); }

   bool enable_Point(bool on) { return enable(Var_Point,on); }

   bool enable_Font(bool on) { return enable(Var_Font,on); }

   bool enable_bool(bool on) { return enable(Var_bool,on); }

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
        return list.at(index).name.str();
       }
     else
       {
        return sublist.at(index).name.str();
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

   virtual void group(DefString) {}

   virtual void space() {}

   virtual void item(DefString name,Coord &var) { base->add(name,var); }

   virtual void item(DefString name,MCoord &var) { base->add(name,var); }

   virtual void item(DefString name,VColor &var) { base->add(name,var); }

   virtual void item(DefString name,Clr &var) { base->add(name,var); }

   virtual void item(DefString name,unsigned &var) { base->add(name,var); }

   virtual void item(DefString name,DefString &var) { base->add(name,var); }

   virtual void item(DefString name,Point &var) { base->add(name,var); }

   virtual void item(DefString name,FontCouple &var) { base->add(name,var); }

   virtual void item(DefString name,bool &var) { base->add(name,var); }
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

bool DesignerWindow::PrefInfo::enable_Clr(bool on)
 {
  return getBase()->enable_Clr(on);
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

bool DesignerWindow::PrefInfo::enable_bool(bool on)
 {
  return getBase()->enable_bool(on);
 }

template <class Func>
void DesignerWindow::PrefInfo::select(ulen index,Func func)
 {
  getBase()->select(index,func);
 }

/* class DesignerWindow */

void DesignerWindow::switchTo(SubWindow *editor,BackSet *backset)
 {
  active_backset=backset;

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

void DesignerWindow::enable_Clr(bool on)
 {
  if( info.enable_Clr(on) ) newList();
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

void DesignerWindow::enable_bool(bool on)
 {
  if( info.enable_bool(on) ) newList();
 }

 // buttons

void DesignerWindow::setPref()
 {
  if( active_backset ) active_backset->set();

  btn_Set.disable();
  btn_Back.disable();
 }

void DesignerWindow::backPref()
 {
  if( active_backset )
    {
     if( active_backset->back() )
       {
        btn_Save.enable();

        update.assert();
       }
    }

  btn_Set.disable();
  btn_Back.disable();
 }

void DesignerWindow::savePref()
 {
  setPref();

  pref.update();

  btn_Save.disable();
 }

void DesignerWindow::selfPref()
 {
  setPref();

  self_pref.take()=pref.get();

  self_pref.update.assert();
 }

 // select

void DesignerWindow::select(Coord &var)
 {
  coord_pad.bind(var);

  switchTo(coord_edit,coord_pad);
 }

void DesignerWindow::select(MCoord &var)
 {
  mcoord_pad.bind(var);

  switchTo(mcoord_edit,mcoord_pad);
 }

void DesignerWindow::select(VColor &var)
 {
  color_pad.bind(var);

  switchTo(color_edit,color_pad);
 }

void DesignerWindow::select(Clr &var)
 {
  clr_pad.bind(var);

  switchTo(clr_edit,clr_pad);
 }

void DesignerWindow::select(unsigned &var)
 {
  unsigned_pad.bind(var);

  switchTo(unsigned_edit,unsigned_pad);
 }

void DesignerWindow::select(DefString &var)
 {
  string_pad.bind(var);

  switchTo(string_edit,string_pad);
 }

void DesignerWindow::select(Point &var)
 {
  point_pad.bind(var);

  switchTo(point_edit,point_pad);
 }

void DesignerWindow::select(FontCouple &var)
 {
  font_pad.bind(var);

  switchTo(font_edit,font_pad);
 }

void DesignerWindow::select(bool &var)
 {
  bool_pad.bind(var);

  switchTo(bool_edit,bool_pad);
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
  backPref();

  info.select(index,FuncSelect{this});
 }

 // changed

void DesignerWindow::changed()
 {
  btn_Set.enable();
  btn_Back.enable();
  btn_Save.enable();

  update.assert();
 }

void DesignerWindow::string_edit_changed()
 {
  if( string_pad.update(String(string_edit.getText())) ) changed();
 }

void DesignerWindow::coord_edit_changed(Coord value)
 {
  if( coord_pad.update(value) ) changed();
 }

void DesignerWindow::mcoord_edit_changed(MCoord value)
 {
  if( mcoord_pad.update(value) ) changed();
 }

void DesignerWindow::font_edit_changed()
 {
  if( font_pad.update(font_edit.getCouple()) ) changed();
 }

void DesignerWindow::unsigned_edit_changed(int value)
 {
  if( unsigned_pad.update((unsigned)value) ) changed();
 }

void DesignerWindow::clr_edit_changed(int value)
 {
  if( clr_pad.update((Clr)value) ) changed();
 }

void DesignerWindow::point_edit_changed(Point value)
 {
  if( point_pad.update(value) ) changed();
 }

void DesignerWindow::color_edit_changed(VColor value)
 {
  if( color_pad.update(value) ) changed();
 }

void DesignerWindow::bool_edit_changed(bool value)
 {
  if( bool_pad.update(value) ) changed();
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
   check_Clr(wlist,cfg.check_cfg),
   check_unsigned(wlist,cfg.check_cfg),
   check_String(wlist,cfg.check_cfg),
   check_Point(wlist,cfg.check_cfg),
   check_Font(wlist,cfg.check_cfg),
   check_bool(wlist,cfg.check_cfg),

   label_all(wlist,cfg.label_cfg,"All"_def,AlignX_Left),
   label_Coord(wlist,cfg.label_cfg,"size"_def,AlignX_Left),
   label_MCoord(wlist,cfg.label_cfg,"milli-size"_def,AlignX_Left),
   label_VColor(wlist,cfg.label_cfg,"color"_def,AlignX_Left),
   label_Clr(wlist,cfg.label_cfg,"color pitch"_def,AlignX_Left),
   label_unsigned(wlist,cfg.label_cfg,"count"_def,AlignX_Left),
   label_String(wlist,cfg.label_cfg,"text"_def,AlignX_Left),
   label_Point(wlist,cfg.label_cfg,"point"_def,AlignX_Left),
   label_Font(wlist,cfg.label_cfg,"font"_def,AlignX_Left),
   label_bool(wlist,cfg.label_cfg,"bool"_def,AlignX_Left),

   btn_Set(wlist,cfg.btn_cfg,"Set"_def),
   btn_Back(wlist,cfg.btn_cfg,"Back"_def),
   btn_Save(wlist,cfg.btn_cfg,"Save"_def),
   btn_Self(wlist,cfg.btn_cfg,"Apply to self"_def),

   string_edit(wlist,cfg.edit_cfg),
   string_pad(string_edit),

   coord_edit(wlist,cfg.coord_cfg),
   coord_pad(coord_edit),

   mcoord_edit(wlist,cfg.mcoord_cfg),
   mcoord_pad(mcoord_edit),

   font_edit(wlist,cfg.font_cfg),
   font_pad(font_edit),

   unsigned_edit(wlist,cfg.unsigned_cfg),
   unsigned_pad(unsigned_edit),

   clr_edit(wlist,cfg.unsigned_cfg),
   clr_pad(clr_edit),

   point_edit(wlist,cfg.point_cfg),
   point_pad(point_edit),

   color_edit(wlist,cfg.color_cfg),
   color_pad(color_edit),

   bool_edit(wlist,cfg.check_cfg),
   bool_pad(bool_edit),

   connector_test_frame_destroyed(this,&DesignerWindow::testDestroyed,test_frame.destroyed),

   connector_check_all_changed(this,&DesignerWindow::enable_all,check_all.changed),
   connector_check_Coord_changed(this,&DesignerWindow::enable_Coord,check_Coord.changed),
   connector_check_MCoord_changed(this,&DesignerWindow::enable_MCoord,check_MCoord.changed),
   connector_check_VColor_changed(this,&DesignerWindow::enable_VColor,check_VColor.changed),
   connector_check_Clr_changed(this,&DesignerWindow::enable_Clr,check_Clr.changed),
   connector_check_unsigned_changed(this,&DesignerWindow::enable_unsigned,check_unsigned.changed),
   connector_check_String_changed(this,&DesignerWindow::enable_String,check_String.changed),
   connector_check_Point_changed(this,&DesignerWindow::enable_Point,check_Point.changed),
   connector_check_Font_changed(this,&DesignerWindow::enable_Font,check_Font.changed),
   connector_check_bool_changed(this,&DesignerWindow::enable_bool,check_bool.changed),

   connector_btnSet_pressed(this,&DesignerWindow::setPref,btn_Set.pressed),
   connector_btnBack_pressed(this,&DesignerWindow::backPref,btn_Back.pressed),
   connector_btnSave_pressed(this,&DesignerWindow::savePref,btn_Save.pressed),
   connector_btnSelf_pressed(this,&DesignerWindow::selfPref,btn_Self.pressed),

   connector_text_list_selected(this,&DesignerWindow::selectVar,text_list.selected),

   connector_string_edit_changed(this,&DesignerWindow::string_edit_changed,string_edit.changed),
   connector_coord_edit_changed(this,&DesignerWindow::coord_edit_changed,coord_edit.changed),
   connector_mcoord_edit_changed(this,&DesignerWindow::mcoord_edit_changed,mcoord_edit.changed),
   connector_font_edit_changed(this,&DesignerWindow::font_edit_changed,font_edit.changed),
   connector_unsigned_edit_changed(this,&DesignerWindow::unsigned_edit_changed,unsigned_edit.changed),
   connector_clr_edit_changed(this,&DesignerWindow::clr_edit_changed,clr_edit.changed),
   connector_point_edit_changed(this,&DesignerWindow::point_edit_changed,point_edit.changed),
   connector_color_edit_changed(this,&DesignerWindow::color_edit_changed,color_edit.changed),
   connector_bool_edit_changed(this,&DesignerWindow::bool_edit_changed,bool_edit.changed)
 {
  pref.sync();

  wlist.insTop(text_list,check_all,check_Coord,check_MCoord,check_VColor,check_Clr,
                         check_unsigned,check_String,check_Point,check_Font,check_bool,
                         label_all,label_Coord,label_MCoord,label_VColor,label_Clr,
                         label_unsigned,label_String,label_Point,label_Font,label_bool,
                         btn_Set,btn_Back,btn_Save,btn_Self);

  btn_Set.disable();
  btn_Back.disable();
  btn_Save.disable();

  unsigned_edit.setValue(0,0,10000);
  clr_edit.setValue(0,0,MaxClr);

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

  Coord check_dxy;

  {
   Point s=SupMinSize(label_all,label_Coord,label_MCoord,label_VColor,label_Clr,
                      label_unsigned,label_String,label_Point,label_Font,label_bool);

   check_dxy=s.y;

   Point btnSave_size=btn_Save.getMinSize();
   Point btnSelf_size=btn_Self.getMinSize();

   Pointsor psor1=psor.cutX(check_dxy);

   psor1.placeY(check_all,check_dxy);
   psor1.placeY(check_Coord,check_dxy);
   psor1.placeY(check_MCoord,check_dxy);
   psor1.placeY(check_VColor,check_dxy);
   psor1.placeY(check_Clr,check_dxy);
   psor1.placeY(check_unsigned,check_dxy);
   psor1.placeY(check_String,check_dxy);
   psor1.placeY(check_Point,check_dxy);
   psor1.placeY(check_Font,check_dxy);
   psor1.placeY(check_bool,check_dxy);

   Point btnSet_size=btn_Set.getMinSize();
   Point btnBack_size=btn_Back.getMinSize();

   psor1.placeMinY(btn_Set);
   psor1.placeMinY(btn_Back);
   psor1.placeMinY(btn_Save);

   {
    Point pos=psor1;

    Coord max_dy=psor1.maxDY(size.y);

    Coord delta=Max<Coord>(0,max_dy-btnSelf_size.y);

    btn_Self.setPlace(Pane(pos.addY(delta),btnSelf_size));
   }

   Coord dx=Max_cast(btnSet_size.x,btnBack_size.x,btnSave_size.x,btnSelf_size.x)-check_dxy-space_dxy;

   Pointsor psor2=psor.cutX(Max(s.x,dx));

   psor2.placeY(label_all,s);
   psor2.placeY(label_Coord,s);
   psor2.placeY(label_MCoord,s);
   psor2.placeY(label_VColor,s);
   psor2.placeY(label_Clr,s);
   psor2.placeY(label_unsigned,s);
   psor2.placeY(label_String,s);
   psor2.placeY(label_Point,s);
   psor2.placeY(label_Font,s);
   psor2.placeY(label_bool,s);
  }

  Point base=psor;

  Coord max_dx=psor.maxDX(size.x);

  // font_edit color_edit

  {
   Pane pane(base,max_dx,max_dy);

   font_edit.setPlace(pane);
   color_edit.setPlace(pane);
  }

  // string_edit

  SetExtXPlace(string_edit,base,max_dx);

  // coord_edit

  SetMinPlace(coord_edit,base);

  // mcoord_edit

  SetMinPlace(mcoord_edit,base);

  // unsigned_edit

  SetMinPlace(unsigned_edit,base);

  // clr_edit

  SetMinPlace(clr_edit,base);

  // point_edit

  SetMinPlace(point_edit,base);

  // bool_edit

  SetExtPlace(bool_edit,base,check_dxy,check_dxy);
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
                       "Test Frame"_def);
    }

  wlist.open();

  if( !wlist.getFocus() ) wlist.focusTop();
 }

} // namespace App


