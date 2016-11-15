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

void UserPreferenceBag::sync(ConfigMap &map)
 {
  map.sync("space_dxy",space_dxy);
  map.sync("text_cursor_dx",text_cursor_dx);
  map.sync("check_dxy",check_dxy);
  map.sync("knob_dxy",knob_dxy);
  map.sync("radio_dxy",radio_dxy);
  map.sync("swtch_dxy",swtch_dxy);
  map.sync("light_dxy",light_dxy);
  map.sync("scroll_dxy",scroll_dxy);
  map.sync("progress_dy",progress_dy);
  map.sync("width",width);
  map.sync("ground",ground);
  map.sync("back",back);
  map.sync("line",line);
  map.sync("inactive",inactive);
  map.sync("bottom",bottom);
  map.sync("bottomUp",bottomUp);
  map.sync("top",top);
  map.sync("topUp",topUp);
  map.sync("border",border);
  map.sync("focus",focus);
  map.sync("face",face);
  map.sync("faceUp",faceUp);
  map.sync("mark",mark);
  map.sync("text_select",text_select);
  map.sync("text_cursor",text_cursor);
  map.sync("label_text",label_text);
  map.sync("contour_text",contour_text);
  map.sync("button_text",button_text);
  map.sync("message_text",message_text);
  map.sync("info_text",info_text);
  map.sync("line_edit_text",line_edit_text);
  map.sync("list_text",list_text);
  map.sync("button_space",button_space);
  map.sync("message_space",message_space);
  map.sync("line_edit_space",line_edit_space);
  map.sync("info_space",info_space);
  map.sync("list_space",list_space);
  map.sync("menu_space",menu_space);
  map.sync("menu_back",menu_back);
  map.sync("menu_text",menu_text);
  map.sync("menu_hilight",menu_hilight);
  map.sync("menu_select",menu_select);
  map.sync("menu_hot",menu_hot);
  map.sync("use_hotcolor",use_hotcolor);
  map.sync("scroll_list_title",scroll_list_title);
  map.sync("scroll_list_title_top",scroll_list_title_top);
  map.sync("scroll_list_title_bottom",scroll_list_title_bottom);
  map.sync("line_edit_period",line_edit_period);
  map.sync("scroll_speedUpPeriod",scroll_speedUpPeriod);
  map.sync("progress_time",progress_time);
  map.sync("progress_period",progress_period);
  map.sync("line_edit_ex",line_edit_ex);
  map.sync("switch_on",switch_on);
  map.sync("switch_off",switch_off);
  map.sync("progress_border",progress_border);
  map.sync("progress_ping_top",progress_ping_top);
  map.sync("progress_ping_bottom",progress_ping_bottom);
  map.sync("progress_active_top",progress_active_top);
  map.sync("progress_active_bottom",progress_active_bottom);
  map.sync("blink_time",blink_time);
  map.sync("blink_period",blink_period);
  map.sync("fatal_error",fatal_error);
  map.sync("frame_dxy",frame_dxy);
  map.sync("title_dy",title_dy);
  map.sync("btn_dx",btn_dx);
  map.sync("btn_dy",btn_dy);
  map.sync("frame",frame);
  map.sync("active_frame",active_frame);
  map.sync("inactive_frame",inactive_frame);
  map.sync("frameHilight",frameHilight);
  map.sync("frameDrag",frameDrag);
  map.sync("title",title);
  map.sync("drag",drag);
  map.sync("dragHilight",dragHilight);
  map.sync("dragActive",dragActive);
  map.sync("btnFace",btnFace);
  map.sync("btnFaceHilight",btnFaceHilight);
  map.sync("btnPict",btnPict);
  map.sync("btnPictClose",btnPictClose);
  map.sync("btnPictAlert",btnPictAlert);
  map.sync("btnPictNoAlert",btnPictNoAlert);
  map.sync("btnPictCloseAlert",btnPictCloseAlert);
  map.sync("shade_color",shade_color);
  map.sync("shade_alpha",shade_alpha);
  map.sync("exw_back",exw_back);
  map.sync("exw_text",exw_text);
  map.sync("exw_divider",exw_divider);
  map.sync("msgw_knob_dxy",msgw_knob_dxy);

  map.sync("label_font",label_font.param);
  map.sync("contour_font",contour_font.param);
  map.sync("message_font",message_font.param);
  map.sync("info_font",info_font.param);
  map.sync("line_edit_font",line_edit_font.param);
  map.sync("list_font",list_font.param);
  map.sync("button_font",button_font.param);
  map.sync("title_font",title_font.param);
  map.sync("menu_font",menu_font.param);

  createFonts();
 }

