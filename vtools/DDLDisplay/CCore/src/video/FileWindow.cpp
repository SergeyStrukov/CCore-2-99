/* FileWindow.cpp */
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

#include <CCore/inc/video/FileWindow.h>

#include <CCore/inc/video/DesktopKey.h>
#include <CCore/inc/video/Desktop.h>

#include <CCore/inc/video/Layout.h>
#include <CCore/inc/video/PaneCut.h>
#include <CCore/inc/video/SmoothDrawArt.h>
#include <CCore/inc/video/FileNameCmp.h>

#include <CCore/inc/RangeDel.h>
#include <CCore/inc/Path.h>
#include <CCore/inc/Print.h>
#include <CCore/inc/Exception.h>
#include <CCore/inc/CompactList.h>

#include <CCore/inc/FileName.h>
#include <CCore/inc/FileToMem.h>

#include <CCore/inc/ddl/DDLEngine.h>
#include <CCore/inc/ddl/DDLTypeSet.h>

namespace CCore {
namespace Video {

/* class DirHitList */

bool DirHitList::Rec::test_and_inc(StrLen dir_name)
 {
  if( Range(dir).equal(dir_name) )
    {
     count++;

     return true;
    }

  return false;
 }

void DirHitList::Rec::init(StrLen dir_name,unsigned count_)
 {
  dir=String(dir_name);
  count=count_;
 }

const char *const DirHitList::HitFile="/HitDirs.ddl";

const char *const DirHitList::Pretext=
"struct HitDirData\r\n"
" {\r\n"
"  text[] hit_list;\r\n"
"  \r\n"
"  struct Rec { text dir; uint count; } [] last_list;\r\n"
" };\r\n\r\n";

DirHitList::DirHitList() noexcept
 {
 }

DirHitList::~DirHitList()
 {
 }

namespace Private_FileWindow {

#include "HitDirs.TypeDef.gen.h"
#include "HitDirs.TypeSet.gen.h"

} // namespace Private_FileWindow

using namespace Private_FileWindow;

void DirHitList::loadDDL(StrLen file_name)
 {
  hit_len=0;
  last_len=0;

  char temp[512];
  PrintBuf eout(Range(temp));
  DDL::FileEngine<FileName,FileToMem> engine(eout);

  auto result=engine.process(file_name,Pretext);

  if( !result )
    {
     Printf(Exception,"CCore::Video::DirHitList::loadDDL(#.q;) : input file processing error\n#;",file_name,eout.close());
    }
  else
    {
     DDL::TypedMap<TypeSet> map(result);
     MemAllocGuard guard(map.getLen());

     map(guard);

     // populate

     TypeDef::HitDirData hits=map.takeConst<TypeDef::HitDirData>("Hits");

     {
      auto r=hits.hit_list.getRange();

      ulen len=Min(r.len,MaxLen);

      for(ulen i=0; i<len ;i++) hit_list[i]=String(r[i]);

      hit_len=len;
     }

     {
      auto r=hits.last_list.getRange();

      ulen len=Min(r.len,MaxLen);

      for(ulen i=0; i<len ;i++) last_list[i].init(r[i].dir,r[i].count);

      last_len=len;
     }
    }
 }

void DirHitList::saveDDL(StrLen file_name)
 {
  PrintFile out(file_name);

  Putobj(out,"HitDirData Hits=\n {\n  {\n");

  for(ulen i=0; i<hit_len ;i++)
    {
     Printf(out,"   '#;'",hit_list[i]);

     if( i+1<hit_len )
       {
        Putobj(out,",\n");
       }
     else
       {
        Putobj(out,"\n");
       }
    }

  Putobj(out,"  },\n  {\n");

  for(ulen i=0; i<last_len ;i++)
    {
     Printf(out,"   { '#;' , #; }",last_list[i].dir,last_list[i].count);

     if( i+1<last_len )
       {
        Putobj(out,",\n");
       }
     else
       {
        Putobj(out,"\n");
       }
    }

  Putobj(out,"  }\n };\n\n");
 }

void DirHitList::load()
 {
  try
    {
     HomeDir home;

     MakeString<MaxPathLen> buf;

     buf.add(home.get(),HomeKey,HitFile);

     if( !buf )
       {
        Printf(Exception,"CCore::Video::DirHitList::load() : too long file name");
       }

     StrLen file_name=buf.get();

     loadDDL(file_name);
    }
  catch(...)
    {
    }
 }

void DirHitList::save()
 {
  try
    {
     HomeDir home;

     MakeString<MaxPathLen> buf;

     buf.add(home.get(),HomeKey);

     StrLen dir=buf.get();

     buf.add(HitFile);

     if( !buf )
       {
        Printf(Exception,"CCore::Video::DirHitList::save() : too long file name");
       }

     StrLen file_name=buf.get();

     FileSystem fs;

     if( fs.getFileType(dir)==FileType_none ) fs.createDir(dir);

     saveDDL(file_name);
    }
  catch(...)
    {
    }
 }

void DirHitList::add(StrLen dir_name)
 {
  if( hit_len<MaxLen )
    {
     hit_list[hit_len++]=String(dir_name);
    }
 }

void DirHitList::last(StrLen dir_name)
 {
  for(ulen i=0; i<last_len ;i++) if( last_list[i].test_and_inc(dir_name) ) return;

  if( last_len<MaxLen )
    {
     last_list[last_len++].init(dir_name);

     return;
    }

  ulen best_ind=0;
  unsigned best_count=last_list[best_ind].count;

  for(ulen i=1; i<last_len ;i++) if( last_list[i].count<best_count ) best_ind=i;

  last_list[best_ind].init(dir_name);
 }

StrLen DirHitList::operator () (int id) const
 {
  ulen ind=(ulen)id;

  if( ind<MaxLen )
    {
     if( ind>=hit_len ) return Empty;

     return Range(hit_list[ind]);
    }
  else
    {
     ind-=MaxLen;

     if( ind>=last_len ) return Empty;

     return Range(last_list[ind].dir);
    }
 }

void DirHitList::del(int id)
 {
  ulen ind=(ulen)id;

  if( ind<MaxLen )
    {
     hit_len-=RangeCopyDel(hit_list,hit_len,ind);
    }
  else
    {
     ind-=MaxLen;

     last_len-=RangeCopyDel(last_list,last_len,ind);
    }
 }

void DirHitList::prepare(MenuData &data)
 {
  data.list.erase();

  for(ulen i=0; i<hit_len ;i++)
    {
     data(MenuTextNoHot,hit_list[i],(int)i);
    }

  data(MenuSeparator);

  for(ulen i=0; i<last_len ;i++)
    {
     data(MenuTextNoHot,last_list[i].dir,(int)(MaxLen+i));
    }

  data.update.assert();
 }

/* class DirEditShape */

VColor DirEditShape::Func::color(ulen,char ch,Point,Point)
 {
  if( PathBase::IsSlash(ch) || PathBase::IsColon(ch) ) return accent;

  return vc;
 }

void DirEditShape::drawText(Font font,const DrawBuf &buf,Pane pane,TextPlace place,StrLen text,VColor vc) const
 {
  auto &ecfg=static_cast<const Config &>(cfg);

  Func func{vc,+ecfg.accent};

  font->text(buf,pane,place,text,func.function_color());
 }

/* class FileFilterWindow */

void FileFilterWindow::check_changed(bool check)
 {
  pad->check_changed(index,check);
 }

void FileFilterWindow::edit_changed()
 {
  filter.reset();

  check.uncheck();

  pad->check_changed(index,false);
 }

void FileFilterWindow::knob_pressed()
 {
  pad->knob_del_pressed(index);
 }

FileFilterWindow::FileFilterWindow(SubWindowHost &host,const Config &cfg_,ulen index_,SignalPad *pad_,StrLen filter,bool check_)
 : ComboWindow(host),
   cfg(cfg_),
   index(index_),
   pad(pad_),

