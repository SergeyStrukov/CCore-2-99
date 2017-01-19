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
  func("back",ptr->back);
  func("line",ptr->line);
  func("inactive",ptr->inactive);

  func("gray",ptr->gray);
  func("grayUp",ptr->grayUp);
  func("snow",ptr->snow);
  func("snowUp",ptr->snowUp);

  func("border",ptr->border);
  func("focus",ptr->focus);
  func("face",ptr->face);
  func("faceUp",ptr->faceUp);
  func("mark",ptr->mark);
  func("alert",ptr->alert);
  func("text_Ok",ptr->text_Ok);
  func("text_Cancel",ptr->text_Cancel);
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
  func("menu_hotcolor",ptr->menu_hotcolor);

  func("scroll_list_title",ptr->scroll_list_title);
  func("scroll_list_titleTop",ptr->scroll_list_titleTop);
  func("scroll_list_titleBottom",ptr->scroll_list_titleBottom);

  func("line_edit_period",ptr->line_edit_period);
  func("scroll_speedUpPeriod",ptr->scroll_speedUpPeriod);
  func("progress_time",ptr->progress_time);
  func("progress_period",ptr->progress_period);
  func("line_edit_ex",ptr->line_edit_ex);
  func("switch_on",ptr->switch_on);
  func("switch_off",ptr->switch_off);

  func("progress_border",ptr->progress_border);
  func("progress_grayUp",ptr->progress_grayUp);
  func("progress_snowUp",ptr->progress_snowUp);
  func("progress_grayPing",ptr->progress_grayPing);
  func("progress_snowPing",ptr->progress_snowPing);

  func("blinkTime",ptr->blinkTime);
  func("blinkPeriod",ptr->blinkPeriod);

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

  func("frame_dxy",ptr->frame_dxy);
  func("title_dy",ptr->title_dy);
  func("btn_dx",ptr->btn_dx);
  func("btn_dy",ptr->btn_dy);
  func("frame",ptr->frame);
  func("titleActive",ptr->titleActive);
  func("titleInactive",ptr->titleInactive);
  func("frameHilight",ptr->frameHilight);
  func("frameDrag",ptr->frameDrag);
  func("title",ptr->title);

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

  func("shadeColor",ptr->shadeColor);
  func("shadeAlpha",ptr->shadeAlpha);

  func("exception_back",ptr->exception_back);
  func("exception_text",ptr->exception_text);
  func("exception_divider",ptr->exception_divider);

  func("message_knob_dxy",ptr->message_knob_dxy);

  func("file_alt_dxy",ptr->file_alt_dxy);
  func("file_accent",ptr->file_accent);
  func("file_filter_text",ptr->file_filter_text);

  func("file_faceRight",ptr->file_faceRight);
  func("file_faceDown",ptr->file_faceDown);

  func("text_New_file",ptr->text_New_file);

  func("label_font",ptr->label_font.param);
  func("contour_font",ptr->contour_font.param);
  func("message_font",ptr->message_font.param);
  func("info_font",ptr->info_font.param);
  func("line_edit_font",ptr->line_edit_font.param);
  func("list_font",ptr->list_font.param);
  func("button_font",ptr->button_font.param);
  func("menu_font",ptr->menu_font.param);
  func("title_font",ptr->title_font.param);
  func("hint_font",ptr->hint_font.param);
  func("file_filter_font",ptr->file_filter_font.param);
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
   binder.item("gray",gray);
   binder.item("grayUp",grayUp);
   binder.item("snow",snow);
   binder.item("snowUp",snowUp);
   binder.space();
   binder.item("back",back);
   binder.item("line",line);
   binder.item("inactive",inactive);
   binder.space();
   binder.item("border",border);
   binder.item("focus",focus);
   binder.space();
   binder.item("face",face);
   binder.item("faceUp",faceUp);
   binder.item("mark",mark);
   binder.item("alert",alert);
   binder.space();
   binder.item("'Ok'",text_Ok);
   binder.item("'Cancel'",text_Cancel);

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
   binder.item("menu hotcolor",menu_hotcolor);

  binder.group("Scroll List");

   binder.item("title",scroll_list_title);
   binder.item("title top",scroll_list_titleTop);
   binder.item("title bottom",scroll_list_titleBottom);

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
   binder.item("progress grayUp",progress_grayUp);
   binder.item("progress snowUp",progress_snowUp);
   binder.item("progress grayPing",progress_grayPing);
   binder.item("progress snowPing",progress_snowPing);

  binder.group("Frame window");

   binder.item("alert blink time",blinkTime);
   binder.item("alert blink period",blinkPeriod);
   binder.space();
   binder.item("'Fatal error'",text_Fatal_error);
   binder.item("'<No hint available>'",text_No_hint);
   binder.space();
   binder.item("'Resize top-left'",hint_ResizeTopLeft);
   binder.item("'Resize left'",hint_ResizeLeft);
   binder.item("'Resize bottom-left'",hint_ResizeBottomLeft);
   binder.item("'Resize bottom'",hint_ResizeBottom);
   binder.item("'Resize bottom-right'",hint_ResizeBottomRight);
   binder.item("'Resize right'",hint_ResizeRight);
   binder.item("'Resize top-right'",hint_ResizeTopRight);
   binder.space();
   binder.item("'Open/close alert view'",hint_Alert);
   binder.item("'Help on/off'",hint_Help);
   binder.item("'Minimize'",hint_Minimize);
   binder.item("'Maximize'",hint_Maximize);
   binder.item("'Restore'",hint_Restore);
   binder.item("'Close'",hint_Close);
   binder.space();
   binder.item("frame width",frame_dxy);
   binder.item("title height",title_dy);
   binder.item("frame button width",btn_dx);
   binder.item("frame button height",btn_dy);
   binder.space();
   binder.item("frame",frame);
   binder.item("hilight frame",frameHilight);
   binder.item("drag frame",frameDrag);
   binder.item("title",title);
   binder.item("active title",titleActive);
   binder.item("inactive title",titleInactive);
   binder.space();
   binder.item("drag",drag);
   binder.item("hilight drag",dragHilight);
   binder.item("active drag",dragActive);
   binder.item("small size indicator",dragSmall);
   binder.space();
   binder.item("button face",btnFace);
   binder.item("button hilight",btnFaceHilight);
   binder.item("button picture",btnPict);
   binder.item("button close picture",btnPictClose);
   binder.item("button alert picture",btnPictAlert);
   binder.item("button no-alert picture",btnPictNoAlert);
   binder.item("button close-alert picture",btnPictCloseAlert);
   binder.space();
   binder.item("hint back",hintBack);
   binder.item("hint text",hintText);
   binder.item("hint border",hintBorder);
   binder.item("hint line width",hintWidth);
   binder.space();
   binder.item("title font",title_font);
   binder.item("hint font",hint_font);
   binder.space();
   binder.item("shade color",shadeColor);
   binder.item("shade alpha",shadeAlpha);

  binder.group("Exception window");

   binder.item("back",exception_back);
   binder.item("text",exception_text);
   binder.item("divider",exception_divider);

  binder.group("Message window");

   binder.item("knob box",message_knob_dxy);

  binder.group("File window");

   binder.item("alt box",file_alt_dxy);
   binder.space();
   binder.item("dir accent",file_accent);
   binder.item("filter text",file_filter_text);
   binder.space();
   binder.item("alt right",file_faceRight);
   binder.item("alt down",file_faceDown);
   binder.space();
   binder.item("filter text",file_filter_text);
   binder.item("filter font",file_filter_font);
   binder.space();
   binder.item("'New file'",text_New_file);
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

  title_font.create();
  hint_font.create();

  file_filter_font.create();
 }

