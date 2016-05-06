/* PrintProxy.h */
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

#ifndef CCore_inc_printf_PrintProxy_h
#define CCore_inc_printf_PrintProxy_h

#include <CCore/inc/printf/StrPrint.h>
#include <CCore/inc/printf/IntPrint.h>

namespace CCore {

/* classes */

struct StrPrintProxy;

template <class UInt> struct UIntPrintProxy;

template <class SInt> struct SIntPrintProxy;

template <class T> struct PrintProxy_other;

template <bool is_UInt,bool is_SInt,class T> struct PrintProxies_noclass;

template <class T> struct PrintProxy_noclass;

struct ProbeSet_PrintOptType;

struct ProbeSet_PrintProxyType;

template <bool has_PrintOptType,class T> struct PrintProxies_class_noproxy;

template <bool has_PrintProxyType,class T> struct PrintProxies_class;

template <bool is_class,class T> struct PrintProxies;

template <class T> struct PrintProxy;

/* struct StrPrintProxy */

struct StrPrintProxy
 {
  using OptType = StrPrintOpt ;
  using ProxyType = StrPrint ;
 };

/* struct UIntPrintProxy<UInt> */

template <class UInt>
struct UIntPrintProxy
 {
  using OptType = IntPrintOpt ;
  using ProxyType = UIntPrint<UInt> ;
 };

/* struct SIntPrintProxy<SInt> */

template <class SInt>
struct SIntPrintProxy
 {
  using OptType = IntPrintOpt ;
  using ProxyType = SIntPrint<SInt> ;
 };

/* struct PrintProxy_other<T> */

template <class T>
struct PrintProxy_other
 {
 };

/* struct PrintProxies_noclass<bool is_UInt,bool is_SInt,T> */

template <class T>
struct PrintProxies_noclass<true,false,T> : UIntPrintProxy<T> {};

template <class T>
struct PrintProxies_noclass<false,true,T> : SIntPrintProxy<T> {};

template <class T>
struct PrintProxies_noclass<false,false,T> : PrintProxy_other<T> {};

/* struct PrintProxy_noclass<T> */

template <class T>
struct PrintProxy_noclass : PrintProxies_noclass<Meta::IsUInt<T>,Meta::IsSInt<T>,T> {};

/* struct ProbeSet_PrintOptType */

struct ProbeSet_PrintOptType
 {
  template <class T,class C=typename T::PrintOptType> struct Condition;
 };

/* const Has_PrintOptType<T> */

template <class T>
const bool Has_PrintOptType = Meta::Detect<ProbeSet_PrintOptType,T> ;

/* struct ProbeSet_PrintProxyType */

struct ProbeSet_PrintProxyType
 {
  template <class T,class C=typename T::PrintProxyType> struct Condition;
 };

/* const Has_PrintProxyType<T> */

template <class T>
const bool Has_PrintProxyType = Meta::Detect<ProbeSet_PrintProxyType,T> ;

/* struct PrintProxies_class_noproxy<bool has_PrintOptType,T> */

template <class T>
struct PrintProxies_class_noproxy<true,T>
 {
  using OptType = typename T::PrintOptType ;

  using ProxyType = const T & ;
 };

template <class T>
struct PrintProxies_class_noproxy<false,T>
 {
  using ProxyType = const T & ;
 };

/* struct PrintProxies_class<bool has_PrintProxyType,T> */

template <class T>
struct PrintProxies_class<true,T> : PrintProxy<typename T::PrintProxyType> {};

template <class T>
struct PrintProxies_class<false,T> : PrintProxies_class_noproxy<Has_PrintOptType<T>,T> {};

/* struct PrintProxies<bool is_class,T> */

template <class T>
struct PrintProxies<true,T> : PrintProxies_class<Has_PrintProxyType<T>,T> {};

template <class T>
struct PrintProxies<false,T> : PrintProxy_noclass<T> {};

/* struct PrintProxy<T> */

template <class T>
struct PrintProxy : PrintProxies<Meta::IsClass<T>,T> {};

template <>
struct PrintProxy<char *> : StrPrintProxy {};

template <>
struct PrintProxy<const char *> : StrPrintProxy {};

template <ulen N>
struct PrintProxy<char [N]> : StrPrintProxy {};

template <ulen N>
struct PrintProxy<const char [N]> : StrPrintProxy {};

template <>
struct PrintProxy<StrLen> : StrPrintProxy {};

} // namespace CCore

#endif


