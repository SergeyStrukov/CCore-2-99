/* FontEdit.cpp */
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

#include <inc/FontEdit.h>

#include <CCore/inc/video/Layout.h>

#include <CCore/inc/Sort.h>

namespace App {

#define DISABLE_NONSCALABLE

/* class FontEditWindow::FDBInfo::Base */

class FontEditWindow::FDBInfo::Base : public InfoBase
 {
   struct Rec
    {
     DefString name;
     const FontInfo *info;

     Rec() noexcept : name("-- Default --"_def),info(0) {}

     explicit Rec(const FontInfo &obj)
      {
       name=StringCat(obj.family," ",obj.style);

       info=&obj;
      }

     bool testFileName(StrLen file_name) const
      {
       return file_name.equal(Range(info->file_name));
      }

     bool operator < (const Rec &obj) const { return StrLess(name.str(),obj.name.str()); }
    };

   DynArray<Rec> list;

  public:

   Base() {}

   virtual ~Base() {}

   // methods

   void build(FontDatabase &fdb)
    {
     auto range=fdb.getList();

     list.erase();
     list.reserve(range.len+1);

     list.append_fill();

     for(const FontInfo &obj : range )
#ifdef DISABLE_NONSCALABLE
       if( obj.scalable )
#endif
         list.append_fill(obj);

     Sort(Range(list).part(1));
    }

   const FontInfo * get(ulen index) const
    {
     if( index<list.getLen() ) return list[index].info;

     return 0;
    }

   DefString getFamily(ulen index) const
    {
     if( index<list.getLen() ) return list[index].name;

     return Empty;
    }

   ulen getIndex(StrLen file_name) const
    {
     for(ulen index=1,count=list.getLen(); index<count ;index++)
       if( list[index].testFileName(file_name) )
         return index;

     return 0;
    }

   // AbstractInfo

   virtual ulen getLineCount() const
    {
     return list.getLen();
    }

   virtual StrLen getLine(ulen index) const
    {
     return list.at(index).name.str();
    }
 };

/* class FontEditWindow::FDBInfo */

auto FontEditWindow::FDBInfo::getBase() const -> Base *
 {
  return castPtr<Base>();
 }

FontEditWindow::FDBInfo::FDBInfo()
 : Info(new Base())
 {
 }

FontEditWindow::FDBInfo::~FDBInfo()
 {
 }

void FontEditWindow::FDBInfo::build(FontDatabase &fdb)
 {
  getBase()->build(fdb);
 }

const FontInfo * FontEditWindow::FDBInfo::get(ulen index) const
 {
  return getBase()->get(index);
 }

DefString FontEditWindow::FDBInfo::getFamily(ulen index) const
 {
  return getBase()->getFamily(index);
 }

ulen FontEditWindow::FDBInfo::getIndex(StrLen file_name) const
 {
  return getBase()->getIndex(file_name);
 }

/* class FontEditWindow */

DefString FontEditWindow::TestText =
"Far over the misty mountains cold\n"
"To dungeons deep and caverns old\n"
"We must away ere break of day,\n"
"To find our long-forgotten gold.\n"_def;

class FontEditWindow::MaxIndexFunc : public Funchor
 {
   int count = 0 ;

  private:

   void size(Coord,Coord)
    {
     count++;
    }

  public:

   MaxIndexFunc() {}

   operator int() const { return (count>0)?count-1:0; }

