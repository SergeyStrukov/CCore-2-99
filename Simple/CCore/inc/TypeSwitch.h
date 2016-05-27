/* TypeSwitch.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 3.00
//
//  Tag: Simple
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2015 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_TypeSwitch_h
#define CCore_inc_TypeSwitch_h

#include <CCore/inc/Gadget.h>

namespace CCore {
namespace Meta {

/* classes */

template <SUIntType SUInt,SUInt Val,class T> struct Case;

/* struct Case<SUInt,SUInt Val,class T> */

template <SUIntType SUInt,SUInt Val_,class T>
struct Case
 {
  using SwType = SUInt ;

  static const SwType Val = Val_ ;

  using Type = T ;
 };

//----------------------------------------------------------------------------------------

/* const IsCaseType<C> */

template <class C>
const bool IsCaseType = false ;

template <SUIntType SUInt,SUInt Val,class T>
const bool IsCaseType<Case<SUInt,Val,T> > = true ;

/* concept CaseType<C> */

template <class C>
concept bool CaseType = IsCaseType<C> ;

/* classes */

template <class Box> struct CaseListBoxCtor;

template <CaseType ... CC> struct CaseList;

/* struct CaseListBoxCtor<Box> */

template <CaseType ... CC>
struct CaseListBoxCtor<TypeListBox<CC...> >
 {
  using Ret = CaseList<CC...> ;
 };

/* type CaseListBox<Box> */

template <class Box>
using CaseListBox = typename CaseListBoxCtor<Box>::Ret ;

/* struct CaseList<CC> */

template <CaseType ... CC>
struct CaseList
 {
  static const unsigned Len = sizeof ... (CC) ;

  using Split = SplitTypeList<Len/2,CC...> ;

  using First = CaseListBox<typename Split::First> ;
  using Last  = CaseListBox<typename Split::Last> ;

  using SwType = CommonType<typename First::SwType,typename Last::SwType> ;
 };

template <>
struct CaseList<>
 {
  static const unsigned Len = 0 ;
 };

template <class C1>
struct CaseList<C1>
 {
  static const unsigned Len = 1 ;

  using SwType = typename C1::SwType ;
 };

template <class C1,class C2>
struct CaseList<C1,C2>
 {
  static const unsigned Len = 2 ;

  using First = CaseList<C1> ;
  using Last  = CaseList<C2> ;

  using SwType = CommonType<typename First::SwType,typename Last::SwType> ;
 };

//----------------------------------------------------------------------------------------

/* const IsCaseListType<CaseList> */

template <class CaseList>
const bool IsCaseListType = false ;

template <CaseType ... CC>
const bool IsCaseListType<CaseList<CC...> > = true ;

/* concept CaseListType<CaseList> */

template <class CaseList>
concept bool CaseListType = IsCaseListType<CaseList> ;

/* classes */

template <CaseListType CaseList> struct TypeSwitch;

template <CaseListType CaseList,class T> struct CaseValCtor;

template <CaseListType CaseList> struct CaseListMaxSizeofCtor;

/* struct TypeSwitch<CaseList> */

template <CaseListType CaseList>
struct TypeSwitch
 {
  using SwType = typename CaseList::SwType ;
  using First = typename CaseList::First ;
  using Last = typename CaseList::Last ;

  template <class Ctx,class RetType_>
  struct Tail
   {
    using RetType = RetType_ ;

    Ctx &ctx;

    explicit Tail(Ctx &ctx_) : ctx(ctx_) {}

    template <class T>
    RetType call() { return ctx.template call<T>(); }

    RetType defcall(SwType val)
     {
      return TypeSwitch<Last>::template Switch<Ctx &,RetType>(val,ctx);
     }
   };

  template <class Ctx,class RetType=typename Ctx::RetType>
  static RetType Switch(SwType val,Ctx ctx)
   {
    return TypeSwitch<First>::Switch(val,Tail<Ctx,RetType>(ctx));
   }
 };

template <>
struct TypeSwitch<CaseList<> >
 {
  template <SUIntType SUInt,class Ctx,class RetType=typename Ctx::RetType>
  static RetType Switch(SUInt val,Ctx ctx)
   {
    return ctx.defcall(val);
   }
 };

template <SUIntType SUInt,SUInt Val1,class T1>
struct TypeSwitch<CaseList<Case<SUInt,Val1,T1> > >
 {
  template <class Ctx,class RetType=typename Ctx::RetType>
  static RetType Switch(SUInt val,Ctx ctx)
   {
    switch( val )
      {
       case Val1 : return ctx.template call<T1>();

       default: return ctx.defcall(val);
      }
   }
 };

