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

#include <CCore/inc/video/MinSizeType.h>

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

#if 0

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

/* Box() */

inline Pane Box(Coord dxy,Pane pane)
 {
  return Pane(pane.x,pane.y+(pane.dy-dxy)/2,dxy,dxy);
 }

inline Pane Box(Pane pane,Coord dxy)
 {
  return Pane(pane.x+(pane.dx-dxy),pane.y+(pane.dy-dxy)/2,dxy,dxy);
 }

/* SplitBox() */

inline Pane SplitBox(Coord dxy,Pane &pane,Coord space)
 {
  return Box(dxy,SplitX(IntAdd(dxy,space),pane));
 }

inline Pane SplitBox(Pane &pane,Coord dxy,Coord space)
 {
  return Box(SplitX(pane,IntAdd(dxy,space)),dxy);
 }

inline Pane SplitBox(Coord dxy,Pane &pane)
 {
  return SplitBox(dxy,pane,dxy/5);
 }

inline Pane SplitBox(Pane &pane,Coord dxy)
 {
  return SplitBox(pane,dxy,dxy/5);
 }

/* classes */

class CenterFunc;

class CenterXFunc;

class CenterYFunc;

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

/* struct UseMinSizeType */

struct UseMinSizeType {};

extern UseMinSizeType MinSize;

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
   void placeMinX(W &window,UseMinSizeType,Coordinate dy)
    {
     placeX(window,window.getMinSize().x,dy);
    }

   template <class W>
   void placeMinX(W &window,Coordinate dx,UseMinSizeType)
    {
     placeX(window,dx,window.getMinSize().y);
    }

   template <class W>
   void placeMinX(W &window)
    {
     placeX(window,window.getMinSize());
    }

   template <class W>
   void placeMinY(W &window,UseMinSizeType,Coordinate dy)
    {
     placeY(window,window.getMinSize().x,dy);
    }

   template <class W>
   void placeMinY(W &window,Coordinate dx,UseMinSizeType)
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

   Panesor(Panesor psor,Ratio rat) : Panesor(psor.x,psor.y,psor.dx,psor.dy,rat*(+psor.space)) {}

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

   // placeRow

   template <class ... WW>
   void placeRow(Point size,WW & ... ww) const
    {
     PlaceRow row(*this,size,+space,sizeof ... (WW));

     row.place(ww...);
    }

   template <class ... WW>
   Point placeRow(WW & ... ww) const
    {
     Point size=SupMinSize(ww...);

     placeRow(size,ww...);

     return size;
    }

   // placeColumn

   template <class ... WW>
   void placeColumn(Point size,WW & ... ww) const
    {
     PlaceColumn col(*this,size,+space,sizeof ... (WW));

     col.place(ww...);
    }

   template <class ... WW>
   Point placeColumn(WW & ... ww) const
    {
     Point size=SupMinSize(ww...);

     placeColumn(size,ww...);

     return size;
    }
 };

#endif

//----------------------------------------------------------------------------------------

/* concept PlaceType<W> */

template <class W> // ref extended
concept bool PlaceType = requires(W &obj,Meta::ToConst<W> &cobj,Pane pane)
 {
  cobj.getMinSize();

  obj.setPlace(pane);
 } ;

/* concept PlaceTypeOf<W,T> */

template <class W,class T> // W ref extended
concept bool PlaceTypeOf = PlaceType<W> && requires(Meta::ToConst<W> &cobj)
 {
  { cobj.getMinSize() } -> T ;
 } ;

/* Align...() */

Pane AlignLeft(Pane pane,Coord dx);

Pane AlignCenterX(Pane pane,Coord dx);

Pane AlignRight(Pane pane,Coord dx);

Pane AlignTop(Pane pane,Coord dy);

Pane AlignCenterY(Pane pane,Coord dy);

Pane AlignBottom(Pane pane,Coord dy);

Pane AlignCenter(Pane pane,Coord dx,Coord dy);

inline Pane AlignCenter(Pane pane,Point size) { return AlignCenter(pane,size.x,size.y); }

/* Box...() */

inline Coord BoxSpace(Coord dxy) { return dxy/5; }

inline Coord BoxExt(Coord dxy) { return IntAdd(dxy,BoxSpace(dxy)); }

/* GetMinSize() */

Point GetMinSize(const PlaceType &window) { return ToSizePoint(window.getMinSize()); }

/* SupMinSize() */

template <class ... WW>
Point SupMinSize(const WW & ... ww) requires ( ... && PlaceType<WW> )
 {
  return Sup( GetMinSize(ww)... );
 }

/* classes */