   check(wlist,cfg.check_cfg,check_),
   edit(wlist,cfg.edit_cfg),
   knob(wlist,cfg.knob_cfg,KnobShape::FaceCross),

   connector_check_changed(this,&FileFilterWindow::check_changed,check.changed),
   connector_edit_changed(this,&FileFilterWindow::edit_changed,edit.changed),
   connector_knob_pressed(this,&FileFilterWindow::knob_pressed,knob.pressed)
 {
  wlist.insTop(check,edit,knob);

  edit.setText(filter);
  edit.hideInactiveCursor();
 }

FileFilterWindow::~FileFilterWindow()
 {
 }

 // methods

Point FileFilterWindow::getMinSize() const
 {
  Point size=edit.getMinSize();

  Coord check_dxy=BoxSize(check);
  Coord knob_dxy=BoxSize(knob);

  return Point( BoxExt(check_dxy)+BoxExt(knob_dxy)+size.x , Max_cast(check_dxy,knob_dxy,size.y) );
 }

 // drawing

void FileFilterWindow::layout()
 {
  PaneCut pane(getSize(),0);

  pane.place_boxLeft(check);

  pane.place_boxRight(knob);

  pane.place(edit);
 }

void FileFilterWindow::draw(DrawBuf buf,bool drag_active) const
 {
  wlist.draw(buf,drag_active);
 }

void FileFilterWindow::draw(DrawBuf buf,Pane pane,bool drag_active) const
 {
  wlist.draw(buf,pane,drag_active);
 }

/* class FileFilterListWindow */

void FileFilterListWindow::knob_add_pressed()
 {
  add(StrLen("*",1),false);
 }

void FileFilterListWindow::check_changed(ulen index,bool check)
 {
  Used(index);
  Used(check);

  changed.assert();
 }

void FileFilterListWindow::knob_del_pressed(ulen index)
 {
  if( RangeSwapDel(Range(filter_list),index) )
    {
     filter_list.shrink_one();

     for(ulen i=index,count=filter_list.getLen(); i<count ;i++) filter_list[i]->setIndex(i);

     layout();

     redraw();

     changed.assert();
    }
 }

FileFilterListWindow::FileFilterListWindow(SubWindowHost &host,const ConfigType &cfg_)
 : ComboWindow(host),
   cfg(cfg_),

