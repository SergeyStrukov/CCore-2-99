/* Meta.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 3.00
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

#ifndef CCore_inc_gadget_Meta_h
#define CCore_inc_gadget_Meta_h

#include <CCore/inc/base/PlatformBase.h>

namespace CCore {
namespace Meta {

/* Const */

template <class T,T Val> const T Const = Val ;

/* classes */

struct Empty;

template <class T> struct DefType;

template <class T,T Val> struct DefConst;

template <int Ind> struct IndexBox;

template <class T> struct TypeBox;

template <class T> struct ToConstCtor;

template <bool Cond,class T1,class T2> struct SelectCtor;

template <class T> struct UIntBitsCtor_extra;

template <class T> struct UIntBitsCtor;

template <class T> struct SIntToUInt_extra;

template <class SInt> struct SIntToUInt;

template <class T> struct PromoteUInt_extra;

template <class UInt> struct PromoteUInt;

template <class T> struct PromoteSInt_extra;

template <class SInt> struct PromoteSInt;

template <class ... TT> struct IndexOfCtor;

template <unsigned Index,class ... TT> struct SelectListCtor;

template <class S,class T> struct CommonTypeCtor;

template <class Split,class ... TT> struct SplitTypeListCtor;

template <class Skip,ulen Count,class ... TT> struct SkipTypeListCtor;

/* struct Empty */

struct Empty
 {
 };

/* struct DefType<T> */

template <class T>
struct DefType
 {
  using Ret = T ;
 };

/* struct DefConst<T,T Val> */

template <class T,T Val>
struct DefConst
 {
  static const T Ret = Val ;
 };

/* struct IndexBox<int Ind> */

template <int Ind>
struct IndexBox
 {
  enum ValueType { Value = Ind };

  constexpr operator int() const { return Ind; }
 };

/* struct TypeBox<T> */

template <class T>
struct TypeBox
 {
  using Type = T ;

  static T Get();
 };

/* struct ToConstCtor<T> */

template <class T>
struct ToConstCtor
 {
  using Ret = const T ;
 };

template <class T>
struct ToConstCtor<T &>
 {
  using Ret = const T & ;
 };

/* type ToConst<T> */

template <class T>
using ToConst = typename ToConstCtor<T>::Ret ;

/* struct SelectCtor<bool Cond,T1,T2> */

template <class T1,class T2>
struct SelectCtor<true,T1,T2>
 {
  using Ret = T1 ;
 };

template <class T1,class T2>
struct SelectCtor<false,T1,T2>
 {
  using Ret = T2 ;
 };

/* type Select<bool Cond,T1,T2> */

template <bool Cond,class T1,class T2>
using Select = typename SelectCtor<Cond,T1,T2>::Ret ;

/* const IsSame<T1,T2> */

template <class T1,class T2>
const bool IsSame = false ;

template <class T>
const bool IsSame<T,T> = true ;

/* const OneOf<T,TT> */

template <class T,class ... TT>
const bool OneOf = ( ... || IsSame<T,TT> ) ;

/* const IsUInt<T> */

template <class T>
const bool IsUInt = ExtraInt::Prop<T>::IsUnsigned ;

template <>
const bool IsUInt<char> = ( char(-1)>=0 ) ;

template <>
const bool IsUInt<unsigned char> = true ;

template <>
const bool IsUInt<unsigned short> = true ;

template <>
const bool IsUInt<unsigned int> = true ;

template <>
const bool IsUInt<unsigned long> = true ;

template <>
const bool IsUInt<unsigned long long> = true ;

/* struct UIntBitsCtor_extra<T> */

template <class T>
struct UIntBitsCtor_extra
 {
  enum RetType { Ret = ExtraInt::Prop<T>::Bits };
 };

/* struct UIntBitsCtor<T> */

template <class T> requires ( ExtraInt::Prop<T>::IsUnsigned )
struct UIntBitsCtor<T> : UIntBitsCtor_extra<T> {};

template <>
struct UIntBitsCtor<unsigned char>
 {
  enum RetType { Ret = Bits::unsigned_char };
 };

template <>
struct UIntBitsCtor<unsigned short>
 {
  enum RetType { Ret = Bits::unsigned_short };
 };

template <>
struct UIntBitsCtor<unsigned int>
 {
  enum RetType { Ret = Bits::unsigned_int };
 };

template <>
struct UIntBitsCtor<unsigned long>
 {
  enum RetType { Ret = Bits::unsigned_long };
 };

template <>
struct UIntBitsCtor<unsigned long long>
 {
  enum RetType { Ret = Bits::unsigned_long_long };
 };

template <>
struct UIntBitsCtor<char> : Select<( char(-1)<0 ), Empty , UIntBitsCtor<unsigned char> > {};

/* const UIntBits<T> */

template <class T>
const unsigned UIntBits = UIntBitsCtor<T>::Ret ;

