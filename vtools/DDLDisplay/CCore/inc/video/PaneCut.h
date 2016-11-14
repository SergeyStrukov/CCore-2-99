/* PaneCut.h */
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

#ifndef CCore_inc_video_PaneCut_h
#define CCore_inc_video_PaneCut_h

#include <CCore/inc/video/Layout.h>

namespace CCore {
namespace Video {

/* concept PlaceType<W> */

template <class W>
concept bool PlaceType = requires(W &obj,Meta::ToConst<W> &cobj,Pane pane)
 {
  { cobj.getMinSize() } -> Point ;

  obj.setPlace(pane);
 } ;

/* functions */

Pane AlignTop(Pane pane,Coord dy);

Pane AlignCenterY(Pane pane,Coord dy);

Pane AlignBottom(Pane pane,Coord dy);

Pane AlignLeft(Pane pane,Coord dx);

Pane AlignCenterX(Pane pane,Coord dx);

Pane AlignRight(Pane pane,Coord dx);

Pane AlignCenter(Pane pane,Coord dx,Coord dy);

inline Pane AlignCenter(Pane pane,Point size) { return AlignCenter(pane,size.x,size.y); }

inline Coord BoxSize(Point size) { return Max(size.x,size.y); }

Coord BoxSize(const PlaceType &window) { return BoxSize(window.getMinSize()); }

inline Coord BoxSpace(Coord dxy) { return dxy/5; }

inline Coord BoxExt(Coord dxy) { return IntAdd(dxy,BoxSpace(dxy)); }

/* classes */

class PaneCut;

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

   PaneCut cutLeft(Coord dx);

   PaneCut cutRight(Coord dx);

   PaneCut cutTop(Coord dy);

   PaneCut cutBottom(Coord dy);


   PaneCut cutLeft(Ratio dx) { return cutLeft(dx*pane.dx); }

   PaneCut cutRight(Ratio dx) { return cutRight(dx*pane.dx); }

   PaneCut cutTop(Ratio dy) { return cutTop(dy*pane.dy); }

   PaneCut cutBottom(Ratio dy) { return cutBottom(dy*pane.dy); }

   // box

   Pane boxLeft(Coord dxy,Coord mini_space);

   Pane boxRight(Coord dxy,Coord mini_space);

   Pane boxLeft(Coord dxy) { return boxLeft(dxy,BoxSpace(dxy)); }

   Pane boxRight(Coord dxy) { return boxRight(dxy,BoxSpace(dxy)); }

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

   void place(PlaceType &window) const { window.setPlace(pane); }


   PaneCut & place_boxLeft(PlaceType &window,Coord dxy,Coord mini_space) { window.setPlace(boxLeft(dxy,mini_space)); return *this; }

   PaneCut & place_boxRight(PlaceType &window,Coord dxy,Coord mini_space) { window.setPlace(boxRight(dxy,mini_space)); return *this; }

   PaneCut & place_boxLeft(PlaceType &window,Coord dxy) { window.setPlace(boxLeft(dxy)); return *this; }

   PaneCut & place_boxRight(PlaceType &window,Coord dxy) { window.setPlace(boxRight(dxy)); return *this; }

   PaneCut & place_boxLeft(PlaceType &window) { place_boxLeft(window,BoxSize(window)); return *this; }

   PaneCut & place_boxRight(PlaceType &window) { place_boxRight(window,BoxSize(window)); return *this; }


   PaneCut & place_cutLeft(PlaceType &window,OneOfTypes<Coord,Ratio> dx) { window.setPlace(cutLeft(dx)); return *this; }

   PaneCut & place_cutRight(PlaceType &window,OneOfTypes<Coord,Ratio> dx) { window.setPlace(cutRight(dx)); return *this; }

   PaneCut & place_cutTop(PlaceType &window,OneOfTypes<Coord,Ratio> dy) { window.setPlace(cutTop(dy)); return *this; }

   PaneCut & place_cutBottom(PlaceType &window,OneOfTypes<Coord,Ratio> dy) { window.setPlace(cutBottom(dy)); return *this; }


   PaneCut & place_cutLeft(PlaceType &window) { place_cutLeft(window,window.getMinSize().x); return *this; }

   PaneCut & place_cutRight(PlaceType &window) { place_cutRight(window,window.getMinSize().x); return *this; }

   PaneCut & place_cutTop(PlaceType &window) { place_cutTop(window,window.getMinSize().y); return *this; }

