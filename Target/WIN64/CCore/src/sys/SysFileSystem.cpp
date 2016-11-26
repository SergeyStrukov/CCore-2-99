/* SysFileSystem.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: Target/WIN64
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2015 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <CCore/inc/sys/SysFileSystem.h>
#include <CCore/inc/sys/SysFileInternal.h>

#include <CCore/inc/Exception.h>
#include <CCore/inc/Array.h>
#include <CCore/inc/Path.h>

namespace CCore {
namespace Sys {

/* namespace Private_SysFileSystem */

namespace Private_SysFileSystem {

/* functions */

FileError EmptyDirRecursive(StrLen dir_name);

FileError DeleteDirRecursive(StrLen dir_name);

/* struct NameBuf */

struct NameBuf
 {
  char buf[MaxPathLen+1];
  ulen dir_len;
  ulen buf_len;

  bool init(StrLen dir_name)
   {
    if( dir_name.len>MaxPathLen-2 ) return false;

    dir_name.copyTo(buf);

    buf[dir_name.len]='/';
    buf[dir_name.len+1]='*';
    buf[dir_name.len+2]=0;

    dir_len=dir_name.len+1;
    buf_len=dir_name.len+2;

    return true;
   }

  bool set(StrLen file_name)
   {
    if( file_name.len>MaxPathLen-dir_len ) return false;

    file_name.copyTo(buf+dir_len);

    buf[dir_len+file_name.len]=0;

    buf_len=dir_len+file_name.len;

    return true;
   }

  operator const char * () const { return buf; }

  StrLen getStr() const { return StrLen(buf,buf_len); }
 };

/* IsSpecial() */

bool IsSpecial(StrLen name)
 {
  return ( name.len==1 && name[0]=='.' ) || ( name.len==2 && name[0]=='.' && name[1]=='.' ) ;
 }

/* EmptyDirRecursive() */

FileError EmptyDirRecursive(StrLen dir_name)
 {
  NameBuf buf;

  if( !buf.init(dir_name) ) return FileError_TooLongPath;

  Win64::FindFileData data;

  Win64::handle_t h_find=Win64::FindFirstFileA(buf,&data);

  if( h_find==Win64::InvalidFileHandle ) return MakeError(FileError_OpFault);

  do
    {
     StrLen file_name(data.file_name);

     if( !IsSpecial(file_name) )
       {
        FileError fe=FileError_Ok;

        if( buf.set(file_name) )
          {
           if( data.attr&Win64::FileAttributes_Directory )
             {
              fe=DeleteDirRecursive(buf.getStr());
             }
           else
             {
              if( !Win64::DeleteFileA(buf) )
                {
                 fe=MakeError(FileError_OpFault);
                }
             }
          }
        else
          {
           fe=FileError_TooLongPath;
          }

        if( fe )
          {
           Win64::FindClose(h_find);

           return fe;
          }
       }
    }
  while( FindNextFileA(h_find,&data) );

  Win64::error_t error_=Win64::GetLastError();

  Win64::FindClose(h_find);

  if( error_!=Win64::ErrorNoMoreFiles ) return MakeError(FileError_OpFault,error_);

  return FileError_Ok;
 }

/* DeleteDirRecursive() */

FileError DeleteDirRecursive(StrLen dir_name)
 {
  if( FileError fe=EmptyDirRecursive(dir_name) ) return fe;

  return MakeErrorIf(FileError_OpFault, !Win64::RemoveDirectoryA(dir_name.ptr) );
 }

/* Execz() */

FileError Execz(char *dir,char *program,char *arg)
 {
  Win64::flags_t flags=Win64::CreateNewConsole;

  Win64::StartupInfo info{};

  info.cb=sizeof info;

  Win64::ProcessInfo pinfo;

  if( Win64::CreateProcessA(program,arg,0,0,false,flags,0,dir,&info,&pinfo) )
    {
     Win64::CloseHandle(pinfo.h_process);
     Win64::CloseHandle(pinfo.h_thread);

     return FileError_Ok;
    }
  else
    {
     return MakeError(FileError_OpFault);
    }
 }

} // namespace Private_SysFileSystem

using namespace Private_SysFileSystem;

