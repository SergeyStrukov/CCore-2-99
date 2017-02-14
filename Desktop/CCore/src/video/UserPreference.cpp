/* UserPreference.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 3.00
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

namespace CCore {
namespace Video {

/* struct UserPreferenceBag */

template <class Ptr,class Func>
void UserPreferenceBag::Members(Ptr ptr,Func func) // Update here
 {
  func("width",ptr->width);

  func("space_dxy",ptr->space_dxy);

  func("check_dxy",ptr->check_dxy);
  func("knob_dxy",ptr->knob_dxy);
  func("radio_dxy",ptr->radio_dxy);
  func("switch_dxy",ptr->switch_dxy);
  func("light_dxy",ptr->light_dxy);
  func("scroll_dxy",ptr->scroll_dxy);
  func("progress_dy",ptr->progress_dy);

  func("gray",ptr->gray);
  func("grayUp",ptr->grayUp);
  func("snow",ptr->snow);
  func("snowUp",ptr->snowUp);

  func("back",ptr->back);
  func("line",ptr->line);
  func("inactive",ptr->inactive);

  func("border",ptr->border);
  func("focus",ptr->focus);

  func("face",ptr->face);
  func("faceUp",ptr->faceUp);
  func("mark",ptr->mark);
  func("alert",ptr->alert);

  func("text_Yes",ptr->text_Yes);
  func("text_No",ptr->text_No);

  func("text_Ok",ptr->text_Ok);
  func("text_Cancel",ptr->text_Cancel);

  func("text_LoadFile",ptr->text_LoadFile);
  func("text_SaveFile",ptr->text_SaveFile);

  func("text_Alert",ptr->text_Alert);
  func("text_AskSave",ptr->text_AskSave);

  func("text_Error",ptr->text_Error);
  func("text_Warning",ptr->text_Warning);

  func("text_Close",ptr->text_Close);
  func("text_Insert",ptr->text_Insert);

  func("text_cursor_dx",ptr->text_cursor_dx);

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

  func("label_font",ptr->label_font.param);
  func("contour_font",ptr->contour_font.param);

  func("button_font",ptr->button_font.param);
  func("message_font",ptr->message_font.param);
  func("info_font",ptr->info_font.param);
  func("line_edit_font",ptr->line_edit_font.param);
  func("list_font",ptr->list_font.param);
  func("menu_font",ptr->menu_font.param);

  func("code_font",ptr->code_font.param);

  func("switch_on",ptr->switch_on);
  func("switch_off",ptr->switch_off);

  func("progress_time",ptr->progress_time);
  func("progress_period",ptr->progress_period);

  func("progress_border",ptr->progress_border);
  func("progress_grayUp",ptr->progress_grayUp);
  func("progress_snowUp",ptr->progress_snowUp);
  func("progress_grayPing",ptr->progress_grayPing);
  func("progress_snowPing",ptr->progress_snowPing);

  func("scroll_speedUpPeriod",ptr->scroll_speedUpPeriod);

  func("line_edit_period",ptr->line_edit_period);

  func("line_edit_ex",ptr->line_edit_ex);

  func("scroll_list_title",ptr->scroll_list_title);
  func("scroll_list_titleTop",ptr->scroll_list_titleTop);
  func("scroll_list_titleBottom",ptr->scroll_list_titleBottom);

  func("menu_back",ptr->menu_back);
  func("menu_text",ptr->menu_text);
  func("menu_hilight",ptr->menu_hilight);
  func("menu_select",ptr->menu_select);
  func("menu_hot",ptr->menu_hot);
  func("menu_hotcolor",ptr->menu_hotcolor);

  func("blinkTime",ptr->blinkTime);
  func("blinkPeriod",ptr->blinkPeriod);

  func("frame_dxy",ptr->frame_dxy);
  func("title_dy",ptr->title_dy);
  func("btn_dx",ptr->btn_dx);
  func("btn_dy",ptr->btn_dy);

  func("frame",ptr->frame);
  func("frameHilight",ptr->frameHilight);
  func("frameDrag",ptr->frameDrag);

  func("title",ptr->title);
  func("titleActive",ptr->titleActive);
  func("titleInactive",ptr->titleInactive);

  func("drag",ptr->drag);
  func("dragHilight",ptr->dragHilight);
  func("dragActive",ptr->dragActive);
  func("dragSmall",ptr->dragSmall);

  func("btnFace",ptr->btnFace);
  func("btnFaceHilight",ptr->btnFaceHilight);
  func("btnPict",ptr->btnPict);
  func("btnPictClose",ptr->btnPictClose);
  func("btnPictAlert",ptr->btnPictAlert);
  func("btnPictNoAlert",ptr->btnPictNoAlert);
  func("btnPictCloseAlert",ptr->btnPictCloseAlert);

  func("hintBack",ptr->hintBack);
  func("hintText",ptr->hintText);
  func("hintBorder",ptr->hintBorder);

  func("hintWidth",ptr->hintWidth);

  func("title_font",ptr->title_font.param);
  func("hint_font",ptr->hint_font.param);

  func("shadeColor",ptr->shadeColor);
  func("shadeAlpha",ptr->shadeAlpha);

  func("text_Fatal_error",ptr->text_Fatal_error);
  func("text_No_hint",ptr->text_No_hint);

  func("hint_ResizeTopLeft",ptr->hint_ResizeTopLeft);
  func("hint_ResizeLeft",ptr->hint_ResizeLeft);
  func("hint_ResizeBottomLeft",ptr->hint_ResizeBottomLeft);
  func("hint_ResizeBottom",ptr->hint_ResizeBottom);
  func("hint_ResizeBottomRight",ptr->hint_ResizeBottomRight);
  func("hint_ResizeRight",ptr->hint_ResizeRight);
  func("hint_ResizeTopRight",ptr->hint_ResizeTopRight);

  func("hint_Alert",ptr->hint_Alert);
  func("hint_Help",ptr->hint_Help);
  func("hint_Minimize",ptr->hint_Minimize);
  func("hint_Maximize",ptr->hint_Maximize);
  func("hint_Restore",ptr->hint_Restore);
  func("hint_Close",ptr->hint_Close);

  func("exception_back",ptr->exception_back);
  func("exception_text",ptr->exception_text);
  func("exception_divider",ptr->exception_divider);

  func("message_knob_dxy",ptr->message_knob_dxy);

  func("message_pos_ry",ptr->message_pos_ry);

  func("file_alt_dxy",ptr->file_alt_dxy);

  func("file_accent",ptr->file_accent);
  func("file_filter_text",ptr->file_filter_text);

  func("file_faceRight",ptr->file_faceRight);
  func("file_faceDown",ptr->file_faceDown);

  func("file_filter_font",ptr->file_filter_font.param);

  func("text_New_file",ptr->text_New_file);

  func("hint_FileHitList",ptr->hint_FileHitList);
  func("hint_FileAddHit",ptr->hint_FileAddHit);
  func("hint_FileUpdir",ptr->hint_FileUpdir);
  func("hint_FileCurdir",ptr->hint_FileCurdir);
  func("hint_FileDirList",ptr->hint_FileDirList);
  func("hint_FileList",ptr->hint_FileList);
  func("hint_FileMakeDir",ptr->hint_FileMakeDir);
  func("hint_FileRemoveDir",ptr->hint_FileRemoveDir);
  func("hint_FileAlt",ptr->hint_FileAlt);

  func("hint_FileEnableFilter",ptr->hint_FileEnableFilter);
  func("hint_FileDelFilter",ptr->hint_FileDelFilter);
  func("hint_FileFilter",ptr->hint_FileFilter);

  func("hint_FileAddFilter",ptr->hint_FileAddFilter);

  func("coord_edit_width",ptr->coord_edit_width);
  func("coord_edit_mark_dy",ptr->coord_edit_mark_dy);

  func("coord_edit_line",ptr->coord_edit_line);
 }

