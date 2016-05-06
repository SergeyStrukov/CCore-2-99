/* RefVal.h */
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

#ifndef CCore_inc_video_RefVal_h
#define CCore_inc_video_RefVal_h

#include <CCore/inc/String.h>

namespace CCore {
namespace Video {

/* GuardCtorRefValLock() */

void GuardCtorRefValLock();

/* classes */

class DefString;

template <class T> class RefVal;

template <class T> class CtorRefVal;

/* class DefString */

class DefString
 {
   StrLen def_const;
   String dynamic_str;

  public:

   DefString() noexcept {}

   DefString(const char *persistent_zstr) noexcept : def_const(persistent_zstr) {}

   DefString(const char *persistent_str,ulen str_len) noexcept : def_const(persistent_str,str_len) {}

   DefString(const String &obj) noexcept : dynamic_str(obj) {}

   ~DefString() {}

   StrLen str() const noexcept
    {
     if( +def_const ) return def_const;

     return Range(dynamic_str);
    }

   String makeString() const
    {
     if( +def_const ) return String(def_const);

     return dynamic_str;
    }

   // print object

   using PrintOptType = StrPrintOpt ;

   template <class P>
   void print(P &out,const PrintOptType &opt) const
    {
     Putobj(out,BindOpt(opt,str()));
    }
 };

/* DefString literal */

inline DefString operator "" _def (const char *str,ulen len) { return DefString(str,len); }

/* class RefVal<T> */

template <class T>
class RefVal
 {
   const T *ptr;
   T val;

  public:

   RefVal() : ptr(0),val() {}

   RefVal(const T &val_) : ptr(0),val(val_) {}

   T operator + () const { return ptr?(*ptr):val; }

   const T & get() const { if( ptr ) return *ptr; return val; }

   T & takeVal() { return val; }

   void bind(const T &obj) { ptr=&obj; }
 };

/* class CtorRefVal<T> */

template <class T>
class CtorRefVal : NoCopy
 {
   const T *ptr;
   T val;
   mutable bool lock = false ;

  public:

   template <class ... SS>
   explicit CtorRefVal(SS && ... ss) : ptr(0),val( std::forward<SS>(ss)... ) {}

   operator const T & () const { lock=true; if( ptr ) return *ptr; return val; }

   T & takeVal() { return val; }

   void bind(const T &obj) { if( lock ) GuardCtorRefValLock(); ptr=&obj; }
 };

} // namespace Video
} // namespace CCore

#endif


