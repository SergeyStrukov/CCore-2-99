/* ScrollList.cpp */
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

#include <inc/ScrollList.h>

namespace App {

/* class ScrollListShape */

Point ScrollListShape::getMinSize() const
 {
 }

void ScrollListShape::setMax()
 {
 }

void ScrollListShape::initSelect() // 0 -> down , MaxULen
 {
  select=MaxULen;

  setSelectDown(0);
 }

bool ScrollListShape::setSelectDown(ulen pos) // pos -> down , -> up , unchanged
 {
  ulen count=info->getLineCount();

  Replace_min(pos,count);

  for(ulen i=pos; i<count ;i++)
    {
     if( info->getLine(i).type==ComboInfoText )
       {
        return Change(select,i);
       }
    }

  for(ulen i=pos; i-- ;)
    {
     if( info->getLine(i).type==ComboInfoText )
       {
        return Change(select,i);
       }
    }

  return false;
 }

bool ScrollListShape::setSelectUp(ulen pos) // pos -> up , -> down , unchanged
 {
  ulen count=info->getLineCount();

  if( !count ) return false;

  Replace_min(pos,count-1);

  pos++;

  for(ulen i=pos; i-- ;)
    {
     if( info->getLine(i).type==ComboInfoText )
       {
        return Change(select,i);
       }
    }

  for(ulen i=pos; i<count ;i++)
    {
     if( info->getLine(i).type==ComboInfoText )
       {
        return Change(select,i);
       }
    }

  return false;
 }

bool ScrollListShape::showSelect()
 {
  if( select<yoff )
    {
     yoff=select;

     return true;
    }
  else
    {
     ulen i=select-yoff;

     if( i>=page && page>0 )
       {
        yoff=Min<ulen>(select-page+1,yoffMax);

        return true;
       }
    }

  return false;
 }

ulen ScrollListShape::getPosition(Point point) const
 {
 }

void ScrollListShape::draw(const DrawBuf &buf) const
 {
 }

} // namespace App