template <SUIntType SUInt,SUInt Val1,class T1,
                          SUInt Val2,class T2>
struct TypeSwitch<CaseList<Case<SUInt,Val1,T1>,
                           Case<SUInt,Val2,T2> > >
 {
  template <class Ctx,class RetType=typename Ctx::RetType>
  static RetType Switch(SUInt val,Ctx ctx)
   {
    switch( val )
      {
       case Val1 : return ctx.template call<T1>();
       case Val2 : return ctx.template call<T2>();

       default: return ctx.defcall(val);
      }
   }
 };

template <SUIntType SUInt,SUInt Val1,class T1,
                          SUInt Val2,class T2,
                          SUInt Val3,class T3>
struct TypeSwitch<CaseList<Case<SUInt,Val1,T1>,
                           Case<SUInt,Val2,T2>,
                           Case<SUInt,Val3,T3> > >
 {
  template <class Ctx,class RetType=typename Ctx::RetType>
  static RetType Switch(SUInt val,Ctx ctx)
   {
    switch( val )
      {
       case Val1 : return ctx.template call<T1>();
       case Val2 : return ctx.template call<T2>();
       case Val3 : return ctx.template call<T3>();

       default: return ctx.defcall(val);
      }
   }
 };

template <SUIntType SUInt,SUInt Val1,class T1,
                          SUInt Val2,class T2,
                          SUInt Val3,class T3,
                          SUInt Val4,class T4>
struct TypeSwitch<CaseList<Case<SUInt,Val1,T1>,
                           Case<SUInt,Val2,T2>,
                           Case<SUInt,Val3,T3>,
                           Case<SUInt,Val4,T4> > >
 {
  template <class Ctx,class RetType=typename Ctx::RetType>
  static RetType Switch(SUInt val,Ctx ctx)
   {
    switch( val )
      {
       case Val1 : return ctx.template call<T1>();
       case Val2 : return ctx.template call<T2>();
       case Val3 : return ctx.template call<T3>();
       case Val4 : return ctx.template call<T4>();

       default: return ctx.defcall(val);
      }
   }
 };

template <SUIntType SUInt,SUInt Val1,class T1,
                          SUInt Val2,class T2,
                          SUInt Val3,class T3,
                          SUInt Val4,class T4,
                          SUInt Val5,class T5>
struct TypeSwitch<CaseList<Case<SUInt,Val1,T1>,
                           Case<SUInt,Val2,T2>,
                           Case<SUInt,Val3,T3>,
                           Case<SUInt,Val4,T4>,
                           Case<SUInt,Val5,T5> > >
 {
  template <class Ctx,class RetType=typename Ctx::RetType>
  static RetType Switch(SUInt val,Ctx ctx)
   {
    switch( val )
      {
       case Val1 : return ctx.template call<T1>();
       case Val2 : return ctx.template call<T2>();
       case Val3 : return ctx.template call<T3>();
       case Val4 : return ctx.template call<T4>();
       case Val5 : return ctx.template call<T5>();

       default: return ctx.defcall(val);
      }
   }
 };

template <SUIntType SUInt,SUInt Val1,class T1,
                          SUInt Val2,class T2,
                          SUInt Val3,class T3,
                          SUInt Val4,class T4,
                          SUInt Val5,class T5,
                          SUInt Val6,class T6>
struct TypeSwitch<CaseList<Case<SUInt,Val1,T1>,
                           Case<SUInt,Val2,T2>,
                           Case<SUInt,Val3,T3>,
                           Case<SUInt,Val4,T4>,
                           Case<SUInt,Val5,T5>,
                           Case<SUInt,Val6,T6> > >
 {
  template <class Ctx,class RetType=typename Ctx::RetType>
  static RetType Switch(SUInt val,Ctx ctx)
   {
    switch( val )
      {
       case Val1 : return ctx.template call<T1>();
       case Val2 : return ctx.template call<T2>();
       case Val3 : return ctx.template call<T3>();
       case Val4 : return ctx.template call<T4>();
       case Val5 : return ctx.template call<T5>();
       case Val6 : return ctx.template call<T6>();

       default: return ctx.defcall(val);
      }
   }
 };

template <SUIntType SUInt,SUInt Val1,class T1,
                          SUInt Val2,class T2,
                          SUInt Val3,class T3,
                          SUInt Val4,class T4,
                          SUInt Val5,class T5,
                          SUInt Val6,class T6,
                          SUInt Val7,class T7>
