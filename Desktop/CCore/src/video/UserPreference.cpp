/* UserPreference.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: Desktop
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <CCore/inc/video/UserPreference.h>

#include <CCore/inc/video/DesktopKey.h>
#include <CCore/inc/video/Desktop.h>

#include <CCore/inc/MakeString.h>
#include <CCore/inc/FileSystem.h>

#include <CCore/inc/Exception.h>

namespace CCore {
namespace Video {

/* struct UserPreferenceBag */

template <class Ptr,class Func>
void UserPreferenceBag::Members(Ptr ptr,Func func) // Update here
 {
  func("space_dxy",ptr->space_dxy);
  func("text_cursor_dx",ptr->text_cursor_dx);
  func("check_dxy",ptr->check_dxy);
  func("knob_dxy",ptr->knob_dxy);
  func("radio_dxy",ptr->radio_dxy);
  func("switch_dxy",ptr->switch_dxy);
  func("light_dxy",ptr->light_dxy);
  func("scroll_dxy",ptr->scroll_dxy);
  func("progress_dy",ptr->progress_dy);
  func("width",ptr->width);
  func("ground",ptr->ground);
  func("back",ptr->back);
  func("line",ptr->line);
  func("inactive",ptr->inactive);
  func("bottom",ptr->bottom);
  func("bottomUp",ptr->bottomUp);
  func("top",ptr->top);
  func("topUp",ptr->topUp);
  func("border",ptr->border);
  func("focus",ptr->focus);
  func("face",ptr->face);
  func("faceUp",ptr->faceUp);
  func("mark",ptr->mark);
  func("text_select",ptr->text_select);
  func("text_cursor",ptr->text_cursor);
  func("label_text",ptr->label_text);
  func("contour_text",ptr->contour_text);
  func("button_text",ptr->button_text);
  func("message_text",ptr->message_text);
  func("info_text",ptr->info_text);
  func("line_edit_text",ptr->line_edit_text);
  func("list_text",ptr->list_text);
  func("button_space",ptr->button_space);
  func("message_space",ptr->message_space);
  func("line_edit_space",ptr->line_edit_space);
  func("info_space",ptr->info_space);
  func("list_space",ptr->list_space);
  func("menu_space",ptr->menu_space);
  func("menu_back",ptr->menu_back);
  func("menu_text",ptr->menu_text);
  func("menu_hilight",ptr->menu_hilight);
  func("menu_select",ptr->menu_select);
  func("menu_hot",ptr->menu_hot);
  func("use_hotcolor",ptr->use_hotcolor);
  func("scroll_list_title",ptr->scroll_list_title);
  func("scroll_list_title_top",ptr->scroll_list_title_top);
  func("scroll_list_title_bottom",ptr->scroll_list_title_bottom);
  func("line_edit_period",ptr->line_edit_period);
  func("scroll_speedUpPeriod",ptr->scroll_speedUpPeriod);
  func("progress_time",ptr->progress_time);
  func("progress_period",ptr->progress_period);
  func("line_edit_ex",ptr->line_edit_ex);
  func("switch_on",ptr->switch_on);
  func("switch_off",ptr->switch_off);
  func("progress_border",ptr->progress_border);
  func("progress_ping_top",ptr->progress_ping_top);
  func("progress_ping_bottom",ptr->progress_ping_bottom);
  func("progress_active_top",ptr->progress_active_top);
  func("progress_active_bottom",ptr->progress_active_bottom);
  func("blink_time",ptr->blink_time);
  func("blink_period",ptr->blink_period);
  func("fatal_error",ptr->fatal_error);
  func("frame_dxy",ptr->frame_dxy);
  func("title_dy",ptr->title_dy);
  func("btn_dx",ptr->btn_dx);
  func("btn_dy",ptr->btn_dy);
  func("frame",ptr->frame);
  func("active_frame",ptr->active_frame);
  func("inactive_frame",ptr->inactive_frame);
  func("frameHilight",ptr->frameHilight);
  func("frameDrag",ptr->frameDrag);
  func("title",ptr->title);
  func("drag",ptr->drag);
  func("dragHilight",ptr->dragHilight);
  func("dragActive",ptr->dragActive);
  func("btnFace",ptr->btnFace);
  func("btnFaceHilight",ptr->btnFaceHilight);
  func("btnPict",ptr->btnPict);
  func("btnPictClose",ptr->btnPictClose);
  func("btnPictAlert",ptr->btnPictAlert);
  func("btnPictNoAlert",ptr->btnPictNoAlert);
  func("btnPictCloseAlert",ptr->btnPictCloseAlert);
  func("shade_color",ptr->shade_color);
  func("shade_alpha",ptr->shade_alpha);
  func("exw_back",ptr->exw_back);
  func("exw_text",ptr->exw_text);
  func("exw_divider",ptr->exw_divider);
  func("msgw_knob_dxy",ptr->msgw_knob_dxy);

  func("label_font",ptr->label_font.param);
  func("contour_font",ptr->contour_font.param);
  func("message_font",ptr->message_font.param);
  func("info_font",ptr->info_font.param);
  func("line_edit_font",ptr->line_edit_font.param);
  func("list_font",ptr->list_font.param);
  func("button_font",ptr->button_font.param);
  func("title_font",ptr->title_font.param);
  func("menu_font",ptr->menu_font.param);
 }

