/* FileBoss.cpp */
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

#include <CCore/inc/video/FileBoss.h>

#include <CCore/inc/FileSystem.h>

namespace CCore {
namespace Video {

namespace Private_FileBoss {

class DefBoss : public FileBossBase
 {
   FileSystem fs;

  public:

   DefBoss() {}

   virtual ~DefBoss() {}

   // AbstractFileBoss

   virtual FileType getFileType(StrLen path) { return fs.getFileType(path); }

   virtual StrLen pathOf(StrLen path,char buf[MaxPathLen+1]) { return fs.pathOf(path,buf); }

   virtual void enumDir(StrLen dir_name,Function<void (StrLen name,FileType type)> func)
    {
     FileSystem::DirCursor cur(fs,dir_name);

     cur.apply( [func] (StrLen name,FileType type) { func(name,type); } );
    }

   virtual StrLen getHitDirFile() const { return "HitDirs.ddl"; }
 };

DefBoss Object CCORE_INITPRI_3 ;

} // namespace Private_FileBoss

using namespace Private_FileBoss;

/* class FileBoss */

FileBoss::FileBoss() noexcept
 : ptr(&Object)
 {
  ptr->incRef();
 }

} // namespace Video
} // namespace CCore