void UserPreferenceBag::bind(ConfigItemBind &binder) // Update here
 {
  binder.group("Common"_def);

   binder.item("line width"_def,width);
   binder.item("space"_def,space_dxy);
   binder.space();
   binder.item("check box"_def,check_dxy);
   binder.item("knob box"_def,knob_dxy);
   binder.item("radio box"_def,radio_dxy);
   binder.item("switch box"_def,switch_dxy);
   binder.item("light box"_def,light_dxy);
   binder.item("scroll width"_def,scroll_dxy);
   binder.item("progress height"_def,progress_dy);
   binder.space();
   binder.item("gray"_def,gray);
   binder.item("grayUp"_def,grayUp);
   binder.item("snow"_def,snow);
   binder.item("snowUp"_def,snowUp);
   binder.space();
   binder.item("back"_def,back);
   binder.item("line"_def,line);
   binder.item("inactive"_def,inactive);
   binder.space();
   binder.item("border"_def,border);
   binder.item("focus"_def,focus);
   binder.space();
   binder.item("face"_def,face);
   binder.item("faceUp"_def,faceUp);
   binder.item("mark"_def,mark);
   binder.item("alert"_def,alert);
   binder.space();
   binder.item("'Yes'"_def,text_Yes);
   binder.item("'No'"_def,text_No);
   binder.item("'Ok'"_def,text_Ok);
   binder.item("'Cancel'"_def,text_Cancel);
   binder.item("'LoadFile'"_def,text_LoadFile);
   binder.item("'SaveFile'"_def,text_SaveFile);
   binder.item("'Alert'"_def,text_Alert);
   binder.item("'AskSave'"_def,text_AskSave);
   binder.item("'Error'"_def,text_Error);
   binder.item("'Warning'"_def,text_Warning);
   binder.item("'Close'"_def,text_Close);
   binder.item("'Insert'"_def,text_Insert);

  binder.group("Text"_def);

   binder.item("cursor width"_def,text_cursor_dx);
   binder.space();
   binder.item("select"_def,text_select);
   binder.item("cursor"_def,text_cursor);
   binder.space();
   binder.item("label text"_def,label_text);
   binder.item("contour text"_def,contour_text);
   binder.space();
   binder.item("button text"_def,button_text);
   binder.item("message text"_def,message_text);
   binder.item("info text"_def,info_text);
   binder.item("editor text"_def,line_edit_text);
   binder.item("list text"_def,list_text);
   binder.space();
   binder.item("button space"_def,button_space);
   binder.item("message space"_def,message_space);
   binder.item("editor space"_def,line_edit_space);
   binder.item("info space"_def,info_space);
   binder.item("list space"_def,list_space);
   binder.item("menu space"_def,menu_space);
   binder.space();
   binder.item("label font"_def,label_font);
   binder.item("contour font"_def,contour_font);
   binder.space();
   binder.item("button font"_def,button_font);
   binder.item("message font"_def,message_font);
   binder.item("info font"_def,info_font);
   binder.item("editor font"_def,line_edit_font);
   binder.item("list font"_def,list_font);
   binder.item("menu font"_def,menu_font);
   binder.space();
   binder.item("code font"_def,code_font);

  binder.group("Switch"_def);

   binder.item("on"_def,switch_on);
   binder.item("off"_def,switch_off);

  binder.group("Progress"_def);

   binder.item("ping time"_def,progress_time);
   binder.item("ping period"_def,progress_period);
   binder.space();
   binder.item("border"_def,progress_border);
   binder.item("grayUp"_def,progress_grayUp);
   binder.item("snowUp"_def,progress_snowUp);
   binder.item("grayPing"_def,progress_grayPing);
   binder.item("snowPing"_def,progress_snowPing);

  binder.group("Scroll"_def);

   binder.item("speedUp period"_def,scroll_speedUpPeriod);

  binder.group("Line editor"_def);

   binder.item("cursor blink period"_def,line_edit_period);
   binder.space();
   binder.item("round ext"_def,line_edit_ex);

  binder.group("Scroll list"_def);

   binder.item("title"_def,scroll_list_title);
   binder.item("title top"_def,scroll_list_titleTop);
   binder.item("title bottom"_def,scroll_list_titleBottom);

  binder.group("Menu"_def);

   binder.item("back"_def,menu_back);
   binder.item("text"_def,menu_text);
   binder.item("hilight"_def,menu_hilight);
   binder.item("select"_def,menu_select);
   binder.item("hot"_def,menu_hot);
   binder.space();
   binder.item("use hotcolor"_def,menu_hotcolor);

  binder.group("Frames"_def);

   binder.item("alert blink time"_def,blinkTime);
   binder.item("alert blink period"_def,blinkPeriod);
   binder.space();
   binder.item("'Fatal error'"_def,text_Fatal_error);
   binder.item("'<No hint available>'"_def,text_No_hint);
   binder.space();
   binder.item("?'Resize top-left'"_def,hint_ResizeTopLeft);
   binder.item("?'Resize left'"_def,hint_ResizeLeft);
   binder.item("?'Resize bottom-left'"_def,hint_ResizeBottomLeft);
   binder.item("?'Resize bottom'"_def,hint_ResizeBottom);
   binder.item("?'Resize bottom-right'"_def,hint_ResizeBottomRight);
   binder.item("?'Resize right'"_def,hint_ResizeRight);
   binder.item("?'Resize top-right'"_def,hint_ResizeTopRight);
   binder.space();
   binder.item("?'Open/close alert view'"_def,hint_Alert);
   binder.item("?'Help on/off'"_def,hint_Help);
   binder.item("?'Minimize'"_def,hint_Minimize);
   binder.item("?'Maximize'"_def,hint_Maximize);
   binder.item("?'Restore'"_def,hint_Restore);
   binder.item("?'Close'"_def,hint_Close);
   binder.space();
   binder.item("frame width"_def,frame_dxy);
   binder.item("title height"_def,title_dy);
   binder.item("frame button width"_def,btn_dx);
   binder.item("frame button height"_def,btn_dy);
   binder.space();
   binder.item("frame"_def,frame);
   binder.item("hilight frame"_def,frameHilight);
   binder.item("drag frame"_def,frameDrag);
   binder.item("title"_def,title);
   binder.item("active title"_def,titleActive);
   binder.item("inactive title"_def,titleInactive);
   binder.space();
   binder.item("drag"_def,drag);
   binder.item("hilight drag"_def,dragHilight);
   binder.item("active drag"_def,dragActive);
   binder.item("small size indicator"_def,dragSmall);
   binder.space();
   binder.item("button face"_def,btnFace);
   binder.item("button hilight"_def,btnFaceHilight);
   binder.item("button picture"_def,btnPict);
   binder.item("button close picture"_def,btnPictClose);
   binder.item("button alert picture"_def,btnPictAlert);
   binder.item("button no-alert picture"_def,btnPictNoAlert);
   binder.item("button close-alert picture"_def,btnPictCloseAlert);
   binder.space();
   binder.item("hint back"_def,hintBack);
   binder.item("hint text"_def,hintText);
   binder.item("hint border"_def,hintBorder);
   binder.item("hint line width"_def,hintWidth);
   binder.space();
   binder.item("title font"_def,title_font);
   binder.item("hint font"_def,hint_font);
   binder.space();
   binder.item("shade color"_def,shadeColor);
   binder.item("shade alpha"_def,shadeAlpha);

  binder.group("Exception window"_def);

   binder.item("back"_def,exception_back);
   binder.item("text"_def,exception_text);
   binder.item("divider"_def,exception_divider);

  binder.group("Message window"_def);

   binder.item("knob box"_def,message_knob_dxy);
   binder.item("Y-position ratio"_def,message_pos_ry);

  binder.group("File window"_def);

   binder.item("alt box"_def,file_alt_dxy);
   binder.space();
   binder.item("slash accent"_def,file_accent);
   binder.space();
   binder.item("alt right"_def,file_faceRight);
   binder.item("alt down"_def,file_faceDown);
   binder.space();
   binder.item("filter text"_def,file_filter_text);
   binder.item("filter font"_def,file_filter_font);
   binder.space();
   binder.item("'New file'"_def,text_New_file);
   binder.space();
   binder.item("?'Hit list'"_def,hint_FileHitList);
   binder.item("?'Add hit'"_def,hint_FileAddHit);
   binder.item("?'Goto the parent'"_def,hint_FileUpdir);
   binder.item("?'Current directory'"_def,hint_FileCurdir);
   binder.item("?'Subdirectory list'"_def,hint_FileDirList);
   binder.item("?'File list'"_def,hint_FileList);
   binder.item("?'Make directory'"_def,hint_FileMakeDir);
   binder.item("?'Remove directory'"_def,hint_FileRemoveDir);
   binder.item("?'Alt new/existing file'"_def,hint_FileAlt);
   binder.item("?'Enable filter'"_def,hint_FileEnableFilter);
   binder.item("?'Delete filter'"_def,hint_FileDelFilter);
   binder.item("?'Filename filter'"_def,hint_FileFilter);
   binder.item("?'Add filter'"_def,hint_FileAddFilter);

  binder.group("CoordEdit window"_def);

    binder.item("line width"_def,coord_edit_width);
    binder.item("mark height"_def,coord_edit_mark_dy);
    binder.space();
    binder.item("line"_def,coord_edit_line);
 }

void UserPreferenceBag::createFonts() // Update fonts here
 {
  label_font.create();
  contour_font.create();
  button_font.create();
  message_font.create();
  info_font.create();
  line_edit_font.create();
  list_font.create();
  menu_font.create();

  code_font.create();

  title_font.create();
  hint_font.create();

  file_filter_font.create();
 }

/* class UserPreference */

StrLen UserPreference::PrefFile() { return "/UserPreference.ddl"_c; }

UserPreference::UserPreference() noexcept
 {
 }

UserPreference::~UserPreference()
 {
 }

void UserPreference::sync() noexcept
 {
  syncHome(HomeKey(),PrefFile());
 }

void UserPreference::update() noexcept
 {
  updateHome(HomeKey(),PrefFile());
 }

} // namespace Video
} // namespace CCore

