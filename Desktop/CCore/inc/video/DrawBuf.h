/* DrawBuf.h */
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

#ifndef CCore_inc_video_DrawBuf_h
#define CCore_inc_video_DrawBuf_h

#include <CCore/inc/video/FrameBuf.h>
#include <CCore/inc/video/Desktop.h>

namespace CCore {
namespace Video {

/* classes */

class DrawBuf;

/* class DrawBuf */

class DrawBuf : public FrameBuf<DesktopColor>
 {
   Point origin; // in FrameBuf coord

  public:

   DrawBuf() {}

   DrawBuf(const FrameBuf<DesktopColor> &buf) : FrameBuf<DesktopColor>(buf) {}

   DrawBuf(const FrameBuf<DesktopColor> &buf,Point origin_) : FrameBuf<DesktopColor>(buf),origin(origin_) {}

   DrawBuf cut(Pane pane) const;

   DrawBuf cutRebase(Pane pane) const;

   // to FrameBuf coord

   template <class T>
   T map(T obj) const { return obj+origin; }

   Coordinate mapX(Coordinate x) const { return x+origin.x; }

   Coordinate mapY(Coordinate y) const { return y+origin.y; }

   PointMap getMapper() const { return PointMap(origin); }
 };

} // namespace Video
} // namespace CCore

#endif


