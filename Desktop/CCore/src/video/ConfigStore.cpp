/* ConfigStore.cpp */
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

#include <CCore/inc/video/ConfigStore.h>

#include <CCore/inc/FileName.h>
#include <CCore/inc/FileToMem.h>

#include <CCore/inc/Print.h>

#include <CCore/inc/ddl/DDLEngine.h>

#include <CCore/inc/Exception.h>

namespace CCore {
namespace Video {

/* GuardItemTypeMismatch() */

void GuardItemTypeMismatch(const char *name)
 {
  Printf(Exception,"CCore::Video::ConfigItem::#;<...>() : item type mismatch",name);
 }

/* class ConfigItem */

struct ConfigItem::DestroyCtx
 {
  Place<void> place;

  using RetType = void ;

  template <class T>
  RetType call()
   {
    T *obj=place;

    obj->~T();
   }

  RetType defcall(int)
   {
    // do nothing
   }
 };

void ConfigItem::Destroy(int id,Place<void> place)
 {
  Meta::TypeSwitch<ConfigTypeList>::Switch(id,DestroyCtx{place});
 }

struct ConfigItem::DDLTypeNameCtx
 {
  using RetType = StrLen ;

  template <class T>
  RetType call();

  RetType defcall(int)
   {
    return "???";
   }
 };

template <>
auto ConfigItem::DDLTypeNameCtx::call<unsigned>() -> RetType { return "uint"; }

template <>
auto ConfigItem::DDLTypeNameCtx::call<Coord>() -> RetType { return "Coord"; }

template <>
auto ConfigItem::DDLTypeNameCtx::call<MCoord>() -> RetType { return "MCoord"; }

template <>
auto ConfigItem::DDLTypeNameCtx::call<VColor>() -> RetType { return "VColor"; }

template <>
auto ConfigItem::DDLTypeNameCtx::call<Point>() -> RetType { return "Point"; }

template <>
auto ConfigItem::DDLTypeNameCtx::call<DefString>() -> RetType { return "text"; }

template <>
auto ConfigItem::DDLTypeNameCtx::call<FontParam>() -> RetType { return "Font"; }

StrLen ConfigItem::getDDLTypeName() const
 {
  return Meta::TypeSwitch<ConfigTypeList>::Switch(id,DDLTypeNameCtx{});
 }

/* class ConfigMap */

ConfigItem & ConfigMap::find_or_add(StrLen name)
 {
  auto result=map.find_or_add(name);

  return *result.obj;
 }

const char *const ConfigMap::Pretext=
"type Bool = uint8 ;\r\n"
"\r\n"
"Bool True = 1 ;\r\n"
"\r\n"
"Bool False = 0 ;\r\n"
"\r\n"
"type Coord = sint16 ;\r\n"
"\r\n"
"type MCoord = sint32 ; \r\n"
"\r\n"
"type VColor = uint32 ;\r\n"
"\r\n"
"struct Point\r\n"
" {\r\n"
"  Coord x;\r\n"
"  Coord y;\r\n"
" };\r\n"
"\r\n"
"struct Font\r\n"
" {\r\n"
"  type EngineType = int ;\r\n"
"\r\n"
"  const EngineType EngineDefault  = 0 ;\r\n"
"  const EngineType EngineFreeType = 1 ;\r\n"
"\r\n"
"  EngineType engine_type;\r\n"
"\r\n"
"  text file_name;\r\n"
"\r\n"
"  type SizeType = int ;\r\n"
"\r\n"
"  const SizeType SizeXY    = 0 ;\r\n"
"  const SizeType SizePoint = 1 ;\r\n"
"  const SizeType SizeIndex = 2 ;\r\n"
"\r\n"
"  SizeType size_type;\r\n"
"  int dx;\r\n"
"  int dy;\r\n"
"\r\n"
"  struct Config\r\n"
"   {\r\n"
"    type FontHintType = int ;\r\n"
"\r\n"
"    const FontHintType FontHintNone   = 0 ;\r\n"
"    const FontHintType FontHintNative = 1 ;\r\n"
"    const FontHintType FontHintAuto   = 2 ;\r\n"
"\r\n"
"    FontHintType fht;\r\n"
"\r\n"
"    type FontSmoothType = int ;\r\n"
"\r\n"
"    const FontSmoothType FontSmoothNone    = 0 ;\r\n"
"    const FontSmoothType FontSmooth        = 1 ;\r\n"
"    const FontSmoothType FontSmoothLCD_RGB = 2 ;\r\n"
"    const FontSmoothType FontSmoothLCD_BGR = 3 ;\r\n"
"\r\n"
"    FontSmoothType fsm;\r\n"
"\r\n"
"    Bool use_kerning;\r\n"
"    int strength;\r\n"
"    int gamma_order; // /1024\r\n"
"   };\r\n"
"\r\n"
"  Config cfg;\r\n"
" };\r\n"
"\r\n"
;

template <class Func>
struct ConfigMap::TypeFilter
 {
  Func func;

