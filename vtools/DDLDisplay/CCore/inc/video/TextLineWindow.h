/* TextLineWindow.h */
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

#ifndef CCore_inc_video_TextLineWindow_h
#define CCore_inc_video_TextLineWindow_h

#include <CCore/inc/video/WindowLib.h>

namespace CCore {
namespace Video {

/* classes */

class TextLineShape;

template <class Shape> class TextLineWindowOf;

/* class TextLineShape */

class TextLineShape
 {
 };

/* class TextLineWindowOf<Shape> */

template <class Shape>
class TextLineWindowOf
 {
 };

/* type TextLineWindow */

using TextLineWindow = TextLineWindowOf<TextLineShape> ;

} // namespace Video
} // namespace CCore

#endif

