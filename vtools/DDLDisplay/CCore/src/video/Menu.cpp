/* Menu.cpp */
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

#include <CCore/inc/video/Menu.h>

namespace CCore {
namespace Video {

/* struct MenuPoint */

void MenuPoint::pickhot()
 {
  StrLen str=text.str();

  if( str.len )
    for(ulen i=0,lim=str.len-1; i<lim ;i++)
      {
       if( str[i]==Hot )
         {
          hotindex=i+1;
          hotkey=str[hotindex];

          return;
         }
      }

  hotindex=0;
  hotkey=0;
 }

/* struct MenuData */

auto MenuData::find(char ch) const -> FindResult
 {
  auto r=Range(list);

  for(ulen i=0; i<r.len ;i++) if( r[i].test(ch) ) return {i,true};

  return {0,false};
 }

auto MenuData::find(Point point) const -> FindResult
 {
  auto r=Range(list);

  for(ulen i=0; i<r.len ;i++) if( r[i].test(point) ) return {i,true};

  return {0,false};
 }

/* class SimpleTopMenuShape */

void SimpleTopMenuShape::layout()
 {
 }

Point SimpleTopMenuShape::getMinSize() const
 {
 }

void SimpleTopMenuShape::draw(const DrawBuf &buf) const
 {
 }

} // namespace Video
} // namespace CCore

