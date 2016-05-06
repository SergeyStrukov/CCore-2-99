/* FontParam.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
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

#ifndef CCore_inc_video_FontParam_h
#define CCore_inc_video_FontParam_h

#include <CCore/inc/video/FreeTypeFont.h>

#include <CCore/inc/String.h>

namespace CCore {
namespace Video {

/* classes */

struct FontParam;

/* struct FontParam */

struct FontParam
 {
  enum EngineType
   {
    EngineDefault,
    EngineFreeType
   };

  friend const char * GetTextDesc(EngineType engine_type);

  EngineType engine_type = EngineDefault ;

  String file_name;

  enum SizeType
   {
    SizeXY,
    SizePoint,
    SizeIndex
   };

  friend const char * GetTextDesc(SizeType size_type);

  SizeType size_type = SizeXY ;

  union SetSize
   {
    Coord size_xy;
    Point size;
    ulen index;

    SetSize() noexcept { size_xy=0; }
   };

  SetSize set_size;

  FreeTypeFont::Config cfg;

  // constructors

  FontParam() noexcept {}

  // methods

  Font create() const;
 };

} // namespace Video
} // namespace CCore

#endif

