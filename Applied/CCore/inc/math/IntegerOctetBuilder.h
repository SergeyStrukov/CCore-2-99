/* IntegerOctetBuilder.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: Applied
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2015 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_math_IntegerOctetBuilder_h
#define CCore_inc_math_IntegerOctetBuilder_h

#include <CCore/inc/math/Integer.h>

namespace CCore {
namespace Math {

/* classes */

template <class Integer> class PrintIntegerOctetsType;

template <class Integer> class IntegerOctetBuilder;

template <class Integer> class OctetInteger;

/* class PrintIntegerOctetsType<Integer> */

template <class Integer>
class PrintIntegerOctetsType
 {
   using Unit = typename Integer::Unit ;

   static const ulen Count = Integer::UnitBits/8 ;

   static_assert( (Integer::UnitBits%8)==0 ,"CCore::Math::PrintIntegerOctets<Integer> : bad Integer::UnitBits");

   Integer a;

  public:

   explicit PrintIntegerOctetsType(const Integer &a_) : a(a_) {}

   ~PrintIntegerOctetsType() {}

   template <class P>
   void print(P &out) const
    {
     auto body=a.getBody();

     Printf(out,"\n {\n");

     ulen ind=0;

     for(Unit unit : body )
       {
        for(unsigned cnt=Count; cnt ;cnt--,unit>>=8,ind++)
          {
           uint8 octet=uint8(unit);

           if( ind&15 )
             {
              Putobj(out,", ");

              if( (ind&3)==0 ) Putch(out,' ');
             }
           else
             {
              if( ind==0 )
                Putobj(out,"  ");
              else
                Putobj(out,",\n  ");
             }

           Printf(out,"#4.xi;",octet);
          }
       }

     Printf(out,"\n }");
    }
 };

/* PrintIntegerOctets() */

template <class Integer>
PrintIntegerOctetsType<Integer> PrintIntegerOctets(const Integer &a) { return PrintIntegerOctetsType<Integer>(a); }

/* class IntegerOctetBuilder<Integer> */

template <class Integer>
class IntegerOctetBuilder
 {
   PtrLen<const uint8> data;

   ulen result;

  private:

   using Unit = typename Integer::Unit ;

   static const ulen Delta = Integer::UnitBits/8 ;

   static_assert( (Integer::UnitBits%8)==0 ,"CCore::Math::IntegerOctetBuilder<Integer> : bad Integer::UnitBits");

   static Unit Build(PtrLen<const uint8> data)
    {
     Unit ret=0;

     for(unsigned off=0; +data ;++data,off+=8)
       {
        ret|=(Unit(*data)<<off);
       }

     return ret;
    }

   static void Build(Unit *out,PtrLen<const uint8> data)
    {
     while( data.len>=Delta ) *(out++)=Build(data+=Delta);

     *out=Build(data);
    }

  public:

   //
   // data is an unsigned integer in the lendian octet representation.
   //

   explicit IntegerOctetBuilder(PtrLen<const uint8> data_)
    : data(data_)
    {
     result=LenAdd(data_.len/Delta,1);
    }

   ulen getLen() const { return result; }

   PtrLen<Unit> operator () (Place<void> place) const
    {
     Unit *base=place;

     Build(base,data);

     return Range(base,result);
    }
 };

/* class OctetInteger<Integer> */

template <class Integer>
class OctetInteger : public Integer
 {
  public:

   explicit OctetInteger(PtrLen<const uint8> data) : Integer(DoBuild,IntegerOctetBuilder<Integer>(data)) {}

   ~OctetInteger() {}
 };

} // namespace Math
} // namespace CCore

#endif


