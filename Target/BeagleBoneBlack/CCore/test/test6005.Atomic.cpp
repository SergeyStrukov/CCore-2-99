/* test6005.Atomic.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: Target/BeagleBoneBlack
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <CCore/test/test.h>
#include <CCore/test/testRun.h>

namespace App {

namespace Private_6005 {

/* test1() */

void test1()
 {
  Atomic atomic;

  {
   RunTask run_task;

   run_task(ulen(10), [&] () {
                               for(ulen cnt=1000000; cnt ;cnt--) atomic++;
                              } );
  }

  Printf(Con,"atomic = #;\n",atomic);
 }

/* test2() */

void test2()
 {
  Atomic atomic;

  {
   RunTask run_task;

   run_task( [&] () {
                      ulen count=0;

                      while( count<1000000 )
                        if( atomic.trySet(0,1)==0 ) count++;
                     } );

   run_task( [&] () {
                      ulen count=0;

                      while( count<1000000 )
                        if( atomic.trySet(1,0)==1 ) count++;
                     } );
  }
 }

} // namespace Private_6005

using namespace Private_6005;

/* Testit<6005> */

template<>
const char *const Testit<6005>::Name="Test6005 Atomic";

template<>
bool Testit<6005>::Main()
 {
  test1();
  test2();

  return true;
 }

} // namespace App

