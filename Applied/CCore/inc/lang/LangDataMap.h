/* LangDataMap.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 3.00
//
//  Tag: Applied CondLangLR1
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2017 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_lang_LangDataMap_h
#define CCore_inc_lang_LangDataMap_h

#include <CCore/inc/ddl/DDLMapTypes.h>

namespace CCore {
namespace Lang {

/* TypeDef */

#include "LangData.TypeDef.gen.h"

/* classes */

class DataMap;

/* class DataMap */

class DataMap : NoCopy
 {
   void *mem;

   TypeDef::Lang lang;

  private:

   template <class T>
   static bool Checkin(T *ptr,PtrLen<T> range)
    {
     if( ptr==0 ) return false;

     ulen index=ptr->index;

     return index<range.len && ptr==range.ptr+index ;
    }

   template <class T>
   static bool Checkin_null(T *ptr,PtrLen<T> range)
    {
     if( ptr==0 ) return true;

     ulen index=ptr->index;

     return index<range.len && ptr==range.ptr+index ;
    }

   void sanity_atoms();
   void sanity_synts();
   void sanity_lang();
   void sanity_rules();
   void sanity_top_rules();
   void sanity_states();
   void sanity_finals();

  public:

   explicit DataMap(StrLen file_name);

   ~DataMap();

   const TypeDef::Lang & getLang() const { return lang; }

   void sanity();
 };

} // namespace Lang
} // namespace CCore

#endif

