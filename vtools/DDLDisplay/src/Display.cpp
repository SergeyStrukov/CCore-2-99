/* Display.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: DDLDisplay 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/Display.h>

#include <CCore/inc/video/Layout.h>

#include <CCore/inc/Printf.h>

#include <CCore/inc/Exception.h>

namespace App {

/* class DDLFile */

void DDLFile::erase()
 {
  result=Null;

  engine.reset();

  updated.assert();
 }

DDLFile::DDLFile() noexcept
 : eout(64_KByte),
   engine(eout)
 {
 }

DDLFile::~DDLFile()
 {
 }

auto DDLFile::open(StrLen file_name) -> OpenResult
 {
  try
    {
     eout.cleanup();

     result=engine.process(file_name,Mutate<const char>(Range(pretext)));

     if( !result )
       {
        String str=eout.close();

        eout.cleanup();

        erase();

        return {str,false};
       }
     else
       {
        eout.cleanup();

        updated.assert();

        return {Null,true};
       }
    }
  catch(CatchType)
    {
     eout.cleanup();

     erase();

     return {"Exception"_def,false};
    }
 }

auto DDLFile::openPretext(StrLen file_name) -> OpenResult
 {
  try
    {
     FileToMem map(file_name);

     eout.cleanup();

     DDL::TextEngine engine(eout,Mutate<const char>(Range(map)));

     auto result=engine.process();

     if( !result )
       {
        String str=eout.close();

        eout.cleanup();

        updated.assert();

        return {str,false};
       }
     else
       {
        eout.cleanup();

        Swap(pretext,map);

        erase();

        return {Null,true};
       }
    }
  catch(CatchType)
    {
     eout.cleanup();

     updated.assert();

     return {"Exception"_def,false};
    }
 }

void DDLFile::noPretext()
 {
  FileToMem map;

  Swap(pretext,map);

  erase();
 }

/* class DDLView */

void DDLView::erase()
 {
  ElementPool temp;

  Swap(temp,pool);

  consts=Empty;

  field_name.erase();
 }

void DDLView::SetTail(PtrLen<char> &ret,char ch)
 {
  ret.back(1)=ch;

  ret.len--;
 }

void DDLView::SetTail(PtrLen<char> &ret,StrLen str)
 {
  ret.suffix(str.len).copy(str.ptr);

  ret.len-=str.len;
 }

struct DDLView::GetStructNode
 {
  DDL::StructNode *struct_node = 0 ;

  explicit GetStructNode(DDL::TypeNode *type) { (*this)(type); }

  auto operator + () const { return struct_node; }

  void operator () (AnyType *) {}

  void operator () (DDL::AliasNode *node)
   {
    (*this)(node->result_type);
   }

  void operator () (DDL::StructNode *node)
   {
    struct_node=node;
   }

  void operator () (DDL::TypeNode::Ref *ptr)
   {
    ElaborateAnyPtr(*this,ptr->ptr);
   }

  void operator () (DDL::TypeNode::Struct *ptr)
   {
    struct_node=ptr->struct_node;
   }

  void operator () (DDL::TypeNode *type)
   {
    ElaborateAnyPtr(*this,type->ptr);
   }
 };

StrLen DDLView::fieldName(ulen index,StrLen name)
 {
  if( !name ) return Null;

  if( ulen len=field_name.getLen() )
    {
     if( index>=len ) field_name.extend_default(index-len+1);
    }
  else
    {
     field_name.extend_default(Max<ulen>(100,LenAdd(index,1)));
    }

  StrLen &obj=field_name[index];

  if( !obj ) obj=pool.dup(name);

  return obj;
 }

StrLen DDLView::build(DDL::ConstNode *node)
 {
  ULenSat len=node->name.name.str.len;

  {
   DDL::ScopeNode *scope=node->parent;

   for(ulen cnt=node->depth; cnt ;cnt--,scope=scope->parent)
     {
      len+=scope->name.name.str.len;
      len+=1u;
     }
  }

  if( len>MaxNameLen )
    {
     Printf(Exception,"App::DDLView::build(...) : too long constant name");
    }

  PtrLen<char> ret=pool.createArray_raw<char>(len.value);

  SetTail(ret,node->name.name.str);

  {
   DDL::ScopeNode *scope=node->parent;

   for(ulen cnt=node->depth; cnt ;cnt--,scope=scope->parent)
     {
      SetTail(ret,'#');
      SetTail(ret,scope->name.name.str);
     }
  }

  return ret;
 }

