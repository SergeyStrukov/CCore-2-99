/* Classification.h */
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
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_gadget_Classification_h
#define CCore_inc_gadget_Classification_h

#include <CCore/inc/gadget/Meta.h>

namespace CCore {

/* simple concepts */

template <class T> concept bool AnyType = true ;

template <class T> concept bool NoType = false ;


template <class T> concept bool UIntType = Meta::IsUInt<T> ;

template <class T> concept bool SIntType = Meta::IsSInt<T> ;

template <class T> concept bool S2sIntType = Meta::IsSInt<T> && Is2sComplementArithmetic ;

template <class T> concept bool SUIntType = Meta::IsSUInt<T> ;


template <class T> concept bool VoidType = Meta::OneOf<T,void,const void> ;

template <class T> concept bool CharType = Meta::OneOf<T,char,const char,unsigned char,const unsigned char> ;

template <class T> concept bool ConstCharType = Meta::OneOf<T,const char,const unsigned char> ;

template <class T> concept bool ULenType = Meta::IsSame<T,ulen> ;


template <class T> concept bool EnumType = Meta::IsEnum<T> ;

template <class T> concept bool ClassType = Meta::IsClass<T> ;

template <class T> concept bool PODType = Meta::IsPOD<T> ;


template <class T> concept bool NothrowDtorType = Meta::HasNothrowDtor<T> ;

template <class T> concept bool TrivDtorType = Meta::HasTrivDtor<T> ;

template <class T> concept bool MovableType = Meta::IsMovable<T> ;

template <class T> concept bool CopyableType = Meta::IsCopyable<T> ;

/* complex concepts */

/* concept FuncType<Func,R,AA> */

template <class Func,class R,class ... AA>
concept bool FuncType = requires(Func func,AA ... aa) { { func(aa...) } -> R ; } ;

/* concept FuncArgType<Func,AA> */

template <class Func,class ... AA>
concept bool FuncArgType = requires(Func func,AA ... aa) { func(aa...); } ;

/* concept OpLessType<T> */

template <class T> concept bool OpLessType = requires(Meta::ToConst<T> &a,Meta::ToConst<T> &b) { { a < b } -> bool ; } ;

/* concept OpEqualType<T> */

template <class T> concept bool OpEqualType = requires(Meta::ToConst<T> &a,Meta::ToConst<T> &b) { { a == b } -> bool ; } ;

/* concept OpNotEqualType<T> */

template <class T> concept bool OpNotEqualType = requires(Meta::ToConst<T> &a,Meta::ToConst<T> &b) { { a != b } -> bool ; } ;

/* concept RangeAccessType<T> */

template <class T,class T1,class T2> requires ( Meta::IsSame<T1,T2> && Meta::OneOf<T1,T,const T> )
void RangeAccessHelper(T *,T1 *,T2 *,ULenType) {}

template <class T>
concept bool RangeAccessType = requires(T &obj,Meta::ToConst<T> &cobj)
 {
  RangeAccessHelper(obj.getPtr(),cobj.getPtr_const(),cobj.getPtr(),cobj.getLen());
 } ;

} // namespace CCore

#endif