struct TypeSwitch<CaseList<Case<SUInt,Val1,T1>,
                           Case<SUInt,Val2,T2>,
                           Case<SUInt,Val3,T3>,
                           Case<SUInt,Val4,T4>,
                           Case<SUInt,Val5,T5>,
                           Case<SUInt,Val6,T6>,
                           Case<SUInt,Val7,T7> > >
 {
  template <class Ctx,class RetType=typename Ctx::RetType>
  static RetType Switch(SUInt val,Ctx ctx)
   {
    switch( val )
      {
       case Val1 : return ctx.template call<T1>();
       case Val2 : return ctx.template call<T2>();
       case Val3 : return ctx.template call<T3>();
       case Val4 : return ctx.template call<T4>();
       case Val5 : return ctx.template call<T5>();
       case Val6 : return ctx.template call<T6>();
       case Val7 : return ctx.template call<T7>();

       default: return ctx.defcall(val);
      }
   }
 };

template <SUIntType SUInt,SUInt Val1,class T1,
                          SUInt Val2,class T2,
                          SUInt Val3,class T3,
                          SUInt Val4,class T4,
                          SUInt Val5,class T5,
                          SUInt Val6,class T6,
                          SUInt Val7,class T7,
                          SUInt Val8,class T8>
struct TypeSwitch<CaseList<Case<SUInt,Val1,T1>,
                           Case<SUInt,Val2,T2>,
                           Case<SUInt,Val3,T3>,
                           Case<SUInt,Val4,T4>,
                           Case<SUInt,Val5,T5>,
                           Case<SUInt,Val6,T6>,
                           Case<SUInt,Val7,T7>,
                           Case<SUInt,Val8,T8> > >
 {
  template <class Ctx,class RetType=typename Ctx::RetType>
  static RetType Switch(SUInt val,Ctx ctx)
   {
    switch( val )
      {
       case Val1 : return ctx.template call<T1>();
       case Val2 : return ctx.template call<T2>();
       case Val3 : return ctx.template call<T3>();
       case Val4 : return ctx.template call<T4>();
       case Val5 : return ctx.template call<T5>();
       case Val6 : return ctx.template call<T6>();
       case Val7 : return ctx.template call<T7>();
       case Val8 : return ctx.template call<T8>();

       default: return ctx.defcall(val);
      }
   }
 };

template <SUIntType SUInt,SUInt Val1,class T1,
                          SUInt Val2,class T2,
                          SUInt Val3,class T3,
                          SUInt Val4,class T4,
                          SUInt Val5,class T5,
                          SUInt Val6,class T6,
                          SUInt Val7,class T7,
                          SUInt Val8,class T8,
                          SUInt Val9,class T9>
struct TypeSwitch<CaseList<Case<SUInt,Val1,T1>,
                           Case<SUInt,Val2,T2>,
                           Case<SUInt,Val3,T3>,
                           Case<SUInt,Val4,T4>,
                           Case<SUInt,Val5,T5>,
                           Case<SUInt,Val6,T6>,
                           Case<SUInt,Val7,T7>,
                           Case<SUInt,Val8,T8>,
                           Case<SUInt,Val9,T9> > >
 {
  template <class Ctx,class RetType=typename Ctx::RetType>
  static RetType Switch(SUInt val,Ctx ctx)
   {
    switch( val )
      {
       case Val1 : return ctx.template call<T1>();
       case Val2 : return ctx.template call<T2>();
       case Val3 : return ctx.template call<T3>();
       case Val4 : return ctx.template call<T4>();
       case Val5 : return ctx.template call<T5>();
       case Val6 : return ctx.template call<T6>();
       case Val7 : return ctx.template call<T7>();
       case Val8 : return ctx.template call<T8>();
       case Val9 : return ctx.template call<T9>();

       default: return ctx.defcall(val);
      }
   }
 };

/* struct CaseValCtor<CaseList,T> */

template <class T,class S,SUIntType SUInt,SUInt Val,CaseType ... CC>
struct CaseValCtor< CaseList< Case<SUInt,Val,S> ,CC...> ,T>
 : Select< IsSame<T,S> , DefConst<SUInt,Val> , CaseValCtor< CaseList<CC...> ,T> > {};

/* const CaseVal<CaseList,T> */

template <CaseListType CaseList,class T>
const auto CaseVal = CaseValCtor<CaseList,T>::Ret ;

/* struct CaseListMaxSizeofCtor<CaseList> */

template <CaseType ... CC>
struct CaseListMaxSizeofCtor< CaseList<CC...> >
 {
  static constexpr ulen Ret = Max_cast( sizeof (typename CC::Type) ... ) ;
 };

/* const CaseListMaxSizeof<CaseList> */

template <CaseListType CaseList>
const ulen CaseListMaxSizeof = CaseListMaxSizeofCtor<CaseList>::Ret ;

} // namespace Meta
} // namespace CCore

#endif