/* const HexWidth<UInt> */

template <class UInt>
const unsigned HexWidth = (UIntBits<UInt> +3)/4 ;

/* const IsSInt<T> */

template <class T>
const bool IsSInt = ExtraInt::Prop<T>::IsSigned ;

template <>
const bool IsSInt<char> = ( char(-1)<0 ) ;

template <>
const bool IsSInt<signed char> = true ;

template <>
const bool IsSInt<short> = true ;

template <>
const bool IsSInt<int> = true ;

template <>
const bool IsSInt<long> = true ;

template <>
const bool IsSInt<long long> = true ;

/* const IsSUInt<T> */

template <class T>
const bool IsSUInt = IsSInt<T> || IsUInt<T> ;

/* type UnRef<T> */

template <class T>
using UnRef = typename std::remove_reference<T>::type ;

/* type UnConst<T> */

template <class T>
using UnConst = typename std::remove_const<T>::type ;

/* type PtrObjType<Ptr> */

template <class Ptr>
using PtrObjType = UnRef<decltype( * TypeBox<Ptr &>::Get() )> ;

/* const IsEnum<E> */

template <class E>
const bool IsEnum = std::is_enum<E>::value ;

/* const IsClass<T> */

template <class T>
const bool IsClass = std::is_class<T>::value ;

/* const IsPOD<T> */

template <class T>
const bool IsPOD = std::is_pod<T>::value ;

/* const HasNothrowDtor<T> */

template <class T>
const bool HasNothrowDtor = std::is_nothrow_destructible<T>::value ;

/* const HasTrivDtor<T> */

template <class T>
const bool HasTrivDtor = std::is_trivially_destructible<T>::value ;

/* const HasDefaultCtor<T> */

template <class T>
const bool HasDefaultCtor = std::is_default_constructible<T>::value ;

/* const HasNothrowDefaultCtor<T> */

template <class T>
const bool HasNothrowDefaultCtor = std::is_nothrow_default_constructible<T>::value ;

/* const HasCopyCtor<T> */

template <class T>
const bool HasCopyCtor = std::is_copy_constructible<T>::value ;

/* const HasNothrowCopyCtor<T> */

template <class T>
const bool HasNothrowCopyCtor = std::is_nothrow_copy_constructible<T>::value ;

/* const HasMoveCtor<T> */

template <class T>
const bool HasMoveCtor = std::is_move_constructible<T>::value ;

/* const IsCopyable<T> */

template <class T>
const bool IsCopyable = std::is_copy_constructible<T>::value && std::is_copy_assignable<T>::value ;

/* const IsNothrowCopyable<T> */

template <class T>
const bool IsNothrowCopyable = std::is_nothrow_copy_constructible<T>::value && std::is_nothrow_copy_assignable<T>::value ;

/* const IsMovable<T> */

template <class T>
const bool IsMovable = std::is_move_constructible<T>::value && std::is_move_assignable<T>::value ;

/* const IsBaseOf<B,D> */

template <class B,class D>
const bool IsBaseOf = std::is_base_of<B,D>::value ;

/* type AlignedStorage<ulen Len> */

template <ulen Len>
using AlignedStorage = typename std::aligned_storage<Len>::type ;

/* struct SIntToUInt_extra<T> */

template <class T>
struct SIntToUInt_extra
 {
  using SType = typename ExtraInt::Prop<T>::SType ;
  using UType = typename ExtraInt::Prop<T>::UType ;
 };

/* struct SIntToUInt<SInt> */

template <class T> requires ( ExtraInt::Prop<T>::IsSigned )
struct SIntToUInt<T> : SIntToUInt_extra<T> {};

template <>
struct SIntToUInt<signed char>
 {
  using SType =   signed char ;
  using UType = unsigned char ;
 };

template <>
struct SIntToUInt<short>
 {
  using SType =          short ;
  using UType = unsigned short ;
 };

template <>
struct SIntToUInt<int>
 {
  using SType =          int ;
  using UType = unsigned int ;
 };

template <>
struct SIntToUInt<long>
 {
  using SType =          long ;
  using UType = unsigned long ;
 };

template <>
struct SIntToUInt<long long>
 {
  using SType =          long long ;
  using UType = unsigned long long ;
 };

template <>
struct SIntToUInt<char> : Select<( char(-1)<0 ), SIntToUInt<signed char> , Empty > {};

/* struct PromoteUInt_extra<T> */

template <class T>
struct PromoteUInt_extra
 {
  using Type = typename ExtraInt::Prop<T>::PromoteUType ;
 };

/* struct PromoteUInt<UInt> */

template <class T> requires ( ExtraInt::Prop<T>::IsUnsigned )
struct PromoteUInt<T> : PromoteUInt_extra<T> {};

template <>
struct PromoteUInt<unsigned char>
 {
  using Type = unsigned int ;
 };

