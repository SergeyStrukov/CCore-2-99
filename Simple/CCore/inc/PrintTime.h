/* PrintTime.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 3.00
//
//  Tag: Simple Mini
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2015 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_PrintTime_h
#define CCore_inc_PrintTime_h

#include <CCore/inc/Printf.h>

namespace CCore {

/* classes */

template <UIntType UInt> class PrintTimeType;

/* class PrintTimeType<UInt> */

template <UIntType UInt>
class PrintTimeType
 {
   UInt value;

  public:

   explicit PrintTimeType(UInt value_) : value(value_) {}

   void print(PrinterType &out) const
    {
     UInt val=value;

     UInt tsec=val%60; val/=60;
     UInt tmin=val%60; val/=60;
     UInt thour=val%24; val/=24;
     UInt tday=val;

     if( tday ) Printf(out,"#;:",tday);

     Printf(out,"#2i;:#2i;:#2i;",thour,tmin,tsec);
    }
 };

/* PrintTime() */

template <UIntType UInt>
PrintTimeType<UInt> PrintTime(UInt value) { return PrintTimeType<UInt>(value); }

} // namespace CCore

#endif