   Function<void (Coord dx,Coord dy)> function_size() { return FunctionOf(this,&MaxIndexFunc::size); }
 };

int FontEditWindow::GetMaxIndex(Font font_)
 {
  try
    {
     FreeTypeFont font;

     (Font &)font=font_;

     MaxIndexFunc func;

     font.getSizeList(func.function_size());

     return func;
    }
  catch(...)
    {
     return 0;
    }
 };

void FontEditWindow::updateFont()
 {
  font.create();

  changed.assert();

  font_test.layout();
  font_test.redraw();
 }

void FontEditWindow::showFont(ulen select)
 {
  if( const FontInfo *font_info=info.get(select) )
    {
     file_name_text.setText(font_info->file_name);
     family_text.setText(info.getFamily(select));

     scalable_light.turn(font_info->scalable);
     monospace_light.turn(font_info->monospace);
     bold_light.turn(font_info->bold);
     italic_light.turn(font_info->italic);
    }
  else
    {
     file_name_text.setText("<none>"_def);
     family_text.setText(info.getFamily(select));

     scalable_light.turn(false);
     monospace_light.turn(true);
     bold_light.turn(false);
     italic_light.turn(false);
    }
 }

void FontEditWindow::noSize()
 {
  fdy_spin.disable();
  fdx_check.check(false);
  fdx_check.disable();
  fdx_spin.disable();
 }

void FontEditWindow::setSize()
 {
  switch( font.param.size_type )
    {
     case FontParam::SizeXY :
      {
       fdy_spin.enable();
       fdx_check.enable();
       fdx_spin.disable();

       fdy_spin.setValue(font.param.set_size.size_xy,1,1000);

       fdx_check.check(false);
      }
     break;

     case FontParam::SizePoint :
      {
       fdy_spin.enable();
       fdx_check.enable();
       fdx_spin.enable();

       fdy_spin.setValue(font.param.set_size.size.y,1,1000);

       fdx_check.check(true);

       fdx_spin.setValue(font.param.set_size.size.x);
      }
     break;

     case FontParam::SizeIndex :
      {
       fdy_spin.enable();
       fdx_check.disable();
       fdx_spin.disable();

       fdy_spin.setValue(font.param.set_size.index,0,GetMaxIndex(font.font));
      }
     break;
    }
 }

void FontEditWindow::setConfig()
 {
  switch( font.param.cfg.fht )
    {
     default:
     case FontHintNone : no_hint_radio.check(); break;

     case FontHintNative : native_hint_radio.check(); break;

     case FontHintAuto : auto_hint_radio.check(); break;
    }

  switch( font.param.cfg.fsm )
    {
     default:
     case FontSmoothNone : no_smooth_radio.check(); break;

     case FontSmooth : smooth_radio.check(); break;

     case FontSmoothLCD_RGB : RGB_radio.check(); break;

     case FontSmoothLCD_BGR : BGR_radio.check(); break;
    }

  kerning_check.check(font.param.cfg.use_kerning);

  strength_spin.setValue(font.param.cfg.strength);
 }

void FontEditWindow::setCouple()
 {
  if( fdb_flag ) return;

  if( font.param.engine_type==FontParam::EngineDefault )
    {
     text_list.select(0);

     noSize();
    }
  else
    {
     text_list.select(info.getIndex(Range(font.param.file_name)));

     setSize();
    }

  setConfig();

  showFont(text_list.getSelect());

  font_test.layout();
  font_test.redraw();
 }

void FontEditWindow::fdbComplete(bool ok)
 {
  fdb_flag=false;

  if( ok )
    {
     info.build(fdb);

     text_list.setInfo(info);

     wlist.insTop(text_list,file_name_text,family_text,
                  scalable_light,monospace_light,bold_light,italic_light,
                  scalable_label,monospace_label,bold_label,italic_label,
                  line1,fdy_spin,fdx_check,fdx_spin,line2,
                  no_hint_radio,native_hint_radio,auto_hint_radio,
                  no_hint_label,native_hint_label,auto_hint_label,
                  hint_contour,
                  no_smooth_radio,smooth_radio,RGB_radio,BGR_radio,
                  no_smooth_label,smooth_label,RGB_label,BGR_label,
                  smooth_contour,
                  kerning_check,kerning_label,strength_spin,strength_label,font_test,test_contour);

     setCouple();
    }
 }

void FontEditWindow::selectFont(ulen select)
 {
  bool def_size=true;

  if( font.param.engine_type==FontParam::EngineFreeType )
    {
     if( font.param.size_type!=FontParam::SizeIndex )
       {
        def_dy=Coord(fdy_spin.getValue());

        def_size=false;
       }
    }

  if( const FontInfo *font_info=info.get(select) )
    {
     font.param.engine_type=FontParam::EngineFreeType;

     font.param.file_name=font_info->file_name;

     if( font_info->scalable )
       {
        if( font.param.size_type==FontParam::SizeIndex || def_size )
          {
           font.param.size_type=FontParam::SizeXY;
           font.param.set_size.size_xy=def_dy;
          }
       }
     else
       {
        font.param.size_type=FontParam::SizeIndex;
        font.param.set_size.index=0;
       }

     setSize();
    }
  else
    {
     font.param.engine_type=FontParam::EngineDefault;

     noSize();
    }

  showFont(select);

  updateFont();
 }

void FontEditWindow::fdxEnable(bool enable)
 {
  switch( font.param.size_type )
    {
     case FontParam::SizeXY :
      {
       if( enable )
         {
          fdx_spin.enable();
          fdx_spin.setValue(fdy_spin.getValue());

          font.param.size_type=FontParam::SizePoint;
          font.param.set_size.size=Point(Coord(fdx_spin.getValue()),Coord(fdy_spin.getValue()));

          updateFont();
         }
      }
     break;

     case FontParam::SizePoint :
      {
       if( !enable )
         {
          fdx_spin.disable();

          font.param.size_type=FontParam::SizeXY;
          font.param.set_size.size_xy=Coord(fdy_spin.getValue());

          updateFont();
         }
      }
     break;
    }
 }

void FontEditWindow::fdxyChanged(int)
 {
  switch( font.param.size_type )
    {
     case FontParam::SizeXY :
      {
       font.param.set_size.size_xy=Coord(fdy_spin.getValue());

       updateFont();
      }
     break;

     case FontParam::SizePoint :
      {
       font.param.set_size.size=Point(Coord(fdx_spin.getValue()),Coord(fdy_spin.getValue()));

       updateFont();
      }
     break;

     case FontParam::SizeIndex :
      {
       font.param.set_size.index=ulen(fdy_spin.getValue());

       updateFont();
      }
     break;
    }
 }

void FontEditWindow::hintChanged(int new_id,int)
 {
  font.param.cfg.fht=FontHintType(new_id);

  updateFont();
 }

void FontEditWindow::smoothChanged(int new_id,int)
 {
  font.param.cfg.fsm=FontSmoothType(new_id);

  updateFont();
 }

void FontEditWindow::kerningChanged(bool check)
 {
  font.param.cfg.use_kerning=check;

  updateFont();
 }

void FontEditWindow::strengthChanged(int strength)
 {
  font.param.cfg.strength=strength;

  updateFont();
 }

FontEditWindow::FontEditWindow(SubWindowHost &host,const ConfigType &cfg_)
 : ComboWindow(host),
   cfg(cfg_),

