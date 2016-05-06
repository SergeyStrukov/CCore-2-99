/* UIntFunc.h */
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

#ifndef CCore_inc_gadget_UIntFunc_h
#define CCore_inc_gadget_UIntFunc_h

#include <CCore/inc/gadget/Meta.h>

#include <CCore/inc/base/Quick.h>

namespace CCore {

/* const MaxUInt<UInt> */

template <class UInt,class=Meta::EnableIf< Meta::IsUInt<UInt> > >
const UInt MaxUInt = UInt(-1) ;

/* functions */

inline constexpr unsigned Bit(unsigned num) { return 1u<<num; }

template <class UInt>
constexpr Meta::EnableIf< Meta::IsUInt<UInt> , UInt > UIntBit(unsigned num) { return UInt(1)<<num; }

/* classes */

template <class UInt,class ExtUInt> struct UIntMulFunc_double;

template <class UInt,class ExtUInt> struct UIntMulFunc_exact;

template <class UInt> struct UIntBitFunc;

template <class UInt> struct UIntBitFunc_quick;

template <class UInt,class=Meta::EnableIf< Meta::IsUInt<UInt> > > struct UIntFunc;

/* struct UIntMulFunc_double<UInt,ExtUInt> */

template <class UInt,class ExtUInt>
struct UIntMulFunc_double
 {
  static_assert( 2*Meta::UIntBits<UInt> <= Meta::UIntBits<ExtUInt> ,"CCore::UIntMulFunc_double<UInt,ExtUInt> : ExtUInt must have the double bitsize than UInt");

  static const unsigned Bits = Meta::UIntBits<UInt> ;

  static ExtUInt Combine(UInt hi,UInt lo)
   {
    return (ExtUInt(hi)<<Bits)|lo;
   }

  struct Mul
   {
    UInt hi;
    UInt lo;

    Mul(UInt a,UInt b)
     {
      ExtUInt p=(ExtUInt)a*b;

      lo=UInt(p);
      hi=UInt(p>>Bits);
     }
   };

  static UInt Div(UInt hi,UInt lo,UInt den)
   {
    return UInt(Combine(hi,lo)/den);
   }

  static UInt Mod(UInt hi,UInt lo,UInt den)
   {
    return UInt(Combine(hi,lo)%den);
   }

  static UInt MulDiv(UInt a,UInt b,UInt den)
   {
    ExtUInt p=(ExtUInt)a*b;

    return UInt(p/den);
   }

  struct DivMod
   {
    UInt div;
    UInt mod;

    DivMod(UInt hi,UInt lo,UInt den)
     {
      ExtUInt p=Combine(hi,lo);

      div=UInt(p/den);
      mod=UInt(p%den);
     }
   };

  static UInt ModMul(UInt a,UInt b,UInt mod)
   {
    ExtUInt p=(ExtUInt)a*b;

    return UInt(p%mod);
   }

  static UInt ModMac(UInt s,UInt a,UInt b,UInt mod)
   {
    ExtUInt p=(ExtUInt)a*b+s;

    return UInt(p%mod);
   }
 };

/* struct UIntMulFunc_exact<UInt,ExtUInt> */

template <class UInt,class ExtUInt>
struct UIntMulFunc_exact
 {
  static_assert( Meta::UIntBits<UInt> == Meta::UIntBits<ExtUInt> ,"CCore::UIntMulFunc_double<UInt,ExtUInt> : ExtUInt must have the same bitsize as UInt");

  using Algo = Quick::UIntMulFunc<ExtUInt> ;

  struct Mul
   {
    UInt hi;
    UInt lo;

    Mul(UInt a,UInt b)
     {
      typename Algo::Mul res(a,b);

      hi=res.hi;
      lo=res.lo;
     }
   };

  static UInt Div(UInt hi,UInt lo,UInt den)
   {
    return Algo::Div(hi,lo,den);
   }

  static UInt Mod(UInt hi,UInt lo,UInt den)
   {
    return Algo::Mod(hi,lo,den);
   }

  static UInt MulDiv(UInt a,UInt b,UInt den)
   {
    return Algo::MulDiv(a,b,den);
   }

  struct DivMod
   {
    UInt div;
    UInt mod;

