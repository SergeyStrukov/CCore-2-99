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

namespace App {

/* class RelPath */

RelPath::RelPath(StrLen base_path,StrLen path) // TODO
 {
  Used(base_path);
  Used(path);
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

      RelPath rel(file_name,abspath);

      if( +rel )
        Printf(out,"  #;,\n",DDLString(rel.getPath()));
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

void AspectData::load(StrLen file_name,ErrorText &etext) // TODO
 {
  Used(file_name);
  Used(etext);
 }

} // namespace App