   PaneCut & place_cutBottom(PlaceType &window) { place_cutBottom(window,window.getMinSize().y); return *this; }


   PaneCut & place_cutLeftTop(PlaceType &window,Point size) { window.setPlace(cutLeftTop(size)); return *this; }

   PaneCut & place_cutLeftCenter(PlaceType &window,Point size) { window.setPlace(cutLeftCenter(size)); return *this; }

   PaneCut & place_cutLeftBottom(PlaceType &window,Point size) { window.setPlace(cutLeftBottom(size)); return *this; }

   PaneCut & place_cutRightTop(PlaceType &window,Point size) { window.setPlace(cutRightTop(size)); return *this; }

   PaneCut & place_cutRightCenter(PlaceType &window,Point size) { window.setPlace(cutRightCenter(size)); return *this; }

   PaneCut & place_cutRightBottom(PlaceType &window,Point size) { window.setPlace(cutRightBottom(size)); return *this; }

   PaneCut & place_cutTopLeft(PlaceType &window,Point size) { window.setPlace(cutTopLeft(size)); return *this; }

   PaneCut & place_cutTopCenter(PlaceType &window,Point size) { window.setPlace(cutTopCenter(size)); return *this; }

   PaneCut & place_cutTopRight(PlaceType &window,Point size) { window.setPlace(cutTopRight(size)); return *this; }

   PaneCut & place_cutBottomLeft(PlaceType &window,Point size) { window.setPlace(cutBottomLeft(size)); return *this; }

   PaneCut & place_cutBottomCenter(PlaceType &window,Point size) { window.setPlace(cutBottomCenter(size)); return *this; }

   PaneCut & place_cutBottomRight(PlaceType &window,Point size) { window.setPlace(cutBottomRight(size)); return *this; }


   PaneCut & place_cutLeftTop(PlaceType &window) { place_cutLeftTop(window,window.getMinSize()); return *this; }

   PaneCut & place_cutLeftCenter(PlaceType &window) { place_cutLeftCenter(window,window.getMinSize()); return *this; }

   PaneCut & place_cutLeftBottom(PlaceType &window) { place_cutLeftBottom(window,window.getMinSize()); return *this; }

   PaneCut & place_cutRightTop(PlaceType &window) { place_cutRightTop(window,window.getMinSize()); return *this; }

   PaneCut & place_cutRightCenter(PlaceType &window) { place_cutRightCenter(window,window.getMinSize()); return *this; }

   PaneCut & place_cutRightBottom(PlaceType &window) { place_cutRightBottom(window,window.getMinSize()); return *this; }

   PaneCut & place_cutTopLeft(PlaceType &window) { place_cutTopLeft(window,window.getMinSize()); return *this; }

   PaneCut & place_cutTopCenter(PlaceType &window) { place_cutTopCenter(window,window.getMinSize()); return *this; }

   PaneCut & place_cutTopRight(PlaceType &window) { place_cutTopRight(window,window.getMinSize()); return *this; }

   PaneCut & place_cutBottomLeft(PlaceType &window) { place_cutBottomLeft(window,window.getMinSize()); return *this; }

   PaneCut & place_cutBottomCenter(PlaceType &window) { place_cutBottomCenter(window,window.getMinSize()); return *this; }

   PaneCut & place_cutBottomRight(PlaceType &window) { place_cutBottomRight(window,window.getMinSize()); return *this; }

   // placeRow

   template <class ... WW>
   PaneCut & placeRow_cutTop(WW & ... ww) requires ( ... && PlaceType<WW> )
    {
     Point size=SupMinSize(ww...);

     PlaceRow row(cutTop(size.y),size,space,sizeof ... (WW));

     row.place(ww...);

     return *this;
    }

   template <class ... WW>
   PaneCut & placeRow_cutBottom(WW & ... ww) requires ( ... && PlaceType<WW> )
    {
     Point size=SupMinSize(ww...);

     PlaceRow row(cutBottom(size.y),size,space,sizeof ... (WW));

     row.place(ww...);

     return *this;
    }

   // placeColumn

   template <class ... WW>
   PaneCut & placeColumn_cutLeft(WW & ... ww) requires ( ... && PlaceType<WW> )
    {
     Point size=SupMinSize(ww...);

     PlaceColumn col(cutLeft(size.x),size,space,sizeof ... (WW));

     col.place(ww...);

     return *this;
    }

   template <class ... WW>
   PaneCut & placeColumn_cutRight(WW & ... ww) requires ( ... && PlaceType<WW> )
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

