/* AspectData.h */
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

#ifndef AspectData_h
#define AspectData_h

#include <inc/ErrorText.h>

#include <CCore/inc/Array.h>
#include <CCore/inc/Swap.h>

namespace App {

/* classes */

class RelPath;

//enum ItemStatus;

struct FileData;

struct DirData;

class AspectData;

/* class RelPath */

class RelPath : NoCopy
 {
   char buf[MaxPathLen];
   ulen len = 0 ;

  public:

   RelPath(StrLen base_path,StrLen path);

   ulen operator + () const { return len; }

   bool operator ! () const { return !len; }

   StrLen getPath() const { return Range(buf,len); }
 };

/* enum ItemStatus */

enum ItemStatus
 {
  Item_New    = 0,
  Item_Ignore = 1,
  Item_Red    = 2,
  Item_Yellow = 3,
  Item_Green  = 4
 };

const char * GetTextDesc(ItemStatus status);

/* struct FileData */

struct FileData
 {
  String name;
  ItemStatus status = Item_New ;

  FileData() noexcept;

  ~FileData();
 };

/* struct DirData */

struct DirData : NoCopy
 {
  String name;
  ItemStatus status = Item_New ;

  SimpleArray<DirData> dirs;
  SimpleArray<FileData> files;

  // work

  mutable ulen index = 0 ;

  DirData() noexcept;

  ~DirData();

  void erase();

  // swap objects

  void objSwap(DirData &obj)
   {
    Swap(name,obj.name);
    Swap(status,obj.status);
    Swap(dirs,obj.dirs);
    Swap(files,obj.files);
   }
 };

/* class AspectData */

class AspectData : NoCopy
 {
   String path; // abs
   DirData root;

  private:

   class DirProc;

   static void Build(DirData &root,StrLen path);

   template <class T,class Func>
   static void CopyByName(PtrLen<T> dst,PtrLen<const T> src,Func copy);

   static void CopyFiles(SimpleArray<FileData> &dst,const SimpleArray<FileData> &src);

   static void CopyDirs(SimpleArray<DirData> &dst,const SimpleArray<DirData> &src);

   static void Copy(DirData &root,const DirData &dir);

   void sync();

   static void PrintDir(PrinterType &out,ulen &index,const DirData &dir);

   static void PrintSub(PrinterType &out,ulen &index,const DirData &dir);

  public:

   AspectData() noexcept;

   ~AspectData();

   // methods

   const String & getPath() const { return path; }

   // save/load

   void blank(StrLen path);

   void save(StrLen file_name,ErrorText &etext) const;

   void load(StrLen file_name,ErrorText &etext);
 };

} // namespace App

#endif


