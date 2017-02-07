/* Contour.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: ContourLab 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2017 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/Contour.h>
#include <inc/Parser.h>

#include <CCore/inc/Exception.h>

namespace App {

/* functions */

void GuardTypeMismatch()
 {
  Printf(Exception,"App::GuardType(...) : type mismatch");
 }

/* struct Formula */

Formular::Object::Base Formular::Object::Base::Default;

/* class Contour::ItemInfo */

class Contour::ItemInfo::Base : public ComboInfoBase
 {
   const DynArray<Item> &data;

  public:

   explicit Base(const DynArray<Item> &data_) : data(data_) {}

   virtual ~Base() {}

   // AbstractComboInfo

   ulen getLineCount() const
    {
     return data.getLen();
    }

   ComboInfoItem getLine(ulen index) const
    {
     return {ComboInfoText,Range(data.at(index).label.name)};
    }
 };

Contour::ItemInfo::ItemInfo(const DynArray<Item> &data)
 : ComboInfo(new Base(data))
 {
 }

Contour::ItemInfo::~ItemInfo()
 {
 }

/* class Contour */

bool Contour::testName(StrLen name) const
 {
  StrKey k(name);

  return !map.find(k);
 }

bool Contour::delItem(DynArray<Item> &a,ulen index)
 {
  if( index<a.getLen() )
    {
     StrKey key(Range(a[index].label.name));

     map.del(key);

     ArrayCopyDel(a,index);

     return true;
    }

  return false;
 }

Contour::Contour()
 {
 }

Contour::~Contour()
 {
 }

 // pad list

ComboInfo Contour::getPadInfo()
 {
  return ItemInfo(pads);
 }

bool Contour::padUp(ulen index)
 {
  return UpItem(pads,index);
 }

bool Contour::padDown(ulen index)
 {
  return DownItem(pads,index);
 }

bool Contour::padDel(ulen index)
 {
  return delItem(pads,index);
 }

class Contour::PadTestParser : public PadTextParser
 {
   const Contour *obj;

  private:

   virtual bool point(StrLen name,StrLen,StrLen) { return obj->testName(name); }

   virtual bool length(StrLen name,StrLen) { return obj->testName(name); }

   virtual bool angle(StrLen name,StrLen) { return obj->testName(name); }

   virtual bool ratio(StrLen name,StrLen) { return obj->testName(name); }

  public:

   PadTestParser(const Contour *obj_,StrLen text,CharAccent *accent) : PadTextParser(text,accent),obj(obj_) {}
 };

bool Contour::padAddTest(StrLen text,CharAccent *accent) const
 {
  PadTestParser parser(this,text,accent);

  parser.run();

  return parser;
 }

class Contour::PadAddParser : public PadTextParser
 {
   Contour *obj;
   ulen index;

  private:

   virtual bool point(StrLen name,StrLen x,StrLen y)
    {
     return obj->addPad<Point>(index,name,{StrToReal(x),StrToReal(y)});
    }

   virtual bool length(StrLen name,StrLen x)
    {
     return obj->addPad<Length>(index,name,StrToReal(x));
    }

   virtual bool angle(StrLen name,StrLen x)
    {
     return obj->addPad<Angle>(index,name,GradToRadian(StrToReal(x)));
    }

   virtual bool ratio(StrLen name,StrLen x)
    {
     return obj->addPad<Ratio>(index,name,StrToReal(x));
    }

  public:

   PadAddParser(Contour *obj_,ulen index_,StrLen text) : PadTextParser(text),obj(obj_),index(index_) {}
 };

bool Contour::padAdd(ulen index,StrLen text)
 {
  PadAddParser parser(this,index,text);

  parser.run();

  return parser;
 }

 // formula list

ComboInfo Contour::getFormulaInfo()
 {
  return ItemInfo(formulas);
 }

bool Contour::formulaUp(ulen index)
 {
  return UpItem(formulas,index);
 }

bool Contour::formulaDown(ulen index)
 {
  return DownItem(formulas,index);
 }

bool Contour::formulaDel(ulen index)
 {
  return delItem(formulas,index);
 }