   progress(wlist,cfg.progress_cfg),
   progress_control(wlist,progress),

   fdb_inc(progress_control),

   text_list(wlist,cfg.text_list_cfg),

   file_name_text(wlist,cfg.text_cfg,AlignX_Left),
   family_text(wlist,cfg.text_cfg,AlignX_Left),

   scalable_light(wlist,cfg.light_cfg,Green),
   monospace_light(wlist,cfg.light_cfg,Green),
   bold_light(wlist,cfg.light_cfg,Green),
   italic_light(wlist,cfg.light_cfg,Green),

   scalable_label(wlist,cfg.label_cfg,"scalable"_def,AlignX_Left),
   monospace_label(wlist,cfg.label_cfg,"monospace"_def,AlignX_Left),
   bold_label(wlist,cfg.label_cfg,"bold"_def,AlignX_Left),
   italic_label(wlist,cfg.label_cfg,"italic"_def,AlignX_Left),

   line1(wlist,cfg.dline_cfg),

   fdy_spin(wlist,cfg.spin_cfg),
   fdx_check(wlist,cfg.check_cfg,false),
   fdx_spin(wlist,cfg.spin_cfg),

   line2(wlist,cfg.dline_cfg),

   no_hint_radio(wlist,FontHintNone,cfg.radio_cfg),
   native_hint_radio(wlist,FontHintNative,cfg.radio_cfg),
   auto_hint_radio(wlist,FontHintAuto,cfg.radio_cfg),

   no_hint_label(wlist,cfg.label_cfg,"No hint"_def,AlignX_Left),
   native_hint_label(wlist,cfg.label_cfg,"Native hint"_def,AlignX_Left),
   auto_hint_label(wlist,cfg.label_cfg,"Auto hint"_def,AlignX_Left),

   hint_contour(wlist,cfg.text_contour_cfg,"Hint"_def),