   knob(wlist,cfg.knob_cfg,KnobShape::FacePlus),

   connector_knob_add_pressed(this,&FileFilterListWindow::knob_add_pressed,knob.pressed)
 {
  wlist.insTop(knob);
 }

FileFilterListWindow::~FileFilterListWindow()
 {
 }

 // methods

Point FileFilterListWindow::getMinSize() const
 {
  Coord knob_dxy=BoxSize(knob);

  if( ulen count=filter_list.getLen() )
    {
     Point size=filter_list[0]->getMinSize();

     Coord delta=BoxExt(size.y);

     return Point(size.x,delta*count+knob_dxy);
    }
  else
    {
     return Point::Diag(knob_dxy);
    }
 }

void FileFilterListWindow::add(StrLen filter,bool check)
 {
  ulen index=filter_list.getLen();

  OwnPtr<FileFilterWindow> obj(new FileFilterWindow(wlist,cfg,index,this,filter,check));

  wlist.insBottom(obj.getPtr());

  filter_list.append_fill(std::move(obj));

  if( getFrame()->isAlive() )
    {
     layout();

     redraw();
    }
 }

 // drawing

void FileFilterListWindow::layout()
 {
  Point size=getSize();

  if( filter_list.getLen() )
    {
     Coord dy=filter_list[0]->getMinSize().y;

     PaneCut pane(size,BoxSpace(dy));

     for(auto &ptr : filter_list ) pane.place_cutTop(*ptr);

     pane.place_cutTopLeft(knob);
    }
  else
    {
     PaneCut pane(size,0);

     pane.place_cutTopLeft(knob);
    }
 }

void FileFilterListWindow::draw(DrawBuf buf,bool drag_active) const
 {
  wlist.draw(buf,drag_active);
 }

void FileFilterListWindow::draw(DrawBuf buf,Pane pane,bool drag_active) const
 {
  wlist.draw(buf,pane,drag_active);
 }

/* class FileSubWindow::Distributor */

class FileSubWindow::Distributor : NoCopy
 {
   struct Basket : NoCopy
    {
     StrLen filter_text;
     const FileNameFilter &filter;
     CompactList2<StrLen> file_list;

     Basket(StrLen filter_text_,const FileNameFilter &filter_) : filter_text(filter_text_),filter(filter_) {}

