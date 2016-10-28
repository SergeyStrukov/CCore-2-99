/* MakeFileName.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 3.00
//
//  Tag: Fundamental
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <CCore/inc/MakeFileName.h>

#include <CCore/inc/Path.h>
#include <CCore/inc/Exception.h>

namespace CCore {

/* class MakeFileName */

void MakeFileName::make(StrLen dir_name,StrLen file_name)
 {
  if( +dir_name && PathBase::IsSlash(dir_name.back(1)) ) dir_name.len--;

  add(dir_name,'/',file_name);

  if( !(*this) )
    {
     Printf(Exception,"CCore::MakeFileName::make(#.q;,#.q;) : too long path",dir_name,file_name);
    }
 }

} // namespace CCore

