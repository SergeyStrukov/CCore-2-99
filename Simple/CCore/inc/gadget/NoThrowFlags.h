/* NoThrowFlags.h */
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

#ifndef CCore_inc_gadget_NoThrowFlags_h
#define CCore_inc_gadget_NoThrowFlags_h

#include <CCore/inc/base/PlatformBase.h>

#include <CCore/inc/gadget/Nothing.h>

namespace CCore {

/* namespace Meta */

namespace Meta {

/* classes */

template <class T,class S> struct SetNoThrowFlagsWarningCtor;

template <class T,bool Flag> struct BadNoThrowFlagCtor;

/* struct SetNoThrowFlagsWarningCtor<T,S> */

template <class T,class S>
struct SetNoThrowFlagsWarningCtor
 {
  static constexpr bool Unused(int unused=0) { return false; }

  enum RetType { Ret = Unused() };
 };

template <class T>
struct SetNoThrowFlagsWarningCtor<T,T>
 {
  enum RetType { Ret = true };
 };

/* const SetNoThrowFlagsWarning<T,S> */

template <class T,class S>
const bool SetNoThrowFlagsWarning = SetNoThrowFlagsWarningCtor<T,S>::Ret ;

/* struct BadNoThrowFlagCtor<T,bool Flag> */

template <class T>
struct BadNoThrowFlagCtor<T,true>
 {
  enum RetType { Ret = true };
 };

template <class T>
struct BadNoThrowFlagCtor<T,false>
 {
  static constexpr bool Unused(int unused=0) { return false; }

  enum RetType { Ret = Unused() };
 };

/* const BadNoThrowFlag<T,bool Flag> */

template <class T,bool Flag>
const bool BadNoThrowFlag = BadNoThrowFlagCtor<T,Flag>::Ret ;

} // namespace Meta

/* classes */

template <class T,bool default_no_throw,bool copy_no_throw> struct SetNoThrowFlags;

struct GetNoThrowFlagsBase;

template <class T> struct GetNoThrowFlags;

/* struct SetNoThrowFlags<T,bool default_no_throw,bool copy_no_throw> */

template <class T,bool default_no_throw,bool copy_no_throw>
struct SetNoThrowFlags
 {
  template <class S>
  static constexpr bool Default_no_throw() { return Meta::SetNoThrowFlagsWarning<T,S> && default_no_throw ; }

  template <class S>
  static constexpr bool Copy_no_throw() { return Meta::SetNoThrowFlagsWarning<T,S> && copy_no_throw ; }
 };

/* struct GetNoThrowFlagsBase */

struct GetNoThrowFlagsBase
 {
  // default

  template <class T>
  static constexpr bool GetDefault(int)
   {
    return Meta::BadNoThrowFlag<T, !std::is_default_constructible<T>::value || std::is_nothrow_default_constructible<T>::value >;
   }

  template <class T,bool Ret=T::template Default_no_throw<T>()>
  static constexpr bool GetDefault(NothingType) { return Ret; }

  // copy

  template <class T>
  static constexpr bool GetCopy(int)
   {
    return Meta::BadNoThrowFlag<T, !std::is_copy_constructible<T>::value || std::is_nothrow_copy_constructible<T>::value >;
   }

  template <class T,bool Ret=T::template Copy_no_throw<T>()>
  static constexpr bool GetCopy(NothingType) { return Ret; }
 };

/* struct GetNoThrowFlags<T> */

template <class T>
struct GetNoThrowFlags : GetNoThrowFlagsBase
 {
  enum NoThrowFlagType
   {
    Default_no_throw = GetDefault<T>(Nothing),
    Copy_no_throw = GetCopy<T>(Nothing)
   };
 };

} // namespace CCore

#endif


