/* IntScan.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 3.00
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

#ifndef CCore_inc_scanf_IntScan_h
#define CCore_inc_scanf_IntScan_h

#include <CCore/inc/scanf/ScanTools.h>

namespace CCore {

/* consts */

const unsigned MinScanIntBase =  2 ;
const unsigned MaxScanIntBase = 16 ;

/* classes */

//enum IntScanBase;

struct IntScanOpt;

template <UIntType UInt> class UIntDigitAcc;

template <SIntType SInt> class SIntDigitAcc;

template <ulen Len> class DetectIntFormat;

template <SUIntType SUInt,class Acc> struct IntScanAlgo;

template <UIntType UInt> class UIntScan;

template <SIntType SInt> class SIntScan;

/* enum IntScanBase */

enum IntScanBase
 {
  IntScanNone,

  IntScanHex,
  IntScanBin,
  IntScan0X,

  IntScanDefault = IntScanNone
 };

template <class Dev>
bool Parse_try(Dev &dev,IntScanBase &ret)
 {
  typename Dev::Peek peek(dev);

  if( !peek )
    {
     return false;
    }
  else
    {
     switch( *peek )
       {
        case 'h' : case 'H' : ret=IntScanHex; ++dev; return true;
        case 'b' : case 'B' : ret=IntScanBin; ++dev; return true;
        case 'x' : case 'X' : ret=IntScan0X;  ++dev; return true;
       }

     return false;
    }
 }

/* struct IntScanOpt */

struct IntScanOpt
 {
  unsigned base; // 2..16, 0 for any
  IntScanBase scan_base;

  void setDefault()
   {
    base=0;
    scan_base=IntScanDefault;
   }

  IntScanOpt() { setDefault(); }

  IntScanOpt(const char *ptr,const char *lim);

  //
  // [.base|.x|.X|.b|.B|.h|.H]
  //
 };

/* class UIntDigitAcc<UInt> */

template <UIntType UInt>
class UIntDigitAcc : NoCopy
 {
   UInt value;

   UInt a;
   UInt b;
   bool minus;

  public:

   static const unsigned Bits = Meta::UIntBits<UInt> ;

   UIntDigitAcc(int dig,unsigned base,bool minus_)
    {
     value=UInt(dig);

     const UInt MaxVal=UIntFunc<UInt>::MaxUnsigned;

     a=UInt(MaxVal/base);
     b=UInt(MaxVal%base);
     minus=minus_;
    }

   bool next(int dig_,unsigned base)
    {
     unsigned dig=(unsigned)dig_;

     if( value>a || ( value==a && dig>b ) ) return false;

     value=UInt(base*value+dig);

     return true;
    }

   using ValueType = UInt ;

   operator UInt() const { return minus?UIntFunc<UInt>::Neg(value):value; }
 };

/* class SIntDigitAcc<SInt> */

template <SIntType SInt>
class SIntDigitAcc : NoCopy
 {
   using UInt = typename SIntFunc<SInt>::UInt ;

   UInt value;

   UInt a;
   UInt b;
   bool minus;

  public:

   static const unsigned Bits = Meta::UIntBits<UInt> ;

   SIntDigitAcc(int dig,unsigned base,bool minus_)
    {
     value=UInt(dig);

     const UInt MaxVal=(minus_?SIntFunc<SInt>::MinNegativeAbs:SIntFunc<SInt>::MaxPositiveAbs);

     a=UInt(MaxVal/base);
     b=UInt(MaxVal%base);
     minus=minus_;
    }

   bool next(int dig_,unsigned base)
    {
     unsigned dig=(unsigned)dig_;

     if( value>a || ( value==a && dig>b ) ) return false;

     value=UInt(base*value+dig);

     return true;
    }

   using ValueType = SInt ;

   operator SInt() const { return minus?SIntFunc<SInt>::NegAbs(value):SIntFunc<SInt>::PosAbs(value); }
 };

/* class DetectIntFormat<ulen Len> */

template <ulen Len>
class DetectIntFormat : NoCopy
 {
   char buf[Len];
   IntScanBase scan_base;

   PtrLen<char> cur;
   bool fail_flag = false ;

  private:

   void put(char ch)
    {
     if( +cur )
       {
        *cur=ch;

        ++cur;
       }
     else
       {
        fail();
       }
    }

   template <class S>
   IntScanBase finish(int max_dig,S &inp);

   template <class S>
   IntScanBase detect_0X(S &inp);

   template <class S>
   IntScanBase detect_0(S &inp);

   template <class S>
   IntScanBase detect_nosign(S &inp);

   template <class S>
   IntScanBase detect(S &inp);

  public:

   template <class S>
   explicit DetectIntFormat(S &inp);

   IntScanBase getFormat() const { return scan_base; }

   // cursor

   ulen operator + () const { return +cur; }

   bool operator ! () const { return !cur; }

   char operator * () const { return *cur; }

   void operator ++ () { ++cur; }

   // error

   bool isOk() const { return !fail_flag; }

   bool isFailed() const { return fail_flag; }

   void fail() { cur=Nothing; fail_flag=true; }
 };

