/* MakeFileName.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 3.00
//
//  Tag: Fundamental
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_MakeFileName_h
#define CCore_inc_MakeFileName_h

#include <CCore/inc/MakeString.h>
#include <CCore/inc/GenFile.h>

namespace CCore {

/* classes */

class MakeFileName;

/* class MakeFileName */

class MakeFileName : public MakeString<MaxPathLen>
 {
  private:

   void make(StrLen dir_name,StrLen file_name);

  public:

   MakeFileName() {}

   MakeFileName(StrLen dir_name,StrLen file_name) { make(dir_name,file_name); }

   StrLen operator () (StrLen dir_name,StrLen file_name)
    {
     reset();

     make(dir_name,file_name);

     return get();
    }
 };

} // namespace CCore

#endif