template <>
struct PromoteUInt<unsigned short>
 {
  using Type = unsigned int ;
 };

template <>
struct PromoteUInt<unsigned int>
 {
  using Type = unsigned int ;
 };

template <>
struct PromoteUInt<unsigned long>
 {
  using Type = unsigned long ;
 };

template <>
struct PromoteUInt<unsigned long long>
 {
  using Type = unsigned long long ;
 };

template <>
struct PromoteUInt<char> : Select<( char(-1)<0 ), Empty , PromoteUInt<unsigned int> > {};

/* struct PromoteSInt_extra<T> */

template <class T>
struct PromoteSInt_extra
 {
  using SType = typename ExtraInt::Prop<T>::PromoteSType ;
  using UType = typename ExtraInt::Prop<T>::PromoteUType ;
 };

/* struct PromoteSInt<SInt> */

template <class T> requires ( ExtraInt::Prop<T>::IsSigned )
struct PromoteSInt<T> : PromoteSInt_extra<T> {};

template <>
struct PromoteSInt<signed char>
 {
  using SType =          int ;
  using UType = unsigned int ;
 };

template <>
struct PromoteSInt<short>
 {
  using SType =          int ;
  using UType = unsigned int ;
 };

template <>
struct PromoteSInt<int>
 {
  using SType =          int ;
  using UType = unsigned int ;
 };

template <>
struct PromoteSInt<long>
 {
  using SType =          long ;
  using UType = unsigned long ;
 };

template <>
struct PromoteSInt<long long>
 {
  using SType =          long long ;
  using UType = unsigned long long ;
 };

template <>
struct PromoteSInt<char> : Select<( char(-1)<0 ), PromoteSInt<int> , Empty > {};

/* struct IndexOfCtor<TT> */

template <class T,class ... TT>
struct IndexOfCtor<T,T,TT...>
 {
  enum RetType : unsigned { Ret = 1 } ;
 };

template <class T,class S,class ... TT>
struct IndexOfCtor<T,S,TT...>
 {
  enum RetType : unsigned { Ret = 1+IndexOfCtor<T,TT...>::Ret } ;
 };

/* const IndexOf<TT> */

template <class ... TT>
const unsigned IndexOf = IndexOfCtor<TT...>::Ret ;

/* struct SelectListCtor<unsigned Index,TT> */

template <unsigned Index,class T,class ... TT>
struct SelectListCtor<Index,T,TT...>
 {
  using Ret = typename SelectListCtor<Index-1,TT...>::Ret ;
 };

template <class T,class ... TT>
struct SelectListCtor<0,T,TT...>
 {
  using Ret = T ;
 };

/* type SelectList<unsigned Index,TT> */

template <unsigned Index,class ... TT>
using SelectList = typename SelectListCtor<Index,TT...>::Ret ;

/* struct CommonTypeCtor<S,T> */

template <class T>
struct CommonTypeCtor<T,T>
 {
  using Ret = T ;
 };

/* type CommonType<S,T> */

template <class S,class T>
using CommonType = typename CommonTypeCtor<S,T>::Ret ;

/* type SplitTypeList<Split,TT> */

template <class Split,class ... TT>
using SplitTypeList = typename SplitTypeListCtor<Split,TT...>::Ret ;

/* struct SplitTypeListCtor<Split,TT> */

template <class Split,class T>
struct SplitTypeListCtor<Split,T>
 {
  using Last = typename Split::template Last<T> ;

  template <class ... SS>
  using Ext = typename Last::template Start<SS...> ;

  using Ret = Ext<> ;
 };

template <class Split,class T,class ... TT>
struct SplitTypeListCtor<Split,T,TT...>
 {
  using Last = typename SplitTypeListCtor<Split,TT...>::Last ;

  template <class ... SS>
  using Ext = typename SplitTypeListCtor<Split,TT...>::template Ext<SS...,T> ;

  using Ret = Ext<> ;
 };

/* type SkipTypeList<Skip,ulen Count,TT> */

template <class Skip,ulen Count,class ... TT>
using SkipTypeList = typename SkipTypeListCtor<Skip,Count,TT...>::Ret ;

/* struct SkipTypeListCtor<Skip,ulen Count,TT> */

template <class Skip>
struct SkipTypeListCtor<Skip,0>
 {
  using Ret = typename Skip::template Ctor<> ;
 };

template <class Skip,class T,class ... TT>
struct SkipTypeListCtor<Skip,0,T,TT...>
 {
  using Ret = typename Skip::template Ctor<T,TT...> ;
 };

template <class Skip,ulen Count,class T,class ... TT>
struct SkipTypeListCtor<Skip,Count,T,TT...>
 {
  using Ret = SkipTypeList<Skip,Count-1,TT...> ;
 };

} // namespace Meta
} // namespace CCore

#endif


