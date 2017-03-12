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
#include <CCore/inc/MakeString.h>

namespace App {

/* functions */

bool IsRelPath(StrLen path);

/* classes */

struct DiffPath;

class RelPath;

class NormalPath;

//enum ItemStatus;

struct FileData;

struct DirData;

class AspectData;

/* struct DiffPath */

struct DiffPath
 {
  StrLen a;
  StrLen b;
  bool ok = false ;

  DiffPath(StrLen a,StrLen b);

  bool operator ! () const { return !ok; }
 };

/* class RelPath */

class RelPath : NoCopy
 {
   MakeString<MaxPathLen> out;
   bool ok = false ;

  private:

   static ulen Down(StrLen path);

   void relPath(ulen down,StrLen path);

  public:

   RelPath(StrLen base_path,StrLen path);

   ulen operator + () const { return ok; }

   bool operator ! () const { return !ok; }

   StrLen get() const { return out.get(); }
 };

/* class NormalPath */

class NormalPath : NoCopy
 {
   char buf[MaxPathLen+1];
   StrLen path;

  public:

   explicit NormalPath(StrLen path);

   StrLen get() const { return path; }
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

   static StrLen Pretext();

   template <class Dir>
   static void Load(DirData &dst,Dir src);

   void toAbs(StrLen file_name);

  public:

   AspectData() noexcept;

   ~AspectData();

   // methods

   bool operator ! () const { return !Range(path); }

   const String & getPath() const { return path; }

   void erase();

   // save/load

   void blank(StrLen path);

   void save(StrLen file_name,ErrorText &etext) const;

   void load(StrLen file_name,ErrorText &etext);
 };

} // namespace App

#endif