void UserPreferenceBag::sync(ConfigMap &map)
 {
  Members(this, [&map] (const char *name,AnyType &obj) { map.sync(name,obj); } );

  createFonts();
 }

void UserPreferenceBag::update(ConfigMap &map) const
 {
  Members(this, [&map] (const char *name,AnyType &obj) { map.update(name,obj); } );
 }

void UserPreferenceBag::bind(Bind &binder) // Update here
 {
  binder.group("Common");

   binder.item("space",space_dxy);
   binder.item("text cursor width",text_cursor_dx);
   binder.space();
   binder.item("check box",check_dxy);
   binder.item("knob box",knob_dxy);
   binder.item("radio box",radio_dxy);
   binder.item("switch box",switch_dxy);
   binder.item("light box",light_dxy);
   binder.item("scroll width",scroll_dxy);
   binder.item("progress height",progress_dy);
   binder.space();
   binder.item("line width",width);
   binder.space();
   binder.item("ground",ground);
   binder.item("back",back);
   binder.item("line",line);
   binder.item("inactive",inactive);
   binder.space();
   binder.item("bottom",bottom);
   binder.item("bottomUp",bottomUp);
   binder.item("top",top);
   binder.item("topUp",topUp);
   binder.space();
   binder.item("border",border);
   binder.item("focus",focus);
   binder.space();
   binder.item("face",face);
   binder.item("faceUp",faceUp);
   binder.item("mark",mark);

  binder.group("Text");

   binder.item("text select",text_select);
   binder.item("text cursor",text_cursor);
   binder.space();
   binder.item("label text",label_text);
   binder.item("contour text",contour_text);
   binder.space();
   binder.item("button text",button_text);
   binder.item("message text",message_text);
   binder.item("info text",info_text);
   binder.item("editor text",line_edit_text);
   binder.item("list text",list_text);
   binder.space();
   binder.item("button space",button_space);
   binder.item("message space",message_space);
   binder.item("editor space",line_edit_space);
   binder.item("info space",info_space);
   binder.item("list space",list_space);
   binder.item("menu space",menu_space);
   binder.space();
   binder.item("label font",label_font);
   binder.item("contour font",contour_font);
   binder.space();
   binder.item("button font",button_font);
   binder.item("message font",message_font);
   binder.item("info font",info_font);
   binder.item("editor font",line_edit_font);
   binder.item("list font",list_font);
   binder.item("menu font",menu_font);

  binder.group("Menu");

   binder.item("menu back",menu_back);
   binder.item("menu text",menu_text);
   binder.item("menu hilight",menu_hilight);
   binder.item("menu select",menu_select);
   binder.item("menu hot",menu_hot);
   binder.space();
   binder.item("use hotcolor",use_hotcolor);

  binder.group("Scroll List");

   binder.item("title",scroll_list_title);
   binder.item("title top",scroll_list_title_top);
   binder.item("title bottom",scroll_list_title_bottom);

  binder.group("Other");

   binder.item("cursor blink period",line_edit_period);
   binder.item("scroll speedUp period",scroll_speedUpPeriod);
   binder.item("progress ping time",progress_time);
   binder.item("progress ping period",progress_period);
   binder.space();
   binder.item("editor round ext",line_edit_ex);
   binder.space();
   binder.item("switch on",switch_on);
   binder.item("switch off",switch_off);
   binder.space();
   binder.item("progress border",progress_border);
   binder.item("progress ping top",progress_ping_top);
   binder.item("progress ping bottom",progress_ping_bottom);
   binder.item("progress active top",progress_active_top);
   binder.item("progress active bottom",progress_active_bottom);

  binder.group("Frame window");

   binder.item("alert blink time",blink_time);
   binder.item("alert blink period",blink_period);
   binder.space();
   binder.item("fatal error title",fatal_error);
   binder.space();
   binder.item("frame width",frame_dxy);
   binder.item("title height",title_dy);
   binder.item("frame button width",btn_dx);
   binder.item("frame button height",btn_dy);
   binder.space();
   binder.item("frame",frame);
   binder.item("active title",active_frame);
   binder.item("inactive title",inactive_frame);
   binder.item("hilight frame",frameHilight);
   binder.item("drag frame",frameDrag);
   binder.item("title",title);
   binder.space();
   binder.item("drag",drag);
   binder.item("hilight drag",dragHilight);
   binder.item("active drag",dragActive);
   binder.space();
   binder.item("button face",btnFace);
   binder.item("button hilight",btnFaceHilight);
   binder.item("button picture",btnPict);
   binder.item("button close picture",btnPictClose);
   binder.item("button alert picture",btnPictAlert);
   binder.item("button no-alert picture",btnPictNoAlert);
   binder.item("button close-alert picture",btnPictCloseAlert);
   binder.space();
   binder.item("title font",title_font);
   binder.space();
   binder.item("shade color",shade_color);
   binder.item("shade alpha",shade_alpha);

  binder.group("Exception window");

   binder.item("back",exw_back);
   binder.item("text",exw_text);
   binder.item("divider",exw_divider);

  binder.group("Message window");

   binder.item("ok knob box",msgw_knob_dxy);
 }

