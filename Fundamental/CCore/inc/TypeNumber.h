/* TypeNumber.h */
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

#ifndef CCore_inc_TypeNumber_h
#define CCore_inc_TypeNumber_h

#include <CCore/inc/PlanInit.h>
#include <CCore/inc/Unid.h>

namespace CCore {

/* GetPlanInitNode_...() */

PlanInitNode * GetPlanInitNode_TypeNumber();

/* classes */

class TypeNumber;

template <class T> class TypeNumberOf;

/* class TypeNumber */

class TypeNumber
 {
   ulen number;

   static bool OneOf(TypeNumber) { return false; }

   static bool OneOf(TypeNumber tn,TypeNumber tn1) { return tn==tn1; }

   static bool OneOf(TypeNumber tn,TypeNumber tn1,TypeNumber tn2) { return tn==tn1 || tn==tn2 ; }

   static bool OneOf(TypeNumber tn,TypeNumber tn1,TypeNumber tn2,TypeNumber tn3) { return tn==tn1 || tn==tn2 || tn==tn3 ; }

   template <class T,class ... TT>
   static bool OneOf(TypeNumber tn,T t,TT ... tt) { return tn==t || OneOf(tn,tt...) ; }

  protected:

   static UnidRegister & GetRegister();

   explicit TypeNumber(ulen number_) : number(number_) {}

  public:

   ulen get() const { return number; }

   bool operator == (TypeNumber obj) const { return number==obj.number; }

   bool operator != (TypeNumber obj) const { return number!=obj.number; }

   template <class ... TT>
   bool oneOf() const { return OneOf(*this,TypeNumberOf<TT>()...); }
 };

/* class TypeNumberOf<T> */

template <class T>
class TypeNumberOf : public TypeNumber
 {
   class Init
    {
      ulen number;

     public:

      Init() : number(GetRegister().template getTypeId<T>()) {}

      operator ulen() const { return number; }
    };

   static Init Number;

  public:

   TypeNumberOf() : TypeNumber(Number) {}
 };

template <class T>
typename TypeNumberOf<T>::Init TypeNumberOf<T>::Number CCORE_INITPRI_3 ;

} // namespace CCore

#endif