     bool tryAdd(StrLen file)
      {
       if( filter(file) )
         {
          file_list.insLast(file);

          return true;
         }

       return false;
      }

     void addTo(ComboInfoBuilder &builder)
      {
       if( +file_list )
         {
          builder.addTitle(filter_text);

          file_list.apply( [&builder] (StrLen file) { builder.add(file); } );
         }
      }
    };

   CompactList2<Basket> basket_list;

  public:

   Distributor()
    {
    }

   ~Distributor()
    {
    }

   void addFilter(StrLen filter_text,const FileNameFilter &filter)
    {
     basket_list.insLast(filter_text,filter);
    }

   void addFile(StrLen file)
    {
     basket_list.apply( [file] (Basket &obj) -> bool { return !obj.tryAdd(file); } );
    }

   ComboInfo build()
    {
     ComboInfoBuilder builder;

     basket_list.apply( [&builder] (Basket &obj) { obj.addTo(builder); } );

     builder.sortGroups(ExtNameLess);

     return builder.complete();
    }
 };

/* class FileSubWindow */

void FileSubWindow::applyFilters()
 {
  try
    {
     Distributor obj;

     filter_list.apply( [&obj] (StrLen filter_text,const FileNameFilter &filter) { obj.addFilter(filter_text,filter); } );

     ulen count=file_info->getLineCount();

     for(ulen i=0; i<count ;i++) obj.addFile(file_info->getLine(i));

     file_list.setInfo(obj.build());
    }
  catch(...)
    {
     file_list.setInfo(ComboInfo());

     throw;
    }
 }

void FileSubWindow::fillLists()
 {
  try
    {
     dir_list.enable();
     file_list.enable();

     FileSystem::DirCursor cur(fs,dir.getText());

     ComboInfoBuilder dir_builder;
     InfoBuilder file_builder;

     cur.apply( [&] (StrLen name,FileType ft)
                    {
                     switch( ft )
                       {
                        case FileType_dir :
                         {
                          if( PathBase::IsDot(name) ) break;

                          dir_builder.add(name);
                         }
                        break;

                        case FileType_file :
                         {
                          file_builder.add(name);
                         }
                        break;
                       }

                   } );

     dir_builder.sortGroups(ExtNameLess);

     dir_list.setInfo(dir_builder.complete());

     file_info=file_builder.complete();
    }
  catch(...)
    {
     dir_list.setInfo(ComboInfo());
     file_list.setInfo(ComboInfo());

     throw;
    }

  applyFilters();
 }

void FileSubWindow::setDir(StrLen dir_name)
 {
  char temp[MaxPathLen+1];

  StrLen path=fs.pathOf(dir_name,temp);

  if( fs.getFileType(path)==FileType_dir )
    {
     dir.setText(path);

     fillLists();
    }
  else
    {
     dir_list.disable();
     file_list.disable();

     Printf(Exception,"CCore::Video::FileSubWindow::setDir(#.q;) : #.q; is not a directory",dir_name,path);
    }
 }

void FileSubWindow::setDir(StrLen dir_name,StrLen sub_dir)
 {
  MakeFileName temp(dir_name,sub_dir);

  setDir(temp.get());
 }

void FileSubWindow::buildFilePath()
 {
  const ComboInfo &info=file_list.getInfo();
  ulen index=file_list.getSelect();

  if( index<info->getLineCount() )
    {
     ComboInfoItem item=info->getLine(index);

     if( item.type==ComboInfoText )
       {
        file_path=file_buf(dir.getText(),item.text);
       }
    }
 }

void FileSubWindow::file_list_entered()
 {
  buildFilePath();

  askFrameClose();
 }

void FileSubWindow::filter_list_changed()
 {
  applyFilters();
 }

void FileSubWindow::dir_list_entered()
 {
  const ComboInfo &info=dir_list.getInfo();
  ulen index=dir_list.getSelect();

  if( index<info->getLineCount() )
    {
     ComboInfoItem item=info->getLine(index);

     if( item.type==ComboInfoText )
       {
        setDir(dir.getText(),item.text);
       }
    }
 }

void FileSubWindow::dir_entered()
 {
  StrLen dir_name=dir.getText();

  setDir(dir_name);
 }

void FileSubWindow::dir_changed()
 {
  dir_list.disable();
  file_list.disable();
 }

void FileSubWindow::btn_Ok_pressed()
 {
  hit_list.last(dir.getText());

  buildFilePath();

  askFrameClose();
 }

void FileSubWindow::btn_Cancel_pressed()
 {
  askFrameClose();
 }

void FileSubWindow::knob_hit_pressed()
 {
  if( hit_menu.isDead() )
    {
     hit_menu.create(getFrame(),hit_data,toScreen(knob_hit.getPlace().addDY()));

     knob_hit.setFace(KnobShape::FaceCross);
    }
  else
    {
     hit_menu.destroy();
    }
 }

void FileSubWindow::knob_add_pressed()
 {
  hit_list.add(dir.getText());

  hit_list.prepare(hit_data);
 }

void FileSubWindow::knob_back_pressed()
 {
  StrLen dir_name=dir.getText();

  SplitPath split1(dir_name);

  SplitName split2(split1.path);

  if( !split2 )
    {
     if( ulen delta=split2.name.len )
       {
        dir_name.len-=delta;

        setDir(dir_name);
       }
    }
  else
    {
     ulen delta=split2.name.len+1;

     if( delta==split1.path.len ) delta--;

     dir_name.len-=delta;

     setDir(dir_name);
    }
 }

void FileSubWindow::hit_menu_destroyed()
 {
  knob_hit.setFace(KnobShape::FaceDown);
 }

void FileSubWindow::hit_menu_selected(int id,Point)
 {
  setDir(hit_list(id));
 }

void FileSubWindow::hit_menu_deleted(int id)
 {
  hit_list.del(id);

  hit_list.prepare(hit_data);
 }

FileSubWindow::FileSubWindow(SubWindowHost &host,const Config &cfg_,const FileWindowParam &param_)
 : ComboWindow(host),
   cfg(cfg_),
   param(param_),