template <ulen Len>
template <class S>
IntScanBase DetectIntFormat<Len>::finish(int max_dig,S &inp)
 {
  bool bin_flag=false;

  for(; +inp ;++inp)
    {
     char ch=*inp;
     int dig=CharHexValue(ch);

     if( dig<0 )
       {
        if( ch=='h' || ch=='H' )
          {
           put(ch);

           ++inp;

           return IntScanHex;
          }

        break;
       }
     else
       {
        bin_flag=( max_dig<2 && ( ch=='b' || ch=='B' ) );

        put(ch);

        Replace_max(max_dig,dig);
       }
    }

  if( bin_flag ) return IntScanBin;

  if( max_dig>=10 ) inp.fail();

  return IntScanNone;
 }

template <ulen Len>
template <class S>
IntScanBase DetectIntFormat<Len>::detect_0X(S &inp)
 {
  if( +inp )
    {
     char ch=*inp;

     if( ch=='0' )
       {
        put(ch);

        ++inp;

        SkipAllOfChar(inp, [] (char ch) { return ch=='0'; } );
       }
     else
       {
        if( CharHexValue(ch)>=0 )
          {
           put(ch);

           ++inp;
          }
        else
          {
           inp.fail();

           return IntScan0X;
          }
       }

     for(char ch; +inp && CharHexValue(ch=*inp)>=0 ;++inp) put(ch);
    }
  else
    {
     inp.fail();
    }

  return IntScan0X;
 }

template <ulen Len>
template <class S>
IntScanBase DetectIntFormat<Len>::detect_0(S &inp)
 {
  if( +inp )
    {
     char ch=*inp;

     switch( ch )
       {
        case 'x' : case 'X' :
         {
          put(ch);

          ++inp;

          return detect_0X(inp);
         }

        default:
         {
          SkipAllOfChar(inp, [] (char ch) { return ch=='0'; } );

          return finish(0,inp);
         }
       }
    }
  else
    {
     return IntScanNone;
    }
 }

template <ulen Len>
template <class S>
IntScanBase DetectIntFormat<Len>::detect_nosign(S &inp)
 {
  if( +inp )
    {
     char ch=*inp;

     if( ch=='0' )
       {
        put(ch);

        ++inp;

        return detect_0(inp);
       }
     else
       {
        int dig=CharHexValue(ch);

        if( dig<0 )
          {
           inp.fail();
          }
        else
          {
           put(ch);

           ++inp;

           return finish(dig,inp);
          }
       }
    }
  else
    {
     inp.fail();
    }

  return IntScanNone;
 }

template <ulen Len>
template <class S>
IntScanBase DetectIntFormat<Len>::detect(S &inp)
 {
  if( +inp )
    {
     char ch=*inp;

     if( ch=='+' || ch=='-' )
       {
        put(ch);

        ++inp;
       }

     return detect_nosign(inp);
    }
  else
    {
     inp.fail();
    }

  return IntScanNone;
 }

template <ulen Len>
template <class S>
DetectIntFormat<Len>::DetectIntFormat(S &inp)
 {
  cur=Range(buf);

  scan_base=detect(inp);

  if( isOk() ) cur=Range(buf).prefix(cur); else inp.fail();
 }

/* struct IntScanAlgo<SUInt,Acc> */

