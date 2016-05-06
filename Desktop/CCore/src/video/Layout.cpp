/* Layout.cpp */
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

#include <CCore/inc/video/Layout.h>

namespace CCore {
namespace Video {

/* GetWindowPlace() */

Pane GetWindowPlace(Pane outer,Ratio ypos_ratio,Point size)
 {
  Point s=outer.getSize();

  size=Inf(size,s-s/4);

  Point p=s-size;

  Point pos( p.x/2 , ypos_ratio*p.y );

  return Pane(outer.getBase()+pos,size);
 }

Pane GetWindowPlace(Desktop *desktop,Ratio ypos_ratio,Point size)
 {
  return GetWindowPlace(desktop->getMaxPane(),ypos_ratio,size);
 }

/* Center() */

Pane Center(Pane outer,Point size)
 {
  Point p=outer.getSize()-size;

  Point pos=p/2;

  return Pane(outer.getBase()+pos,size);
 }

Pane CenterX(Pane outer,Coord dx)
 {
  return Pane(IntAdd(outer.x,IntSub(outer.dx,dx)/2),outer.y,dx,outer.dy);
 }

Pane CenterY(Pane outer,Coord dy)
 {
  return Pane(outer.x,IntAdd(outer.y,IntSub(outer.dy,dy)/2),outer.dx,dy);
 }

/* class PlaceRow */

PlaceRow::PlaceRow(Pane outer,Point size_,Coord space,ulen count_)
 : size(size_),
   delta_x(IntAdd(size_.x,space))
 {
  auto count=ToCoordinate(count_);

  auto total=count*size_.x+(count-1)*space;

  base=Center(outer,{+total,size_.y}).getBase();
 }

/* class PlaceColumn */

PlaceColumn::PlaceColumn(Pane outer,Point size_,Coord space,ulen count_)
 : size(size_),
   delta_y(IntAdd(size_.y,space))
 {
  auto count=ToCoordinate(count_);

  auto total=count*size_.y+(count-1)*space;

  base=Center(outer,{size_.x,+total}).getBase();
 }

/* struct MinSizeType */

MinSizeType MinSize;

} // namespace Video
} // namespace CCore