   dir(wlist,cfg.edit_cfg),
   knob_hit(wlist,cfg.knob_cfg,KnobShape::FaceDown),
   knob_add(wlist,cfg.knob_cfg,KnobShape::FacePlus),
   knob_back(wlist,cfg.knob_cfg,KnobShape::FaceLeft),
   dir_list(wlist,cfg.list_cfg),
   file_list(wlist,cfg.list_cfg),
   filter_list(wlist,cfg.filter_list_cfg),
   btn_Ok(wlist,cfg.btn_cfg,"Ok"_def),
   btn_Cancel(wlist,cfg.btn_cfg,"Cancel"_def),

   hit_menu(host.getFrame()->getDesktop(),cfg.hit_menu_cfg),

   check_new(wlist,cfg.check_cfg,true),
   label_new_file(wlist,cfg.label_cfg,"New file"_def),
   new_file(wlist,cfg.edit_cfg),

   connector_file_list_entered(this,&FileSubWindow::file_list_entered,file_list.entered),
   connector_file_list_dclicked(this,&FileSubWindow::file_list_entered,file_list.dclicked),
   connector_filter_list_changed(this,&FileSubWindow::filter_list_changed,filter_list.changed),
   connector_dir_list_entered(this,&FileSubWindow::dir_list_entered,dir_list.entered),
   connector_dir_list_dclicked(this,&FileSubWindow::dir_list_entered,dir_list.dclicked),
   connector_dir_entered(this,&FileSubWindow::dir_entered,dir.entered),
   connector_dir_changed(this,&FileSubWindow::dir_changed,dir.changed),
   connector_btn_Ok_pressed(this,&FileSubWindow::btn_Ok_pressed,btn_Ok.pressed),
   connector_btn_Cancel_pressed(this,&FileSubWindow::btn_Cancel_pressed,btn_Cancel.pressed),
   connector_knob_hit_pressed(this,&FileSubWindow::knob_hit_pressed,knob_hit.pressed),
   connector_knob_add_pressed(this,&FileSubWindow::knob_add_pressed,knob_add.pressed),
   connector_knob_back_pressed(this,&FileSubWindow::knob_back_pressed,knob_back.pressed),
   connector_hit_menu_destroyed(this,&FileSubWindow::hit_menu_destroyed,hit_menu.takeDestroyed()),
   connector_hit_menu_selected(this,&FileSubWindow::hit_menu_selected,hit_menu.takeSelected()),
   connector_hit_menu_deleted(this,&FileSubWindow::hit_menu_deleted,hit_menu.takeDeleted())
 {
  wlist.insTop(dir,knob_hit,knob_add,knob_back,dir_list,file_list,filter_list,btn_Ok,btn_Cancel);

  if( param.new_file ) wlist.insBottom(check_new,label_new_file,new_file);

  dir.hideInactiveCursor();
 }

FileSubWindow::~FileSubWindow()
 {
 }