  void operator () (DDL::TypeNode::Base *ptr)
   {
    switch( ptr->type )
      {
       case DDL::TypeNode::Base::Type_uint : func.do_uint(); break;

       case DDL::TypeNode::Base::Type_text : func.do_text(); break;
      }
   }

  void operator () (DDL::TypeNode::Ref *ptr)
   {
    struct Dispatch
     {
      Func &func;

      void operator () (DDL::AliasNode *ptr)
       {
        if( ptr->depth==0 )
          {
           StrLen name=ptr->name.getStr();

           if( name.equal(StrLen("Coord",5)) )
             {
              func.do_Coord();
             }
           else if( name.equal(StrLen("MCoord",6)) )
             {
              func.do_MCoord();
             }
           else if( name.equal(StrLen("VColor",6)) )
             {
              func.do_VColor();
             }
          }
       }

      void operator () (DDL::StructNode *ptr)
       {
        if( ptr->depth==0 )
          {
           StrLen name=ptr->name.getStr();

           if( name.equal(StrLen("Font",4)) )
             {
              func.do_Font();
             }
           else if( name.equal(StrLen("Point",5)) )
             {
              func.do_Point();
             }
          }
       }
     };

    ptr->ptr.apply(Dispatch{func});
   }

  template <class T>
  void operator () (T *)
   {
    // do nothing
   }
 };

template <class Ptr,class Func>
void ConfigMap::Filter(Ptr ptr,Func func)
 {
  ptr.apply(TypeFilter<Func>{func});
 }

struct ConfigMap::AddItem
 {
  ConfigMap *obj;
  StrLen name;
  DDL::Value value;

  // Get... ()

  static int Get_int(DDL::Value value) { return value.get<DDL::imp_sint>().value; }

  static bool Get_Bool(DDL::Value value) { return value.get<DDL::imp_uint8>().value; }

  static StrLen Get_text(DDL::Value value) { return value.get<DDL::Text>().str; }

  static Coord Get_Coord(DDL::Value value) { return value.get<DDL::imp_sint16>().value; }

  // get...()

  unsigned get_uint() const { return value.get<DDL::imp_uint>().value; }

  StrLen get_text() const { return Get_text(value); }

  Coord get_Coord() const { return Get_Coord(value); }

  MCoord get_MCoord() const { return value.get<DDL::imp_sint32>().value; }

  uint32 get_VColor() const { return value.get<DDL::imp_uint32>().value; }

  Point get_Point() const
   {
    auto r=value.get<DDL::Block>().data;

    return Point(Get_Coord(r[0]),Get_Coord(r[1]));
   }