    DivMod(UInt hi,UInt lo,UInt den)
     {
      typename Algo::DivMod res(hi,lo,den);

      div=res.div;
      mod=res.mod;
     }
   };

  static UInt ModMul(UInt a,UInt b,UInt mod)
   {
    return Algo::ModMul(a,b,mod);
   }

  static UInt ModMac(UInt s,UInt a,UInt b,UInt mod)
   {
    return Algo::ModMac(s,a,b,mod);
   }
 };

/* struct UIntBitFunc<UInt> */

template <class UInt>
struct UIntBitFunc
 {
  static const unsigned Bits = Meta::UIntBits<UInt> ;

  static const UInt MaxUnsigned = MaxUInt<UInt> ;

  static const UInt MSBit = MaxUnsigned^(MaxUnsigned>>1) ;

  static const unsigned ScanBits = Meta::UIntBits<Quick::ScanUInt> ;

  static unsigned CountZeroMSB(UInt a)
   {
    if( !a ) return Bits;

    if( a&MSBit ) return 0;

    unsigned ret=0;
    unsigned n=Bits;

    while( n>ScanBits )
      {
       unsigned m=n/2;

       UInt mask=MaxUnsigned<<(Bits-m);

       if( a&mask )
         {
          n=m;
         }
       else
         {
          ret+=m;
          a<<=m;
          n-=m;
         }
      }

    unsigned msbit=Quick::ScanMSBit(Quick::ScanUInt( a>>(Bits-n) ));

    return ret+(n-1-msbit);
   }

  static unsigned CountZeroLSB(UInt a)
   {
    if( !a ) return Bits;

    if( a&UInt(1) ) return 0;

    unsigned ret=0;
    unsigned n=Bits;

    while( n>ScanBits )
      {
       unsigned m=n/2;

       UInt mask=(UInt(1)<<m)-1;

       if( a&mask )
         {
          n=m;
         }
       else
         {
          ret+=m;
          a>>=m;
          n-=m;
         }
      }

    unsigned lsbit=Quick::ScanLSBit(Quick::ScanUInt(a));

    return ret+lsbit;
   }

  static unsigned BitsOf(UInt a) { return Bits-CountZeroMSB(a); }
 };

/* struct UIntBitFunc_quick<UInt> */

template <class UInt>
struct UIntBitFunc_quick
 {
  static const unsigned Bits = Meta::UIntBits<UInt> ;

  static unsigned CountZeroMSB(UInt a)
   {
    if( !a ) return Bits;

    return Bits-1-Quick::ScanMSBit(a);
   }

  static unsigned CountZeroLSB(UInt a)
   {
    if( !a ) return Bits;

    return Quick::ScanLSBit(a);
   }

