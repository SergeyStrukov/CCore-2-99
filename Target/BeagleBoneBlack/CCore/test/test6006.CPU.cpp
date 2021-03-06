/* test6006.CPU.cpp */
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

#include <CCore/inc/dev/AM3359.h>

static const CCore::ulen TTLen = 1<<12 ;

namespace CCore {
namespace Dev {

extern uint32 TTable[TTLen];

} // namespace Dev
} // namespace CCore

namespace App {

namespace Private_6006 {

void PrintTT(uint32 i)
 {
  using namespace AM3359::CP15;

  Printf(Con,"TTable[#3.16i;] = #8.16i;",i,CCore::Dev::TTable[i]);

  uint32 address=i<<20;

  uint32 par=VAtoPA(address);

  Type_PAROK parok(par);

  if( parok.maskbit(PAROK_NOK) )
    {
     Type_PARNOK parnok(par);

     Printf(Con," #;\n",parnok);
    }
  else
    {
     Printf(Con," #;\n",parok);
    }
 }

} // namespace Private_6006

using namespace Private_6006;

/* Testit<6006> */

template<>
const char *const Testit<6006>::Name="Test6006 CPU";

template<>
bool Testit<6006>::Main()
 {
  using namespace AM3359::CP15;

  // CPSR

  Printf(Con,"CPSR = #;\n\n",GetCPSR());

  // NSAC

  Printf(Con,"NonsecureAccessControl = #;\n\n",GetNonsecureAccessControl());

  // Features

  CPUFeatures features;

  //Printf(Con,"MainID = #;\n\n",features.getMainID());

  Printf(Con,"CacheType = #;\n\n",features.getCacheType());

  //Printf(Con,"TCMType = #;\n\n",features.getTCMType());

  //Printf(Con,"TLBType = #;\n\n",features.getTLBType());

  //Printf(Con,"MultiprocessorID = #;\n\n",features.getMultiprocessorID());

  //Printf(Con,"ProcessorFeature0 = #;\n\n",features.getProcessorFeature0());

  //Printf(Con,"ProcessorFeature1 = #;\n\n",features.getProcessorFeature1());

  //Printf(Con,"DebugFeature = #;\n\n",features.getDebugFeature());

  //Printf(Con,"AuxFeature = #;\n\n",features.getAuxFeature());

  //Printf(Con,"MemoryFeature0 = #;\n\n",features.getMemoryFeature0());

  //Printf(Con,"MemoryFeature1 = #;\n\n",features.getMemoryFeature1());

  //Printf(Con,"MemoryFeature2 = #;\n\n",features.getMemoryFeature2());

  //Printf(Con,"MemoryFeature3 = #;\n\n",features.getMemoryFeature3());

  //Printf(Con,"Instruction0 = #;\n\n",features.getInstructionSetAttributes0());

  //Printf(Con,"Instruction1 = #;\n\n",features.getInstructionSetAttributes1());

  //Printf(Con,"Instruction2 = #;\n\n",features.getInstructionSetAttributes2());

  //Printf(Con,"Instruction3 = #;\n\n",features.getInstructionSetAttributes3());

  //Printf(Con,"Instruction4 = #;\n\n",features.getInstructionSetAttributes4());

  Printf(Con,"CacheLevelID = #;\n\n",features.getCacheLevelID());

  // Cache Size

  Type_CacheSizeSel().set_Level(0).set(SetCacheSizeSel);

  Printf(Con,"L1D #;\n\n",GetCacheSizeID());

  Type_CacheSizeSel().set_Level(0).setbit(CacheSizeSel_InD).set(SetCacheSizeSel);

  Printf(Con,"L1I #;\n\n",GetCacheSizeID());

  Type_CacheSizeSel().set_Level(1).set(SetCacheSizeSel);

  Printf(Con,"L2U #;\n\n",GetCacheSizeID());

  // GetControl

  Printf(Con,"Control = #;\n\n",GetControl());

  Printf(Con,"AuxControl = #;\n\n",GetAuxControl());

  //Printf(Con,"TTControl = #;\n\n",GetTTControl());

  //Printf(Con,"TTBase0 = #;\n\n",GetTTBase0());

  //Printf(Con,"DomainAccessControl = #;\n\n",GetDomainAccessControl());

  //Printf(Con,"FCSE = #;\n\n",GetFCSE());

  //Printf(Con,"ContextID = #;\n\n",GetContextID());

  Printf(Con,"L2CacheAuxControl = #;\n\n",GetL2CacheAuxControl());

  PrintTT(0x000);
  PrintTT(0x400);
  PrintTT(0x800);
  PrintTT(0x808);
  PrintTT(0x810);
  PrintTT(0x900);
  PrintTT(0xA00);

  return true;
 }

} // namespace App