void UserPreferenceBag::createFonts()
 {
  label_font.create();
  contour_font.create();
  message_font.create();
  info_font.create();
  line_edit_font.create();
  list_font.create();
  button_font.create();
  title_font.create();
  menu_font.create();
 }

/* class UserPreference */

const char *const UserPreference::PrefFile="/UserPreference.ddl";

UserPreference::UserPreference() noexcept // Update here
 {
  // main windows

  cfg_ExceptionWindow.back.bind(exw_back);
  cfg_ExceptionWindow.text.bind(exw_text);
  cfg_ExceptionWindow.divider.bind(exw_divider);
  cfg_ExceptionWindow.scroll_dxy.bind(scroll_dxy);
  cfg_ExceptionWindow.font.bind(info_font.font);
  cfg_ExceptionWindow.scroll_cfg.bind(cfg_XScrollWindow);

  cfg_MessageSubWindow.back.bind(back);
  cfg_MessageSubWindow.knob_dxy.bind(msgw_knob_dxy);
  cfg_MessageSubWindow.space_dxy.bind(space_dxy);
  cfg_MessageSubWindow.info_cfg.bind(cfg_InfoWindow);
  cfg_MessageSubWindow.knob_cfg.bind(cfg_KnobWindow);
  cfg_MessageSubWindow.btn_cfg.bind(cfg_ButtonWindow);
  cfg_MessageSubWindow.dline_cfg.bind(cfg_XDoubleLineWindow);

  cfg_MessageWindow.frame_cfg.bind(cfg_FixedWindow);
  cfg_MessageWindow.msg_cfg.bind(cfg_MessageSubWindow);

  cfg_DragWindow.width.bind(width);
  cfg_DragWindow.frame_dxy.bind(frame_dxy);
  cfg_DragWindow.title_dy.bind(title_dy);
  cfg_DragWindow.btn_dx.bind(btn_dx);
  cfg_DragWindow.btn_dy.bind(btn_dy);
  cfg_DragWindow.top.bind(bottom);
  cfg_DragWindow.bottom.bind(top);
  cfg_DragWindow.frame.bind(frame);
  cfg_DragWindow.active.bind(active_frame);
  cfg_DragWindow.inactive.bind(inactive_frame);
  cfg_DragWindow.title.bind(title);
  cfg_DragWindow.title_font.bind(title_font.font);
  cfg_DragWindow.drag.bind(drag);
  cfg_DragWindow.dragHilight.bind(dragHilight);
  cfg_DragWindow.dragActive.bind(dragActive);

  cfg_DragWindow.btnFace.bind(btnFace);
  cfg_DragWindow.btnFaceHilight.bind(btnFaceHilight);
  cfg_DragWindow.btnPict.bind(btnPict);
  cfg_DragWindow.btnPictClose.bind(btnPictClose);
  cfg_DragWindow.btnPictAlert.bind(btnPictAlert);
  cfg_DragWindow.btnPictNoAlert.bind(btnPictNoAlert);
  cfg_DragWindow.btnPictCloseAlert.bind(btnPictCloseAlert);
  cfg_DragWindow.blink_time.bind(blink_time);
  cfg_DragWindow.blink_period.bind(blink_period);
  cfg_DragWindow.fatal_error.bind(fatal_error);
  cfg_DragWindow.shade_color.bind(shade_color);
  cfg_DragWindow.shade_alpha.bind(shade_alpha);

  cfg_FixedWindow.width.bind(width);
  cfg_FixedWindow.frame_dxy.bind(frame_dxy);
  cfg_FixedWindow.title_dy.bind(title_dy);
  cfg_FixedWindow.btn_dx.bind(btn_dx);
  cfg_FixedWindow.btn_dy.bind(btn_dy);
  cfg_FixedWindow.top.bind(bottom);
  cfg_FixedWindow.bottom.bind(top);
  cfg_FixedWindow.frame.bind(frame);
  cfg_FixedWindow.frameHilight.bind(frameHilight);
  cfg_FixedWindow.frameDrag.bind(frameDrag);
  cfg_FixedWindow.active.bind(active_frame);
  cfg_FixedWindow.inactive.bind(inactive_frame);
  cfg_FixedWindow.title.bind(title);
  cfg_FixedWindow.btnFace.bind(btnFace);
  cfg_FixedWindow.btnFaceHilight.bind(btnFaceHilight);
  cfg_FixedWindow.btnPictClose.bind(btnPictClose);
  cfg_FixedWindow.shade_color.bind(shade_color);
  cfg_FixedWindow.shade_alpha.bind(shade_alpha);

  cfg_ToolWindow.shade_color.bind(shade_color);
  cfg_ToolWindow.shade_alpha.bind(shade_alpha);

  cfg_SimpleCascadeMenu.frame_cfg.bind(cfg_ToolWindow);
  cfg_SimpleCascadeMenu.menu_cfg.bind(cfg_SimpleCascadeMenuWindow);

  // sub windows

  cfg_ButtonWindow.width.bind(width);
  cfg_ButtonWindow.space.bind(button_space);
  cfg_ButtonWindow.border.bind(border);
  cfg_ButtonWindow.focus.bind(focus);
  cfg_ButtonWindow.bottom.bind(bottom);
  cfg_ButtonWindow.top.bind(top);
  cfg_ButtonWindow.topUp.bind(topUp);
  cfg_ButtonWindow.text.bind(button_text);
  cfg_ButtonWindow.font.bind(button_font.font);

  cfg_KnobWindow.width.bind(width);
  cfg_KnobWindow.dxy.bind(knob_dxy);
  cfg_KnobWindow.border.bind(border);
  cfg_KnobWindow.focus.bind(focus);
  cfg_KnobWindow.bottom.bind(bottom);
  cfg_KnobWindow.top.bind(top);
  cfg_KnobWindow.topUp.bind(topUp);
  cfg_KnobWindow.face.bind(face);

  cfg_CheckWindow.dxy.bind(check_dxy);
  cfg_CheckWindow.border.bind(border);
  cfg_CheckWindow.focus.bind(focus);
  cfg_CheckWindow.bottom.bind(top);
  cfg_CheckWindow.top.bind(bottom);
  cfg_CheckWindow.bottomUp.bind(bottomUp);
  cfg_CheckWindow.mark.bind(mark);

  cfg_SwitchWindow.dxy.bind(switch_dxy);
  cfg_SwitchWindow.border.bind(border);
  cfg_SwitchWindow.focus.bind(focus);
  cfg_SwitchWindow.top.bind(top);
  cfg_SwitchWindow.bottom.bind(bottom);
  cfg_SwitchWindow.face.bind(face);
  cfg_SwitchWindow.faceUp.bind(faceUp);
  cfg_SwitchWindow.on.bind(switch_on);
  cfg_SwitchWindow.off.bind(switch_off);

  cfg_RadioWindow.dxy.bind(radio_dxy);
  cfg_RadioWindow.border.bind(border);
  cfg_RadioWindow.focus.bind(focus);
  cfg_RadioWindow.bottom.bind(top);
  cfg_RadioWindow.top.bind(bottom);
  cfg_RadioWindow.bottomUp.bind(bottomUp);
  cfg_RadioWindow.mark.bind(mark);

  cfg_TextWindow.width.bind(width);
  cfg_TextWindow.space.bind(message_space);
  cfg_TextWindow.top.bind(bottom);
  cfg_TextWindow.bottom.bind(top);
  cfg_TextWindow.inactive.bind(inactive);
  cfg_TextWindow.back.bind(back);
  cfg_TextWindow.text.bind(message_text);
  cfg_TextWindow.font.bind(message_font.font);

  cfg_LabelWindow.inactive.bind(inactive);
  cfg_LabelWindow.text.bind(label_text);
  cfg_LabelWindow.font.bind(label_font.font);

  cfg_XSingleLineWindow.width.bind(width);
  cfg_XSingleLineWindow.line.bind(line);

  cfg_YSingleLineWindow.width.bind(width);
  cfg_YSingleLineWindow.line.bind(line);

  cfg_XDoubleLineWindow.width.bind(width);
  cfg_XDoubleLineWindow.top.bind(top);
  cfg_XDoubleLineWindow.bottom.bind(bottom);

  cfg_YDoubleLineWindow.width.bind(width);
  cfg_YDoubleLineWindow.top.bind(top);
  cfg_YDoubleLineWindow.bottom.bind(bottom);

  cfg_ContourWindow.width.bind(width);
  cfg_ContourWindow.top.bind(bottom);
  cfg_ContourWindow.bottom.bind(top);

  cfg_TextContourWindow.width.bind(width);
  cfg_TextContourWindow.top.bind(bottom);
  cfg_TextContourWindow.bottom.bind(top);
  cfg_TextContourWindow.text.bind(contour_text);
  cfg_TextContourWindow.font.bind(contour_font.font);

  cfg_LightWindow.dxy.bind(light_dxy);
  cfg_LightWindow.top.bind(bottom);
  cfg_LightWindow.bottom.bind(top);
  cfg_LightWindow.inactive.bind(inactive);

  cfg_XScrollWindow.width.bind(width);
  cfg_XScrollWindow.dxy.bind(scroll_dxy);
  cfg_XScrollWindow.top.bind(top);
  cfg_XScrollWindow.bottom.bind(bottom);
  cfg_XScrollWindow.back.bind(back);
  cfg_XScrollWindow.focus.bind(focus);
  cfg_XScrollWindow.face.bind(face);
  cfg_XScrollWindow.faceUp.bind(faceUp);
  cfg_XScrollWindow.speedUpPeriod.bind(scroll_speedUpPeriod);

  cfg_YScrollWindow.width.bind(width);
  cfg_YScrollWindow.dxy.bind(scroll_dxy);
  cfg_YScrollWindow.top.bind(top);
  cfg_YScrollWindow.bottom.bind(bottom);
  cfg_YScrollWindow.back.bind(back);
  cfg_YScrollWindow.focus.bind(focus);
  cfg_YScrollWindow.face.bind(face);
  cfg_YScrollWindow.faceUp.bind(faceUp);
  cfg_YScrollWindow.speedUpPeriod.bind(scroll_speedUpPeriod);

  cfg_ProgressWindow.width.bind(width);
  cfg_ProgressWindow.dy.bind(progress_dy);
  cfg_ProgressWindow.top.bind(top);
  cfg_ProgressWindow.bottom.bind(bottom);
  cfg_ProgressWindow.border.bind(progress_border);
  cfg_ProgressWindow.ping_top.bind(progress_ping_top);
  cfg_ProgressWindow.ping_bottom.bind(progress_ping_bottom);
  cfg_ProgressWindow.active_top.bind(progress_active_top);
  cfg_ProgressWindow.active_bottom.bind(progress_active_bottom);
  cfg_ProgressWindow.time.bind(progress_time);
  cfg_ProgressWindow.period.bind(progress_period);

  cfg_InfoWindow.width.bind(width);
  cfg_InfoWindow.space.bind(info_space);
  cfg_InfoWindow.focus.bind(focus);
  cfg_InfoWindow.inactive.bind(inactive);
  cfg_InfoWindow.focus.bind(focus);
  cfg_InfoWindow.font.bind(info_font.font);

  cfg_LineEditWindow.width.bind(width);
  cfg_LineEditWindow.space.bind(line_edit_space);
  cfg_LineEditWindow.ex.bind(line_edit_ex);
  cfg_LineEditWindow.cursor_dx.bind(text_cursor_dx);
  cfg_LineEditWindow.back.bind(back);
  cfg_LineEditWindow.top.bind(bottom);
  cfg_LineEditWindow.bottom.bind(top);
  cfg_LineEditWindow.focus.bind(focus);
  cfg_LineEditWindow.inactive.bind(inactive);
  cfg_LineEditWindow.text.bind(line_edit_text);
  cfg_LineEditWindow.select.bind(text_select);
  cfg_LineEditWindow.cursor.bind(text_cursor);
  cfg_LineEditWindow.font.bind(line_edit_font.font);
  cfg_LineEditWindow.period.bind(line_edit_period);

  cfg_SimpleTextListWindow.width.bind(width);
  cfg_SimpleTextListWindow.space.bind(list_space);
  cfg_SimpleTextListWindow.back.bind(back);
  cfg_SimpleTextListWindow.top.bind(bottom);
  cfg_SimpleTextListWindow.bottom.bind(top);
  cfg_SimpleTextListWindow.text.bind(list_text);
  cfg_SimpleTextListWindow.inactive.bind(inactive);
  cfg_SimpleTextListWindow.focus.bind(focus);
  cfg_SimpleTextListWindow.select.bind(text_select);
  cfg_SimpleTextListWindow.font.bind(list_font.font);

  cfg_SimpleTopMenuWindow.width.bind(width);
  cfg_SimpleTopMenuWindow.space.bind(menu_space);
  cfg_SimpleTopMenuWindow.use_hotcolor.bind(use_hotcolor);
  cfg_SimpleTopMenuWindow.back.bind(menu_back);
  cfg_SimpleTopMenuWindow.text.bind(menu_text);
  cfg_SimpleTopMenuWindow.inactive.bind(inactive);
  cfg_SimpleTopMenuWindow.hilight.bind(menu_hilight);
  cfg_SimpleTopMenuWindow.select.bind(menu_select);
  cfg_SimpleTopMenuWindow.hot.bind(menu_hot);
  cfg_SimpleTopMenuWindow.left.bind(top);
  cfg_SimpleTopMenuWindow.right.bind(bottom);
  cfg_SimpleTopMenuWindow.font.bind(menu_font.font);

  cfg_SimpleCascadeMenuWindow.width.bind(width);
  cfg_SimpleCascadeMenuWindow.space.bind(menu_space);
  cfg_SimpleCascadeMenuWindow.use_hotcolor.bind(use_hotcolor);
  cfg_SimpleCascadeMenuWindow.back.bind(menu_back);
  cfg_SimpleCascadeMenuWindow.text.bind(menu_text);
  cfg_SimpleCascadeMenuWindow.inactive.bind(inactive);
  cfg_SimpleCascadeMenuWindow.hilight.bind(menu_hilight);
  cfg_SimpleCascadeMenuWindow.select.bind(menu_select);
  cfg_SimpleCascadeMenuWindow.hot.bind(menu_hot);
  cfg_SimpleCascadeMenuWindow.top.bind(top);
  cfg_SimpleCascadeMenuWindow.bottom.bind(bottom);
  cfg_SimpleCascadeMenuWindow.font.bind(menu_font.font);

  cfg_ScrollListWindow.width.bind(width);
  cfg_ScrollListWindow.space.bind(list_space);
  cfg_ScrollListWindow.back.bind(back);
  cfg_ScrollListWindow.top.bind(bottom);
  cfg_ScrollListWindow.bottom.bind(top);
  cfg_ScrollListWindow.text.bind(list_text);
  cfg_ScrollListWindow.inactive.bind(inactive);
  cfg_ScrollListWindow.focus.bind(focus);
  cfg_ScrollListWindow.select.bind(text_select);
  cfg_ScrollListWindow.font.bind(list_font.font);
  cfg_ScrollListWindow.title.bind(scroll_list_title);
  cfg_ScrollListWindow.title_top.bind(scroll_list_title_top);
  cfg_ScrollListWindow.title_bottom.bind(scroll_list_title_bottom);
  cfg_ScrollListWindow.x_cfg.bind(cfg_XScrollWindow);
  cfg_ScrollListWindow.y_cfg.bind(cfg_YScrollWindow);
 }