  static unsigned BitsOf(UInt a)
   {
    if( !a ) return 0;

    return Quick::ScanMSBit(a)+1;
   }
 };

/* struct UIntFunc<UInt> */

template <class UInt,class>
struct UIntFunc : Meta::SelectBuild2< Quick::UIntMulSelect< Meta::UIntBits<UInt> >::IsDoubleType , UIntMulFunc_double ,
                                                                                                   UIntMulFunc_exact ,
                                                                                                   UInt , typename Quick::UIntMulSelect< Meta::UIntBits<UInt> >::ExtType >,
                  Meta::SelectBuild1<( MaxUInt<UInt><=MaxUInt<Quick::ScanUInt> ), UIntBitFunc_quick , UIntBitFunc , UInt >
 {
  // consts

  static const unsigned Bits = Meta::UIntBits<UInt> ;

  static const UInt MaxUnsigned = MaxUInt<UInt> ;

  static const UInt MSBit = MaxUnsigned^(MaxUnsigned>>1) ;

  static const UInt MaxPositive = MaxUnsigned>>1 ;

  static const UInt MinNegative = MSBit ;

  // sign

  static UInt Neg(UInt value) { return UInt(-value); }

  static bool IsNegative(UInt value) { return value&MSBit; }

  static bool NotNegative(UInt value) { return !(value&MSBit); }

  static bool IsPositive(UInt value) { return UInt(value-1)<=(MaxPositive-1); }

  static bool NotPositive(UInt value) { return UInt(value-1)>(MaxPositive-1); }

  // additive

  struct Add
   {
    UInt result;
    bool carry;

    Add(UInt a,UInt b)
     {
      result=a+b;
      carry=(result<a);
     }
   };

  struct Sub
   {
    UInt result;
    bool borrow;

    Sub(UInt a,UInt b)
     {
      result=a-b;
      borrow=(a<b);
     }
   };

  struct Inc
   {
    UInt result;
    bool carry;

    explicit Inc(UInt a)
     {
      result=a+1;
      carry=(result==0);
     }
   };

  struct Dec
   {
    UInt result;
    bool borrow;

    explicit Dec(UInt a)
     {
      result=a-1;
      borrow=(a==0);
     }
   };

  // square root

  static UInt SqRoot(UInt S,UInt x) // S,x > 0
   {
    {
     UInt y=S/x;

     if( y<x )
       {
        x=y+(x-y)/2;
       }
     else
       {
        x=x+(y-x)/2;
       }
    }

    for(;;)
      {
       UInt y=S/x;

       if( y<x )
         {
          x=y+(x-y)/2;
         }
       else
         {
          if( y==x ) return x;

          if( y==x+1 && S==x*y ) return x;

          return x+1;
         }
      }
   }

  static UInt SqRoot(UInt S)
   {
    if( !S ) return 0;

    unsigned bits=UIntFunc<UInt>::BitsOf(S);

    UInt x=UInt(1)<<((bits+1)/2);

    return SqRoot(S,x);
   }
 };

/* functions */

template <class UInt>
Meta::EnableIf< Meta::IsUInt<UInt> , UInt > UIntNeg(UInt a)
 {
  return UIntFunc<UInt>::Neg(a);
 }

template <class UInt>
Meta::EnableIf< Meta::IsUInt<UInt> , unsigned > UIntBitsOf(UInt a)
 {
  return UIntFunc<UInt>::BitsOf(a);
 }

template <class UInt>
Meta::EnableIf< Meta::IsUInt<UInt> , bool > UIntAdd(UInt &a,UInt b)
 {
  typename UIntFunc<UInt>::Add add(a,b);

  a=add.result;

  return add.carry;
 }

template <class UInt>
Meta::EnableIf< Meta::IsUInt<UInt> , bool > UIntSub(UInt &a,UInt b)
 {
  typename UIntFunc<UInt>::Sub sub(a,b);

  a=sub.result;

  return sub.borrow;
 }

template <class UInt>
Meta::EnableIf< Meta::IsUInt<UInt> , bool > UIntInc(UInt &a)
 {
  typename UIntFunc<UInt>::Inc inc(a);

  a=inc.result;

  return inc.carry;
 }

template <class UInt>
Meta::EnableIf< Meta::IsUInt<UInt> , bool > UIntDec(UInt &a)
 {
  typename UIntFunc<UInt>::Dec dec(a);

  a=dec.result;

  return dec.borrow;
 }

template <class UInt>
Meta::EnableIf< Meta::IsUInt<UInt> , UInt > UIntMulDiv(UInt a,UInt b,UInt den)
 {
  return UIntFunc<UInt>::MulDiv(a,b,den);
 }

template <class UInt>
Meta::EnableIf< Meta::IsUInt<UInt> , UInt > UIntDiv(UInt hi,UInt lo,UInt den)
 {
  return UIntFunc<UInt>::Div(hi,lo,den);
 }

template <class UInt>
Meta::EnableIf< Meta::IsUInt<UInt> , UInt > UIntMod(UInt hi,UInt lo,UInt den)
 {
  return UIntFunc<UInt>::Mod(hi,lo,den);
 }

template <class UInt>
Meta::EnableIf< Meta::IsUInt<UInt> , UInt > UIntModMul(UInt a,UInt b,UInt mod)
 {
  return UIntFunc<UInt>::ModMul(a,b,mod);
 }

template <class UInt>
Meta::EnableIf< Meta::IsUInt<UInt> , UInt > UIntModMac(UInt s,UInt a,UInt b,UInt mod)
 {
  return UIntFunc<UInt>::ModMac(s,a,b,mod);
 }

template <class UInt>
Meta::EnableIf< Meta::IsUInt<UInt> , UInt > UIntSqRoot(UInt S)
 {
  return UIntFunc<UInt>::SqRoot(S);
 }

} // namespace CCore

#endif


