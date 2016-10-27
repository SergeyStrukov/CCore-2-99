/* Layout.h */
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

#ifndef CCore_inc_video_Layout_h
#define CCore_inc_video_Layout_h

#include <CCore/inc/video/Desktop.h>

namespace CCore {
namespace Video {

/* ToCoordinate() */

Coordinate ToCoordinate(UIntType count) { IntGuard( count<=MaxCoord ); return Coord(count); }

/* FitToScreen() */

Pane FitToScreen(Point base,Point size,Point screen_size);

/* GetWindowPlace() */

Pane GetWindowPlace(Pane outer,Ratio ypos_ratio,Point size);

Pane GetWindowPlace(Desktop *desktop,Ratio ypos_ratio,Point size);

/* Center...() */

Pane Center(Pane outer,Point size);

inline Pane Center(Pane outer,Coord dxy) { return Center(outer,{dxy,dxy}); }

Pane CenterX(Pane outer,Coord dx);

Pane CenterY(Pane outer,Coord dy);

/* Envelope...() */

inline Pane EnvelopeX(Point base,Coord dy,Coord delta_x) { return Pane(IntSub(base.x,delta_x),base.y,IntAdd(delta_x,delta_x),dy); }

inline Pane EnvelopeY(Point base,Coord dx,Coord delta_y) { return Pane(base.x,IntSub(base.y,delta_y),dx,IntAdd(delta_y,delta_y)); }

/* SetMinPlace() */

template <class W>
void SetMinPlace(W &window,Point base)
 {
  Pane pane(base,window.getMinSize());

  window.setPlace(pane);
 }

/* SetExt...Place() */

template <class W>
void SetExtXPlace(W &window,Point base,Coord dx)
 {
  Pane pane(base,window.getMinSize().supX(dx));

  window.setPlace(pane);
 }

template <class W>
void SetExtYPlace(W &window,Point base,Coord dy)
 {
  Pane pane(base,window.getMinSize().supY(dy));

  window.setPlace(pane);
 }

template <class W>
void SetExtPlace(W &window,Point base,Coord dx,Coord dy)
 {
  Pane pane(base,window.getMinSize().sup(dx,dy));

  window.setPlace(pane);
 }

template <class W>
void SetExtPlace(W &window,Point base,Point s)
 {
  Pane pane(base,window.getMinSize().sup(s));

  window.setPlace(pane);
 }

/* SupMinSize() */

template <class ... WW>
Point SupMinSize(const WW & ... ww)
 {
  return Sup( ww.getMinSize()... );
 }

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
   Coordinate x;
   Coordinate y;
   Coordinate space;

  public:

   explicit Pointsor(Coordinate space_) : x(space_),y(space_),space(space_) {}

   Pointsor(Coordinate x_,Coordinate y_,Coordinate space_) : x(x_),y(y_),space(space_) {}

   Pointsor(Point point,Coordinate space_) : x(point.x),y(point.y),space(space_) {}

   // methods

   operator Point() const { return {+x,+y}; }

   Coord maxDX(Coordinate dx) const { return +(dx-x-space); }

   Coord maxDY(Coordinate dy) const { return +(dy-y-space); }

   // cut

   Pointsor cutX(Coordinate delta)
    {
     Pointsor ret(x,y,space);

     x+=delta+space;

     return ret;
    }

   Pointsor cutY(Coordinate delta)
    {
     Pointsor ret(x,y,space);

     y+=delta+space;

     return ret;
    }

   // place

   template <class W>
   void placeX(W &window,Coordinate dx,Coordinate dy)
    {
     window.setPlace(Pane(cutX(dx),+dx,+dy));
    }

   template <class W>
   void placeX(W &window,Coordinate dxy)
    {
     placeX(window,dxy,dxy);
    }

   template <class W>
   void placeX(W &window,Point size)
    {
     placeX(window,size.x,size.y);
    }

   template <class W>
   void placeY(W &window,Coordinate dx,Coordinate dy)
    {
     window.setPlace(Pane(cutY(dy),+dx,+dy));
    }

   template <class W>
   void placeY(W &window,Coordinate dxy)
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
   void placeMinX(W &window,MinSizeType,Coordinate dy)
    {
     placeX(window,window.getMinSize().x,dy);
    }

   template <class W>
   void placeMinX(W &window,Coordinate dx,MinSizeType)
    {
     placeX(window,dx,window.getMinSize().y);
    }

   template <class W>
   void placeMinX(W &window)
    {
     placeX(window,window.getMinSize());
    }

