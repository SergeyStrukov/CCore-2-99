/* BinaryFile.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 3.00
//
//  Tag: HCore Mini
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2015 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <CCore/inc/BinaryFile.h>

#include <CCore/inc/Exception.h>

namespace CCore {

/* class BinaryFile */

void BinaryFile::provide()
 {
  if( !isOpened() )
    {
     Printf(Exception,"CCore::BinaryFile::provide(...) : file is not opened");
    }

  flush();

  out=Range(buf);
  has_data=true;
 }

BinaryFile::BinaryFile() noexcept
 {
 }

BinaryFile::BinaryFile(StrLen file_name,FileOpenFlags oflags)
 : BinaryFile()
 {
  open(file_name,oflags);
 }

BinaryFile::~BinaryFile()
 {
  if( isOpened() )
    {
     FileMultiError errout;

     soft_close(errout);

     if( +errout )
       {
        Printf(NoException,"CCore::BinaryFile::~BinaryFile() : #;",errout);
       }
    }
 }

void BinaryFile::open(StrLen file_name,FileOpenFlags oflags)
 {
  if( !buf.getLen() ) buf.extend_raw(BufLen);

  if( FileError error=file.open(file_name,oflags) )
    {
     Printf(Exception,"CCore::BinaryFile::open(#.q;,#;) : #;",file_name,oflags,error);
    }

  no_flush_exception=false;
  out=Empty;
  has_data=false;
 }

void BinaryFile::disableExceptions()
 {
  if( !no_flush_exception )
    {
     no_flush_exception=true;

     flush_error=FileError_Ok;
    }
 }

void BinaryFile::soft_close(FileMultiError &errout)
 {
  if( isOpened() )
    {
     disableExceptions();

     flush();

     errout.add(flush_error);

     file.close(errout);
    }
  else
    {
     errout.add(FileError_NoMethod);
    }
 }

void BinaryFile::close()
 {
  FileMultiError errout;

  soft_close(errout);

  if( +errout )
    {
     Printf(Exception,"CCore::BinaryFile::close() : #;",errout);
    }
 }

void BinaryFile::do_put(const uint8 *ptr,ulen len)
 {
  auto src=Range(ptr,len);

  while( +src )
    {
     if( !out ) provide();

     ulen delta=Min(src.len,out.len);

     (out+=delta).copy( (src+=delta).ptr );
    }
 }

void BinaryFile::flush()
 {
  if( !has_data ) return;

  ulen len=buf.getLen()-out.len;

  out=Empty;
  has_data=false;

  if( !len ) return;

  if( FileError error=file.write(buf.getPtr(),len) )
    {
     if( no_flush_exception )
       {
        flush_error=error;
       }
     else
       {
        Printf(Exception,"CCore::BinaryFile::flush(...) : #;",error);
       }
    }
 }

} // namespace CCore


