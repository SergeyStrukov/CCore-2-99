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

namespace App {

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

/* class AspectData */

AspectData::AspectData() noexcept
 {
 }

AspectData::~AspectData()
 {
 }

 // save/load

void AspectData::blank(StrLen path) // TODO
 {
  Used(path);
 }

void AspectData::save(StrLen file_name,ErrorText &etext) const // TODO
 {
  Used(file_name);
  Used(etext);
 }

void AspectData::load(StrLen file_name,ErrorText &etext) // TODO
 {
  Used(file_name);
  Used(etext);
 }

} // namespace App