   no_smooth_radio(wlist,FontSmoothNone,cfg.radio_cfg),
   smooth_radio(wlist,FontSmooth,cfg.radio_cfg),
   RGB_radio(wlist,FontSmoothLCD_RGB,cfg.radio_cfg),
   BGR_radio(wlist,FontSmoothLCD_BGR,cfg.radio_cfg),

   no_smooth_label(wlist,cfg.label_cfg,"No smooth"_def,AlignX_Left),
   smooth_label(wlist,cfg.label_cfg,"Smooth"_def,AlignX_Left),
   RGB_label(wlist,cfg.label_cfg,"LCD RGB"_def,AlignX_Left),
   BGR_label(wlist,cfg.label_cfg,"LCD BGR"_def,AlignX_Left),

   smooth_contour(wlist,cfg.text_contour_cfg,"Smooth"_def),

   kerning_check(wlist,cfg.check_cfg),

   kerning_label(wlist,cfg.label_cfg,"Kerning"_def,AlignX_Left),

   strength_spin(wlist,cfg.spin_cfg),

   strength_label(wlist,cfg.label_cfg,"Strength"_def,AlignX_Left),

   font_test(wlist,info_cfg,InfoFromString(TestText)),

   test_contour(wlist,cfg.contour_cfg),

   connector_fdb_complete(this,&FontEditWindow::fdbComplete,fdb_inc.completed),
   connector_text_list_selected(this,&FontEditWindow::selectFont,text_list.selected),
   connector_fdx_check_changed(this,&FontEditWindow::fdxEnable,fdx_check.changed),
   connector_fdy_spin_changed(this,&FontEditWindow::fdxyChanged,fdy_spin.changed),
   connector_fdx_spin_changed(this,&FontEditWindow::fdxyChanged,fdx_spin.changed),
   connector_hint_group_changed(this,&FontEditWindow::hintChanged,hint_group.changed),
   connector_smooth_group_changed(this,&FontEditWindow::smoothChanged,smooth_group.changed),
   connector_kerning_check_changed(this,&FontEditWindow::kerningChanged,kerning_check.changed),
   connector_strength_spin_changed(this,&FontEditWindow::strengthChanged,strength_spin.changed)
 {
  fdy_spin.setValue(def_dy,1,1000);
  fdx_spin.setValue(def_dy,1,1000);

  fdy_spin.disable();
  fdx_check.disable();
  fdx_spin.disable();

  hint_group.add(no_hint_radio,native_hint_radio,auto_hint_radio);

  smooth_group.add(no_smooth_radio,smooth_radio,RGB_radio,BGR_radio);

  strength_spin.setValue(0,-1000,1000);

  info_cfg.font.bind(font.font);
 }

FontEditWindow::~FontEditWindow()
 {
 }

 // methods

void FontEditWindow::setCouple(const FontCouple &font_)
 {
  font=font_;

  setCouple();
 }

