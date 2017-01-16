/* Shape.FixedFrame.h */
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
//  Copyright (c) 2017 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_video_lib_Shape_FixedFrame_h
#define CCore_inc_video_lib_Shape_FixedFrame_h

#include <CCore/inc/video/FrameBase.h>
#include <CCore/inc/video/Color.h>
#include <CCore/inc/video/Font.h>
#include <CCore/inc/video/RefVal.h>

#include <CCore/inc/DeferCall.h>

namespace CCore {
namespace Video {

/* classes */

class FixedFrameShape;

/* class FixedFrameShape */

class FixedFrameShape
 {
  public:

   static const bool EnableAlert = false ;
   static const bool EnableMinimize = false ;
   static const bool EnableMaximize = false ;
   static const bool EnableFixed = true ;

   static VKey Filter(VKey vkey,KeyMod kmod)
    {
     if( kmod&KeyMod_Alt )
       {
        switch( vkey )
          {
           case VKey_Left  :
           case VKey_Right :
           case VKey_Up    :
           case VKey_Down  : return (kmod&KeyMod_Shift)?VKey_Null:vkey;

           case VKey_F4    : return vkey;

           default: return VKey_Null;
          }
       }
     else
       {
        switch( vkey )
          {
           case VKey_F1    : return vkey;

           default: return VKey_Null;
          }
       }
    }
 };

} // namespace Video
} // namespace CCore

#endif