template <SUIntType SUInt,class Acc>
struct IntScanAlgo
 {
  // Hex

  template <class S>
  static void ScanHex(S &inp,SUInt &ret,bool minus)
   {
    if( +inp )
      {
       int dig=CharHexValue(*inp);

       if( dig<0 )
         {
          inp.fail();
         }
       else
         {
          Acc acc(dig,16,minus);

          for(++inp; +inp ;++inp)
            {
             char ch=*inp;

             dig=CharHexValue(ch);

             if( dig<0 )
               {
                if( ch=='h' || ch=='H' )
                  {
                   ++inp;

                   ret=acc;
                  }
                else
                  {
                   inp.fail();
                  }

                return;
               }
             else
               {
                if( !acc.next(dig,16) )
                  {
                   inp.fail();

                   return;
                  }
               }
            }

          inp.fail();
         }
      }
    else
      {
       inp.fail();
      }
   }

  template <class S>
  static void ScanHex(S &inp,SUInt &ret)
   {
    if( +inp )
      {
       switch( *inp )
         {
          case '+' : ++inp; // falldown;

          default: ScanHex(inp,ret,false); break;

          case '-' : ++inp; ScanHex(inp,ret,true);
         }
      }
    else
      {
       inp.fail();
      }
   }

  // Bin

  template <class S>
  static void ScanBin(S &inp,SUInt &ret,bool minus)
   {
    if( +inp )
      {
       int dig=CharBinValue(*inp);

       if( dig<0 )
         {
          inp.fail();
         }
       else
         {
          Acc acc(dig,2,minus);

          for(++inp; +inp ;++inp)
            {
             char ch=*inp;

             dig=CharBinValue(ch);

             if( dig<0 )
               {
                if( ch=='b' || ch=='B' )
                  {
                   ++inp;

                   ret=acc;
                  }
                else
                  {
                   inp.fail();
                  }

                return;
               }
             else
               {
                if( !acc.next(dig,2) )
                  {
                   inp.fail();

                   return;
                  }
               }
            }

          inp.fail();
         }
      }
    else
      {
       inp.fail();
      }
   }

  template <class S>
  static void ScanBin(S &inp,SUInt &ret)
   {
    if( +inp )
      {
       switch( *inp )
         {
          case '+' : ++inp; // falldown;

          default: ScanBin(inp,ret,false); break;

          case '-' : ++inp; ScanBin(inp,ret,true);
         }
      }
    else
      {
       inp.fail();
      }
   }

  // 0X

  template <class S>
  static void Scan0X(S &inp,SUInt &ret,bool minus)
   {
    PassChars(inp,'0');
    PassOneOfChar(inp, [] (char ch) { return ch=='x' || ch=='X'; } );

    if( +inp )
      {
       int dig=CharHexValue(*inp);

       if( dig<0 )
         {
          inp.fail();
         }
       else
         {
          Acc acc(dig,16,minus);

          for(++inp; +inp && (dig=CharHexValue(*inp))>=0 ;++inp)
            {
             if( !acc.next(dig,16) )
               {
                inp.fail();

                return;
               }
            }

          ret=acc;
         }
      }
    else
      {
       inp.fail();
      }
   }

  template <class S>
  static void Scan0X(S &inp,SUInt &ret)
   {
    if( +inp )
      {
       switch( *inp )
         {
          case '+' : ++inp; // falldown;

          default: Scan0X(inp,ret,false); break;

          case '-' : ++inp; Scan0X(inp,ret,true);
         }
      }
    else
      {
       inp.fail();
      }
   }

  // base

  template <class S>
  static void ScanBase(S &inp,SUInt &ret,unsigned base,bool minus)
   {
    if( +inp )
      {
       int dig=CharBaseValue(*inp,base);

       if( dig<0 )
         {
          inp.fail();
         }
       else
         {
          Acc acc(dig,base,minus);

          for(++inp; +inp && (dig=CharBaseValue(*inp,base))>=0 ;++inp)
            {
             if( !acc.next(dig,base) )
               {
                inp.fail();

                return;
               }
            }

          ret=acc;
         }
      }
    else
      {
       inp.fail();
      }
   }

  template <class S>
  static void ScanBase(S &inp,SUInt &ret,unsigned base)
   {
    if( +inp )
      {
       switch( *inp )
         {
          case '+' : ++inp; // falldown;

          default: ScanBase(inp,ret,base,false); break;

          case '-' : ++inp; ScanBase(inp,ret,base,true);
         }
      }
    else
      {
       inp.fail();
      }
   }

  // any

  template <class S>
  static void ScanAny(S &inp,SUInt &ret) // Hex | Bin | 0X | Base(10)
   {
    DetectIntFormat<Acc::Bits+10> detect(inp);

    if( inp.isFailed() ) return;

    switch( detect.getFormat() )
      {
       case IntScanHex : ScanHex(detect,ret); break;

       case IntScanBin : ScanBin(detect,ret); break;

       case IntScan0X : Scan0X(detect,ret); break;

       case IntScanNone : ScanBase(detect,ret,10); break;
      }

    if( detect.isFailed() ) inp.fail();
   }

  // opt

  template <class S>
  static void Scan(S &inp,IntScanOpt opt,SUInt &ret)
   {
    switch( opt.scan_base )
      {
       case IntScanHex : ScanHex(inp,ret); break;

       case IntScanBin : ScanBin(inp,ret); break;

       case IntScan0X : Scan0X(inp,ret); break;

       case IntScanNone :
        {
         if( opt.base )
           {
            ScanBase(inp,ret,opt.base);
           }
         else
           {
            ScanAny(inp,ret);
           }
        }
      }
   }
 };

/* class UIntScan<UInt> */

template <UIntType UInt>
class UIntScan : NoCopy
 {
   UInt &var;

  public:

   explicit UIntScan(UInt &var_) : var(var_) {}

   template <class S>
   void scan(S &inp,IntScanOpt opt)
    {
     var=0;

     IntScanAlgo<UInt,UIntDigitAcc<UInt> >::Scan(inp,opt,var);
    }
 };

/* class SIntScan<SInt> */

template <SIntType SInt>
class SIntScan : NoCopy
 {
   SInt &var;

  public:

   explicit SIntScan(SInt &var_) : var(var_) {}

   template <class S>
   void scan(S &inp,IntScanOpt opt)
    {
     var=0;

     IntScanAlgo<SInt,SIntDigitAcc<SInt> >::Scan(inp,opt,var);
    }
 };

} // namespace CCore

#endif