/* class UserPreference */

const char *const UserPreference::PrefFile="/UserPreference.ddl";

UserPreference::UserPreference() noexcept // Update here
 {
  // main windows

  cfg_ExceptionWindow.bind(get(),getSmartConfig());

  cfg_MessageSubWindow.bind(get(),getSmartConfig());

  cfg_MessageFrame.bind(get(),getSmartConfig());

  cfg_AlertMessageSubWindow=cfg_MessageSubWindow;
  cfg_AlertMessageSubWindow.back.bind(alert);

  cfg_AlertMessageFrame.frame_cfg.bind(cfg_FixedFrame);
  cfg_AlertMessageFrame.msg_cfg.bind(cfg_AlertMessageSubWindow);

  cfg_ToolFrame.bind(get());

  cfg_SimpleCascadeMenu.bind(getSmartConfig());

  cfg_DragFrame.bind(get());
  cfg_FixedFrame.bind(get());

  cfg_WindowReport.bind(get());

  // sub windows

  cfg_ButtonWindow.bind(get());

  cfg_KnobWindow.bind(get());

  cfg_CheckWindow.bind(get());

  cfg_SwitchWindow.bind(get());

  cfg_RadioWindow.bind(get());

  cfg_LightWindow.bind(get());

  cfg_LabelWindow.bind(get());

  cfg_TextWindow.bind(get());

  cfg_TextLineWindow.bind(get());

  cfg_XSingleLineWindow.bind(get());

  cfg_YSingleLineWindow.bind(get());

  cfg_XDoubleLineWindow.bind(get());

  cfg_YDoubleLineWindow.bind(get());

  cfg_ContourWindow.bind(get());

  cfg_TextContourWindow.bind(get());

  cfg_ProgressWindow.bind(get());

  cfg_InfoWindow.bind(get());

  cfg_SimpleTextListWindow.bind(get());

  cfg_LineEditWindow.bind(get());

  cfg_XScrollWindow.bind(get());

  cfg_YScrollWindow.bind(get());

  cfg_ScrollListWindow.bind(get(),getSmartConfig());

  cfg_SimpleTopMenuWindow.bind(get());

  cfg_SimpleCascadeMenuWindow.bind(get());

  // FileWindow

  cfg_DirEditWindow.bind(get());

  cfg_FileFilterWindow.bind(get(),getSmartConfig());

  cfg_FileCheckShape.bind(get());

  cfg_FileSubWindow.space_dxy.bind(space_dxy);
  cfg_FileSubWindow.back.bind(back);
  cfg_FileSubWindow.text_Ok.bind(text_Ok);
  cfg_FileSubWindow.text_Cancel.bind(text_Cancel);
  cfg_FileSubWindow.text_New_file.bind(text_New_file);
  cfg_FileSubWindow.edit_cfg.bind(cfg_DirEditWindow);
  cfg_FileSubWindow.list_cfg.bind(cfg_ScrollListWindow);
  cfg_FileSubWindow.filter_list_cfg.bind(cfg_FileFilterWindow);
  cfg_FileSubWindow.btn_cfg.bind(cfg_ButtonWindow);
  cfg_FileSubWindow.knob_cfg.bind(cfg_KnobWindow);
  cfg_FileSubWindow.hit_menu_cfg.bind(cfg_SimpleCascadeMenu);
  cfg_FileSubWindow.check_cfg.bind(cfg_CheckWindow);
  cfg_FileSubWindow.label_cfg.bind(cfg_LabelWindow);
  cfg_FileSubWindow.alt_cfg.bind(cfg_FileCheckShape);
  cfg_FileSubWindow.line_cfg.bind(cfg_XDoubleLineWindow);

  cfg_FileWindow.frame_cfg.bind(cfg_DragFrame);
  cfg_FileWindow.file_cfg.bind(cfg_FileSubWindow);
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

