/* Layout.h */
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

#ifndef CCore_inc_video_Layout_h
#define CCore_inc_video_Layout_h

#include <CCore/inc/video/Desktop.h>

namespace CCore {
namespace Video {

/* ToCoord() */

inline Coordinate ToCoordinate(ulen count) { IntGuard( count<=MaxCoord ); return Coord(count); }

/* GetWindowPlace() */

Pane GetWindowPlace(Pane outer,Ratio ypos_ratio,Point size);

Pane GetWindowPlace(Desktop *desktop,Ratio ypos_ratio,Point size);

/* Center() */

Pane Center(Pane outer,Point size);

inline Pane Center(Pane outer,Coord dxy) { return Center(outer,{dxy,dxy}); }

Pane CenterX(Pane outer,Coord dx);

Pane CenterY(Pane outer,Coord dy);

/* Envelope...() */

inline Pane EnvelopeX(Point base,Coord dy,Coord delta_x) { return Pane(IntSub(base.x,delta_x),base.y,IntAdd(delta_x,delta_x),dy); }

inline Pane EnvelopeY(Point base,Coord dx,Coord delta_y) { return Pane(base.x,IntSub(base.y,delta_y),dx,IntAdd(delta_y,delta_y)); }

/* classes */

class CenterFunc;

class CenterXFunc;

class CenterYFunc;

class PlaceRow;

class PlaceColumn;

struct MinSizeType;

class Pointsor;

class Panesor;

/* class CenterFunc */

class CenterFunc
 {
   Point size;

  public:

   explicit CenterFunc(Coord dxy) : size(dxy,dxy) {}

   explicit CenterFunc(Point size_) : size(size_) {}

   CenterFunc(Coord dx,Coord dy) : size(dx,dy) {}

   Pane operator () (Pane pane) const { return Center(pane,size); }
 };

/* class CenterXFunc */

class CenterXFunc
 {
   Coord dx;

  public:

   explicit CenterXFunc(Coord dx_) : dx(dx_) {}

   Pane operator () (Pane pane) const { return CenterX(pane,dx); }
 };

/* class CenterYFunc */

class CenterYFunc
 {
   Coord dy;

  public:

   explicit CenterYFunc(Coord dy_) : dy(dy_) {}

   Pane operator () (Pane pane) const { return CenterY(pane,dy); }
 };

/* class PlaceRow */

class PlaceRow
 {
   Point base;
   Point size;
   Coord delta_x;

  public:

   PlaceRow(Pane outer,Point size,Coord space,ulen count);

   Pane operator * () const { return Pane(base,size); }

   void operator ++ () { base.x+=delta_x; }
 };

/* class PlaceColumn */

class PlaceColumn
 {
   Point base;
   Point size;
   Coord delta_y;

  public:

   PlaceColumn(Pane outer,Point size,Coord space,ulen count);

   Pane operator * () const { return Pane(base,size); }

   void operator ++ () { base.y+=delta_y; }
 };

/* struct MinSizeType */

struct MinSizeType {};

extern MinSizeType MinSize;

/* class Pointsor */

class Pointsor
 {
   Coord x;
   Coord y;
   Coord space;

  public:

   explicit Pointsor(Coord space_) : x(space_),y(space_),space(space_) {}

   Pointsor(Coord x_,Coord y_,Coord space_) : x(x_),y(y_),space(space_) {}

   Pointsor(Point point,Coord space_) : x(point.x),y(point.y),space(space_) {}

   // methods

   operator Point() const { return {x,y}; }

   Coord maxDX(Coord dx) const { return IntSub(IntSub(dx,x),space); }

   Coord maxDY(Coord dy) const { return IntSub(IntSub(dy,y),space); }

   // cut

   Pointsor cutX(Coord delta)
    {
     Pointsor ret(x,y,space);

     x=IntAdd(x,IntAdd(delta,space));

     return ret;
    }

   Pointsor cutY(Coord delta)
    {
     Pointsor ret(x,y,space);

     y=IntAdd(y,IntAdd(delta,space));

     return ret;
    }

   // place

   template <class W>
   void placeX(W &window,Coord dx,Coord dy)
    {
     window.setPlace(Pane(cutX(dx),dx,dy));
    }

   template <class W>
   void placeX(W &window,Coord dxy)
    {
     placeX(window,dxy,dxy);
    }

   template <class W>
   void placeX(W &window,Point size)
    {
     placeX(window,size.x,size.y);
    }

   template <class W>
   void placeY(W &window,Coord dx,Coord dy)
    {
     window.setPlace(Pane(cutY(dy),dx,dy));
    }

   template <class W>
   void placeY(W &window,Coord dxy)
    {
     placeY(window,dxy,dxy);
    }

