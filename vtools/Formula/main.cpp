/* main.cpp */
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

#include <CCore/inc/lang/LangDataMap.h>

#include <CCore/inc/Print.h>
#include <CCore/inc/Exception.h>

namespace App {

using namespace CCore;

enum AtomClass
 {
  Atom_Nothing = 0,

  Atom_Name,
  Atom_Number,
  Atom_Angle,
  Atom_Length,

  Atom_obr,       // (
  Atom_cbr,       // )
  Atom_asterisk,  // *
  Atom_div,       // /
  Atom_plus,      // +
  Atom_minus,     // -
  Atom_comma,     // ,
  Atom_assign     // =
 };

AtomClass Map(int atom)
 {
  switch( atom )
    {
     case 0 : return Atom_obr;
     case 1 : return Atom_cbr;
     case 2 : return Atom_asterisk;
     case 3 : return Atom_plus;
     case 4 : return Atom_comma;
     case 5 : return Atom_minus;
     case 6 : return Atom_div;
     case 7 : return Atom_assign;
     case 8 : return Atom_Angle;
     case 9 : return Atom_Length;
     case 10 : return Atom_Name;
     case 11 : return Atom_Number;

     default: return Atom_Nothing;
    }
 }

void Main()
 {
  Lang::DataMap data("Formula.ddl");

  data.sanity();

  PrintFile out("CodeGen.txt");

  // Property

  Printf(out,"int FormulaParserData::Property(int state)\n {\n");

  Printf(out,"  switch( state )\n    {\n");

  for(auto &state : data.getLang().states.getRange() )
    {
     auto index=state.index;
     auto final_index=state.final->index;

     Printf(out,"     case #; : return #;;\n",index,final_index);
    }

  Printf(out,"     default: return -1;\n");

  Printf(out,"    }\n");
  Printf(out," }\n\n");

  // Rule

  Printf(out,"int FormulaParserData::Rule(int prop,AtomClass ac)\n {\n");

  Printf(out,"  switch( prop )\n   {\n");

  for(auto &final : data.getLang().finals.getRange() )
    {
     auto index=final.index;

     Printf(out,"    case #; :\n",index);
     Printf(out,"     switch( ac )\n");
     Printf(out,"       {\n");

     for(auto &action : final.actions.getRange() )
       {
        Lang::TypeDef::AtomIndex atom_index;

        if( +action.atom )
          atom_index=Map(action.atom->index);
        else
          atom_index=0;

        Lang::TypeDef::RuleIndex rule_index;

        if( +action.rule )
          rule_index=action.rule->index+1;
        else
          rule_index=0;

        Printf(out,"        case #; : return #;;\n",atom_index,rule_index);
       }

     Printf(out,"        default: return -1;\n");
     Printf(out,"       }\n");
     Printf(out,"    break;\n\n");
    }

  Printf(out,"    default: return -1;\n");

  Printf(out,"   }\n");
  Printf(out," }\n\n");

  // ElemRule

  Printf(out,"int FormulaParserData::ElemRule(int rule)\n {\n");

  Printf(out,"  switch( rule )\n    {\n");

  for(auto &rule : data.getLang().rules.getRange() )
    {
     auto index=rule.index+1;
     auto element_index=rule.result->element->index;

     Printf(out,"     case #; : return #;;\n",index,element_index);
    }

  Printf(out,"     default: return -1;\n");

  Printf(out,"    }\n");
  Printf(out," }\n\n");

  // ElemAtom

  Printf(out,"int FormulaParserData::ElemAtom(AtomClass ac)\n {\n");

  Printf(out,"  switch( ac )\n    {\n");

  for(auto &atom : data.getLang().atoms.getRange() )
    {
     auto index=Map(atom.index);
     auto element_index=atom.element->index;

     Printf(out,"     case #; : return #;;\n",index,element_index);
    }

  Printf(out,"     default: return -1;\n");

  Printf(out,"    }\n");
  Printf(out," }\n\n");

  // Transition

  Printf(out,"int FormulaParserData::Transition(int state,int element)\n {\n");

  Printf(out,"  switch( state )\n    {\n");

  for(auto &state : data.getLang().states.getRange() )
    {
     auto index=state.index;

     Printf(out,"     case #; :\n",index);
     Printf(out,"      switch( element )\n");
     Printf(out,"        {\n");

     for(auto &trans : state.transitions.getRange() )
       {
        auto element_index=trans.element->index;
        auto dst_index=trans.state->index;

        Printf(out,"         case #; : return #;;\n",element_index,dst_index);
       }

     Printf(out,"         default: return -1;\n");
     Printf(out,"        }\n");
     Printf(out,"     break;\n\n");
    }

  Printf(out,"     default: return -1;\n");

  Printf(out,"    }\n");
  Printf(out," }\n\n");
 }

} // namespace App

/* main() */

using namespace App;

int main()
 {
  try
    {
     ReportException report;

     Main();

     report.guard();

     return 0;
    }
  catch(CatchType)
    {
     return 1;
    }

  return 0;
 }
