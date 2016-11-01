/* ScrollList.h */
//----------------------------------------------------------------------------------------
//
//  Project: DDLDisplay 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/Application.h>

#include <CCore/inc/RefObjectBase.h>
#include <CCore/inc/Sort.h>

#include <CCore/inc/video/WindowLib.h>

namespace App {

/* classes */

class ScrollListShape;

template <class Shape> class ScrollListWindowOf;

/* class ScrollListShape */

class ScrollListShape
 {
 };

/* class ScrollListWindowOf<Shape> */

template <class Shape>
class ScrollListWindowOf
 {
 };

/* type ScrollListWindow */

using ScrollListWindow = ScrollListWindowOf<ScrollListShape> ;

} // namespace App