  FontParam get_Font() const
   {
    auto r=value.get<DDL::Block>().data;

    FontParam param;

    param.engine_type=FontParam::EngineType(Get_int(r[0]));

    param.file_name=Get_text(r[1]);

    param.size_type=FontParam::SizeType(Get_int(r[2]));

    int x=Get_int(r[3]);
    int y=Get_int(r[4]);

    switch( param.size_type )
      {
       case FontParam::SizeXY : param.set_size.size_xy=Coord(x); break;

       case FontParam::SizePoint : param.set_size.size=Point(Coord(x),Coord(y)); break;

       case FontParam::SizeIndex : param.set_size.index=ulen(x); break;
      }

    auto cfg=r[5].get<DDL::Block>().data;

    param.cfg.fht=FontHintType(Get_int(cfg[0]));
    param.cfg.fsm=FontSmoothType(Get_int(cfg[1]));
    param.cfg.use_kerning=Get_Bool(cfg[2]);
    param.cfg.strength=Get_int(cfg[3]);
    param.cfg.gamma_order=IntToGamma(Get_int(cfg[4]));

    return param;
   }

  // do...()

  void do_uint() { obj->add_uint(name,get_uint()); }

  void do_text() { obj->add_text(name,get_text()); }

  void do_Coord() { obj->add_Coord(name,get_Coord()); }

  void do_MCoord() { obj->add_MCoord(name,get_MCoord()); }

  void do_VColor() { obj->add_VColor(name,get_VColor()); }

  void do_Point() { obj->add_Point(name,get_Point()); }

  void do_Font() { obj->add_Font(name,get_Font()); }
 };

template <class T>
void ConfigMap::add(StrLen name,const T &value)
 {
  auto result=map.find_or_add(name);

  if( !result.new_flag )
    {
     Printf(Exception,"CCore::Value::ConfigMap::add(#.q;,...) : name duplication",name);
    }

  result.obj->update(value);
 }

void ConfigMap::add_uint(StrLen name,unsigned value) { add(name,value); }

void ConfigMap::add_text(StrLen name,StrLen value) { add(name,DefString(value)); }

void ConfigMap::add_Coord(StrLen name,Coord value) { add(name,value); }

void ConfigMap::add_MCoord(StrLen name,MCoord value) { add(name,value); }

void ConfigMap::add_VColor(StrLen name,uint32 value) { add(name,VColor(value)); }

void ConfigMap::add_Point(StrLen name,Point value) { add(name,value); }

void ConfigMap::add_Font(StrLen name,const FontParam &value) { add(name,value); }

ConfigMap::ConfigMap() noexcept
 {
 }

ConfigMap::~ConfigMap()
 {
 }

void ConfigMap::saveDDL(StrLen file_name) const
 {
  PrintFile out(file_name);

  //Printf(out,"include <ConfigTypes.ddl>\n\n");

  map.applyIncr( [&out] (const Key &key,const ConfigItem &item)
                        {
                         Printf(out,"#; #; = #; ;\n",item.getDDLTypeName(),key.name,item);

                        } );
 }

void ConfigMap::loadDDL(StrLen file_name)
 {
  modified=false;
  map.erase();

  char temp[512];
  PrintBuf eout(Range(temp));
  DDL::FileEngine<FileName,FileToMem> engine(eout);

  auto result=engine.process(file_name,Pretext);

  if( !result )
    {
     Printf(Exception,"CCore::Video::ConfigMap::loadDDL(#.q;) : input file processing error\n#;",file_name,eout.close());
    }
  else
    {
     for(const auto &res : result.eval->const_table )
       {
        auto *node=res.node;

        if( node->depth ) continue;

        auto *type=res.type;

        StrLen name=node->name.getStr();

        Filter(type->ptr,AddItem{this,name,res.value});
       }
    }
 }

bool ConfigMap::loadDDL_safe(StrLen file_name) noexcept
 {
  try
    {
     ConfigMap temp;

     temp.loadDDL(file_name);

     Swap(*this,temp);

     return true;
    }
  catch(...)
    {
     return false;
    }
 }

} // namespace Video
} // namespace CCore

