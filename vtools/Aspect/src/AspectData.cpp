/* AspectData.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: Aspect 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2017 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/AspectData.h>

#include <CCore/inc/Path.h>
#include <CCore/inc/DirTreeRun.h>
#include <CCore/inc/ElementPool.h>

#include <CCore/inc/Cmp.h>
#include <CCore/inc/algon/SortUnique.h>

#include <CCore/inc/Print.h>
#include <CCore/inc/PrintStem.h>

#include <CCore/inc/video/PrintDDL.h>

#include <CCore/inc/ddl/DDLEngine.h>
#include <CCore/inc/ddl/DDLTypeSet.h>

#include <CCore/inc/FileName.h>
#include <CCore/inc/FileToMem.h>
#include <CCore/inc/FileSystem.h>

namespace App {

/* functions */

bool IsRelPath(StrLen path)
 {
  if( !path ) return false;

  SplitDev split(path);

  if( !split )
    {
     if( PathBase::IsSlash(path[0]) ) return false;

     return true;
    }

  return false;
 }

/* class RelPath */

ulen RelPath::Down(StrLen path)
 {
  ulen ret=0;

  while( +path )
    {
     SplitDir split(path);

     if( !split )
       {
        if( PathBase::IsDot(path) )
          {
           // continue;
          }
        else if( PathBase::IsDotDot(path) )
          {
           if( ret ) ret--; else return MaxULen;
          }
        else
          {
           ret++;
          }

        break;
       }
     else
       {
        if( PathBase::IsDot(split.dir) )
          {
           // continue;
          }
        else if( PathBase::IsDotDot(split.dir) )
          {
           if( ret ) ret--; else return MaxULen;
          }
        else
          {
           ret++;
          }

        path=split.path;
       }
    }

  return ret;
 }

void RelPath::relPath(ulen down,StrLen path)
 {
  if( +path )
    {
     if( PathBase::IsSlash(path[0]) ) return;

     for(; down ;down--) out.add("../"_c);

     out.add(path);
    }
  else
    {
     if( down )
       {
        for(down--; down ;down--) out.add("../"_c);

        out.add(".."_c);
       }
     else
       {
        out.add("."_c);
       }
    }

  ok=+out;
 }

void RelPath::relPath(StrLen base_path,StrLen path)
 {
  while( +path && +base_path )
    {
     SplitDir split(path);
     SplitDir base_split(base_path);

     if( !split )
       {
        if( !base_split )
          {
           if( path.equal(base_path) )
             {
              path=Null;
              base_path=Null;
             }
          }
        else
          {
           if( path.equal(base_split.dir.inner(0,1)) )
             {
              path=Null;
              base_path=base_split.path;
             }
          }

        break;
       }
     else
       {
        if( !base_split )
          {
           if( split.dir.inner(0,1).equal(base_path) )
             {
              base_path=Null;
              path=split.path;
             }

           break;
          }
        else
          {
           if( !split.dir.equal(base_split.dir) ) break;

           base_path=base_split.path;
           path=split.path;
          }
       }
    }

  ulen down=Down(base_path);

  if( down!=MaxULen ) relPath(down,path);
 }

RelPath::RelPath(StrLen base_path,StrLen path)
 {
  SplitPath base_split(base_path);
  SplitPath split(path);

  if( !split )
    {
     if( !base_split )
       {
        // continue;
       }
     else
       {
        return;
       }
    }
  else
    {
     if( !base_split )
       {
        return;
       }
     else
       {
        if( !split.dev.equal(base_split.dev) ) return;

        // continue;
       }
    }

  relPath(base_split.path,split.path);
 }

/* class NormalPath */

NormalPath::NormalPath(StrLen path_)
 {
  FileSystem fs;

  path=fs.pathOf(path_,buf);
 }

/* enum ItemStatus */

const char * GetTextDesc(ItemStatus status)
 {
  switch( status )
    {
     case Item_New    : return "New";
     case Item_Ignore : return "Ignore";
     case Item_Red    : return "Red";
     case Item_Yellow : return "Yellow";
     case Item_Green  : return "Green";

     default: return "???";
    }
 }

/* struct FileData */

FileData::FileData() noexcept
 {
 }

FileData::~FileData()
 {
 }

/* struct DirData */

DirData::DirData() noexcept
 {
 }

DirData::~DirData()
 {
 }

void DirData::erase()
 {
  name=Null;
  status=Item_New;

  files.erase();
  dirs.erase();
 }

/* class AspectData */

