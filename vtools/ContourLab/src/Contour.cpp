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

Contour::Contour()
 {
  addPad(0,"A"_c,Point{50,50});

  addPad(1,"B"_c,Point{200,300});

  addPad(2,"C"_c,Point{400,200});

  addFormula(0,"(AB)"_c,FormulaType<decltype(LineOf)>::Create<LineOf>,pads[0].obj,pads[1].obj);

  addFormula(1,"(AC)"_c,FormulaType<decltype(LineOf)>::Create<LineOf>,pads[0].obj,pads[2].obj);

  addFormula(2,"(BC)"_c,FormulaType<decltype(LineOf)>::Create<LineOf>,pads[1].obj,pads[2].obj);

  addFormula(3,"Outer"_c,FormulaType<decltype(CircleOuter)>::Create<CircleOuter>,pads[0].obj,pads[1].obj,pads[2].obj);
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
  return DelItem(pads,index);
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
     return obj->addPad<Geometry::Point>(index,name,{StrToReal(x),StrToReal(y)});
    }

   virtual bool length(StrLen name,StrLen x)
    {
     return obj->addPad<Geometry::Length>(index,name,StrToReal(x));
    }

   virtual bool angle(StrLen name,StrLen x)
    {
     return obj->addPad<Geometry::Angle>(index,name,GradToRadian(StrToReal(x)));
    }

   virtual bool ratio(StrLen name,StrLen x)
    {
     return obj->addPad<Geometry::Ratio>(index,name,StrToReal(x));
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
  return DelItem(formulas,index);
 }

bool Contour::formulaAddTest(StrLen text,CharAccent *accent) const // TODO
 {
  ulen len=text.len;
  ulen off=len/2;

  Range(accent,off).set(CharNormal);
  Range(accent+off,len-off).set(CharError);

  return false;
 }

bool Contour::formulaAdd(ulen index,StrLen text) // TODO
 {
  Used(index);
  Used(text);

  return false;
 }

 // save/load

void Contour::erase()
 {
  pads.erase();
  formulas.erase();
 }

void Contour::save(StrLen file_name,ErrorText &etext) const // TODO
 {
  Used(file_name);

  etext.setText("Not implemented yet"_c);
 }

void Contour::load(StrLen file_name,ErrorText &etext) // TODO
 {
  Used(file_name);

  etext.setText("Not implemented yet"_c);
 }

} // namespace App

