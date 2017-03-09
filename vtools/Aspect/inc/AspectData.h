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

#include <inc/Application.h>

#include <CCore/inc/Array.h>

namespace App {

/* classes */

//enum ItemStatus;

struct FileData;

struct DirData;

class AspectData;

/* enum ItemStatus */

enum ItemStatus
 {
  Item_New    = 0,
  Item_Ignore = 1,
  Item_Red    = 2,
  Item_Yellow = 3,
  Item_Green  = 4
 };

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

  SimpleArray<FileData> files;
  SimpleArray<DirData> dirs;

  DirData() noexcept;

  ~DirData();
 };

/* class AspectData */

class AspectData : NoCopy
 {
   String path;
   DirData root;

  public:

   AspectData() noexcept;

   ~AspectData();
 };

} // namespace App

#endif