class Contour::FormulaTestContext : NoCopy
 {
   struct NegFunc
    {
     Object &ret;
     Object a;

     bool operator () (auto)
      {
       return false;
      }

     template <OneOfTypes<Ratio,Length,Angle> S>
     bool operator () (S)
      {
       ret=Formula<S (S)>::template Create<Neg>(a);

       return true;
      }

     bool operator () () { return false; }
    };

   struct AddFunc
    {
     Object &ret;
     Object a;
     Object b;

     bool operator () (auto)
      {
       return false;
      }

     template <OneOfTypes<Ratio,Length,Angle> S>
     bool operator () (S)
      {
       if( b.getTypeId()!=S::TypeId ) return false;

       ret=Formula<S (S,S)>::template Create<Add>(a,b);

       return true;
      }

     bool operator () () { return false; }
    };

   struct SubFunc
    {
     Object &ret;
     Object a;
     Object b;

     bool operator () (auto)
      {
       return false;
      }

     template <OneOfTypes<Ratio,Length,Angle> S>
     bool operator () (S)
      {
       if( b.getTypeId()!=S::TypeId ) return false;

       ret=Formula<S (S,S)>::template Create<Sub>(a,b);

       return true;
      }

     bool operator () () { return false; }
    };

   struct MulFunc
    {
     Object &ret;
     Object a;
     Object b;

     bool operator () (auto)
      {
       return false;
      }

     template <OneOfTypes<Ratio,Length,Angle> S>
     bool operator () (S)
      {
       if( a.getTypeId()!=Ratio::TypeId ) return false;

       ret=Formula<S (Ratio,S)>::template Create<Mul>(a,b);

       return true;
      }

     bool operator () () { return false; }
    };

   struct DivFunc
    {
     Object &ret;
     Object a;
     Object b;

     bool operator () (auto)
      {
       return false;
      }

     template <OneOfTypes<Ratio,Angle> S>
     bool operator () (S)
      {
       if( b.getTypeId()!=Ratio::TypeId ) return false;

       ret=Formula<S (S,Ratio)>::template Create<Div>(a,b);

       return true;
      }

     bool operator () (Length)
      {
       switch( b.getTypeId() )
         {
          case Length::TypeId :
            ret=Formula<Ratio (Length,Length)>::Create<Div>(a,b);
          return true;

          case Ratio::TypeId :
            ret=Formula<Length (Length,Ratio)>::Create<Div>(a,b);
          return true;
         }

       return false;
      }

     bool operator () () { return false; }
    };

  protected:

   const Contour *obj;

  public:

   explicit FormulaTestContext(const Contour *obj_) : obj(obj_) {}

   using ExprType = Object ;

   bool set(StrLen name,ExprType)
    {
     return obj->testName(name);
    }

   // functions

   bool add(ExprType &ret,ExprType a,ExprType b)
    {
     return a.call(AddFunc{ret,a,b});
    }

   bool sub(ExprType &ret,ExprType a,ExprType b)
    {
     return a.call(SubFunc{ret,a,b});
    }

   bool mul(ExprType &ret,ExprType a,ExprType b)
    {
     return b.call(MulFunc{ret,a,b});
    }

   bool div(ExprType &ret,ExprType a,ExprType b)
    {
     return a.call(DivFunc{ret,a,b});
    }

   bool neg(ExprType &ret,ExprType a)
    {
     return a.call(NegFunc{ret,a});
    }

   bool func(ExprType &ret,StrLen name,PtrLen<const ExprType> list)
    {
#define DEF(N,F) if( name.equal( #N ## _c ) ) return Formula<decltype(F)>::SafeCreate<F>(ret,list);

     DEF(Len,LengthOf)
     DEF(Angle,AngleOf)
     DEF(Line,LineOf)
     DEF(Cir,CircleOf)
     DEF(OCir,CircleOuter)
     DEF(Mid,Middle)

     DEF(Part,Part)
     DEF(MidOrt,MidOrt)
     DEF(Proj,Proj)
     DEF(AngleC,AngleC)
     DEF(Meet,Meet)
     DEF(MeetCircle,MeetCircle)
     DEF(MeetCircles,MeetCircles)
     DEF(Rot,Rotate)
     DEF(RotOrt,RotateOrt)
     DEF(Move,Move)
     DEF(MoveLen,MoveLen)
     DEF(Mirror,Mirror)
     DEF(First,First)
     DEF(Second,Second)
     DEF(Up,Up)
     DEF(Down,Down)
     DEF(Left,Left)
     DEF(Right,Right)

     DEF(Solid,SolidOf)

#undef DEF

#define DEF(N,T,A,F) if( name.equal( #N ## _c ) ) return Formula<T (A[])>::SafeCreate<F>(ret,list);

     DEF(Step,Step,Point,StepOf)

     DEF(Path,Path,Point,PathOf)

     DEF(Loop,Loop,Point,LoopOf)

     DEF(BPath,Path,Step,PathOf)

     DEF(BLoop,Loop,Step,LoopOf)

#undef DEF

     return false;
    }

   // args

   bool arg(ExprType &ret,StrLen name)
    {
     StrKey key(name);

     if( const Object *res=obj->map.find(key) )
       {
        ret=*res;

        return true;
       }

     return false;
    }

   bool number(ExprType &ret,StrLen number)
    {
     ret=Pad<Ratio>::Create(StrToReal(number));

     return true;
    }

   bool angle(ExprType &ret,StrLen number)
    {
     ret=Pad<Angle>::Create(StrToReal(number));

     return true;
    }

   bool length(ExprType &ret,StrLen number)
    {
     ret=Pad<Length>::Create(StrToReal(number));

     return true;
    }

   bool point(ExprType &ret,StrLen number_x,StrLen number_y)
    {
     ret=Pad<Point>::Create(Point(StrToReal(number_x),StrToReal(number_y)));

     return true;
    }
 };

bool Contour::formulaAddTest(StrLen text,CharAccent *accent) const
 {
  FormulaTestContext ctx(this);
  FormulaTextParser<FormulaTestContext> parser(ctx,text,accent);

  parser.run();

  return parser;
 }

class Contour::FormulaAddContext : public FormulaTestContext
 {
   ulen index;

  public:

   FormulaAddContext(Contour *obj,ulen index_) : FormulaTestContext(obj),index(index_) {}

   bool set(StrLen name,ExprType value)
    {
     return const_cast<Contour *>(obj)->addFormula(index,name,value);
    }
 };

bool Contour::formulaAdd(ulen index,StrLen text)
 {
  FormulaAddContext ctx(this,index);
  FormulaTextParser<FormulaAddContext> parser(ctx,text);

  parser.run();

  return parser;
 }

 // save/load

void Contour::erase()
 {
  pads.erase();
  formulas.erase();
  map.erase();
 }

void Contour::save(StrLen file_name,ErrorText &etext) const // TODO
 {
  Used(file_name);

  etext.setText("Not implemented yet"_c);
 }

void Contour::load(StrLen file_name,ErrorText &etext) // TODO
 {
  erase();

  Used(file_name);

  etext.setText("Not implemented yet"_c);
 }

} // namespace App

