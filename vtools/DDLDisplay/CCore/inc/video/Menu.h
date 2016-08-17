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

namespace CCore {
namespace Video {

/* consts */

enum MenuType
 {
  MenuText,
  MenuDisabled,
  MenuHidden,
  MenuSeparator
 };

/* classes */

struct MenuPoint;

struct MenuData;

struct SimpleTopMenuShape;

template <class Shape> class SimpleTopMenuOf;

/* struct MenuPoint */

struct MenuPoint
 {
  MenuType type;
  DefString text;
  int id;

  // constructors

  MenuPoint() noexcept : type(MenuHidden),text("<not defined>"),id(-1) {}

  MenuPoint(DefString text_,int id_) noexcept : type(MenuText),text(text_),id(id_) {}

  MenuPoint(MenuType type_,DefString text_,int id_) noexcept : type(type_),text(text_),id(id_) {}

  MenuPoint(MenuType type_) noexcept : MenuPoint() { type=type_; }

  ~MenuPoint() {}
 };

/* struct MenuData */

struct MenuData : NoCopy
 {
  DynArray<MenuPoint> list;

  Signal<> update;

  // constructors

  MenuData() {}

  template <class ... TT> requires ( ... && IsType<TT,MenuPoint> )
  explicit MenuData(TT ... tt) : list{tt...} {}

  template <class ... SS>
  MenuData & operator () (SS && ... ss)
   {
    list.append_fill( std::forward<SS>(ss)... );

    return *this;
   }
 };

/* struct SimpleTopMenuShape */



/* class SimpleTopMenuOf<Shape> */



/* type SimpleTopMenu */

using SimpleTopMenu = SimpleTopMenuOf<SimpleTopMenuShape> ;

} // namespace Video
} // namespace CCore

#endif

