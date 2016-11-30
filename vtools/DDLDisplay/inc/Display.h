/* Display.h */
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

#include <inc/Application.h>

#include <CCore/inc/video/MessageWindow.h>

#include <CCore/inc/FileName.h>
#include <CCore/inc/FileToMem.h>
#include <CCore/inc/String.h>
#include <CCore/inc/ElementPool.h>
#include <CCore/inc/Array.h>

#include <CCore/inc/ddl/DDLEngine.h>

namespace App {

/* classes */

class DDLFile;

struct uPoint;

struct uPane;

struct FieldDesc;

struct StructDesc;

struct PtrDesc;

struct ValueDesc;

struct ConstDesc;

class DDLView;

class DDLInnerWindow;

class DDLWindow;

class DisplayWindow;

/* class DDLFile */

class DDLFile : NoCopy
 {
   PrintString eout;

   FileToMem pretext;

   DDL::FileEngine<FileName,FileToMem> engine;

   DDL::EngineResult result;

  private:

   void erase();

  public:

   DDLFile() noexcept;

   ~DDLFile();

   // methods

   auto getResult() const { return result; }

   struct OpenResult
    {
     DefString etext;
     bool ok;
    };

   OpenResult open(StrLen file_name);

   OpenResult openPretext(StrLen file_name);

   void noPretext();

   // signals

   Signal<> updated;
 };

/* struct uPoint */

struct uPoint
 {
  ulen x = 0 ;
  ulen y = 0 ;
 };

/* struct uPane */

struct uPane
 {
  uPoint base;
  uPoint size;
 };

/* struct FieldDesc */

struct FieldDesc
 {
  StrLen name;
  uPane place;
 };

/* struct StructDesc */

struct StructDesc
 {
  PtrLen<FieldDesc> fields;
  PtrLen<PtrLen<ValueDesc> > table;
 };

/* struct PtrDesc */

struct PtrDesc
 {
  StrLen name;

  struct Index
   {
    ulen index = 0 ;
    StrLen field;

    void set(ulen index_)
     {
      index=index_;
      field=Null;
     }
   };

  PtrLen<Index> index;

  PtrLen<ValueDesc> target;
 };

/* struct ValueDesc */

struct ValueDesc
 {
  uPane place;

  AnyPtr<StrLen,PtrLen<ValueDesc>,StructDesc,PtrDesc> ptr;
 };

/* struct ConstDesc */

struct ConstDesc
 {
  StrLen name;
  uPane place;

  ValueDesc value;
 };

/* class DDLView */

class DDLView : NoCopy
 {
   static const ulen TextBufLen = 1_KByte ;

   static const ulen MaxNameLen = 8_KByte ;

   ElementPool pool;

   PtrLen<ConstDesc> consts;

   DynArray<StrLen> field_name;

   DynArray<PtrDesc::Index> index_buf;

   DynArray<PtrDesc *> ptr_buf;

  private:

   void erase();

   template <class T>
   static PtrLen<T> Single(T &obj) { return Range(&obj,1); }

   static void SetTail(PtrLen<char> &ret,char ch);

   static void SetTail(PtrLen<char> &ret,StrLen str);

   static void ProvideIndex(AnyType &obj,ulen index,ulen min_len);

   static void ProvideIndex(AnyType &obj,ulen index) { ProvideIndex(obj,index,100); }

   struct GetStructNode;

   StrLen fieldName(ulen index,StrLen name);

   StrLen fieldName(DDL::FieldNode &field) { return fieldName(field.index,field.name.name.str); }

   StrLen fieldName(DDL::StructNode *struct_node,ulen index);

   StrLen build(DDL::ConstNode *node);

   StrLen toString(StrLen value) { return pool.dup(value); }

   StrLen toString(AnyType value);

   StrLen toString(DDL::Text value) { return toString(value.str); }

   StrLen toString(DDL::TypeNode::Base *type,DDL::Value value);

   void setPtr(PtrDesc *desc,DDL::Value value);

   ValueDesc build(DDL::TypeNode::Base *type,DDL::Value value);

   PtrLen<FieldDesc> buildFields(DDL::StructNode *struct_node);

   ValueDesc build(DDL::StructNode *struct_node,PtrLen<DDL::Value> value);

   ValueDesc build(DDL::TypeNode *type,PtrLen<DDL::Value> value);

   ValueDesc build(DDL::StructNode *struct_node,DDL::Value value);

   ValueDesc build(DDL::AliasNode *alias_node,DDL::Value value);

   ValueDesc build(DDL::TypeNode::Ref *type,DDL::Value value);

   ValueDesc build(DDL::TypeNode::Ptr *type,DDL::Value value);

   ValueDesc build(DDL::TypeNode::PolyPtr *type,DDL::Value value);

   ValueDesc build(DDL::TypeNode::Array *type,DDL::Value value);

   ValueDesc build(DDL::TypeNode::ArrayLen *type,DDL::Value value);

   ValueDesc build(DDL::TypeNode::Struct *type,DDL::Value value);

   ValueDesc build(DDL::TypeNode *type,DDL::Value value);

   void setPtrTarget(PtrDesc *desc);

