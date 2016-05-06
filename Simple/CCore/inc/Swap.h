/* Swap.h */
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

#ifndef CCore_inc_Swap_h
#define CCore_inc_Swap_h

#include <CCore/inc/Gadget.h>

namespace CCore {

/* functions */

template <class T>
Meta::EnableIf< Meta::IsCopyable<T> && !Meta::IsNothrowCopyable<T> > CopySwap(T &a,T &b,int unused=0) noexcept // emit warning
 {
  T temp(a);

  a=b;

  b=temp;
 }

template <class T>
Meta::EnableIf< Meta::IsNothrowCopyable<T> > CopySwap(T &a,T &b) noexcept
 {
  T temp(a);

  a=b;

  b=temp;
 }

template <class T>
Meta::EnableIf< Meta::IsMovable<T> > MoveSwap(T &a,T &b) noexcept
 {
  T temp( std::move(a) );

  a = std::move(b) ;

  b = std::move(temp) ;
 }

/* classes */

struct ProbeSet_objSwap;

template <bool has_objSwap,bool is_movable,class T> struct SwapAdapters;

/* struct ProbeSet_objSwap */

struct ProbeSet_objSwap
 {
  template <class T,void (T::*M)(T &)> struct Host;

  template <class T,class C=Host<T,&T::objSwap> > struct Condition;
 };

/* const Has_objSwap<T> */

template <class T>
const bool Has_objSwap = Meta::Detect<ProbeSet_objSwap,T> ;

/* struct SwapAdapters<bool has_objSwap,bool is_movable,T> */

template <bool is_movable,class T>
struct SwapAdapters<true,is_movable,T>
 {
  static void Swap(T &a,T &b) { a.objSwap(b); }
 };

template <class T>
struct SwapAdapters<false,true,T>
 {
  static void Swap(T &a,T &b) { MoveSwap(a,b); }
 };

template <class T>
struct SwapAdapters<false,false,T>
 {
  static void Swap(T &a,T &b) { CopySwap(a,b); }
 };

/* Swap() */

template <class T>
void Swap(T &a,T &b) noexcept { SwapAdapters<Has_objSwap<T>,Meta::IsMovable<T>,T>::Swap(a,b); }

/* NullBySwap() */

template <class T>
void NullBySwap(T &obj)
 {
  T temp{};

  Swap(temp,obj);
 }

/* MoveBySwap() */

template <class T>
void MoveBySwap(T &dst,T &src)
 {
  if( &dst!=&src )
    {
     Swap(dst,src);

     NullBySwap(src);
    }
 }

} // namespace CCore

#endif


