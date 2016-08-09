/* Menu.h */
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

#ifndef CCore_inc_video_Menu_h
#define CCore_inc_video_Menu_h

#include <CCore/inc/video/SubWindow.h>
#include <CCore/inc/video/Font.h>
#include <CCore/inc/video/RefVal.h>

#include <CCore/inc/String.h>

namespace CCore {
namespace Video {

/* classes */

struct SimpleTopMenuShape;

template <class Shape> class SimpleTopMenuOf;

/* struct SimpleTopMenuShape */



/* class SimpleTopMenuOf<Shape> */



/* type SimpleTopMenu */

using SimpleTopMenu = SimpleTopMenuOf<SimpleTopMenuShape> ;

} // namespace Video
} // namespace CCore

#endif