StrLen DDLView::toString(AnyType value)
 {
  char buf[TextBufLen];
  PrintBuf out(Range(buf));

  Putobj(out,value);

  return toString(out.close());
 }

StrLen DDLView::toString(DDL::TypeNode::Base *type,DDL::Value value)
 {
  switch( type->type )
    {
     case DDL::TypeNode::Base::Type_sint : return toString(value.val_sint);

     case DDL::TypeNode::Base::Type_uint : return toString(value.val_uint);

     case DDL::TypeNode::Base::Type_ulen : return toString(value.val_ulen);

     case DDL::TypeNode::Base::Type_sint8 : return toString(value.val_sint8);

     case DDL::TypeNode::Base::Type_uint8 : return toString(value.val_uint8);

     case DDL::TypeNode::Base::Type_sint16 : return toString(value.val_sint16);

     case DDL::TypeNode::Base::Type_uint16 : return toString(value.val_uint16);

     case DDL::TypeNode::Base::Type_sint32 : return toString(value.val_sint32);

     case DDL::TypeNode::Base::Type_uint32 : return toString(value.val_uint32);

     case DDL::TypeNode::Base::Type_sint64 : return toString(value.val_sint64);

     case DDL::TypeNode::Base::Type_uint64 : return toString(value.val_uint64);

     case DDL::TypeNode::Base::Type_text : return toString(value.val_text);

     case DDL::TypeNode::Base::Type_ip : return toString(value.val_ip);

     default:
      {
       Printf(Exception,"App::DDLView::toString(...) : internal");

       return Null;
      }
    }
 }

void DDLView::setPtr(PtrDesc *desc,DDL::Value value) // TODO
 {
  Used(desc);
  Used(value);
 }

ValueDesc DDLView::build(DDL::TypeNode::Base *type,DDL::Value value)
 {
  ValueDesc ret;

  StrLen *desc=pool.create<StrLen>(toString(type,value));

  ret.ptr=desc;

  return ret;
 }

PtrLen<FieldDesc> DDLView::buildFields(DDL::StructNode *struct_node)
 {
  PtrLen<FieldDesc> ret=pool.createArray<FieldDesc>(struct_node->field_list.count);

  ulen i=0;

  for(DDL::FieldNode &field : struct_node->field_list )
    {
     ret[i].name=fieldName(field.index,field.name.name.str);

     i++;
    }

  return ret;
 }

ValueDesc DDLView::build(DDL::StructNode *struct_node,PtrLen<DDL::Value> value)
 {
  // create table

  PtrLen<PtrLen<ValueDesc> > table=pool.createArray<PtrLen<ValueDesc> >(value.len);

  for(ulen i=0; i<value.len ;i++)
    {
     PtrLen<DDL::Value> src=value[i].val_block.data;

     PtrLen<ValueDesc> dst=pool.createArray<ValueDesc>(src.len);

     ulen j=0;

     for(DDL::FieldNode &field : struct_node->field_list )
       {
        dst[j]=build(field.type_node,src[j]);

        j++;
       }

     table[i]=dst;
    }

  // create desc

  ValueDesc ret;

  StructDesc *desc=pool.create<StructDesc>();

  desc->fields=buildFields(struct_node);

  desc->table=table;

  ret.ptr=desc;

  return ret;
 }

ValueDesc DDLView::build(DDL::TypeNode *type,PtrLen<DDL::Value> value)
 {
  GetStructNode get(type);

  if( DDL::StructNode *struct_node = +get )
    {
     return build(struct_node,value);
    }
  else
    {
     PtrLen<ValueDesc> list=pool.createArray<ValueDesc>(value.len);

     for(ulen i=0; i<list.len ;i++) list[i]=build(type,value[i]);

     ValueDesc ret;

     PtrLen<ValueDesc> *desc=pool.create<PtrLen<ValueDesc> >(list);

     ret.ptr=desc;

     return ret;
    }
 }