   void setPtrTargets();

   void build(const DDL::EvalResult &result);

  public:

   DDLView();

   ~DDLView();

   PtrLen<ConstDesc> getConstList() const { return consts; }

   void update(DDL::EngineResult result);
 };

/* class DDLInnerWindow */

class DDLInnerWindow : public SubWindow
 {
  public:

   struct Config
    {
     Config() noexcept {}

     explicit Config(const UserPreference &pref) // TODO
      {
       Used(pref);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   DDLView view;

   // scroll

   ulen x_pos   = 0 ;
   ulen x_total = 0 ;
   ulen x_page  = 0 ;

   ulen y_pos   = 0 ;
   ulen y_total = 0 ;
   ulen y_page  = 0 ;

  private:

   void posX(ulen pos);

   void posY(ulen pos);

   SignalConnector<DDLInnerWindow,ulen> connector_posX;
   SignalConnector<DDLInnerWindow,ulen> connector_posY;

  public:

   DDLInnerWindow(SubWindowHost &host,const Config &cfg);

   virtual ~DDLInnerWindow();

   // special methods

   bool shortDX() const { return x_page<x_total; }

   bool shortDY() const { return y_page<y_total; }

   template <class W>
   void setScrollX(W &window) { window.setRange(x_total,x_page,x_pos); }

   template <class W>
   void setScrollY(W &window) { window.setRange(y_total,y_page,y_pos); }

   void bind(Signal<ulen> &scroll_x,Signal<ulen> &scroll_y)
    {
     connector_posX.connect(scroll_x);
     connector_posY.connect(scroll_y);
    }

   // methods

   Point getMinSize() const { return Null; }

   void update(DDL::EngineResult result);

   // drawing

   virtual void layout();

   virtual void draw(DrawBuf buf,bool) const;

   // base

   virtual void open();

   // signals

   Signal<ulen> scroll_x;
   Signal<ulen> scroll_y;
 };

/* class DDLWindow */

class DDLWindow : public ComboWindow
 {
  public:

   struct Config : DDLInnerWindow::ConfigType
    {
     CtorRefVal<XScrollWindow::ConfigType> x_cfg;
     CtorRefVal<YScrollWindow::ConfigType> y_cfg;

     Config() noexcept {}

     explicit Config(const UserPreference &pref)
      : DDLInnerWindow::ConfigType(pref),
        x_cfg(SmartBind,pref),
        y_cfg(SmartBind,pref)
      {
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;
   const DDLFile &file;

   DDLInnerWindow inner;
   XScrollWindow scroll_x;
   YScrollWindow scroll_y;

  private:

   void setScroll()
    {
     if( scroll_x.isListed() ) inner.setScrollX(scroll_x);

     if( scroll_y.isListed() ) inner.setScrollY(scroll_y);
    }

  private:

   SignalConnector<XScrollWindow,ulen> connector_posx;
   SignalConnector<YScrollWindow,ulen> connector_posy;

   void file_updated();

   SignalConnector<DDLWindow> connector_file_updated;

  public:

   DDLWindow(SubWindowHost &host,const Config &cfg,DDLFile &file);

   virtual ~DDLWindow();

   // methods

   Point getMinSize() const { return Null; }

   // drawing

   virtual void layout();
 };

/* class DisplayWindow */

class DisplayWindow : public ComboWindow
 {
  public:

   struct Config
    {
     RefVal<Coord> space_dxy = 10 ;

     RefVal<VColor> back = Silver ;

     CtorRefVal<MessageWindow::ConfigType> msg_cfg;
     CtorRefVal<LabelWindow::ConfigType> label_cfg;
     CtorRefVal<TextLineWindow::ConfigType> textline_cfg;
     CtorRefVal<XDoubleLineWindow::ConfigType> dline_cfg;

     DDLWindow::Config ddl_cfg;

     Config() noexcept {}

     explicit Config(const UserPreference &pref)
      : msg_cfg(pref.getAlertMessageWindowConfig()),
        label_cfg(SmartBind,pref),
        textline_cfg(SmartBind,pref),
        dline_cfg(SmartBind,pref),
        ddl_cfg(pref)
      {
       space_dxy.bind(pref.get().space_dxy);
       back.bind(pref.get().back);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   DDLFile file;

   // subwins

   LabelWindow label_pretext;
   LabelWindow label_file;

   TextLineWindow text_pretext;
   TextLineWindow text_file;

   XDoubleLineWindow dline;

   DDLWindow ddl;

   // msg

   MessageWindow msg;

  private:

   void error(StrLen str);

   void setPretextFileName(StrLen file_name);

   void setFileName(StrLen file_name);

  public:

   DisplayWindow(SubWindowHost &host,const Config &cfg);

   virtual ~DisplayWindow();

   // methods

   void open(StrLen file_name);

   void openPretext(StrLen file_name);

   void noPretext();

   // drawing

   virtual void layout();

   virtual void draw(DrawBuf buf,bool drag_active) const;

   virtual void draw(DrawBuf buf,Pane pane,bool drag_active) const;

   // signals

   Signal<StrLen,bool> opened; // file_name ,  ok
 };

} // namespace App