class AspectData::DirProc : NoCopy
 {
  public:

   struct FileNode : NoCopy
    {
     FileNode *next;
     StrLen name;

     FileNode(FileNode *next_,StrLen name_) : next(next_),name(name_) {}
    };

   struct DirNode;

   struct Final
    {
     SimpleArray<DirData> dirs;
     SimpleArray<FileData> files;
    };

   struct DataType : NoCopy
    {
     DirNode *dirs = 0 ;
     FileNode *files = 0 ;

     Final *final;

     explicit DataType(Final *final_) : final(final_) {}

     void finish()
      {
       {
        ulen count=0;

        for(auto *node=dirs; node ;node=node->next) count++;

        SimpleArray<DirData> temp(count);

        auto *ptr=temp.getPtr();

        for(auto *node=dirs; node ;node=node->next,ptr++)
          {
           ptr->name=String(node->name);

           Swap(ptr->dirs,node->data.final->dirs);
           Swap(ptr->files,node->data.final->files);
          }

        Swap(final->dirs,temp);
       }

       {
        ulen count=0;

        for(auto *node=files; node ;node=node->next) count++;

        SimpleArray<FileData> temp(count);

        auto *ptr=temp.getPtr();

        for(auto *node=files; node ;node=node->next,ptr++) ptr->name=String(node->name);

        Swap(final->files,temp);
       }
      }
    };

   struct DirNode : NoCopy
    {
     DirNode *next;
     StrLen name;
     DataType data;

     DirNode(DirNode *next_,StrLen name_,Final *final) : next(next_),name(name_),data(final) {}
    };

  private:

   ElementPool pool;
   Collector<Final> finals;

   DataType *root = 0 ;

  private:

   Final * createFinal()
    {
     return finals.append_default();
    }

   FileNode * create(FileNode *top,StrLen name)
    {
     return pool.create<FileNode>(top,pool.dup(name));
    }

   DirNode * create(DirNode *top,StrLen name)
    {
     return pool.create<DirNode>(top,pool.dup(name),createFinal());
    }

  public:

   DirProc() {}

   ~DirProc() {}

   void finish(DirData &dir)
    {
     if( !root ) return;

     Swap(dir.dirs,root->final->dirs);
     Swap(dir.files,root->final->files);
    }

   DataType * dir(StrLen)
    {
     return root=pool.create<DataType>(createFinal());
    }

   DataType * dir(StrLen,StrLen name,DataType *parent_data)
    {
     DirNode *node=create(parent_data->dirs,name);

     parent_data->dirs=node;

     return &node->data;
    }

   void file(StrLen,StrLen name,DataType *parent_data)
    {
     parent_data->files=create(parent_data->files,name);
    }

   void enddir(StrLen,StrLen,DataType *data)
    {
     data->finish();
    }
 };

void AspectData::Build(DirData &root,StrLen path)
 {
  DirTreeRun run(Range(path));

  DirProc proc;

  run.apply(proc);

  proc.finish(root);
 }

template <class T,class Func>
void AspectData::CopyByName(PtrLen<T> dst,PtrLen<const T> src,Func copy)
 {
  struct Item
   {
    union
     {
      T *dst;
      const T *src;
     };

    bool is_dst;

    void setDst(T &dst_)
     {
      is_dst=true;
      dst=&dst_;
     }

    void setSrc(const T &src_)
     {
      is_dst=false;
      src=&src_;
     }

    StrLen getName() const { return is_dst?Range(dst->name):Range(src->name); }
   };

  SimpleArray<Item> temp(LenAdd(dst.len,src.len));

  Item *ptr=temp.getPtr();

  for(; +dst ;++dst,ptr++) ptr->setDst(*dst);

  for(; +src ;++src,ptr++) ptr->setSrc(*src);

  Algon::IncrSortThenApplyUniqueRangeBy(Range(temp), [] (const Item &item) { return CmpAsStr(item.getName()); } ,
                                                     [copy] (PtrLen<Item> r)
                                                            {
                                                             const T *src=0;

                                                             for(auto s=r; +s ;++s)
                                                               if( !s->is_dst )
                                                                 {
                                                                  if( src ) return;

                                                                  src=s->src;
                                                                 }

                                                             if( !src ) return;

                                                             for(auto s=r; +s ;++s)
                                                               if( s->is_dst )
                                                                 {
                                                                  copy(*(s->dst),*src);
                                                                 }

                                                            } );
 }

void AspectData::CopyFiles(SimpleArray<FileData> &dst,const SimpleArray<FileData> &src)
 {
  CopyByName(Range(dst),Range(src), [] (FileData &dst,const FileData &src) { dst.status=src.status; } );
 }

void AspectData::CopyDirs(SimpleArray<DirData> &dst,const SimpleArray<DirData> &src)
 {
  CopyByName(Range(dst),Range(src),Copy);
 }

void AspectData::Copy(DirData &root,const DirData &dir)
 {
  root.status=dir.status;

  CopyDirs(root.dirs,dir.dirs);
  CopyFiles(root.files,dir.files);
 }

void AspectData::sync()
 {
  DirData temp;

  Build(temp,Range(path));

  Copy(temp,root);

  Swap(temp,root);
 }

AspectData::AspectData() noexcept
 {
 }

AspectData::~AspectData()
 {
 }

void AspectData::erase()
 {
  path=Null;
  root.erase();
 }

 // save/load

void AspectData::blank(StrLen path_)
 {
  path=String(path_);
  root.erase();

  sync();
 }