ValueDesc DDLView::build(DDL::StructNode *struct_node,DDL::Value value)
 {
  return build(struct_node,Range(&value,1));
 }

ValueDesc DDLView::build(DDL::AliasNode *alias_node,DDL::Value value)
 {
  return build(alias_node->result_type,value);
 }

ValueDesc DDLView::build(DDL::TypeNode::Ref *type,DDL::Value value)
 {
  ValueDesc ret;

  auto obj = [this,&ret] (auto *ptr,DDL::Value value) { ret=this->build(ptr,value); } ;

  ElaborateAnyPtr(obj,type->ptr,value);

  return ret;
 }

ValueDesc DDLView::build(DDL::TypeNode::Ptr *type,DDL::Value value)
 {
  Used(type);

  ValueDesc ret;

  PtrDesc *desc=pool.create<PtrDesc>();

  setPtr(desc,value);

  ret.ptr=desc;

  return ret;
 }

ValueDesc DDLView::build(DDL::TypeNode::PolyPtr *type,DDL::Value value)
 {
  Used(type);

  ValueDesc ret;

  PtrDesc *desc=pool.create<PtrDesc>();

  setPtr(desc,value);

  ret.ptr=desc;

  return ret;
 }

ValueDesc DDLView::build(DDL::TypeNode::Array *type,DDL::Value value)
 {
  return build(type->type_node,value.val_block.data);
 }

ValueDesc DDLView::build(DDL::TypeNode::ArrayLen *type,DDL::Value value)
 {
  return build(type->type_node,value.val_block.data);
 }

ValueDesc DDLView::build(DDL::TypeNode::Struct *type,DDL::Value value)
 {
  return build(type->struct_node,value);
 }

ValueDesc DDLView::build(DDL::TypeNode *type,DDL::Value value)
 {
  ValueDesc ret;

  auto obj = [this,&ret] (auto *ptr,DDL::Value value) { ret=this->build(ptr,value); } ;

  ElaborateAnyPtr(obj,type->ptr,value);

  return ret;
 }

void DDLView::build(const DDL::EvalResult &result)
 {
  auto list=Range(result.const_table);

  consts=pool.createArray<ConstDesc>(list.len);

  for(ulen i=0; i<list.len ;i++)
    {
     auto &src=list[i];
     auto &dst=consts[i];

     dst.name=build(src.node);
     dst.value=build(src.type,src.value);
    }
 }

DDLView::DDLView()
 {
 }

DDLView::~DDLView()
 {
 }

void DDLView::update(DDL::EngineResult result)
 {
  erase();

  if( !result ) return;

  try
    {
     build(*result.eval);
    }
  catch(CatchType)
    {
     erase();

     throw;
    }
 }

/* class DDLInnerWindow */

void DDLInnerWindow::posX(ulen pos)
 {
  x_pos=pos;

  redraw();
 }

void DDLInnerWindow::posY(ulen pos)
 {
  y_pos=pos;

  redraw();
 }

DDLInnerWindow::DDLInnerWindow(SubWindowHost &host,const Config &cfg_)
 : SubWindow(host),
   cfg(cfg_),

   connector_posX(this,&DDLInnerWindow::posX),
   connector_posY(this,&DDLInnerWindow::posY)
 {
 }

DDLInnerWindow::~DDLInnerWindow()
 {
 }

 // methods

void DDLInnerWindow::update(DDL::EngineResult result) // TODO
 {
  view.update(result);

  x_total=600;
  y_total=600;

  x_pos=0;
  y_pos=0;
 }

 // drawing

void DDLInnerWindow::layout() // TODO
 {
  Point size=getSize();

  x_page=size.x;

  if( x_page>=x_total ) x_pos=0;

  y_page=size.y;

  if( y_page>=y_total ) y_pos=0;
 }

void DDLInnerWindow::draw(DrawBuf buf,bool) const // TODO
 {
  buf.erase(Black);
 }

 // base

void DDLInnerWindow::open() // TODO
 {
 }

/* class DDLWindow */