   template <class W>
   void placeMinY(W &window,MinSizeType,Coordinate dy)
    {
     placeY(window,window.getMinSize().x,dy);
    }

   template <class W>
   void placeMinY(W &window,Coordinate dx,MinSizeType)
    {
     placeY(window,dx,window.getMinSize().y);
    }

   template <class W>
   void placeMinY(W &window)
    {
     placeY(window,window.getMinSize());
    }

   // place transform

   template <class W>
   void placeX(W &window,Coordinate dx,Coordinate dy,FuncType<Pane,Pane> func)
    {
     window.setPlace(func(Pane(cutX(dx),+dx,+dy)));
    }

   template <class W>
   auto placeX(W &window,Coordinate dxy,FuncType<Pane,Pane> func)
    {
     placeX(window,dxy,dxy,func);
    }

   template <class W>
   auto placeX(W &window,Point size,FuncType<Pane,Pane> func)
    {
     placeX(window,size.x,size.y,func);
    }

   template <class W>
   void placeY(W &window,Coordinate dx,Coordinate dy,FuncType<Pane,Pane> func)
    {
     window.setPlace(func(Pane(cutY(dy),+dx,+dy)));
    }

   template <class W>
   auto placeY(W &window,Coordinate dxy,FuncType<Pane,Pane> func)
    {
     placeY(window,dxy,dxy,func);
    }

   template <class W>
   auto placeY(W &window,Point size,FuncType<Pane,Pane> func)
    {
     placeY(window,size.x,size.y,func);
    }
 };

/* class Panesor */

class Panesor
 {
   Coordinate x;
   Coordinate y;
   Coordinate dx;
   Coordinate dy;
   Coordinate space;

  public:

   Panesor(Coordinate x_,Coordinate y_,Coordinate dx_,Coordinate dy_,Coordinate space_) : x(x_),y(y_),dx(dx_),dy(dy_),space(space_) {}

   Panesor(Point size,Coordinate space) : Panesor(0,0,size.x,size.y,space) {}

   Panesor(Pane pane,Coordinate space) : Panesor(pane.x,pane.y,pane.dx,pane.dy,space) {}

   // methods

   operator Pane() const { return Pane(+x,+y,+dx,+dy); }

   void shrink()
    {
     x+=space;
     y+=space;

     Coordinate space2=space+space;

     dx-=space2;
     dy-=space2;
    }

   // cut

   Panesor cutX(Coordinate delta)
    {
     Panesor ret(x,y,delta,dy,space);

     delta+=space;

     x+=delta;
     dx-=delta;

     return ret;
    }

   Panesor cutY(Coordinate delta)
    {
     Panesor ret(x,y,dx,delta,space);

     delta+=space;

     y+=delta;
     dy-=delta;

     return ret;
    }

   Panesor cutX_rest(Coordinate rest)
    {
     return cutX(dx-space-rest);
    }

   Panesor cutY_rest(Coordinate rest)
    {
     return cutY(dy-space-rest);
    }

   // place

   template <class W>
   void placeX(W &window,Coordinate delta)
    {
     window.setPlace(cutX(delta));
    }

   template <class W>
   void placeY(W &window,Coordinate delta)
    {
     window.setPlace(cutY(delta));
    }

   template <class W>
   void placeX_rest(W &window,Coordinate rest)
    {
     window.setPlace(cutX_rest(rest));
    }

   template <class W>
   void placeY_rest(W &window,Coordinate rest)
    {
     window.setPlace(cutY_rest(rest));
    }

   template <class W>
   void placeX(W &window,Ratio rat)
    {
     placeX(window,rat*(+dx));
    }

   template <class W>
   void placeY(W &window,Ratio rat)
    {
     placeY(window,rat*(+dy));
    }

   // place transform

   template <class W>
   void placeX(W &window,Coordinate delta,FuncType<Pane,Pane> func)
    {
     window.setPlace(func(cutX(delta)));
    }

   template <class W>
   void placeY(W &window,Coordinate delta,FuncType<Pane,Pane> func)
    {
     window.setPlace(func(cutY(delta)));
    }

   // ext

   template <class W>
   void extX(W &window,Coordinate ex=0)
    {
     placeX(window,ex+window.getMinSize().x);
    }

   template <class W>
   void extY(W &window,Coordinate ey=0)
    {
     placeY(window,ey+window.getMinSize().y);
    }
 };

} // namespace Video
} // namespace CCore

#endif

