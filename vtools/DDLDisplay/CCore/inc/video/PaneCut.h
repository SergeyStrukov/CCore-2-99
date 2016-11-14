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

#include <CCore/inc/video/Point.h>

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


   void place_boxLeft(PlaceType &window,Coord dxy,Coord mini_space) { window.setPlace(boxLeft(dxy,mini_space)); }

   void place_boxRight(PlaceType &window,Coord dxy,Coord mini_space) { window.setPlace(boxRight(dxy,mini_space)); }

   void place_boxLeft(PlaceType &window,Coord dxy) { window.setPlace(boxLeft(dxy)); }

   void place_boxRight(PlaceType &window,Coord dxy) { window.setPlace(boxRight(dxy)); }

   void place_boxLeft(PlaceType &window) { place_boxLeft(window,BoxSize(window)); }

   void place_boxRight(PlaceType &window) { place_boxRight(window,BoxSize(window)); }


   void place_cutLeft(PlaceType &window,Coord dx) { window.setPlace(cutLeft(dx)); }

   void place_cutRight(PlaceType &window,Coord dx) { window.setPlace(cutRight(dx)); }

   void place_cutTop(PlaceType &window,Coord dy) { window.setPlace(cutTop(dy)); }

   void place_cutBottom(PlaceType &window,Coord dy) { window.setPlace(cutBottom(dy)); }


   void place_cutLeft(PlaceType &window) { place_cutLeft(window,window.getMinSize().x); }

   void place_cutRight(PlaceType &window) { place_cutRight(window,window.getMinSize().x); }

   void place_cutTop(PlaceType &window) { place_cutTop(window,window.getMinSize().y); }

   void place_cutBottom(PlaceType &window) { place_cutBottom(window,window.getMinSize().y); }


   void place_cutLeftTop(PlaceType &window,Point size) { window.setPlace(cutLeftTop(size)); }

   void place_cutLeftCenter(PlaceType &window,Point size) { window.setPlace(cutLeftCenter(size)); }

   void place_cutLeftBottom(PlaceType &window,Point size) { window.setPlace(cutLeftBottom(size)); }

   void place_cutRightTop(PlaceType &window,Point size) { window.setPlace(cutRightTop(size)); }

   void place_cutRightCenter(PlaceType &window,Point size) { window.setPlace(cutRightCenter(size)); }

   void place_cutRightBottom(PlaceType &window,Point size) { window.setPlace(cutRightBottom(size)); }

   void place_cutTopLeft(PlaceType &window,Point size) { window.setPlace(cutTopLeft(size)); }

   void place_cutTopCenter(PlaceType &window,Point size) { window.setPlace(cutTopCenter(size)); }

   void place_cutTopRight(PlaceType &window,Point size) { window.setPlace(cutTopRight(size)); }

   void place_cutBottomLeft(PlaceType &window,Point size) { window.setPlace(cutBottomLeft(size)); }

   void place_cutBottomCenter(PlaceType &window,Point size) { window.setPlace(cutBottomCenter(size)); }

   void place_cutBottomRight(PlaceType &window,Point size) { window.setPlace(cutBottomRight(size)); }


   void place_cutLeftTop(PlaceType &window) { place_cutLeftTop(window,window.getMinSize()); }

   void place_cutLeftCenter(PlaceType &window) { place_cutLeftCenter(window,window.getMinSize()); }

   void place_cutLeftBottom(PlaceType &window) { place_cutLeftBottom(window,window.getMinSize()); }

   void place_cutRightTop(PlaceType &window) { place_cutRightTop(window,window.getMinSize()); }

   void place_cutRightCenter(PlaceType &window) { place_cutRightCenter(window,window.getMinSize()); }

   void place_cutRightBottom(PlaceType &window) { place_cutRightBottom(window,window.getMinSize()); }

   void place_cutTopLeft(PlaceType &window) { place_cutTopLeft(window,window.getMinSize()); }

   void place_cutTopCenter(PlaceType &window) { place_cutTopCenter(window,window.getMinSize()); }

   void place_cutTopRight(PlaceType &window) { place_cutTopRight(window,window.getMinSize()); }

   void place_cutBottomLeft(PlaceType &window) { place_cutBottomLeft(window,window.getMinSize()); }

   void place_cutBottomCenter(PlaceType &window) { place_cutBottomCenter(window,window.getMinSize()); }

   void place_cutBottomRight(PlaceType &window) { place_cutBottomRight(window,window.getMinSize()); }
 };

} // namespace Video
} // namespace CCore

#endif