void UserPreferenceBag::update(ConfigMap &map) const
 {
  map.update("space_dxy",space_dxy);
  map.update("text_cursor_dx",text_cursor_dx);
  map.update("check_dxy",check_dxy);
  map.update("knob_dxy",knob_dxy);
  map.update("radio_dxy",radio_dxy);
  map.update("swtch_dxy",swtch_dxy);
  map.update("light_dxy",light_dxy);
  map.update("scroll_dxy",scroll_dxy);
  map.update("progress_dy",progress_dy);
  map.update("width",width);
  map.update("ground",ground);
  map.update("back",back);
  map.update("line",line);
  map.update("inactive",inactive);
  map.update("bottom",bottom);
  map.update("bottomUp",bottomUp);
  map.update("top",top);
  map.update("topUp",topUp);
  map.update("border",border);
  map.update("focus",focus);
  map.update("face",face);
  map.update("faceUp",faceUp);
  map.update("mark",mark);
  map.update("text_select",text_select);
  map.update("text_cursor",text_cursor);
  map.update("label_text",label_text);
  map.update("contour_text",contour_text);
  map.update("button_text",button_text);
  map.update("message_text",message_text);
  map.update("info_text",info_text);
  map.update("line_edit_text",line_edit_text);
  map.update("list_text",list_text);
  map.update("button_space",button_space);
  map.update("message_space",message_space);
  map.update("line_edit_space",line_edit_space);
  map.update("info_space",info_space);
  map.update("list_space",list_space);
  map.update("menu_space",menu_space);
  map.update("menu_back",menu_back);
  map.update("menu_text",menu_text);
  map.update("menu_hilight",menu_hilight);
  map.update("menu_select",menu_select);
  map.update("menu_hot",menu_hot);
  map.update("use_hotcolor",use_hotcolor);
  map.update("scroll_list_title",scroll_list_title);
  map.update("scroll_list_title_top",scroll_list_title_top);
  map.update("scroll_list_title_bottom",scroll_list_title_bottom);
  map.update("line_edit_period",line_edit_period);
  map.update("scroll_speedUpPeriod",scroll_speedUpPeriod);
  map.update("progress_time",progress_time);
  map.update("progress_period",progress_period);
  map.update("line_edit_ex",line_edit_ex);
  map.update("switch_on",switch_on);
  map.update("switch_off",switch_off);
  map.update("progress_border",progress_border);
  map.update("progress_ping_top",progress_ping_top);
  map.update("progress_ping_bottom",progress_ping_bottom);
  map.update("progress_active_top",progress_active_top);
  map.update("progress_active_bottom",progress_active_bottom);
  map.update("blink_time",blink_time);
  map.update("blink_period",blink_period);
  map.update("fatal_error",fatal_error);
  map.update("frame_dxy",frame_dxy);
  map.update("title_dy",title_dy);
  map.update("btn_dx",btn_dx);
  map.update("btn_dy",btn_dy);
  map.update("frame",frame);
  map.update("active_frame",active_frame);
  map.update("inactive_frame",inactive_frame);
  map.update("frameHilight",frameHilight);
  map.update("frameDrag",frameDrag);
  map.update("title",title);
  map.update("drag",drag);
  map.update("dragHilight",dragHilight);
  map.update("dragActive",dragActive);
  map.update("btnFace",btnFace);
  map.update("btnFaceHilight",btnFaceHilight);
  map.update("btnPict",btnPict);
  map.update("btnPictClose",btnPictClose);
  map.update("btnPictAlert",btnPictAlert);
  map.update("btnPictNoAlert",btnPictNoAlert);
  map.update("btnPictCloseAlert",btnPictCloseAlert);
  map.update("shade_color",shade_color);
  map.update("shade_alpha",shade_alpha);
  map.update("exw_back",exw_back);
  map.update("exw_text",exw_text);
  map.update("exw_divider",exw_divider);
  map.update("msgw_knob_dxy",msgw_knob_dxy);

  map.update("label_font",label_font.param);
  map.update("contour_font",contour_font.param);
  map.update("message_font",message_font.param);
  map.update("info_font",info_font.param);
  map.update("line_edit_font",line_edit_font.param);
  map.update("list_font",list_font.param);
  map.update("button_font",button_font.param);
  map.update("title_font",title_font.param);
  map.update("menu_font",menu_font.param);
 }

