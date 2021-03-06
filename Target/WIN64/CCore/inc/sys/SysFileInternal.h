/* SysFileInternal.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 3.00
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

#include <CCore/inc/GenFile.h>
#include <CCore/inc/MakeString.h>

//#include <CCore/inc/Print.h>

#include <CCore/inc/win64/Win64.h>

namespace CCore {
namespace Sys {

/* functions */

inline FileError MakeError(FileError fe,Win64::error_t error)
 {
  switch( error )
    {
     case Win64::ErrorFileNotFound     : return FileError_NoFile;
     case Win64::ErrorPathNotFound     : return FileError_NoPath;
     case Win64::ErrorTooManyOpenFiles : return FileError_SysOverload;
     case Win64::ErrorAccessDenied     : return FileError_NoAccess;
     case Win64::ErrorNotEnoughMemory  : return FileError_SysOverload;
     case Win64::ErrorOutOfMemory      : return FileError_SysOverload;
     case Win64::ErrorInvalidDrive     : return FileError_NoDevice;
     case Win64::ErrorWriteProtect     : return FileError_WriteProtect;
     case Win64::ErrorSeek             : return FileError_PosFault;
     case Win64::ErrorWriteFault       : return FileError_WriteFault;
     case Win64::ErrorReadFault        : return FileError_ReadFault;
     case Win64::ErrorHandleEOF        : return FileError_BadPosition;
     case Win64::ErrorFileExists       : return FileError_FileExist;
     case Win64::ErrorDiskFull         : return FileError_DiskFull;
     case Win64::ErrorInvalidHandle    : return FileError_BadId;
     case Win64::ErrorInvalidName      : return FileError_BadName;
     case Win64::ErrorAlreadyExists    : return FileError_FileExist;
     case Win64::ErrorDirNotEmpty      : return FileError_DirIsNotEmpty;

     default:
      {
       //Printf(Con,"error = #;\n",error);

       return fe;
      }
    }
 }

inline FileError MakeError(FileError fe)
 {
  return MakeError(fe,Win64::GetLastError());
 }

inline FileError MakeErrorIf(FileError fe,bool nok)
 {
  if( nok ) return MakeError(fe);

  return FileError_Ok;
 }

inline void AddErrorIf(FileMultiError &errout,FileError fe,bool nok)
 {
  if( nok ) errout.add(MakeError(fe));
 }

inline CmpFileTimeType ToCmpFileTime(Win64::FileTime ft)
 {
  CmpFileTimeType ret=ft.lo|(CmpFileTimeType(ft.hi)<<32);

  if( !ret ) ret=1;

  return ret;
 }

/* classes */

struct FileName;

/* struct FileName */

struct FileName
 {
  MakeString<MaxPathLen+1> buf;

  operator const char * () const { return buf.getZStr(); }

  template <class ... TT>
  bool set(TT ... tt)
   {
    return +buf.add(tt...,Null);
   }
 };

} // namespace Sys
} // namespace CCore

