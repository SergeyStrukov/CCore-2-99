/* DrawTools.h */
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

#ifndef CCore_inc_video_DragTools_h
#define CCore_inc_video_DragTools_h

#include <CCore/inc/video/Point.h>

namespace CCore {
namespace Video {

/* concept PointSetTypes<TT> */

template <class ... TT>
concept bool PointSetTypes = ( ... && IsType<TT,Point> ) ;

/* concept PlotType<Plot> */

template <NothrowCopyableType Plot>
concept bool PlotType = requires(Plot &obj,Point p)
 {
  obj(p);
 } ;

/* concept SmoothPlotType<SPlot> */

template <NothrowCopyableType SPlot>
concept bool SmoothPlotType = PlotType<SPlot> && requires(SPlot &obj,Point p,unsigned alpha)
 {
  obj(p,alpha);
 } ;

/* concept HPlotType<HPlot> */

template <NothrowCopyableType HPlot>
concept bool HPlotType = PlotType<HPlot> && requires(HPlot &obj,Coord y,Coord a,Coord b)
 {
  obj(y,a,b);
 } ;

/* concept MapType<Map> */

template <NothrowCopyableType Map>
concept bool MapType = requires(Map &obj,Point p)
 {
  { obj(p) } -> Point ;
 } ;

/* enum SolidFlag */

enum SolidFlag
 {
  SolidOdd,
  SolidAll
 };

/* enum HalfFlag */

enum HalfFlag // contraclockwise
 {
  HalfPos,
  HalfNeg,
 };

/* classes */

struct CircleSpline;

struct PaneBorder;

struct PaneSub;

/* struct CircleSpline */

struct CircleSpline
 {
  Point buf[12]; // contraclockwise

  CircleSpline(Point center,Coord radius); // radius > 0

  PtrLen<const Point> get() const { return Range(buf); }
 };

/* struct PaneBorder */

struct PaneBorder
 {
  Point buf[4]; // contraclockwise

  explicit PaneBorder(Pane pane); // +pane

  Point topLeft() const { return buf[0]; }

  Point bottomLeft() const { return buf[1]; }

  Point bottomRight() const { return buf[2]; }

  Point topRight() const { return buf[3]; }

  PtrLen<const Point> get() const { return Range(buf); }
 };

/* struct PaneSub */

struct PaneSub
 {
  Pane top;    // full width
  Pane bottom; // full width

  Pane left;
  Pane right;

  PaneSub(Pane outer,Pane inner);
 };

} // namespace Video
} // namespace CCore

#endif


