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

PaneCut PaneCut::cutLeft(Coord dx,Coord space)
 {
  if( dx<=pane.dx )
    {
     PaneCut ret(SplitX(dx,pane),this->space);

     SplitX(space,pane);

     return ret;
    }
  else
    {
     return PaneCut(space);
    }
 }

PaneCut PaneCut::cutRight(Coord dx,Coord space)
 {
  if( dx<=pane.dx )
    {
     PaneCut ret(SplitX(pane,dx),this->space);

     SplitX(pane,space);

     return ret;
    }
  else
    {
     return PaneCut(space);
    }
 }

PaneCut PaneCut::cutTop(Coord dy,Coord space)
 {
  if( dy<=pane.dy )
    {
     PaneCut ret(SplitY(dy,pane),this->space);

     SplitY(space,pane);

     return ret;
    }
  else
    {
     return PaneCut(space);
    }
 }

PaneCut PaneCut::cutBottom(Coord dy,Coord space)
 {
  if( dy<=pane.dy )
    {
     PaneCut ret(SplitY(pane,dy),this->space);

     SplitY(pane,space);

     return ret;
    }
  else
    {
     return PaneCut(space);
    }
 }

} // namespace Video
} // namespace CCore