 // methods

Point FileSubWindow::getMinSize(StrLen sample_text) const
 {
  Coord space=+cfg.space_dxy;

  Point dir_size=dir.getMinSize(sample_text);

  Point btn_size=SupMinSize(btn_Ok,btn_Cancel);

  Coord dx=3*space+Max<Coord>(dir_size.x+dir_size.y,2*btn_size.x+space)+3*dir_size.y;
  Coord dy=5*space+20*dir_size.y+btn_size.y;

  return Point(dx,dy);
 }

 // base

void FileSubWindow::open()
 {
  ComboWindow::open();

  file_path=Empty;

  setDir(".");

  hit_list.load();

  hit_list.prepare(hit_data);
 }

void FileSubWindow::close()
 {
  hit_list.save();
 }

 // drawing

void FileSubWindow::layout()
 {
  Coord space=+cfg.space_dxy;

  Panesor psor(getSize(),space);

  psor.shrink();

  // dir

  {
   Coord dy=dir.getMinSize().y;

   Pane pane=psor.cutY(dy);
   Coord dxy=Min(dy,+cfg.knob_dxy);

   knob_hit.setPlace(SplitBox(dxy,pane));

   knob_add.setPlace(SplitBox(dxy,pane));

   knob_back.setPlace(SplitBox(pane,dxy));

   dir.setPlace(pane);
  }

  // dir_list

  {
   psor.placeY(dir_list,Rational(1,3));
  }

  // new_file

  if( param.new_file )
    {
     Point label_size=label_new_file.getMinSize();
     Point edit_size=new_file.getMinSize();

     Coord dy=Max(label_size.y,edit_size.y);

     Pane pane=psor.cutY(dy);

     Coord dxy=Min(+cfg.check_dxy,dy);

     check_new.setPlace(SplitBox(dxy,pane));

     Panesor p(pane,space);

     p.placeX(label_new_file,label_size.x);

     new_file.setPlace(p);
    }

  // file_list

  Point btn_size=SupMinSize(btn_Ok,btn_Cancel);

  {
   Panesor p=psor.cutY_rest(btn_size.y);

   p.placeX(file_list,Rational(2,3));

   filter_list.setPlace(p);
  }

  // btn

  {
   psor.placeRow(btn_size,btn_Ok,btn_Cancel);
  }
 }

void FileSubWindow::draw(DrawBuf buf,bool drag_active) const
 {
  buf.erase(+cfg.back);

  wlist.draw(buf,drag_active);
 }

void FileSubWindow::draw(DrawBuf buf,Pane pane,bool drag_active) const
 {
  buf.block_safe(pane,+cfg.back);

  wlist.draw(buf,pane,drag_active);
 }

/* class FileWindow */

const char *const FileWindow::SampleDir="/cygdrive/d/active/home/C++/CCore-2-99/vtools/DDLDisplay";

FileWindow::FileWindow(Desktop *desktop,const Config &cfg,const FileWindowParam &param)
 : DragWindow(desktop,cfg.frame_cfg),
   sub_win(*this,cfg.file_cfg,param)
 {
  bindClient(sub_win);
 }

FileWindow::FileWindow(Desktop *desktop,const Config &cfg,const FileWindowParam &param,Signal<> &update)
 : FileWindow(desktop,cfg,param)
 {
  bindUpdate(update);
 }

FileWindow::~FileWindow()
 {
 }

 // create

Pane FileWindow::getPane(StrLen title,Point base) const
 {
  Point size=getMinSize(false,title,sub_win.getMinSize(SampleDir));

  Point screen_size=getDesktop()->getScreenSize();

  return FitToScreen(base,size,screen_size);
 }

} // namespace Video
} // namespace CCore

