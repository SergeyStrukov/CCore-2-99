/* Cmp.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
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

#ifndef CCore_inc_Cmp_h
#define CCore_inc_Cmp_h

#include <CCore/inc/Gadget.h>

namespace CCore {

/* enum CmpResult */

enum CmpResult
 {
  CmpLess    = -1,
  CmpEqual   =  0,
  CmpGreater =  1
 };

inline CmpResult operator - (CmpResult cmp) { return CmpResult(-int(cmp)); }

const char * GetTextDesc(CmpResult cmp);

/* functions */

template <class T>
CmpResult LessCmp(const T &a,const T &b)
 {
  return (a<b)?CmpLess:( (b<a)?CmpGreater:CmpEqual );
 }

/* Str...() */

CmpResult StrCmp(StrLen a,StrLen b);

bool StrLess(StrLen a,StrLen b);

template <class T>
CmpResult StrCmpOf(const T &a,const T &b) { return StrCmp(Range(a),Range(b)); }

template <class T>
bool StrLessOf(const T &a,const T &b) { return StrLess(Range(a),Range(b)); }

/* classes */

class CmpAsStr;

template <class T> struct LessComparable;

template <class T> struct CmpComparable;

struct ProbeSet_objCmp;

template <bool has_objCmp,class T> struct CmpAdapters;

/* class CmpAsStr */

class CmpAsStr
 {
   StrLen str;

  public:

   explicit CmpAsStr(StrLen str_) : str(str_) {}

   template <class T>
   explicit CmpAsStr(const T &obj) : str(Range(obj)) {}

   // cmp objects

   CmpResult objCmp(CmpAsStr obj) const { return StrCmp(str,obj.str); }

   friend bool operator < (const CmpAsStr &a,const CmpAsStr &b) { return StrLess(a.str,b.str); }

   friend bool operator > (const CmpAsStr &a,const CmpAsStr &b) { return StrLess(b.str,a.str); }

   friend bool operator <= (const CmpAsStr &a,const CmpAsStr &b) { return !StrLess(b.str,a.str); }

   friend bool operator >= (const CmpAsStr &a,const CmpAsStr &b) { return !StrLess(a.str,b.str); }

   friend bool operator == (const CmpAsStr &a,const CmpAsStr &b) { return StrCmp(a.str,b.str)==0; }

   friend bool operator != (const CmpAsStr &a,const CmpAsStr &b) { return StrCmp(a.str,b.str)!=0; }
 };

/* struct LessComparable<T> */

template <class T>
struct LessComparable
 {
  friend bool operator > (const T &a,const T &b) { return b<a; }

  friend bool operator <= (const T &a,const T &b) { return !(b<a); }

  friend bool operator >= (const T &a,const T &b) { return !(a<b); }

  friend bool operator == (const T &a,const T &b) { return !( a<b || b<a ); }

  friend bool operator != (const T &a,const T &b) { return ( a<b || b<a ); }
 };

/* struct CmpComparable<T> */

template <class T>
struct CmpComparable
 {
  friend bool operator < (const T &a,const T &b) { return a.objCmp(b)<0; }

  friend bool operator > (const T &a,const T &b) { return a.objCmp(b)>0; }

  friend bool operator <= (const T &a,const T &b) { return a.objCmp(b)<=0; }

  friend bool operator >= (const T &a,const T &b) { return a.objCmp(b)>=0; }

  friend bool operator == (const T &a,const T &b) { return a.objCmp(b)==0; }

  friend bool operator != (const T &a,const T &b) { return a.objCmp(b)!=0; }
 };

/* struct ProbeSet_objCmp */

struct ProbeSet_objCmp
 {
  template <class T,CmpResult (T::*M)(const T &) const> struct Host;

  template <class T,class C=Host<T,&T::objCmp> > struct Condition;
 };

/* const Has_objCmp<T> */

template <class T>
const bool Has_objCmp = Meta::Detect<ProbeSet_objCmp,T> ;

/* struct CmpAdapters<bool has_objCmp,T> */

template <class T>
struct CmpAdapters<false,T>
 {
  static CmpResult Cmp(const T &a,const T &b) { return LessCmp(a,b); }
 };

template <class T>
struct CmpAdapters<true,T>
 {
  static CmpResult Cmp(const T &a,const T &b) { return a.objCmp(b); }
 };

/* Cmp() */

template <class T>
CmpResult Cmp(const T &a,const T &b) { return CmpAdapters<Has_objCmp<T>,T>::Cmp(a,b); }

