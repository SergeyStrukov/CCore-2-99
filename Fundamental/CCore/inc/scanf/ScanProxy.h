/* ScanProxy.h */
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

#ifndef CCore_inc_ScanProxy_h
#define CCore_inc_ScanProxy_h

#include <CCore/inc/scanf/IntScan.h>
#include <CCore/inc/scanf/StringScan.h>

namespace CCore {

/* classes */

template <class UInt> struct UIntScanProxy;

template <class SInt> struct SIntScanProxy;

template <class T> struct ScanProxy_other;

template <bool is_UInt,bool is_SInt,class T> struct ScanProxies_noclass;

template <class T> struct ScanProxy_noclass;

struct ProbeSet_ScanOptType;

struct ProbeSet_ScanProxyType;

template <bool has_ScanOptType,class T> struct ScanProxies_class_noproxy;

template <bool has_ScanProxyType,class T> struct ScanProxies_class;

template <bool is_class,class T> struct ScanProxies;

template <class T> struct ScanProxy;

/* struct UIntScanProxy<UInt> */

template <class UInt>
struct UIntScanProxy
 {
  using OptType = IntScanOpt ;

  using ProxyType = UIntScan<UInt> ;
 };

/* struct SIntScanProxy<SInt> */

template <class SInt>
struct SIntScanProxy
 {
  using OptType = IntScanOpt ;

  using ProxyType = SIntScan<SInt> ;
 };

/* struct ScanProxy_other<T> */

template <class T>
struct ScanProxy_other
 {
 };

/* struct ScanProxies_noclass<bool is_UInt,bool is_SInt,T> */

template <class T>
struct ScanProxies_noclass<true,false,T> : UIntScanProxy<T> {};

template <class T>
struct ScanProxies_noclass<false,true,T> : SIntScanProxy<T> {};

template <class T>
struct ScanProxies_noclass<false,false,T> : ScanProxy_other<T> {};

/* struct ScanProxy_noclass<T> */

template <class T>
struct ScanProxy_noclass : ScanProxies_noclass<Meta::IsUInt<T>,Meta::IsSInt<T>,T> {};

/* struct ProbeSet_ScanOptType */

struct ProbeSet_ScanOptType
 {
  template <class T,class C=typename T::ScanOptType> struct Condition;
 };

/* const Has_ScanOptType<T> */

template <class T>
const bool Has_ScanOptType = Meta::Detect<ProbeSet_ScanOptType,T> ;

/* struct ProbeSet_ScanProxyType */

struct ProbeSet_ScanProxyType
 {
  template <class T,class C=typename T::ScanProxyType> struct Condition;
 };

/* const Has_ScanProxyType<T> */

template <class T>
const bool Has_ScanProxyType = Meta::Detect<ProbeSet_ScanProxyType,T> ;

/* struct ScanProxies_class_noproxy<bool has_ScanOptType,T> */

template <class T>
struct ScanProxies_class_noproxy<true,T>
 {
  using OptType = typename T::ScanOptType ;

  using ProxyType = T & ;
 };

template <class T>
struct ScanProxies_class_noproxy<false,T>
 {
  using ProxyType = T & ;
 };

/* struct ScanProxies_class<bool has_ScanProxyType,T> */

template <class T>
struct ScanProxies_class<true,T> : ScanProxy<typename T::ScanProxyType> {};

template <class T>
struct ScanProxies_class<false,T> : ScanProxies_class_noproxy<Has_ScanOptType<T>,T> {};

/* struct ScanProxies<bool is_class,T> */

template <class T>
struct ScanProxies<true,T> : ScanProxies_class<Has_ScanProxyType<T>,T> {};

template <class T>
struct ScanProxies<false,T> : ScanProxy_noclass<T> {};

/* struct ScanProxy<T> */

template <class T>
struct ScanProxy : ScanProxies<Meta::IsClass<T>,T> {};

template <>
struct ScanProxy<String>
 {
  using OptType = StringScanOpt ;

  using ProxyType = StringScan ;
 };

} // namespace CCore

#endif


