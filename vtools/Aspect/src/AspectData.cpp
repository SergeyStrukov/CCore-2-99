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

void AspectData::sync() // TODO
 {
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
     Printf(out,"Dir#;=\n ",sub.index);

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