template <PlaceTypeOf<Point> W,Pane Func(Pane pane,Coord dx)> struct AlignXProxy;

template <PlaceTypeOf<Point> W,Pane Func(Pane pane,Coord dy)> struct AlignYProxy;

template <PlaceTypeOf<SizeBox> W> struct CutBoxProxy;

template <PlaceType W> struct CutPointProxy;

class PlaceRow;

class PlaceColumn;

class PaneCut;

/* struct AlignXProxy<W,Pane Func(Pane pane,Coord dx)> */

template <PlaceTypeOf<Point> W,Pane Func(Pane pane,Coord dx)>
struct AlignXProxy
 {
  W &window;
  Point size;

  explicit AlignXProxy(W &window_) : window(window_),size(window_.getMinSize()) {}

  SizeY getMinSize() const { return size.y; }

  void setPlace(Pane pane) { window.setPlace(Func(pane,size.x)); }
 };

/* AlignX() */

template <Pane Func(Pane pane,Coord dx),PlaceTypeOf<Point> W>
auto AlignX(W &window) { return AlignXProxy<W,Func>(window); }

template <PlaceTypeOf<Point> W>
auto AlignLeft(W &window) { return AlignXProxy<W,AlignLeft>(window); }

template <PlaceTypeOf<Point> W>
auto AlignCenterX(W &window) { return AlignXProxy<W,AlignCenterX>(window); }

template <PlaceTypeOf<Point> W>
auto AlignRight(W &window) { return AlignXProxy<W,AlignRight>(window); }

/* struct AlignYProxy<W,Pane Func(Pane pane,Coord dy)> */

template <PlaceTypeOf<Point> W,Pane Func(Pane pane,Coord dy)>
struct AlignYProxy
 {
  W &window;
  Point size;

  explicit AlignYProxy(W &window_) : window(window_),size(window_.getMinSize()) {}

  SizeX getMinSize() const { return size.x; }

  void setPlace(Pane pane) { window.setPlace(Func(pane,size.y)); }
 };

/* AlignY() */

template <Pane Func(Pane pane,Coord dy),PlaceTypeOf<Point> W>
auto AlignY(W &window) { return AlignYProxy<W,Func>(window); }

template <PlaceTypeOf<Point> W>
auto AlignTop(W &window) { return AlignYProxy<W,AlignTop>(window); }

template <PlaceTypeOf<Point> W>
auto AlignCenterY(W &window) { return AlignYProxy<W,AlignCenterY>(window); }

template <PlaceTypeOf<Point> W>
auto AlignBottom(W &window) { return AlignYProxy<W,AlignBottom>(window); }

/* struct CutBoxProxy<W> */

template <PlaceTypeOf<SizeBox> W>
struct CutBoxProxy
 {
  W &window;
  Coord dxy;

  explicit CutBoxProxy(W &window_) : window(window_),dxy(window_.getMinSize().dxy) {}

  SizeXSpace getMinSize() const { return SizeXSpace(dxy,BoxSpace(dxy)); }

  void setPlace(Pane pane) { window.setPlace(AlignCenterY(pane,dxy)); }
 };

/* CutBox() */

template <PlaceTypeOf<SizeBox> W>
auto CutBox(W &window) { return CutBoxProxy<W>(window); }

/* struct CutPointProxy<W> */

template <PlaceType W>
struct CutPointProxy
 {
  W &window;
  Point size;

  explicit CutPointProxy(W &window_) : window(window_),size(GetMinSize(window_)) {}

  Point getMinSize() const { return size; }

  void setPlace(Pane pane) { window.setPlace(pane); }
 };

/* CutPoint() */

template <PlaceType W>
auto CutPoint(W &window) { return CutPointProxy<W>(window); }

/* class PlaceRow */

class PlaceRow
 {
   Point base;
   Point size;
   Coord delta_x;

  private:

   struct Place
    {
     PlaceRow &obj;

     template <class W>
     Place operator + (W &window)
      {
       window.setPlace(*obj);

       ++obj;

       return *this;
      }
    };

  public:

   PlaceRow(Pane outer,Point size,Coord space,ulen count);

   Pane operator * () const { return Pane(base,size); }

   void operator ++ () { base.x+=delta_x; }

   template <class ... WW>
   void place(WW && ... ww) requires ( ... && PlaceType<WW> )
    {
     ( Place{*this} + ... + ww );
    }
 };

/* class PlaceColumn */

