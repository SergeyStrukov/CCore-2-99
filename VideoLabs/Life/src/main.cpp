/* main.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: Life 1.00
//
//  Tag: VideoLabs
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2015 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/Application.h>

#include <CCore/inc/video/Desktop.h>

#include <CCore/inc/Abort.h>

/* main() */

using namespace CCore::Video;

const CCore::AbortMsgFuncType CCore::AbortMsgFunc = AbortMsgBox ;

int main()
 {
  return App::Main(StartDisplay());
 }





