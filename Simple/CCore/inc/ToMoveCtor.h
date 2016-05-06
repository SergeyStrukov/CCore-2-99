/* ToMoveCtor.h */
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

#ifndef CCore_inc_ToMoveCtor_h
#define CCore_inc_ToMoveCtor_h

#include <CCore/inc/Gadget.h>

namespace CCore {

/* classes */

template <class T> struct ToMoveCtor;

struct ProbeSet_ToMoveCtor;

/* struct ToMoveCtor<T> */

template <class T>
struct ToMoveCtor
 {
  T *obj;

  explicit ToMoveCtor(T &obj_) { obj=&obj_; }

  T * operator -> () const { return obj; }

  template <class S>
  ToMoveCtor<S> cast() const { return ToMoveCtor<S>(*obj); }
 };

/* struct ProbeSet_ToMoveCtor */

struct ProbeSet_ToMoveCtor
 {
  static Place<void> GetPlace();

  template <class T>
  static ToMoveCtor<T> GetObj();

  template <class T,class C=decltype( new(GetPlace()) T(GetObj<T>()) )> struct Condition;
 };

/* const Has_ToMoveCtor<T> */

template <class T>
const bool Has_ToMoveCtor = Meta::Detect<ProbeSet_ToMoveCtor,T> ;

/* ObjToMove() */

template <class T>
ToMoveCtor<T> ObjToMove(T &obj) { return ToMoveCtor<T>(obj); }

} // namespace CCore

#endif


