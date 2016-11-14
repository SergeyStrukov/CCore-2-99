/* PaneCut.cpp */
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

#include <CCore/inc/video/PaneCut.h>

namespace CCore {
namespace Video {

/* functions */

Pane AlignTop(Pane pane,Coord dy)
 {
  if( dy<=pane.dy )
    {
     return Pane(pane.x,pane.y,pane.dx,dy);
    }
  else
    {
     return Empty;
    }
 }

Pane AlignCenterY(Pane pane,Coord dy)
 {
  if( dy<=pane.dy )
    {
     Coord off=(pane.dy-dy)/2;

     return Pane(pane.x,pane.y+off,pane.dx,dy);
    }
  else
    {
     return Empty;
    }
 }

Pane AlignBottom(Pane pane,Coord dy)
 {
  if( dy<=pane.dy )
    {
     Coord off=pane.dy-dy;

     return Pane(pane.x,pane.y+off,pane.dx,dy);
    }
  else
    {
     return Empty;
    }
 }

Pane AlignLeft(Pane pane,Coord dx)
 {
  if( dx<=pane.dx )
    {
     return Pane(pane.x,pane.y,dx,pane.dy);
    }
  else
    {
     return Empty;
    }
 }

Pane AlignCenterX(Pane pane,Coord dx)
 {
  if( dx<=pane.dx )
    {
     Coord off=(pane.dx-dx)/2;

     return Pane(pane.x+off,pane.y,dx,pane.dy);
    }
  else
    {
     return Empty;
    }
 }

Pane AlignRight(Pane pane,Coord dx)
 {
  if( dx<=pane.dx )
    {
     Coord off=pane.dx-dx;

     return Pane(pane.x+off,pane.y,dx,pane.dy);
    }
  else
    {
     return Empty;
    }
 }

Pane AlignCenter(Pane pane,Coord dx,Coord dy)
 {
  if( dx<=pane.dx && dy<=pane.dy )
    {
     Coord off_x=(pane.dx-dx)/2;
     Coord off_y=(pane.dy-dy)/2;

     return Pane(pane.x+off_x,pane.y+off_y,dx,dy);
    }
  else
    {
     return Empty;
    }
 }

/* class PaneCut */

 // methods

void PaneCut::shrink()
 {
  pane=pane.shrink(space);
 }

 // cut

PaneCut PaneCut::cutLeft(Coord dx)
 {
  if( dx<=pane.dx )
    {
     PaneCut ret(SplitX(dx,pane),space);

     SplitX(space,pane);

     return ret;
    }
  else
    {
     return PaneCut(space);
    }
 }

PaneCut PaneCut::cutRight(Coord dx)
 {
  if( dx<=pane.dx )
    {
     PaneCut ret(SplitX(pane,dx),space);

     SplitX(pane,space);

     return ret;
    }
  else
    {
     return PaneCut(space);
    }
 }

PaneCut PaneCut::cutTop(Coord dy)
 {
  if( dy<=pane.dy )
    {
     PaneCut ret(SplitY(dy,pane),space);

     SplitY(space,pane);

     return ret;
    }
  else
    {
     return PaneCut(space);
    }
 }

PaneCut PaneCut::cutBottom(Coord dy)
 {
  if( dy<=pane.dy )
    {
     PaneCut ret(SplitY(pane,dy),space);

     SplitY(pane,space);

     return ret;
    }
  else
    {
     return PaneCut(space);
    }
 }

 // box

Pane PaneCut::boxLeft(Coord dxy,Coord mini_space)
 {
  if( dxy<=pane.dx && dxy<=pane.dy )
    {
     Pane ret=AlignCenterY(SplitX(dxy,pane),dxy);

     SplitX(mini_space,pane);

     return ret;
    }
  else
    {
     return Empty;
    }
 }

Pane PaneCut::boxRight(Coord dxy,Coord mini_space)
 {
  if( dxy<=pane.dx && dxy<=pane.dy )
    {
     Pane ret=AlignCenterY(SplitX(pane,dxy),dxy);

     SplitX(pane,mini_space);

     return ret;
    }
  else
    {
     return Empty;
    }
 }

} // namespace Video
} // namespace CCore


