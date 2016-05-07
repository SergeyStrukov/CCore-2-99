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

/* class FontEditWindow::FDBInfo::Base */

class FontEditWindow::FDBInfo::Base : public InfoBase
 {
   struct Rec : SetDefaultNoThrowFlag<Rec,false>
    {
     String name;
     const FontInfo *info;

     Rec() : name("-- Default --"),info(0) {}

     explicit Rec(const FontInfo &obj)
      {
       name=StringCat(obj.family," ",obj.style);

       info=&obj;
      }

     bool testFileName(StrLen file_name) const
      {
       return file_name.equal(Range(info->file_name));
      }

     bool operator < (const Rec &obj) const { return StrLess(Range(name),Range(obj.name)); }
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
       if( obj.scalable )
         list.append_fill(obj);

     Sort(Range(list).part(1));
    }

   const FontInfo * get(ulen index) const
    {
     if( index<list.getLen() ) return list[index].info;

     return 0;
    }

   String getFamily(ulen index) const
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
     return Range(list.at(index).name);
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

String FontEditWindow::FDBInfo::getFamily(ulen index) const
 {
  return getBase()->getFamily(index);
 }

ulen FontEditWindow::FDBInfo::getIndex(StrLen file_name) const
 {
  return getBase()->getIndex(file_name);
 }

/* class FontEditWindow */

const char *const FontEditWindow::TestText =
"Far over the misty mountains cold\n"
"To dungeons deep and caverns old\n"
"We must away ere break of day,\n"
"To find our long-forgotten gold.\n";

void FontEditWindow::updateFont()
 {
  font=param.create();

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
     file_name_text.setText(String("<none>"));
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
  switch( param.size_type )
    {
     case FontParam::SizeXY :
      {
       fdy_spin.enable();
       fdx_check.enable();
       fdx_spin.disable();

       fdy_spin.setValue(param.set_size.size_xy);

       fdx_check.check(false);
      }
     break;

     case FontParam::SizePoint :
      {
       fdy_spin.enable();
       fdx_check.enable();
       fdx_spin.enable();

       fdy_spin.setValue(param.set_size.size.y);

       fdx_check.check(true);

       fdx_spin.setValue(param.set_size.size.x);
      }
     break;

     case FontParam::SizeIndex :
      {
       noSize();
      }
     break;
    }
 }

void FontEditWindow::setConfig()
 {
  switch( param.cfg.fht )
    {
     default:
     case FontHintNone : no_hint_radio.check(); break;

     case FontHintNative : native_hint_radio.check(); break;

     case FontHintAuto : auto_hint_radio.check(); break;
    }

  switch( param.cfg.fsm )
    {
     default:
     case FontSmoothNone : no_smooth_radio.check(); break;

     case FontSmooth : smooth_radio.check(); break;

     case FontSmoothLCD_RGB : RGB_radio.check(); break;

     case FontSmoothLCD_BGR : BGR_radio.check(); break;
    }

  kerning_check.check(param.cfg.use_kerning);

  strength_spin.setValue(param.cfg.strength);
 }

void FontEditWindow::setParam()
 {
  if( fdb_flag ) return;

  if( param.engine_type==FontParam::EngineDefault )
    {
     text_list.select(0);

     noSize();
    }
  else
    {
     text_list.select(info.getIndex(Range(param.file_name)));

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

     setParam();
    }
 }

void FontEditWindow::selectFont(ulen select)
 {
  bool def_size=true;

  if( param.engine_type==FontParam::EngineFreeType )
    {
     if( param.size_type!=FontParam::SizeIndex )
       {
        def_dy=Coord(fdy_spin.getValue());

        def_size=false;
       }
    }

  if( const FontInfo *font_info=info.get(select) )
    {
     param.engine_type=FontParam::EngineFreeType;

     param.file_name=font_info->file_name;

     if( font_info->scalable )
       {
        if( param.size_type==FontParam::SizeIndex || def_size )
          {
           param.size_type=FontParam::SizeXY;
           param.set_size.size_xy=def_dy;
          }
       }
     else
       {
        param.size_type=FontParam::SizeIndex;
        param.set_size.index=0;
       }

     setSize();
    }
  else
    {
     param.engine_type=FontParam::EngineDefault;

     noSize();
    }

  showFont(select);

  updateFont();
 }

void FontEditWindow::fdxEnable(bool enable)
 {
  switch( param.size_type )
    {
     case FontParam::SizeXY :
      {
       if( enable )
         {
          fdx_spin.enable();
          fdx_spin.setValue(fdy_spin.getValue());

          param.size_type=FontParam::SizePoint;
          param.set_size.size=Point(Coord(fdx_spin.getValue()),Coord(fdy_spin.getValue()));

          updateFont();
         }
      }
     break;

     case FontParam::SizePoint :
      {
       if( !enable )
         {
          fdx_spin.disable();

          param.size_type=FontParam::SizeXY;
          param.set_size.size_xy=Coord(fdy_spin.getValue());

          updateFont();
         }
      }
     break;
    }
 }

void FontEditWindow::fdxyChanged(int)
 {
  switch( param.size_type )
    {
     case FontParam::SizeXY :
      {
       param.set_size.size_xy=Coord(fdy_spin.getValue());

       updateFont();
      }
     break;

     case FontParam::SizePoint :
      {
       param.set_size.size=Point(Coord(fdx_spin.getValue()),Coord(fdy_spin.getValue()));

       updateFont();
      }
     break;
    }
 }

void FontEditWindow::hintChanged(int new_id,int)
 {
  param.cfg.fht=FontHintType(new_id);

  updateFont();
 }

void FontEditWindow::smoothChanged(int new_id,int)
 {
  param.cfg.fsm=FontSmoothType(new_id);

  updateFont();
 }

void FontEditWindow::kerningChanged(bool check)
 {
  param.cfg.use_kerning=check;

  updateFont();
 }

void FontEditWindow::strengthChanged(int strength)
 {
  param.cfg.strength=strength;

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

   scalable_light(wlist,cfg.light_cfg),
   monospace_light(wlist,cfg.light_cfg),
   bold_light(wlist,cfg.light_cfg),
   italic_light(wlist,cfg.light_cfg),

   scalable_label(wlist,cfg.label_cfg,String("scalable"),AlignX_Left),
   monospace_label(wlist,cfg.label_cfg,String("monospace"),AlignX_Left),
   bold_label(wlist,cfg.label_cfg,String("bold"),AlignX_Left),
   italic_label(wlist,cfg.label_cfg,String("italic"),AlignX_Left),

   line1(wlist,cfg.dline_cfg),

   fdy_spin(wlist,cfg.spin_cfg),
   fdx_check(wlist,cfg.check_cfg,false),
   fdx_spin(wlist,cfg.spin_cfg),

   line2(wlist,cfg.dline_cfg),

   no_hint_radio(wlist,FontHintNone,cfg.radio_cfg),
   native_hint_radio(wlist,FontHintNative,cfg.radio_cfg),
   auto_hint_radio(wlist,FontHintAuto,cfg.radio_cfg),

   no_hint_label(wlist,cfg.label_cfg,String("No hint"),AlignX_Left),
   native_hint_label(wlist,cfg.label_cfg,String("Native hint"),AlignX_Left),
   auto_hint_label(wlist,cfg.label_cfg,String("Auto hint"),AlignX_Left),

   hint_contour(wlist,cfg.text_contour_cfg,String("Hint")),

   no_smooth_radio(wlist,FontSmoothNone,cfg.radio_cfg),
   smooth_radio(wlist,FontSmooth,cfg.radio_cfg),
   RGB_radio(wlist,FontSmoothLCD_RGB,cfg.radio_cfg),
   BGR_radio(wlist,FontSmoothLCD_BGR,cfg.radio_cfg),

   no_smooth_label(wlist,cfg.label_cfg,String("No smooth"),AlignX_Left),
   smooth_label(wlist,cfg.label_cfg,String("Smooth"),AlignX_Left),
   RGB_label(wlist,cfg.label_cfg,String("LCD RGB"),AlignX_Left),
   BGR_label(wlist,cfg.label_cfg,String("LCD BGR"),AlignX_Left),

   smooth_contour(wlist,cfg.text_contour_cfg,String("Smooth")),

   kerning_check(wlist,cfg.check_cfg),

   kerning_label(wlist,cfg.label_cfg,String("Kerning"),AlignX_Left),

   strength_spin(wlist,cfg.spin_cfg),

   strength_label(wlist,cfg.label_cfg,String("Strength"),AlignX_Left),

   font_test(wlist,info_cfg,InfoFromString(TestText)),

   test_contour(wlist,cfg.contour_cfg),

   connector_fdb_complete(this,&FontEditWindow::fdbComplete,fdb_inc.complete),
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

  info_cfg.font.bind(font);
 }

FontEditWindow::~FontEditWindow()
 {
 }

 // methods

void FontEditWindow::setParam(const FontParam &param_,const Font &font_)
 {
  param=param_;
  font=font_;

  setParam();
 }

 // drawing

void FontEditWindow::layout()
 {
  Point size=getSize();

  Coord space_dxy=+cfg.space_dxy;
  Coord check_dxy=+cfg.check_dxy;
  Coord light_dxy=+cfg.light_dxy;

  // progress

  {
   Pane pane(Null,size.x,+cfg.progress_dy);

   progress.setPlace(pane);
  }

  Panesor cur(size,space_dxy);

  cur.placeX(text_list,Rational(1,3));

  cur.extY(file_name_text);

  cur.extY(family_text);

  // lights

  {
   Coord dy=scalable_label.getMinSize().y;

   Panesor cur1=cur.cutY(dy);

   // scalable

   cur1.placeX(scalable_light,dy,CenterFunc(light_dxy));

   cur1.extX(scalable_label);

   // monospace

   cur1.placeX(monospace_light,dy,CenterFunc(light_dxy));

   cur1.extX(monospace_label);

   // bold

   cur1.placeX(bold_light,dy,CenterFunc(light_dxy));

   cur1.extX(bold_label);

   // italic

   cur1.placeX(italic_light,dy,CenterFunc(light_dxy));

   cur1.extX(italic_label);
  }

  cur.extY(line1);

  // size spins

  {
   Point s=fdy_spin.getMinSize();

   Panesor cur1=cur.cutY(s.y);

   cur1.placeX(fdy_spin,s.x);
   cur1.placeX(fdx_check,s.y,CenterFunc(check_dxy));
   cur1.placeX(fdx_spin,s.x);
  }

  cur.extY(line2);

  // hint and smooth

  {
   Point hint_size=Sup(no_hint_label.getMinSize(),native_hint_label.getMinSize(),auto_hint_label.getMinSize());

   Point hint_inner_size(hint_size.x+hint_size.y+3*space_dxy,3*hint_size.y+4*space_dxy);

   Point hint_outer_size=hint_contour.getMinSize(hint_inner_size);

   Point smooth_size=Sup(no_smooth_label.getMinSize(),smooth_label.getMinSize(),RGB_label.getMinSize(),BGR_label.getMinSize());

   Point smooth_inner_size(smooth_size.x+smooth_size.y+3*space_dxy,4*smooth_size.y+5*space_dxy);

   Point smooth_outer_size=smooth_contour.getMinSize(smooth_inner_size);

   Panesor cur1=cur.cutY(Max(hint_outer_size.y,smooth_outer_size.y));

   cur1.placeX(hint_contour,hint_outer_size.x);

   cur1.placeX(smooth_contour,smooth_outer_size.x);

   {
    Panesor cur(hint_contour.getInner().shrink(space_dxy),space_dxy);

    {
     Panesor cur1=cur.cutY(hint_size.y);

     cur1.placeX(no_hint_radio,hint_size.y,CenterFunc(check_dxy));
     cur1.placeX(no_hint_label,hint_size.x);
    }

    {
     Panesor cur1=cur.cutY(hint_size.y);

     cur1.placeX(native_hint_radio,hint_size.y,CenterFunc(check_dxy));
     cur1.placeX(native_hint_label,hint_size.x);
    }

    {
     Panesor cur1=cur.cutY(hint_size.y);

     cur1.placeX(auto_hint_radio,hint_size.y,CenterFunc(check_dxy));
     cur1.placeX(auto_hint_label,hint_size.x);
    }
   }

   {
    Panesor cur(smooth_contour.getInner().shrink(space_dxy),space_dxy);

    {
     Panesor cur1=cur.cutY(smooth_size.y);

     cur1.placeX(no_smooth_radio,smooth_size.y,CenterFunc(check_dxy));
     cur1.placeX(no_smooth_label,smooth_size.x);
    }

    {
     Panesor cur1=cur.cutY(smooth_size.y);

     cur1.placeX(smooth_radio,smooth_size.y,CenterFunc(check_dxy));
     cur1.placeX(smooth_label,smooth_size.x);
    }

    {
     Panesor cur1=cur.cutY(smooth_size.y);

     cur1.placeX(RGB_radio,smooth_size.y,CenterFunc(check_dxy));
     cur1.placeX(RGB_label,smooth_size.x);
    }

    {
     Panesor cur1=cur.cutY(smooth_size.y);

     cur1.placeX(BGR_radio,smooth_size.y,CenterFunc(check_dxy));
     cur1.placeX(BGR_label,smooth_size.x);
    }
   }
  }

  // kerning

  {
   Point s=kerning_label.getMinSize();

   Panesor cur1=cur.cutY(s.y);

   cur1.placeX(kerning_check,s.y,CenterFunc(check_dxy));

   cur1.placeX(kerning_label,s.x);
  }

  // strength

  {
   Point s1=strength_spin.getMinSize();
   Point s2=strength_label.getMinSize();

   Panesor cur1=cur.cutY(Max(s1.y,s2.y));

   cur1.placeX(strength_spin,s1.x);

   cur1.placeX(strength_label,s2.x);
  }

  test_contour.setPlace(cur);

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


