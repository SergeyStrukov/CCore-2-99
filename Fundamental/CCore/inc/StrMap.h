/* StrMap.h */
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
//  Copyright (c) 2017 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_StrMap_h
#define CCore_inc_StrMap_h

#include <CCore/inc/Array.h>
#include <CCore/inc/Cmp.h>
#include <CCore/inc/Sort.h>
#include <CCore/inc/algon/BinarySearch.h>

namespace CCore {

/* classes */

template <class T> class StrMap;

/* class StrMap<T> */

template <class T>
class StrMap : NoCopy
 {
   struct Rec : CmpComparable<Rec>
    {
     StrLen key;
     T obj;

     Rec() noexcept( Meta::HasNothrowDefaultCtor<T> ) : obj() {}

     template <class ... SS>
     Rec(StrLen key_,SS && ... ss) : key(key_),obj( std::forward<SS>(ss)... ) {}

     // cmp objects

     CmpResult objCmp(const Rec &obj) const { return StrCmp(key,obj.key); }

     // swap objects

     void objSwap(Rec &rec)
      {
       Swap(key,rec.key);
       Swap(obj,rec.obj);
      }
    };

   DynArray<Rec> map;

  private:

   template <class R>
   static R Search(R r,ulen i,char ch)
    {
     Algon::BinarySearch_if(r, [=] (const Rec &rec) { StrLen key=rec.key; return key.len>i && key[i]>=ch ; } );

     return Algon::BinarySearch_if(r, [=] (const Rec &rec) { StrLen key=rec.key; return key[i]>ch ; } );
    }

   template <class R>
   static R Search(R r,ulen len)
    {
     return Algon::BinarySearch_if(r, [=] (const Rec &rec) { StrLen key=rec.key; return key.len>len; } );
    }

   static T * Pick(PtrLen<Rec> r)
    {
     if( +r ) return &r->obj;

     return 0;
    }

   static const T * Pick(PtrLen<const Rec> r)
    {
     if( +r ) return &r->obj;

     return 0;
    }

   template <class R>
   static auto Search(R r,StrLen key)
    {
     for(ulen i=0; i<key.len ;i++) r=Search(r,i,key[i]);

     r=Search(r,key.len);

     return Pick(r);
    }

  protected:

   StrMap() {}

   template <class ... SS>
   void add(StrLen key,SS && ... ss) // key is persistent
    {
     map.append_fill(key, std::forward<SS>(ss)... );
    }

   void complete()
    {
     Sort(Range(map));
    }

  public:

   ~StrMap() {}

   T * operator [] (StrLen key)
    {
     return Search(Range(map),key);
    }

   const T * operator [] (StrLen key) const
    {
     return Search(Range(map),key);
    }
 };

} // namespace CCore

#endif