void UserPreferenceBag::bind(Bind &binder)
 {
  binder.group("Common");

   binder.item("space",space_dxy);
   binder.item("text cursor width",text_cursor_dx);
   binder.space();
   binder.item("check box",check_dxy);
   binder.item("knob box",knob_dxy);
   binder.item("radio box",radio_dxy);
   binder.item("switch box",swtch_dxy);
   binder.item("light box",light_dxy);
   binder.item("scroll width",scroll_dxy);
   binder.item("progress width",progress_dy);
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
   binder.item("shade_color",shade_color);
   binder.item("shade_alpha",shade_alpha);

  binder.group("Exception window");

   binder.item("exception back",exw_back);
   binder.item("exception text",exw_text);
   binder.item("exception divider",exw_divider);

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

UserPreference::UserPreference() noexcept
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

  cfg_RadioWindow.dxy.bind(radio_dxy);
  cfg_RadioWindow.border.bind(border);
  cfg_RadioWindow.focus.bind(focus);
  cfg_RadioWindow.bottom.bind(top);
  cfg_RadioWindow.top.bind(bottom);
  cfg_RadioWindow.bottomUp.bind(bottomUp);
  cfg_RadioWindow.mark.bind(mark);

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

  cfg_LabelWindow.inactive.bind(inactive);
  cfg_LabelWindow.text.bind(label_text);
  cfg_LabelWindow.font.bind(label_font.font);

  cfg_TextWindow.width.bind(width);
  cfg_TextWindow.space.bind(message_space);
  cfg_TextWindow.top.bind(bottom);
  cfg_TextWindow.bottom.bind(top);
  cfg_TextWindow.inactive.bind(inactive);
  cfg_TextWindow.back.bind(back);
  cfg_TextWindow.text.bind(message_text);
  cfg_TextWindow.font.bind(message_font.font);

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

  cfg_XScrollWindow.width.bind(width);
  cfg_XScrollWindow.top.bind(top);
  cfg_XScrollWindow.bottom.bind(bottom);
  cfg_XScrollWindow.back.bind(back);
  cfg_XScrollWindow.focus.bind(focus);
  cfg_XScrollWindow.face.bind(face);
  cfg_XScrollWindow.faceUp.bind(faceUp);
  cfg_XScrollWindow.speedUpPeriod.bind(scroll_speedUpPeriod);

  cfg_YScrollWindow.width.bind(width);
  cfg_YScrollWindow.top.bind(top);
  cfg_YScrollWindow.bottom.bind(bottom);
  cfg_YScrollWindow.back.bind(back);
  cfg_YScrollWindow.focus.bind(focus);
  cfg_YScrollWindow.face.bind(face);
  cfg_YScrollWindow.faceUp.bind(faceUp);
  cfg_YScrollWindow.speedUpPeriod.bind(scroll_speedUpPeriod);

  cfg_LightWindow.dxy.bind(light_dxy);
  cfg_LightWindow.top.bind(bottom);
  cfg_LightWindow.bottom.bind(top);
  cfg_LightWindow.inactive.bind(inactive);

  cfg_SwitchWindow.dxy.bind(swtch_dxy);
  cfg_SwitchWindow.border.bind(border);
  cfg_SwitchWindow.focus.bind(focus);
  cfg_SwitchWindow.top.bind(top);
  cfg_SwitchWindow.bottom.bind(bottom);
  cfg_SwitchWindow.face.bind(face);
  cfg_SwitchWindow.faceUp.bind(faceUp);
  cfg_SwitchWindow.on.bind(switch_on);
  cfg_SwitchWindow.off.bind(switch_off);

  cfg_ProgressWindow.width.bind(width);
  cfg_ProgressWindow.top.bind(top);
  cfg_ProgressWindow.bottom.bind(bottom);
  cfg_ProgressWindow.border.bind(progress_border);
  cfg_ProgressWindow.ping_top.bind(progress_ping_top);
  cfg_ProgressWindow.ping_bottom.bind(progress_ping_bottom);
  cfg_ProgressWindow.active_top.bind(progress_active_top);
  cfg_ProgressWindow.active_bottom.bind(progress_active_bottom);
  cfg_ProgressWindow.time.bind(progress_time);
  cfg_ProgressWindow.period.bind(progress_period);

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

  cfg_ScrollListWindow.scroll_dxy.bind(scroll_dxy);
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