/* AlphaCmp() */

template <class ... TT>
CmpResult AlphaCmpAny(const TT & ... tt);

inline CmpResult AlphaCmp(CmpResult cmp)
 {
  return cmp;
 }

inline CmpResult AlphaCmp(CmpResult cmp1,CmpResult cmp2)
 {
  if( cmp1 ) return cmp1;

  return cmp2;
 }

template <class ... TT>
CmpResult AlphaCmp(CmpResult cmp,const TT & ... tt)
 {
  if( cmp ) return cmp;

  return AlphaCmpAny(tt...);
 }

template <class ... TT>
CmpResult AlphaCmp(CmpResult cmp1,CmpResult cmp2,const TT & ... tt)
 {
  if( cmp1 ) return cmp1;

  if( cmp2 ) return cmp2;

  return AlphaCmpAny(tt...);
 }

template <class T>
CmpResult AlphaCmp(const T &a,const T &b)
 {
  return Cmp(a,b);
 }

template <class T,class ... TT>
CmpResult AlphaCmp(const T &a,const T &b,const TT & ... tt)
 {
  if( CmpResult ret=Cmp(a,b) ) return ret;

  return AlphaCmp(tt...);
 }

template <class ... TT>
CmpResult AlphaCmpAny(const TT & ... tt)
 {
  return AlphaCmp(tt...);
 }

/* Range...() */

template <class T>
CmpResult RangeCmp(const T *a,const T *b,ulen count)
 {
  for(; count ;count--)
    if( CmpResult ret=Cmp(*(a++),*(b++)) )
      return ret;

  return CmpEqual;
 }

template <class T>
CmpResult RangeCmp(PtrLen<T> a,PtrLen<T> b)
 {
  if( a.len<b.len )
    {
     if( CmpResult ret=RangeCmp(a.ptr,b.ptr,a.len) ) return ret;

     return CmpLess;
    }
  else if( a.len>b.len )
    {
     if( CmpResult ret=RangeCmp(a.ptr,b.ptr,b.len) ) return ret;

     return CmpGreater;
    }
  else
    {
     return RangeCmp(a.ptr,b.ptr,b.len);
    }
 }

template <class T>
bool RangeLess(PtrLen<T> a,PtrLen<T> b)
 {
  if( a.len<b.len )
    {
     return RangeCmp(a.ptr,b.ptr,a.len) <= 0 ;
    }
  else
    {
     return RangeCmp(a.ptr,b.ptr,b.len) < 0 ;
    }
 }

template <class T>
CmpResult RangeCmpOf(const T &a,const T &b) { return RangeCmp(Range(a),Range(b)); }

template <class T>
bool RangeLessOf(const T &a,const T &b) { return RangeLess(Range(a),Range(b)); }

/* Range...By() */

template <class T,class Func>
CmpResult RangeCmpBy(const T *a,const T *b,ulen count,Func by)
 {
  for(; count ;count--)
    if( CmpResult ret=Cmp(by(*(a++)),by(*(b++))) )
      return ret;

  return CmpEqual;
 }

template <class T,class Func>
CmpResult RangeCmpBy(PtrLen<T> a,PtrLen<T> b,Func by)
 {
  if( a.len<b.len )
    {
     if( CmpResult ret=RangeCmpBy(a.ptr,b.ptr,a.len,by) ) return ret;

     return CmpLess;
    }
  else if( a.len>b.len )
    {
     if( CmpResult ret=RangeCmpBy(a.ptr,b.ptr,b.len,by) ) return ret;

     return CmpGreater;
    }
  else
    {
     return RangeCmpBy(a.ptr,b.ptr,b.len,by);
    }
 }

template <class T,class Func>
bool RangeLessBy(PtrLen<T> a,PtrLen<T> b,Func by)
 {
  if( a.len<b.len )
    {
     return RangeCmpBy(a.ptr,b.ptr,a.len,by) <= 0 ;
    }
  else
    {
     return RangeCmpBy(a.ptr,b.ptr,b.len,by) < 0 ;
    }
 }

template <class T,class Func>
CmpResult RangeCmpOfBy(const T &a,const T &b,Func by) { return RangeCmpBy(Range(a),Range(b),by); }

template <class T,class Func>
bool RangeLessOfBy(const T &a,const T &b,Func by) { return RangeLessBy(Range(a),Range(b),by); }

} // namespace CCore

#endif


