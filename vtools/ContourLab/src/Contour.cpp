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

#include <CCore/inc/Print.h>
#include <CCore/inc/PrintStem.h>
#include <CCore/inc/Exception.h>

#include <CCore/inc/FileName.h>
#include <CCore/inc/FileToMem.h>

#include <CCore/inc/ddl/DDLEngine.h>
#include <CCore/inc/ddl/DDLTypeSet.h>

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

bool Contour::UpItem(DynArray<Item> &a,ulen index)
 {
  auto r=Range(a);

  if( index>0 && index<r.len )
    {
     Swap(r[index],r[index-1]);

     r[index].obj.setIndex(index);
     r[index-1].obj.setIndex(index-1);

     return true;
    }

  return false;
 }

bool Contour::DownItem(DynArray<Item> &a,ulen index)
 {
  auto r=Range(a);

  if( r.len && index<r.len-1 )
    {
     Swap(r[index],r[index+1]);

     r[index].obj.setIndex(index);
     r[index+1].obj.setIndex(index+1);

     return true;
    }

  return false;
 }

bool Contour::addPad(ulen index,StrLen name,Object obj)
 {
  pads.reserve(1);

  StrKey k(name);

  auto result=map.find_or_add(k,obj);

  if( !result.new_flag ) return false;

  Label label(result.key->name);

  Item item{label,obj};

  ArrayCopyIns(pads,index,item);

  obj.setIndex(index);

  return true;
 }

bool Contour::addFormula(ulen index,StrLen name,Object obj)
 {
  formulas.reserve(1);

  StrKey k(name);

  auto result=map.find_or_add(k,obj);

  if( !result.new_flag ) return false;

  Label label(result.key->name);

  Item item{label,obj};

  ArrayCopyIns(formulas,index,item);

  obj.setIndex(index);

  return true;
 }

bool Contour::setFormula(ulen index,StrLen name,Object obj)
 {
  StrKey k(name);

  auto result=map.find_or_add(k,obj);

  if( !result.new_flag ) return false;

  Label label(result.key->name);

  Item item{label,obj};

  formulas[index]=item;

  obj.setIndex(index);

  return true;
 }

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

     a[index].obj.setIndex(MaxULen);

     ArrayCopyDel(a,index);

     for(ulen i=index,len=a.getLen(); i<len ;i++) a[i].obj.setIndex(i);

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

struct Contour::CreateOp
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
      ret=Formula<S (S)>::template Create<Neg>("Neg"_c,a);

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

      ret=Formula<S (S,S)>::template Create<Add>("Add"_c,a,b);

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

      ret=Formula<S (S,S)>::template Create<Sub>("Sub"_c,a,b);

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

      ret=Formula<S (Ratio,S)>::template Create<Mul>("Mul"_c,a,b);

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

      ret=Formula<S (S,Ratio)>::template Create<Div>("Div"_c,a,b);

      return true;
     }

    bool operator () (Length)
     {
      switch( b.getTypeId() )
        {
         case Length::TypeId :
           ret=Formula<Ratio (Length,Length)>::Create<Div>("Div"_c,a,b);
         return true;

         case Ratio::TypeId :
           ret=Formula<Length (Length,Ratio)>::Create<Div>("Div"_c,a,b);
         return true;
        }

      return false;
     }

    bool operator () () { return false; }
   };

  using ExprType = Object ;

  static bool add(ExprType &ret,ExprType a,ExprType b)
   {
    return a.call(AddFunc{ret,a,b});
   }

  static bool sub(ExprType &ret,ExprType a,ExprType b)
   {
    return a.call(SubFunc{ret,a,b});
   }

  static bool mul(ExprType &ret,ExprType a,ExprType b)
   {
    return b.call(MulFunc{ret,a,b});
   }

  static bool div(ExprType &ret,ExprType a,ExprType b)
   {
    return a.call(DivFunc{ret,a,b});
   }

  static bool neg(ExprType &ret,ExprType a)
   {
    return a.call(NegFunc{ret,a});
   }
 };