class PlaceColumn
 {
   Point base;
   Point size;
   Coord delta_y;

  private:

   struct Place
    {
     PlaceColumn &obj;

     template <class W>
     Place operator + (W &window)
      {
       window.setPlace(*obj);

       ++obj;

       return *this;
      }
    };

  public:

   PlaceColumn(Pane outer,Point size,Coord space,ulen count);

   Pane operator * () const { return Pane(base,size); }

   void operator ++ () { base.y+=delta_y; }

   template <class ... WW>
   void place(WW && ... ww) requires ( ... && PlaceType<WW> )
    {
     ( Place{*this} + ... + ww );
    }
 };

/* class PaneCut */

class PaneCut
 {
   Pane pane;
   Coord space; // >= 0

  public:

   explicit PaneCut(Coord space_) : space(space_) {}

   PaneCut(Pane pane_,Coord space_) : pane(pane_),space(space_) {}

   PaneCut(Point size,Coord space_) : pane(Null,size),space(space_) {}

   // methods

   operator Pane() const { return pane; }

   void shrink();

   // cut

   PaneCut cutLeft(Coord dx,Coord space);

   PaneCut cutRight(Coord dx,Coord space);

   PaneCut cutTop(Coord dy,Coord space);

   PaneCut cutBottom(Coord dy,Coord space);


   PaneCut cutLeft(Coord dx) { return cutLeft(dx,space); }

   PaneCut cutRight(Coord dx) { return cutRight(dx,space); }

   PaneCut cutTop(Coord dy) { return cutTop(dy,space); }

   PaneCut cutBottom(Coord dy) { return cutBottom(dy,space); }


   PaneCut cutLeft(SizeXSpace size) { return cutLeft(size.dx,size.space); }

   PaneCut cutRight(SizeXSpace size) { return cutRight(size.dx,size.space); }

   PaneCut cutTop(SizeYSpace size) { return cutTop(size.dy,size.space); }

   PaneCut cutBottom(SizeYSpace size) { return cutBottom(size.dy,size.space); }


   PaneCut cutLeft(Ratio dx) { return cutLeft(dx*pane.dx); }

   PaneCut cutRight(Ratio dx) { return cutRight(dx*pane.dx); }

   PaneCut cutTop(Ratio dy) { return cutTop(dy*pane.dy); }

   PaneCut cutBottom(Ratio dy) { return cutBottom(dy*pane.dy); }

   // cut + align

   Pane cutLeftTop(Coord dx,Coord dy) { return AlignTop(cutLeft(dx),dy); }

   Pane cutLeftCenter(Coord dx,Coord dy) { return AlignCenterY(cutLeft(dx),dy); }

   Pane cutLeftBottom(Coord dx,Coord dy) { return AlignBottom(cutLeft(dx),dy); }

   Pane cutRightTop(Coord dx,Coord dy) { return AlignTop(cutRight(dx),dy); }

   Pane cutRightCenter(Coord dx,Coord dy) { return AlignCenterY(cutRight(dx),dy); }

   Pane cutRightBottom(Coord dx,Coord dy) { return AlignBottom(cutRight(dx),dy); }

   Pane cutTopLeft(Coord dx,Coord dy) { return AlignLeft(cutTop(dy),dx); }

   Pane cutTopCenter(Coord dx,Coord dy) { return AlignCenterX(cutTop(dy),dx); }

   Pane cutTopRight(Coord dx,Coord dy) { return AlignRight(cutTop(dy),dx); }

   Pane cutBottomLeft(Coord dx,Coord dy) { return AlignLeft(cutBottom(dy),dx); }

   Pane cutBottomCenter(Coord dx,Coord dy) { return AlignCenterX(cutBottom(dy),dx); }

   Pane cutBottomRight(Coord dx,Coord dy) { return AlignRight(cutBottom(dy),dx); }


   Pane cutLeftTop(Point size) { return cutLeftTop(size.x,size.y); }

   Pane cutLeftCenter(Point size) { return cutLeftCenter(size.x,size.y); }

   Pane cutLeftBottom(Point size) { return cutLeftBottom(size.x,size.y); }

   Pane cutRightTop(Point size) { return cutRightTop(size.x,size.y); }

   Pane cutRightCenter(Point size) { return cutRightCenter(size.x,size.y); }

   Pane cutRightBottom(Point size) { return cutRightBottom(size.x,size.y); }

   Pane cutTopLeft(Point size) { return cutTopLeft(size.x,size.y); }

   Pane cutTopCenter(Point size) { return cutTopCenter(size.x,size.y); }

   Pane cutTopRight(Point size) { return cutTopRight(size.x,size.y); }

   Pane cutBottomLeft(Point size) { return cutBottomLeft(size.x,size.y); }

   Pane cutBottomCenter(Point size) { return cutBottomCenter(size.x,size.y); }

   Pane cutBottomRight(Point size) { return cutBottomRight(size.x,size.y); }

   // place

   void place(PlaceType &&window) const { window.setPlace(pane); }

   void placeMin(PlaceType &&window) const { window.setPlace(Pane(pane.getBase(),GetMinSize(window))); }

   PaneCut & place_cutLeft(PlaceType &&window,OneOfTypes<Coord,Ratio> dx) { window.setPlace(cutLeft(dx)); return *this; }

   PaneCut & place_cutRight(PlaceType &&window,OneOfTypes<Coord,Ratio> dx) { window.setPlace(cutRight(dx)); return *this; }

   PaneCut & place_cutTop(PlaceType &&window,OneOfTypes<Coord,Ratio> dy) { window.setPlace(cutTop(dy)); return *this; }

   PaneCut & place_cutBottom(PlaceType &&window,OneOfTypes<Coord,Ratio> dy) { window.setPlace(cutBottom(dy)); return *this; }


   PaneCut & place_cutLeft(PlaceType &&window,Coord dx,Coord space) { window.setPlace(cutLeft(dx,space)); return *this; }

   PaneCut & place_cutRight(PlaceType &&window,Coord dx,Coord space) { window.setPlace(cutRight(dx,space)); return *this; }

   PaneCut & place_cutTop(PlaceType &&window,Coord dy,Coord space) { window.setPlace(cutTop(dy,space)); return *this; }

   PaneCut & place_cutBottom(PlaceType &&window,Coord dy,Coord space) { window.setPlace(cutBottom(dy,space)); return *this; }


   PaneCut & place_cutLeft(PlaceType &&window,SizeXSpace size) { window.setPlace(cutLeft(size)); return *this; }

   PaneCut & place_cutRight(PlaceType &&window,SizeXSpace size) { window.setPlace(cutRight(size)); return *this; }

   PaneCut & place_cutTop(PlaceType &&window,SizeYSpace size) { window.setPlace(cutTop(size)); return *this; }

   PaneCut & place_cutBottom(PlaceType &&window,SizeYSpace size) { window.setPlace(cutBottom(size)); return *this; }


   PaneCut & place_cutLeftTop(PlaceType &&window,Point size) { window.setPlace(cutLeftTop(size)); return *this; }

   PaneCut & place_cutLeftCenter(PlaceType &&window,Point size) { window.setPlace(cutLeftCenter(size)); return *this; }

   PaneCut & place_cutLeftBottom(PlaceType &&window,Point size) { window.setPlace(cutLeftBottom(size)); return *this; }

   PaneCut & place_cutRightTop(PlaceType &&window,Point size) { window.setPlace(cutRightTop(size)); return *this; }

   PaneCut & place_cutRightCenter(PlaceType &&window,Point size) { window.setPlace(cutRightCenter(size)); return *this; }

   PaneCut & place_cutRightBottom(PlaceType &&window,Point size) { window.setPlace(cutRightBottom(size)); return *this; }

   PaneCut & place_cutTopLeft(PlaceType &&window,Point size) { window.setPlace(cutTopLeft(size)); return *this; }

   PaneCut & place_cutTopCenter(PlaceType &&window,Point size) { window.setPlace(cutTopCenter(size)); return *this; }

   PaneCut & place_cutTopRight(PlaceType &&window,Point size) { window.setPlace(cutTopRight(size)); return *this; }

   PaneCut & place_cutBottomLeft(PlaceType &&window,Point size) { window.setPlace(cutBottomLeft(size)); return *this; }

   PaneCut & place_cutBottomCenter(PlaceType &&window,Point size) { window.setPlace(cutBottomCenter(size)); return *this; }

   PaneCut & place_cutBottomRight(PlaceType &&window,Point size) { window.setPlace(cutBottomRight(size)); return *this; }


   PaneCut & place_cutLeftTop(PlaceType &&window) { window.setPlace(cutLeftTop(GetMinSize(window))); return *this; }

   PaneCut & place_cutLeftCenter(PlaceType &&window) { window.setPlace(cutLeftCenter(GetMinSize(window))); return *this; }

   PaneCut & place_cutLeftBottom(PlaceType &&window) { window.setPlace(cutLeftBottom(GetMinSize(window))); return *this; }

   PaneCut & place_cutRightTop(PlaceType &&window) { window.setPlace(cutRightTop(GetMinSize(window))); return *this; }

   PaneCut & place_cutRightCenter(PlaceType &&window) { window.setPlace(cutRightCenter(GetMinSize(window))); return *this; }

   PaneCut & place_cutRightBottom(PlaceType &&window) { window.setPlace(cutRightBottom(GetMinSize(window))); return *this; }

   PaneCut & place_cutTopLeft(PlaceType &&window) { window.setPlace(cutTopLeft(GetMinSize(window))); return *this; }

   PaneCut & place_cutTopCenter(PlaceType &&window) { window.setPlace(cutTopCenter(GetMinSize(window))); return *this; }

   PaneCut & place_cutTopRight(PlaceType &&window) { window.setPlace(cutTopRight(GetMinSize(window))); return *this; }

   PaneCut & place_cutBottomLeft(PlaceType &&window) { window.setPlace(cutBottomLeft(GetMinSize(window))); return *this; }

   PaneCut & place_cutBottomCenter(PlaceType &&window) { window.setPlace(cutBottomCenter(GetMinSize(window))); return *this; }

   PaneCut & place_cutBottomRight(PlaceType &&window) { window.setPlace(cutBottomRight(GetMinSize(window))); return *this; }

   // place Point

   PaneCut & place_cutLeft(PlaceTypeOf<Point> &&window) { place_cutLeft(window,window.getMinSize().x); return *this; }

   PaneCut & place_cutRight(PlaceTypeOf<Point> &&window) { place_cutRight(window,window.getMinSize().x); return *this; }

   PaneCut & place_cutTop(PlaceTypeOf<Point> &&window) { place_cutTop(window,window.getMinSize().y); return *this; }

   PaneCut & place_cutBottom(PlaceTypeOf<Point> &&window) { place_cutBottom(window,window.getMinSize().y); return *this; }

   // place X

   PaneCut & place_cutLeft(PlaceTypeOf<SizeX> &&window) { place_cutLeft(window,window.getMinSize().dx); return *this; }

   PaneCut & place_cutRight(PlaceTypeOf<SizeX> &&window) { place_cutRight(window,window.getMinSize().dx); return *this; }

   // place XSpace

   PaneCut & place_cutLeft(PlaceTypeOf<SizeXSpace> &&window) { place_cutLeft(window,window.getMinSize()); return *this; }

   PaneCut & place_cutRight(PlaceTypeOf<SizeXSpace> &&window) { place_cutRight(window,window.getMinSize()); return *this; }

   // place Y

   PaneCut & place_cutTop(PlaceTypeOf<SizeY> &&window) { place_cutTop(window,window.getMinSize().dy); return *this; }

   PaneCut & place_cutBottom(PlaceTypeOf<SizeY> &&window) { place_cutBottom(window,window.getMinSize().dy); return *this; }

   // place YSpace

   PaneCut & place_cutTop(PlaceTypeOf<SizeYSpace> &&window) { place_cutTop(window,window.getMinSize()); return *this; }

   PaneCut & place_cutBottom(PlaceTypeOf<SizeYSpace> &&window) { place_cutBottom(window,window.getMinSize()); return *this; }

   // place Box

   PaneCut & place_cutLeft(PlaceTypeOf<SizeBox> &&window) { place_cutLeft(CutBox(window)); return *this; }

   PaneCut & place_cutRight(PlaceTypeOf<SizeBox> &&window) { place_cutRight(CutBox(window)); return *this; }

   // placeRow

   template <class ... WW>
   PaneCut & placeRow_cutTop(WW && ... ww) requires ( ... && PlaceTypeOf<WW,Point> )
    {
     Point size=SupMinSize(ww...);

     PlaceRow row(cutTop(size.y),size,space,sizeof ... (WW));

     row.place(ww...);

     return *this;
    }

   template <class ... WW>
   PaneCut & placeRow_cutBottom(WW && ... ww) requires ( ... && PlaceTypeOf<WW,Point> )
    {
     Point size=SupMinSize(ww...);

     PlaceRow row(cutBottom(size.y),size,space,sizeof ... (WW));

     row.place(ww...);

     return *this;
    }

   // placeColumn

   template <class ... WW>
   PaneCut & placeColumn_cutLeft(WW && ... ww) requires ( ... && PlaceTypeOf<WW,Point> )
    {
     Point size=SupMinSize(ww...);

     PlaceColumn col(cutLeft(size.x),size,space,sizeof ... (WW));

     col.place(ww...);

     return *this;
    }

   template <class ... WW>
   PaneCut & placeColumn_cutRight(WW && ... ww) requires ( ... && PlaceTypeOf<WW,Point> )
    {
     Point size=SupMinSize(ww...);

     PlaceColumn col(cutRight(size.x),size,space,sizeof ... (WW));

     col.place(ww...);

     return *this;
    }
 };

} // namespace Video
} // namespace CCore

#endif

