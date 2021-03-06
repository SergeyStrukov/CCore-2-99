/* DevIntHandle.h */
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

#ifndef CCore_inc_dev_DevIntHandle_h
#define CCore_inc_dev_DevIntHandle_h

#include <CCore/inc/PlanInit.h>

namespace CCore {
namespace Dev {

/* GetPlanInitNode_...() */

PlanInitNode * GetPlanInitNode_DevIntHandle();

/* enum IntSource */

enum IntSource
 {
  Int_LCDCINT = 36,
  Int_I2C0INT = 70,
  Int_I2C1INT = 71,
  Int_I2C2INT = 30,
  Int_TINT4   = 92,

  Int_3PGSWRXTHR0 = 40,
  Int_3PGSWRXINT0 = 41,
  Int_3PGSWTXINT0 = 42,
  Int_3PGSWMISC0  = 43,

  Int_GPIO1_0 = 98,
  Int_GPIO1_1 = 99,

  Int_TableLen = 128
 };

/* functions */

void SetupIntHandler(IntSource int_source,Function<void (void)> handle_int,unsigned priority=0); // priority 0-63

void CleanupIntHandler(IntSource int_source);

void EnableInt(IntSource int_source);

void DisableInt(IntSource int_source);

} // namespace Dev
} // namespace CCore

#endif

