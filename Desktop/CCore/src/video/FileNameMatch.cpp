/* FileNameMatch.cpp */
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

#include <CCore/inc/video/FileNameMatch.h>

namespace CCore {
namespace Video {

/* class FileNameFilter */

bool FileNameFilter::Match(StrLen filter,StrLen file)
 {
  for(; +filter ;++filter)
    {
     switch( char ch=*filter )
       {
        case '*' :
         {
          StrLen next=filter.part(1);

          for(;;++file)
            {
             if( Match(next,file) ) return true;

             if( !file ) return false;
            }
         }
        break;

        case '?' :
         {
          if( !file ) return false;

          ++file;
         }
        break;

        default:
         {
          if( !file || (*file)!=ch ) return false;

          ++file;
         }
       }
    }

  return !file;
 }

FileNameFilter::FileNameFilter() noexcept
 {
 }

FileNameFilter::FileNameFilter(StrLen filter_)
 : filter(filter_),
   empty(false)
 {
 }

FileNameFilter::~FileNameFilter()
 {
 }

void FileNameFilter::reset()
 {
  if( !empty )
    {
     filter=Empty;
     empty=true;
    }
 }

void FileNameFilter::reset(StrLen filter_)
 {
  filter=String(filter_);
  empty=false;
 }

bool FileNameFilter::operator () (StrLen file) const
 {
  if( empty ) return false;

  return Match(Range(filter),file);
 }

} // namespace Video
} // namespace CCore