/* struct FileSystem::DirCursor */

void FileSystem::DirCursor::init(FileSystem *,StrLen dir_name) noexcept
 {
  is_closed=true;

  FileName path;

  if( path.set(dir_name,StrLen("/*")) )
    {
     Win64::FindFileData data;

     handle=Win64::FindFirstFileA(path,&data);

     if( handle==Win64::InvalidFileHandle )
       {
        Win64::error_t error_=Win64::GetLastError();

        if( error_!=Win64::ErrorNoMoreFiles )
          {
           error=MakeError(FileError_OpFault,error_);
          }
        else
          {
           error=FileError_Ok;
          }
       }
     else
       {
        is_first=true;
        is_closed=false;

        StrLen str(data.file_name);

        str.copyTo(file_name);

        len=str.len;

        type=(data.attr&Win64::FileAttributes_Directory)?FileType_dir:FileType_file;

        error=FileError_Ok;
       }
    }
  else
    {
     error=FileError_TooLongPath;
    }
 }

void FileSystem::DirCursor::exit() noexcept
 {
  if( !is_closed )
    {
     Win64::FindClose(handle);
    }
 }

bool FileSystem::DirCursor::next() noexcept
 {
  if( is_closed ) return false;

  if( is_first )
    {
     is_first=false;

     return true;
    }

  Win64::FindFileData data;

  if( FindNextFileA(handle,&data) )
    {
     StrLen str(data.file_name);

     str.copyTo(file_name);

     len=str.len;

     type=(data.attr&Win64::FileAttributes_Directory)?FileType_dir:FileType_file;

     return true;
    }

  Win64::error_t error_=Win64::GetLastError();

  if( error_!=Win64::ErrorNoMoreFiles )
    {
     error=MakeError(FileError_OpFault,error_);
    }

  is_closed=true;

  Win64::FindClose(handle);

  return false;
 }

/* struct FileSystem */

FileError FileSystem::init() noexcept
 {
  return FileError_Ok;
 }

FileError FileSystem::exit() noexcept
 {
  return FileError_Ok;
 }

auto FileSystem::getFileType(StrLen path_) noexcept -> TypeResult
 {
  TypeResult ret;
  FileName path;

  if( path.set(path_) )
    {
     Win64::flags_t attr=Win64::GetFileAttributesA(path);

     if( attr==Win64::InvalidFileAttributes )
       {
        ret.type=FileType_none;
        ret.error=FileError_Ok;
       }
     else
       {
        ret.type=(attr&Win64::FileAttributes_Directory)?FileType_dir:FileType_file;
        ret.error=FileError_Ok;
       }
    }
  else
    {
     ret.type=FileType_none;
     ret.error=FileError_TooLongPath;
    }

  return ret;
 }

auto FileSystem::getFileUpdateTime(StrLen path_) noexcept -> CmpTimeResult
 {
  CmpTimeResult ret;
  FileName path;

  if( path.set(path_) )
    {
     Win64::handle_t h_file=Win64::CreateFileA(path,Win64::AccessRead,Win64::ShareRead,0,Win64::OpenExisting,Win64::FileBackupSemantic,0);

     if( h_file==Win64::InvalidFileHandle )
       {
        ret.time=0;
        ret.error=FileError_Ok;
       }
     else
       {
        Win64::FileTime result;

        if( Win64::GetFileTime(h_file,0,0,&result) )
          {
           ret.time=ToCmpFileTime(result);
           ret.error=FileError_Ok;
          }
        else
          {
           ret.time=0;
           ret.error=MakeError(FileError_OpFault);
          }

        Win64::CloseHandle(h_file);
       }
    }
  else
    {
     ret.time=0;
     ret.error=FileError_TooLongPath;
    }

  return ret;
 }

FileError FileSystem::createFile(StrLen file_name) noexcept
 {
  FileName path;

  if( !path.set(file_name) ) return FileError_TooLongPath;

  Win64::flags_t access_flags = 0 ;

  Win64::flags_t share_flags = 0 ;

  Win64::options_t creation_options = Win64::CreateNew ;

  Win64::flags_t file_flags = Win64::FileAttributeNormal ;

  Win64::handle_t h_file = Win64::CreateFileA(path,access_flags,share_flags,0,creation_options,file_flags,0) ;

  if( h_file==Win64::InvalidFileHandle ) return MakeError(FileError_OpenFault);

  Win64::CloseHandle(h_file);

  return FileError_Ok;
 }