class Contour::FormulaTestContext : NoCopy , public CreateOp
 {
  protected:

   const Contour *obj;

  public:

   explicit FormulaTestContext(const Contour *obj_) : obj(obj_) {}

   bool set(StrLen name,ExprType)
    {
     return obj->testName(name);
    }

   // functions

   bool func(ExprType &ret,StrLen name,PtrLen<const ExprType> list)
    {
#define DEF(N,F) if( name.equal( #N ## _c ) ) return Formula<decltype(F)>::SafeCreate<F>(#F,ret,list);

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

#define DEF(N,T,A,F,TN) if( name.equal( #N ## _c ) ) return Formula<T (A[])>::SafeCreate<F>(#TN,ret,list);

     DEF(Step,Step,Point,StepOf,StepOf)

     DEF(Path,Path,Point,PathOf,PathOf)

     DEF(Loop,Loop,Point,LoopOf,LoopOf)

     DEF(BPath,Path,Step,PathOf,BPathOf)

     DEF(BLoop,Loop,Step,LoopOf,BLoopOf)

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
     if( value.getIndex()!=MaxULen ) return false;

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

struct Contour::PrintPad
 {
  PrintBase &out;
  StrLen name;
  ulen ind;

  void operator () (auto) {}

  void operator () () {}

  void operator () (Ratio s)
   {
    Printf(out,"Ratio #;#;={#;,#;};\n\n",name,ind,s.val.toBin(),(uint8)s.rex);
   }

  void operator () (Angle s)
   {
    Printf(out,"Angle #;#;={#;,#;};\n\n",name,ind,s.val.toBin(),(uint8)s.rex);
   }

  void operator () (Length s)
   {
    Printf(out,"Length #;#;={#;,#;};\n\n",name,ind,s.val.toBin(),(uint8)s.rex);
   }

  void operator () (Point s)
   {
    Printf(out,"Point #;#;={#;,#;,#;};\n\n",name,ind,s.x.toBin(),s.y.toBin(),(uint8)s.rex);
   }
 };

struct Contour::PrintArg
 {
  PrintBase &out;
  ulen free_ind;

  void printTop(Object o,ulen ind)
   {
    auto t=o.getText();

    print(o,ind,t);
   }

  void print(Object o,ulen ind)
   {
    auto t=o.getText();

    if( t.index==MaxULen ) print(o,ind,t);
   }

  void print(Object o,ulen ind,Text t)
   {
    switch( t.type )
      {
       case TextFormulaFixed :
       case TextFormulaVariable :
        {
         Printf(out,"#; F#;={",t.name,ind);

         if( t.type==TextFormulaVariable ) Putch(out,'{');

         ulen base_ind=free_ind;

         free_ind+=t.args.len;

         PrintFirst stem(""_c,","_c);

         for(ulen i=0; i<t.args.len ;i++)
           {
            auto ta=t.args[i].getText();

            if( ta.index==MaxULen )
              {
               Printf(out,"#;&F#;",stem,base_ind+i);
              }
            else
              {
               if( ta.type==TextPad )
                 Printf(out,"#;Data.pads+#;",stem,ta.index);
               else
                 Printf(out,"#;Data.formulas+#;",stem,ta.index);
              }
           }

         if( t.type==TextFormulaVariable ) Putch(out,'}');

         Printf(out,"};\n\n");

         for(ulen i=0; i<t.args.len ;i++) print(t.args[i],base_ind+i);
        }
       break;

       case TextPad :
        {
         o.call(PrintPad{out,"F"_c,ind});
        }
       break;
      }
   }
 };

void Contour::save(StrLen file_name,ErrorText &etext) const
 {
  char temp[TextBufLen];
  PrintBuf eout(Range(temp));

  ReportExceptionTo<PrintBuf> report(eout);

  try
    {
     PrintFile out(file_name);

     Printf(out,"//##include <Contour.ddl>\n\n");

     Printf(out,"Contour Data=\n {\n");

     // pads

     Printf(out,"  {");

     {
      auto r=Range(pads);

      PrintFirst stem("\n   "_c,",\n   "_c);

      for(ulen i=0; i<r.len ;i++)
        {
         Printf(out,"#;{ #; , #; , &Pad#; }",stem,r[i].label,i,i);
        }
     }

     Printf(out,"\n  },\n");

     // formulas

     Printf(out,"  {");

     {
      auto r=Range(formulas);

      PrintFirst stem("\n  "_c,",\n  "_c);

      for(ulen i=0; i<r.len ;i++)
        {
         Printf(out,"#;{ #; , #; , &F#; }",stem,r[i].label,i,i);
        }
     }

     Printf(out,"\n  }\n");

     Printf(out," };\n\n");

     // pads

     {
      auto r=Range(pads);

      for(ulen i=0; i<r.len ;i++)
        {
         r[i].obj.call(PrintPad{out,"Pad"_c,i});
        }
     }

     // formulas

     {
      auto r=Range(formulas);

      PrintArg parg{out,r.len};

      for(ulen i=0; i<r.len ;i++)
        {
         parg.printTop(r[i].obj,i);
        }
     }
    }
  catch(CatchType)
    {
     Printf(eout,"\n@ #.q;",file_name);

     etext.setText(eout.close());
    }
 }

#include "Contour.TypeDef.gen.h"
#include "Contour.TypeSet.gen.h"

const char *const Contour::Pretext=
"/* --- Common ------------------------------------------------------------------------- */\r\n"
"\r\n"
"type Bool = uint8 ;\r\n"
"\r\n"
"Bool True = 1 ;\r\n"
"\r\n"
"Bool False = 0 ;\r\n"
"\r\n"
"struct Label\r\n"
" {\r\n"
"  text name;\r\n"
"\r\n"
"  Bool show;\r\n"
"  Bool gray;\r\n"
"  Bool show_name;\r\n"
" };\r\n"
"\r\n"
"type Exception = uint8 ;\r\n"
"\r\n"
"/* --- Geometry ----------------------------------------------------------------------- */\r\n"
"\r\n"
"struct Real\r\n"
" {\r\n"
"  sint64 mantissa;\r\n"
"  sint16 exp;\r\n"
" };\r\n"
" \r\n"
"struct Ratio\r\n"
" {\r\n"
"  Real val;\r\n"
"  Exception rex;\r\n"
" };\r\n"
"  \r\n"
"struct Length\r\n"
" {\r\n"
"  Real val;\r\n"
"  Exception rex;\r\n"
" }; \r\n"
" \r\n"
"struct Angle\r\n"
" {\r\n"
"  Real val;\r\n"
"  Exception rex;\r\n"
" };\r\n"
"  \r\n"
"struct Point\r\n"
" {\r\n"
"  Real x;\r\n"
"  Real y;\r\n"
"  Exception rex;\r\n"
" }; \r\n"
" \r\n"
"struct Line\r\n"
" {\r\n"
"  Point a;\r\n"
"  Point ort;\r\n"
"  Exception rex;\r\n"
" };\r\n"
" \r\n"
"struct Circle \r\n"
" {\r\n"
"  Point center;\r\n"
"  Length radius;\r\n"
"  Exception rex;\r\n"
" };\r\n"
" \r\n"
"struct Couple\r\n"
" {\r\n"
"  Point a;\r\n"
"  Point b;\r\n"
"  Exception rex;\r\n"
" };\r\n"
" \r\n"
"struct Dot \r\n"
" {\r\n"
"  Point point;\r\n"
"  Bool break_flag;\r\n"
" };\r\n"
" \r\n"
"struct Step \r\n"
" {\r\n"
"  Point[] points;\r\n"
"  Exception rex;\r\n"
" };\r\n"
" \r\n"
"struct Path\r\n"
" {\r\n"
"  Dot[] dots;\r\n"
"  Exception rex;\r\n"
" };\r\n"
" \r\n"
"struct Loop\r\n"
" {\r\n"
"  Dot[] dots;\r\n"
"  Exception rex;\r\n"
" };\r\n"
"  \r\n"
"struct Solid\r\n"
" {\r\n"
"  Dot[] dots;\r\n"
"  Exception rex;\r\n"
" };\r\n"
"\r\n"
"/* --- Pad ---------------------------------------------------------------------------- */ \r\n"
"\r\n"
"struct Pad\r\n"
" {\r\n"
"  Label label;\r\n"
"  ulen index;\r\n"
"  \r\n"
"  {Ratio,Length,Angle,Point} *object;\r\n"
" };\r\n"
"\r\n"
"/* --- Formula ------------------------------------------------------------------------ */\r\n"
"\r\n"
"struct Neg\r\n"
" {\r\n"
"  Arg a;\r\n"
" };\r\n"
" \r\n"
"struct Add\r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
" };\r\n"
" \r\n"
"struct Sub\r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
" };\r\n"
" \r\n"
"struct Mul\r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
" };\r\n"
" \r\n"
"struct Div\r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
" };\r\n"
"\r\n"
"struct LengthOf\r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
" };\r\n"
" \r\n"
"struct AngleOf\r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;\r\n"
"  Arg c;  \r\n"
" };\r\n"
" \r\n"
"struct LineOf\r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
" };  \r\n"
" \r\n"
"struct Middle\r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
" };\r\n"
"   \r\n"
"struct Part\r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
"  Arg c;  \r\n"
" };\r\n"
" \r\n"
"struct MidOrt \r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
" };\r\n"
" \r\n"
"struct CircleOf \r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
" };\r\n"
" \r\n"
"struct CircleOuter \r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
"  Arg c;  \r\n"
" };\r\n"
" \r\n"
"struct Proj \r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
" };\r\n"
" \r\n"
"struct AngleC \r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
"  Arg c;  \r\n"
" };\r\n"
" \r\n"
"struct Meet \r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
" };\r\n"
" \r\n"
"struct MeetCircle\r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
" };\r\n"
" \r\n"
"struct MeetCircles\r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
" };\r\n"
" \r\n"
"struct Rotate \r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
"  Arg c;  \r\n"
" };\r\n"
"\r\n"
"struct RotateOrt\r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
" };\r\n"
"\r\n"
"struct Move\r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
"  Arg c;  \r\n"
" };\r\n"
" \r\n"
"struct MoveLen\r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
"  Arg c;  \r\n"
" };\r\n"
" \r\n"
"struct Mirror \r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
" };\r\n"
" \r\n"
"struct First\r\n"
" {\r\n"
"  Arg a;\r\n"
" };\r\n"
"  \r\n"
"struct Second\r\n"
" {\r\n"
"  Arg a;\r\n"
" }; \r\n"
" \r\n"
"struct Up \r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
" };\r\n"
" \r\n"
"struct Down \r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
" };\r\n"
" \r\n"
"struct Left \r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
" };\r\n"
" \r\n"
"struct Right \r\n"
" {\r\n"
"  Arg a;\r\n"
"  Arg b;  \r\n"
" };\r\n"
" \r\n"
"struct StepOf\r\n"
" {\r\n"
"  Arg[] args;\r\n"
" }; \r\n"
" \r\n"
"struct PathOf\r\n"
" {\r\n"
"  Arg[] args;\r\n"
" };\r\n"
" \r\n"
"struct BPathOf\r\n"
" {\r\n"
"  Arg[] args;\r\n"
" };\r\n"
"  \r\n"
"struct LoopOf\r\n"
" {\r\n"
"  Arg[] args;\r\n"
" };\r\n"
" \r\n"
"struct BLoopOf\r\n"
" {\r\n"
"  Arg[] args;\r\n"
" };\r\n"
" \r\n"
"struct SolidOf \r\n"
" {\r\n"
"  Arg a;\r\n"
" }; \r\n"
" \r\n"
"type Arg = {\r\n"
"            Ratio,Length,Angle,Point,Pad,Formula,\r\n"
"            Neg,Add,Sub,Mul,Div, \r\n"
"            LengthOf,AngleOf,LineOf,Middle,Part,MidOrt,CircleOf,CircleOuter,\r\n"
"            Proj,AngleC,Meet,MeetCircle,MeetCircles,Rotate,RotateOrt,Move,MoveLen,\r\n"
"            Mirror,First,Second,Up,Down,Left,Right,\r\n"
"            StepOf,PathOf,BPathOf,LoopOf,BLoopOf,SolidOf\r\n"
"           } * ;\r\n"
"\r\n"
"struct Formula\r\n"
" {\r\n"
"  Label label;\r\n"
"  ulen index;\r\n"
"  \r\n"
"  Arg object;\r\n"
" };\r\n"
"\r\n"
"/* --- Contour ------------------------------------------------------------------------ */\r\n"
" \r\n"
"struct Contour\r\n"
" {\r\n"
"  Pad[] pads;\r\n"
"  Formula[] formulas;\r\n"
" };\r\n"
"";

Geometry::Real Contour::ToReal(AnyType x)
 {
  return Real::Bin{x.mantissa,x.exp};
 }

void Contour::CopyFlags(Label &label,AnyType x)
 {
  label.show=x.show;
  label.gray=x.gray;
  label.show_name=x.show_name;
 }

struct Contour::CreateDataPadObject : NoCopy
 {
  Object obj;

  void create(AnyType ptr)
   {
    if( !ptr )
      {
       Printf(Exception,"App::Contour::load(...) : null pad object");
      }

    ElaborateAnyPtr(*this,ptr);
   }

  void operator () (TypeDef::Ratio *ptr)
   {
    Ratio s;

    s.val=ToReal(ptr->val);
    s.rex=RealException(ptr->rex);

    obj=Pad<Ratio>::Create(s);
   }

  void operator () (TypeDef::Angle *ptr)
   {
    Angle s;

    s.val=ToReal(ptr->val);
    s.rex=RealException(ptr->rex);

    obj=Pad<Angle>::Create(s);
   }

  void operator () (TypeDef::Length *ptr)
   {
    Length s;

    s.val=ToReal(ptr->val);
    s.rex=RealException(ptr->rex);

    obj=Pad<Length>::Create(s);
   }

  void operator () (TypeDef::Point *ptr)
   {
    Point s;

    s.x=ToReal(ptr->x);
    s.y=ToReal(ptr->y);
    s.rex=RealException(ptr->rex);

    obj=Pad<Point>::Create(s);
   }
 };

struct Contour::CreateDataFormulaObject : CreateDataPadObject
 {
  Contour *host;
  PtrLen<TypeDef::Formula> data_formulas;
  bool *lock;

  CreateDataFormulaObject(Contour *host_,PtrLen<TypeDef::Formula> data_formulas_,bool *lock_)
   : host(host_),data_formulas(data_formulas_),lock(lock_) {}

  void create(AnyType ptr)
   {
    if( !ptr )
      {
       Printf(Exception,"App::Contour::load(...) : null formula object");
      }

    ElaborateAnyPtr(*this,ptr);
   }

  void set(ulen i)
   {
    Object o=host->formulas[i].obj;

    if( o.getTypeId()!=0 )
      {
       obj=o;

       return;
      }

    if( !Change(lock[i],true) )
      {
       Printf(Exception,"App::Contour::load(...) : formula self-dependence");
      }

    TypeDef::Formula formula=data_formulas[i];

    create(formula.object.getPtr());

    if( !host->setFormula(i,formula.label.name,obj) )
      {
       Printf(Exception,"App::Contour::load(...) : cannot create formula");
      }

    CopyFlags(host->formulas[i].label,formula.label);
   }

  Object createArg(AnyType ptr)
   {
    CreateDataFormulaObject func(host,data_formulas,lock);

    func.create(ptr);

    return func.obj;
   }

  static void Guard(bool ok)
   {
    if( !ok )
      {
       Printf(Exception,"App::Contour::load(...) : argument type mismatch");
      }
   }

  using CreateDataPadObject::operator ();

  void operator () (TypeDef::Pad *ptr)
   {
    obj=host->pads.at(ptr->index).obj;
   }

  void operator () (TypeDef::Formula *ptr)
   {
    ulen index=ptr->index;

    GuardIndex(index,data_formulas.len);

    set(index);
   }

  void operator () (TypeDef::Neg *ptr)
   {
    Guard( CreateOp::neg(obj,createArg(ptr->a.getPtr())) );
   }

  void operator () (TypeDef::Add *ptr)
   {
    Guard( CreateOp::add(obj,createArg(ptr->a.getPtr()),createArg(ptr->b.getPtr())) );
   }

  void operator () (TypeDef::Sub *ptr)
   {
    Guard( CreateOp::sub(obj,createArg(ptr->a.getPtr()),createArg(ptr->b.getPtr())) );
   }

  void operator () (TypeDef::Mul *ptr)
   {
    Guard( CreateOp::mul(obj,createArg(ptr->a.getPtr()),createArg(ptr->b.getPtr())) );
   }

  void operator () (TypeDef::Div *ptr)
   {
    Guard( CreateOp::div(obj,createArg(ptr->a.getPtr()),createArg(ptr->b.getPtr())) );
   }

#define DEF1(F)                                                                     \
  void operator () (TypeDef::F *ptr)                                                \
   {                                                                                \
    Object list[1]={createArg(ptr->a.getPtr())};                                    \
                                                                                    \
    Guard( Formula<decltype(F)>::SafeCreate<F>(#F,obj,Range(list)) );               \
   }

#define DEF2(F)                                                                     \
  void operator () (TypeDef::F *ptr)                                                \
   {                                                                                \
    Object list[2]={createArg(ptr->a.getPtr()),createArg(ptr->b.getPtr())};         \
                                                                                    \
    Guard( Formula<decltype(F)>::SafeCreate<F>(#F,obj,Range(list)) );               \
   }

#define DEF3(F)                                                                     \
  void operator () (TypeDef::F *ptr)                                                \
   {                                                                                \
    Object list[3]={createArg(ptr->a.getPtr()),createArg(ptr->b.getPtr()),createArg(ptr->c.getPtr())};     \
                                                                                    \
    Guard( Formula<decltype(F)>::SafeCreate<F>(#F,obj,Range(list)) );               \
   }

  DEF2(LengthOf)
  DEF3(AngleOf)
  DEF2(LineOf)
  DEF2(CircleOf)
  DEF3(CircleOuter)
  DEF2(Middle)

  DEF3(Part)
  DEF2(MidOrt)
  DEF2(Proj)
  DEF3(AngleC)
  DEF2(Meet)
  DEF2(MeetCircle)
  DEF2(MeetCircles)
  DEF3(Rotate)
  DEF2(RotateOrt)
  DEF3(Move)
  DEF3(MoveLen)
  DEF2(Mirror)
  DEF1(First)
  DEF1(Second)
  DEF2(Up)
  DEF2(Down)
  DEF2(Left)
  DEF2(Right)

  DEF1(SolidOf)

#undef DEF1
#undef DEF2
#undef DEF3

  void fill(DynArray<Object> &list,PtrLen<TypeDef::Arg> r)
   {
    list.extend_default(r.len);

    for(ulen i=0; i<r.len ;i++) list[i]=createArg(r[i].getPtr());
   }

  void operator () (TypeDef::StepOf *ptr)
   {
    DynArray<Object> list;

    fill(list,ptr->args.getRange());

    Guard( Formula<Step (Point[])>::SafeCreate<StepOf>("StepOf",obj,Range(list)) );
   }

  void operator () (TypeDef::PathOf *ptr)
   {
    DynArray<Object> list;

    fill(list,ptr->args.getRange());

    Guard( Formula<Path (Point[])>::SafeCreate<PathOf>("PathOf",obj,Range(list)) );
   }

  void operator () (TypeDef::BPathOf *ptr)
   {
    DynArray<Object> list;

    fill(list,ptr->args.getRange());

    Guard( Formula<Path (Step[])>::SafeCreate<PathOf>("BPathOf",obj,Range(list)) );
   }

  void operator () (TypeDef::LoopOf *ptr)
   {
    DynArray<Object> list;

    fill(list,ptr->args.getRange());

    Guard( Formula<Loop (Point[])>::SafeCreate<LoopOf>("LoopOf",obj,Range(list)) );
   }

  void operator () (TypeDef::BLoopOf *ptr)
   {
    DynArray<Object> list;

    fill(list,ptr->args.getRange());

    Guard( Formula<Loop (Step[])>::SafeCreate<LoopOf>("BLoopOf",obj,Range(list)) );
   }
 };

void Contour::load(StrLen file_name,ErrorText &etext)
 {
  erase();

  SimpleArray<char> temp(64_KByte);
  PrintBuf eout(Range(temp));

  ReportExceptionTo<PrintBuf> report(eout);

  try
    {
     DDL::FileEngine<FileName,FileToMem> engine(eout);

     auto result=engine.process(file_name,Pretext);

     if( !result )
       {
        Printf(eout,"\n@ #.q;",file_name);

        etext.setText(eout.close());

        return;
       }
     else
       {
        DDL::TypedMap<TypeSet> map(result);
        MemAllocGuard guard(map.getLen());

        map(guard);

        // populate

        TypeDef::Contour data=map.takeConst<TypeDef::Contour>("Data");

        PtrLen<TypeDef::Pad> data_pads=data.pads.getRange();

        pads.reserve(data_pads.len);

        for(ulen i=0; i<data_pads.len ;i++)
          {
           TypeDef::Pad pad=data_pads[i];

           CreateDataPadObject func;

           func.create(pad.object.getPtr());

           if( !addPad(i,pad.label.name,func.obj) )
             {
              Printf(Exception,"App::Contour::load(...) : cannot create pad");
             }

           CopyFlags(pads[i].label,pad.label);
          }

        PtrLen<TypeDef::Formula> data_formulas=data.formulas.getRange();

        formulas.extend_default(data_formulas.len);

        DynArray<bool> lock(data_formulas.len);

        CreateDataFormulaObject func(this,data_formulas,lock.getPtr());

        for(ulen i=0; i<data_formulas.len ;i++) func.set(i);
       }
    }
  catch(CatchType)
    {
     erase();

     Printf(eout,"\n@ #.q;",file_name);

     etext.setText(eout.close());
    }
 }

} // namespace App