 // drawing

void FontEditWindow::layout()
 {
  Coord space_dxy=+cfg.space_dxy;

  PaneCut pane(getSize(),space_dxy);

  // progress

  {
   PaneCut p=pane;

   p.place_cutTop(progress,+cfg.progress_dy);
  }

  // text_list , file_name_text , family_text

  pane.place_cutLeft(text_list,Div(1,3))
      .place_cutTop(file_name_text)
      .place_cutTop(family_text);

  // lights

  {
   auto scalable__light=CutBox(scalable_light);
   auto scalable__label=CutPoint(scalable_label);

   Coord dy=Max(scalable__label.size.y,scalable__light.dxy);

   PaneCut p=pane.cutTop(dy);

   p.place_cutLeft(scalable__light)
    .place_cutLeft(scalable__label)
    .place_cutLeft(monospace_light)
    .place_cutLeft(monospace_label)
    .place_cutLeft(bold_light)
    .place_cutLeft(bold_label)
    .place_cutLeft(italic_light)
    .place_cutLeft(italic_label);
  }

  // line1

  pane.place_cutTop(line1);

  // size spins

  {
   auto fdy__spin=CutPoint(fdy_spin);
   auto fdx__check=CutBox(fdx_check);

   Coord dy=Max(fdy__spin.size.y,fdx__check.dxy);

   PaneCut p=pane.cutTop(dy);

   p.place_cutLeft(fdy__spin)
    .place_cutLeft(fdx__check)
    .place_cutLeft(fdx_spin);
  }

  // line2

  pane.place_cutTop(line2);

  // hint and smooth

  {
   auto no_hint__radio=CutBox(no_hint_radio);
   auto native_hint__radio=CutBox(native_hint_radio);
   auto auto_hint__radio=CutBox(auto_hint_radio);

   auto no_hint__label=CutPoint(no_hint_label);
   auto native_hint__label=CutPoint(native_hint_label);
   auto auto_hint__label=CutPoint(auto_hint_label);

   Coord line_dy=Max(no_hint__radio.dxy,no_hint__label.size.y);

   Coord hint_dx=Sup(no_hint__label.size.x,native_hint__label.size.x,auto_hint__label.size.x);

   Point hint_inner_size( BoxExt(no_hint__radio.dxy)+hint_dx+2*space_dxy , 3*line_dy+4*space_dxy );

   Point hint_outer_size=hint_contour.getMinSize(hint_inner_size);

   auto no_smooth__radio=CutBox(no_smooth_radio);
   auto smooth__radio=CutBox(smooth_radio);
   auto RGB__radio=CutBox(RGB_radio);
   auto BGR__radio=CutBox(BGR_radio);

   auto no_smooth__label=CutPoint(no_smooth_label);
   auto smooth__label=CutPoint(smooth_label);
   auto RGB__label=CutPoint(RGB_label);
   auto BGR__label=CutPoint(BGR_label);

   Coord smooth_dx=Sup(no_smooth__label.size.x,smooth__label.size.x,RGB__label.size.x,BGR__label.size.x);

   Point smooth_inner_size( BoxExt(no_smooth__radio.dxy)+smooth_dx+2*space_dxy , 4*line_dy+5*space_dxy );

   Point smooth_outer_size=smooth_contour.getMinSize(smooth_inner_size);

   PaneCut p=pane.cutTop(Max(hint_outer_size.y,smooth_outer_size.y));

   p.place_cutLeftTop(hint_contour,hint_outer_size)
    .place_cutLeftTop(smooth_contour,smooth_outer_size);

   // hint

   {
    PaneCut pane(hint_contour.getInner(),space_dxy);

    pane.shrink();

    pane.cutTop(line_dy).place_cutLeft(no_hint__radio).place_cutLeft(no_hint__label);
    pane.cutTop(line_dy).place_cutLeft(native_hint__radio).place_cutLeft(native_hint__label);
    pane.cutTop(line_dy).place_cutLeft(auto_hint__radio).place_cutLeft(auto_hint__label);
   }

   // smooth

   {
    PaneCut pane(smooth_contour.getInner(),space_dxy);

    pane.shrink();

    pane.cutTop(line_dy).place_cutLeft(no_smooth__radio).place_cutLeft(no_smooth__label);
    pane.cutTop(line_dy).place_cutLeft(smooth__radio).place_cutLeft(smooth__label);
    pane.cutTop(line_dy).place_cutLeft(RGB__radio).place_cutLeft(RGB__label);
    pane.cutTop(line_dy).place_cutLeft(BGR__radio).place_cutLeft(BGR__label);
   }
  }

  // kerning

  {
   auto kerning__check=CutBox(kerning_check);
   auto kerning__label=CutPoint(kerning_label);

   Coord dy=Max(kerning__check.dxy,kerning__label.size.y);

   PaneCut p=pane.cutTop(dy);

   p.place_cutLeft(kerning__check)
    .place_cutLeft(kerning__label);
  }

  // strength

  {
   auto strength__spin=CutPoint(strength_spin);
   auto strength__label=CutPoint(strength_label);

   Coord dy=Max(strength__spin.size.y,strength__label.size.y);

   PaneCut p=pane.cutTop(dy);

   p.place_cutLeft(AlignCenterY(strength__spin))
    .place_cutLeft(AlignCenterY(strength__label));
  }

  pane.place(test_contour);

  font_test.setPlace(test_contour.getInner());
 }

 // base

void FontEditWindow::open()
 {
  wlist.open();

  wlist.focusTop();

  if( fdb_flag )
    {
     fdb.cache(fdb_inc);
    }
 }

} // namespace App