void DDLWindow::file_updated()
 {
  inner.update(file.getResult());

  layout();

  redraw();
 }

DDLWindow::DDLWindow(SubWindowHost &host,const Config &cfg_,DDLFile &file_)
 : ComboWindow(host),
   cfg(cfg_),
   file(file_),

   inner(wlist,cfg),
   scroll_x(wlist,cfg.x_cfg),
   scroll_y(wlist,cfg.y_cfg),

   connector_posx(&scroll_x,&XScrollWindow::setPos,inner.scroll_x),
   connector_posy(&scroll_y,&YScrollWindow::setPos,inner.scroll_y),

   connector_file_updated(this,&DDLWindow::file_updated,file_.updated)
 {
  wlist.insTop(inner);

  inner.bind(scroll_x.changed,scroll_y.changed);
 }

DDLWindow::~DDLWindow()
 {
 }

void DDLWindow::layout()
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

/* class DisplayWindow */

void DisplayWindow::error(StrLen str)
 {
  msg.setInfo(str);

  if( msg.isDead() ) msg.create(getFrame(),"Error"_def);
 }

void DisplayWindow::setPretextFileName(StrLen file_name)
 {
  text_pretext.setText(SafeString(file_name));
 }

void DisplayWindow::setFileName(StrLen file_name)
 {
  text_file.setText(SafeString(file_name));
 }

DisplayWindow::DisplayWindow(SubWindowHost &host,const Config &cfg_)
 : ComboWindow(host),
   cfg(cfg_),

   label_pretext(wlist,cfg.label_cfg,"Pretext"_def),
   label_file(wlist,cfg.label_cfg,"File"_def),
   text_pretext(wlist,cfg.textline_cfg),
   text_file(wlist,cfg.textline_cfg),
   dline(wlist,cfg.dline_cfg),
   ddl(wlist,cfg.ddl_cfg,file),

   msg(getFrame()->getDesktop(),cfg.msg_cfg)
 {
  wlist.insTop(label_pretext,label_file,text_pretext,text_file,dline,ddl);
 }

DisplayWindow::~DisplayWindow()
 {
 }

 // methods

void DisplayWindow::open(StrLen file_name)
 {
  auto result=file.open(file_name);

  if( result.ok )
    {
     opened.assert(file_name,true);

     setFileName(file_name);

     redraw();
    }
  else
    {
     opened.assert(file_name,false);

     setFileName(Empty);

     redraw();

     error(result.etext.str());
    }
 }

void DisplayWindow::openPretext(StrLen file_name)
 {
  auto result=file.openPretext(file_name);

  if( result.ok )
    {
     setPretextFileName(file_name);

     redraw();
    }
  else
    {
     error(result.etext.str());
    }
 }

void DisplayWindow::noPretext()
 {
  file.noPretext();

  setPretextFileName(Empty);

  redraw();
 }

 // drawing

void DisplayWindow::layout()
 {
  PaneCut pane(getSize(),+cfg.space_dxy);

  pane.shrink();

  // label_pretext , text_pretext

  {
   auto label__pretext=CutPoint(label_pretext);
   auto text__pretext=CutPoint(text_pretext);

   Coord dy=SupDY(label__pretext,text__pretext);

   pane.cutTop(dy).place_cutLeftCenter(label__pretext).place(AlignCenterY(text__pretext));
  }

  // label_file , text_file

  {
   auto label__file=CutPoint(label_file);
   auto text__file=CutPoint(text_file);

   Coord dy=SupDY(label__file,text__file);

   pane.cutTop(dy).place_cutLeftCenter(label__file).place(AlignCenterY(text__file));
  }

  // dline

  pane.place_cutTop(dline);

  // ddl

  pane.place(ddl);
 }

void DisplayWindow::draw(DrawBuf buf,bool drag_active) const
 {
  buf.erase(+cfg.back);

  wlist.draw(buf,drag_active);
 }

void DisplayWindow::draw(DrawBuf buf,Pane pane,bool drag_active) const
 {
  buf.block_safe(pane,+cfg.back);

  wlist.draw(buf,pane,drag_active);
 }

} // namespace App



