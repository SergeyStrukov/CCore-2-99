/* FileNameMatch.h */
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

#ifndef CCore_inc_video_FileNameMatch_h
#define CCore_inc_video_FileNameMatch_h

#include <CCore/inc/String.h>

namespace CCore {
namespace Video {

/* classes */

class FileNameFilter;

/* class FileNameFilter */

class FileNameFilter : NoCopy
 {
   String filter;
   bool empty = true ;

  private:

   static bool Match(StrLen filter,StrLen file);

  public:

   FileNameFilter() noexcept;

   explicit FileNameFilter(StrLen filter);

   ~FileNameFilter();

   // props

   bool operator + () const { return !empty; }

   bool operator ! () const { return empty; }

   // methods

   void reset();

   void reset(StrLen filter);

   bool operator () (StrLen file) const;
 };

} // namespace Video
} // namespace CCore

#endif