void AspectData::PrintDir(PrinterType &out,ulen &index,const DirData &dir)
 {
  Printf(out,"  {\n");

  Printf(out,"   #;,\n",DDLString(dir.name));

  Printf(out,"   #;,\n",dir.status);

  {
   Printf(out,"   {");

   PrintFirst stem("\n",",\n");

   for(const DirData &sub : dir.dirs )
     {
      sub.index=index;

      Printf(out,"#;    &Dir#;",stem,index++);
     }

   Printf(out,"\n   },\n");
  }

  {
   Printf(out,"   {");

   PrintFirst stem("\n",",\n");

   for(const FileData &file : dir.files )
     {
      Printf(out,"#;    { #; , #; }",stem,DDLString(file.name),file.status);
     }

   Printf(out,"\n   }\n");
  }

  Printf(out,"  }");
 }

void AspectData::PrintSub(PrinterType &out,ulen &index,const DirData &dir)
 {
  for(const DirData &sub : dir.dirs )
    {
     Printf(out,"Dir Dir#;=\n",sub.index);

     PrintDir(out,index,sub);

     Printf(out,";\n\n");
    }

  for(const DirData &sub : dir.dirs )
    {
     PrintSub(out,index,sub);
    }
 }

void AspectData::save(StrLen file_name,ErrorText &etext) const
 {
  PrintBuf eout(etext.getBuf());

  ReportExceptionTo<PrintBuf> report(eout);

  try
    {
     PrintFile out(file_name);

     Printf(out,"//##include <Aspect.ddl>\n\n");

     Printf(out,"Aspect Data=\n {\n");

     {
      StrLen abspath=Range(path);

      RelPath rel(PrefixPath(file_name),abspath);

      if( +rel )
        Printf(out,"  #;,\n",DDLString(rel.get()));
      else
        Printf(out,"  #;,\n",DDLString(abspath));
     }

     ulen index=0;

     PrintDir(out,index,root);

     Printf(out,"\n };\n\n");

     PrintSub(out,index,root);
    }
  catch(CatchType)
    {
     Printf(eout,"\n@ #.q;",file_name);

     etext.setTextLen(eout.close().len);
    }
 }

#include "Aspect.TypeDef.gen.h"
#include "Aspect.TypeSet.gen.h"

StrLen AspectData::Pretext()
 {
  return
"type Status = uint8 ;"

"Status New    = 0 ;"
"Status Ignore = 1 ;"
"Status Red    = 2 ;"
"Status Yellow = 3 ;"
"Status Green  = 4 ;"

"struct File"
" {"
"  text name;"
"  Status status;"
" };"

"struct Dir"
" {"
"  text name;"
"  Status status;"

"  Dir * [] dirs;"
"  File[] files;"
" };"

"struct Aspect"
" {"
"  text path;"

"  Dir root;"
" };"_c;
 }

template <class Dir>
void AspectData::Load(DirData &dst,Dir src)
 {
  dst.name=String(src.name.getStr());
  dst.status=ItemStatus(src.status);

  {
   SimpleArray<FileData> temp(src.files.len);

   FileData *ptr=temp.getPtr();

   for(const TypeDef::File &f : src.files.getRange() )
     {
      ptr->name=String(f.name.getStr());
      ptr->status=ItemStatus(f.status);

      ptr++;
     }

   Swap(dst.files,temp);
  }

  {
   SimpleArray<DirData> temp(src.dirs.len);

   DirData *ptr=temp.getPtr();

   for(auto d : src.dirs.getRange() )
     {
      Load(*ptr,*d);

      ptr++;
     }

   Swap(dst.dirs,temp);
  }
 }

void AspectData::toAbs(StrLen file_name)
 {
  StrLen str=Range(path);

  if( IsRelPath(str) )
    {
     MakeFileName temp(PrefixPath(file_name),str);

     if( +temp )
       {
        NormalPath normal(temp.get());

        path=String(normal.get());
       }
    }
 }

void AspectData::load(StrLen file_name,ErrorText &etext)
 {
  erase();

  PrintBuf eout(etext.getBuf());

  ReportExceptionTo<PrintBuf> report(eout);

  try
    {
     DDL::FileEngine<FileName,FileToMem> engine(eout);

     auto result=engine.process(file_name,Pretext());

     if( !result )
       {
        Printf(eout,"\n@ #.q;",file_name);

        etext.setTextLen(eout.close().len);

        return;
       }
     else
       {
        DDL::TypedMap<TypeSet> map(result);
        MemAllocGuard guard(map.getLen());

        map(guard);

        // populate

        TypeDef::Aspect data=map.takeConst<TypeDef::Aspect>("Data");

        path=String(data.path.getStr());

        Load(root,data.root);

        toAbs(file_name);
       }
    }
  catch(CatchType)
    {
     erase();

     Printf(eout,"\n@ #.q;",file_name);

     etext.setTextLen(eout.close().len);
    }
 }

} // namespace App