UserPreference::~UserPreference()
 {
 }

void UserPreference::sync() noexcept
 {
  try
    {
     HomeDir home;

     MakeString<MaxPathLen> buf;

     buf.add(home.get(),HomeKey);

     StrLen dir=buf.get();

     buf.add(PrefFile);

     if( !buf )
       {
        Printf(Exception,"CCore::Video::UserPreference::sync() : too long file name");
       }

     StrLen file_name=buf.get();

     ConfigMap map;

     map.loadDDL_safe(file_name);

     UserPreferenceBag::sync(map);

     if( map.isModified() )
       {
        FileSystem fs;

        if( fs.getFileType(dir)==FileType_none ) fs.createDir(dir);

        map.saveDDL(file_name);
       }
    }
  catch(...)
    {
    }
 }

void UserPreference::update() noexcept
 {
  try
    {
     HomeDir home;

     MakeString<MaxPathLen> buf;

     buf.add(home.get(),HomeKey);

     StrLen dir=buf.get();

     buf.add(PrefFile);

     if( !buf )
       {
        Printf(Exception,"CCore::Video::UserPreference::update() : too long file name");
       }

     StrLen file_name=buf.get();

     ConfigMap map;

     map.loadDDL_safe(file_name);

     UserPreferenceBag::update(map);

     FileSystem fs;

     if( fs.getFileType(dir)==FileType_none ) fs.createDir(dir);

     map.saveDDL(file_name);
    }
  catch(...)
    {
    }
 }

} // namespace Video
} // namespace CCore

