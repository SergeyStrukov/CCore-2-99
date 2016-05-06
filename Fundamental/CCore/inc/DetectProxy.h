/* DetectProxy.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: Fundamental Mini
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2015 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_DetectProxy_h
#define CCore_inc_DetectProxy_h

#include <CCore/inc/Gadget.h>

namespace CCore {

/* classes */

struct ProbeSet_ProxyType;

struct ProbeSet_OptType;

/* struct ProbeSet_ProxyType */

struct ProbeSet_ProxyType
 {
  template <class T,class C=typename T::ProxyType> struct Condition;
 };

/* const Has_ProxyType<T> */

template <class T>
const bool Has_ProxyType = Meta::Detect<ProbeSet_ProxyType,T> ;

/* struct ProbeSet_OptType */

struct ProbeSet_OptType
 {
  template <class T,class C=typename T::OptType> struct Condition;
 };

/* const Has_OptType<T> */

template <class T>
const bool Has_OptType = Meta::Detect<ProbeSet_OptType,T> ;

/* const ProxySwitch<T> */

template <class T>
const int ProxySwitch = Has_ProxyType<T>?(Has_OptType<T>?2:1):0 ;

} // namespace CCore

#endif


