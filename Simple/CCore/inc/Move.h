/* Move.h */
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

#ifndef CCore_inc_Move_h
#define CCore_inc_Move_h

#include <CCore/inc/Swap.h>
#include <CCore/inc/ToMoveCtor.h>

namespace CCore {

/* functions */

template <class T>
Meta::EnableIf< Meta::HasCopyCtor<T> && !Meta::HasNothrowCopyCtor<T> , T * > CopyMove(T *obj,Place<void> place,int unused=0) noexcept // emit warning
 {
  T *ret=new(place) T(*obj);

  obj->~T();

  return ret;
 }

template <class T>
Meta::EnableIf< Meta::HasNothrowCopyCtor<T> , T * > CopyMove(T *obj,Place<void> place) noexcept
 {
  T *ret=new(place) T(*obj);

  obj->~T();

  return ret;
 }

template <class T>
Meta::EnableIf< Meta::HasNothrowDefaultCtor<T> , T * > SwapMove(T *obj,Place<void> place) noexcept
 {
  T *ret=new(place) T();

  Swap(*obj,*ret);

  obj->~T();

  return ret;
 }

template <class T>
Meta::EnableIf< Meta::HasMoveCtor<T> , T * > MoveMove(T *obj,Place<void> place) noexcept
 {
  T *ret=new(place) T(std::move(*obj));

  obj->~T();

  return ret;
 }

template <class T>
Meta::EnableIf< Has_ToMoveCtor<T> , T * > CtorMove(T *obj,Place<void> place) noexcept
 {
  T *ret=new(place) T(ObjToMove(*obj));

  obj->~T();

  return ret;
 }

/* classes */

struct ProbeSet_objMove;

template <bool has_objMove,bool has_ToMoveCtor,bool is_movable,bool has_objSwap,class T> struct MoveAdapters;

/* struct ProbeSet_objMove */

struct ProbeSet_objMove
 {
  template <class T,T * (T::*M)(Place<void>)> struct Host;

  template <class T,class C=Host<T,&T::objMove> > struct Condition;
 };

/* const Has_objMove<T> */

template <class T>
const bool Has_objMove = Meta::Detect<ProbeSet_objMove,T> ;

/* struct MoveAdapters<bool has_objMove,bool has_ToMoveCtor,bool is_movable,bool has_objSwap,T> */

template <bool has_ToMoveCtor,bool is_movable,bool has_objSwap,class T>
struct MoveAdapters<true,has_ToMoveCtor,is_movable,has_objSwap,T>
 {
  static T * Move(T *obj,Place<void> place) { return obj->objMove(place); }
 };

template <bool is_movable,bool has_objSwap,class T>
struct MoveAdapters<false,true,is_movable,has_objSwap,T>
 {
  static T * Move(T *obj,Place<void> place) { return CtorMove(obj,place); }
 };

template <bool has_objSwap,class T>
struct MoveAdapters<false,false,true,has_objSwap,T>
 {
  static T * Move(T *obj,Place<void> place) { return MoveMove(obj,place); }
 };

template <class T>
struct MoveAdapters<false,false,false,true,T>
 {
  static T * Move(T *obj,Place<void> place) { return SwapMove(obj,place); }
 };

template <class T>
struct MoveAdapters<false,false,false,false,T>
 {
  static T * Move(T *obj,Place<void> place) { return CopyMove(obj,place); }
 };

/* Move() */

template <class T>
T * Move(T *obj,Place<void> place) noexcept
 {
  return MoveAdapters<Has_objMove<T>,Has_ToMoveCtor<T>,Meta::IsMovable<T>,Has_objSwap<T>&&Meta::HasNothrowDefaultCtor<T>,T>::Move(obj,place);
 }

} // namespace CCore

#endif