FileError FileSystem::deleteFile(StrLen file_name) noexcept
 {
  FileName path;

  if( !path.set(file_name) ) return FileError_TooLongPath;

  return MakeErrorIf(FileError_OpFault, !Win64::DeleteFileA(path) );
 }

FileError FileSystem::createDir(StrLen dir_name) noexcept
 {
  FileName path;

  if( !path.set(dir_name) ) return FileError_TooLongPath;

  return MakeErrorIf(FileError_OpFault, !Win64::CreateDirectoryA(path,0) );
 }

FileError FileSystem::deleteDir(StrLen dir_name,bool recursive) noexcept
 {
  FileName path;

  if( !path.set(dir_name) ) return FileError_TooLongPath;

  if( recursive ) return DeleteDirRecursive(StrLen(path,dir_name.len));

  return MakeErrorIf(FileError_OpFault, !Win64::RemoveDirectoryA(path) );
 }

FileError FileSystem::rename(StrLen old_path_,StrLen new_path_,bool allow_overwrite) noexcept
 {
  FileName old_path;

  if( !old_path.set(old_path_) ) return FileError_TooLongPath;

  FileName new_path;

  if( !new_path.set(new_path_) ) return FileError_TooLongPath;

  Win64::flags_t flags=allow_overwrite?Win64::MoveFileEx_AllowOverwrite:0;

  return MakeErrorIf(FileError_OpFault, !Win64::MoveFileExA(old_path,new_path,flags) );
 }

FileError FileSystem::remove(StrLen path_) noexcept
 {
  FileName path;

  if( !path.set(path_) ) return FileError_TooLongPath;

  Win64::flags_t attr=Win64::GetFileAttributesA(path);

  if( attr==Win64::InvalidFileAttributes ) return MakeError(FileError_NoPath);

  if( attr&Win64::FileAttributes_Directory )
    {
     return MakeErrorIf(FileError_OpFault, !Win64::RemoveDirectoryA(path) );
    }
  else
    {
     return MakeErrorIf(FileError_OpFault, !Win64::DeleteFileA(path) );
    }
 }

FileError FileSystem::exec(StrLen dir,StrLen program,StrLen arg) noexcept
 {
  SilentReportException report;

  try
    {
     if( dir.len>MaxPathLen || program.len>MaxPathLen ) return FileError_TooLongPath;

     if( arg.len>32_KByte ) return FileError_SysOverload;

     SimpleArray<char> buf(dir.len+program.len+arg.len+3);

     char *dirz;
     char *programz;
     char *argz;

     char *out=buf.getPtr();

     {
      dir.copyTo(dirz=out);

      out[dir.len]=0;

      out+=dir.len+1;
     }

     {
      program.copyTo(programz=out);

      out[program.len]=0;

      out+=program.len+1;
     }

     {
      arg.copyTo(argz=out);

      out[arg.len]=0;

      out+=arg.len+1;
     }

     return Execz(dirz,programz,argz);
    }
  catch(CatchType)
    {
     return FileError_SysOverload;
    }
 }

auto FileSystem::pathOf(StrLen path_,char buf[MaxPathLen+1]) noexcept -> PathOfResult
 {
  PathOfResult ret;

  FileName path;

  if( !path.set(path_) )
    {
     ret.path=StrLen();
     ret.error=FileError_TooLongPath;

     return ret;
    }

  ulen len=Win64::GetFullPathNameA(path,MaxPathLen+1,buf,0);

  if( len )
    {
     if( len>MaxPathLen )
       {
        ret.path=StrLen();
        ret.error=FileError_TooLongPath;
       }
     else
       {
        PathBase::TurnSlash(Range(buf,len));

        ret.path=StrLen(buf,len);
        ret.error=FileError_Ok;
       }
    }
  else
    {
     ret.path=StrLen();
     ret.error=MakeError(FileError_OpFault);
    }

  return ret;
 }

} // namespace Sys
} // namespace CCore