   template <class W>
   void placeY(W &window,Point size)
    {
     placeY(window,size.x,size.y);
    }

   // place min size

   template <class W>
   void placeMinX(W &window,MinSizeType,Coord dy)
    {
     placeX(window,window.getMinSize().x,dy);
    }

   template <class W>
   void placeMinX(W &window,Coord dx,MinSizeType)
    {
     placeX(window,dx,window.getMinSize().y);
    }

   template <class W>
   void placeMinX(W &window)
    {
     placeX(window,window.getMinSize());
    }

   template <class W>
   void placeMinY(W &window,MinSizeType,Coord dy)
    {
     placeY(window,window.getMinSize().x,dy);
    }

   template <class W>
   void placeMinY(W &window,Coord dx,MinSizeType)
    {
     placeY(window,dx,window.getMinSize().y);
    }

   template <class W>
   void placeMinY(W &window)
    {
     placeY(window,window.getMinSize());
    }

   // place transform

   template <class W,class Func>
   void placeX(W &window,Coord dx,Coord dy,Func func)
    {
     window.setPlace(func(Pane(cutX(dx),dx,dy)));
    }

   template <class W,class Func>
   auto placeX(W &window,Coord dxy,Func func) -> decltype( (void)func(Pane()) )
    {
     placeX(window,dxy,dxy,func);
    }

   template <class W,class Func>
   auto placeX(W &window,Point size,Func func) -> decltype( (void)func(Pane()) )
    {
     placeX(window,size.x,size.y,func);
    }

   template <class W,class Func>
   void placeY(W &window,Coord dx,Coord dy,Func func)
    {
     window.setPlace(func(Pane(cutY(dy),dx,dy)));
    }

   template <class W,class Func>
   auto placeY(W &window,Coord dxy,Func func) -> decltype( (void)func(Pane()) )
    {
     placeY(window,dxy,dxy,func);
    }

   template <class W,class Func>
   auto placeY(W &window,Point size,Func func) -> decltype( (void)func(Pane()) )
    {
     placeY(window,size.x,size.y,func);
    }
 };

/* class Panesor */

class Panesor
 {
   Coord x;
   Coord y;
   Coord dx;
   Coord dy;
   Coord space;

  public:

   Panesor(Coord x_,Coord y_,Coord dx_,Coord dy_,Coord space_) : x(x_),y(y_),dx(dx_),dy(dy_),space(space_) {}

   Panesor(Point size,Coord space) : Panesor(0,0,size.x,size.y,space) {}

   Panesor(Pane pane,Coord space) : Panesor(pane.x,pane.y,pane.dx,pane.dy,space) {}

   // methods

   operator Pane() const { return Pane(x,y,dx,dy); }

   void shrink()
    {
     x=IntAdd(x,space);
     y=IntAdd(y,space);

     Coord space2=IntAdd(space,space);

     dx=IntSub(dx,space2);
     dy=IntSub(dy,space2);
    }

   // cut

   Panesor cutX(Coord delta)
    {
     Panesor ret(x,y,delta,dy,space);

     delta=IntAdd(delta,space);

     x=IntAdd(x,delta);
     dx=IntSub(dx,delta);

     return ret;
    }

   Panesor cutY(Coord delta)
    {
     Panesor ret(x,y,dx,delta,space);

     delta=IntAdd(delta,space);

     y=IntAdd(y,delta);
     dy=IntSub(dy,delta);

     return ret;
    }

   // place

   template <class W>
   void placeX(W &window,Coord delta)
    {
     window.setPlace(cutX(delta));
    }

   template <class W>
   void placeY(W &window,Coord delta)
    {
     window.setPlace(cutY(delta));
    }

   template <class W>
   void placeX(W &window,Ratio rat)
    {
     placeX(window,rat*dx);
    }

   template <class W>
   void placeY(W &window,Ratio rat)
    {
     placeY(window,rat*dy);
    }

   // place transform

   template <class W,class Func>
   void placeX(W &window,Coord delta,Func func)
    {
     window.setPlace(func(cutX(delta)));
    }

   template <class W,class Func>
   void placeY(W &window,Coord delta,Func func)
    {
     window.setPlace(func(cutY(delta)));
    }

   // ext

   template <class W>
   void extX(W &window,Coord ex=0)
    {
     Coord dx=IntAdd(window.getMinSize().x,ex);

     placeX(window,dx);
    }

   template <class W>
   void extY(W &window,Coord ey=0)
    {
     Coord dy=IntAdd(window.getMinSize().y,ey);

     placeY(window,dy);
    }
 };

} // namespace Video
} // namespace CCore

#endif

