/* main.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: ParserGen 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2017 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <CCore/inc/Print.h>
#include <CCore/inc/Exception.h>
#include <CCore/inc/PrintStem.h>

#include <CCore/inc/lang/LangDataMap.h>

using namespace CCore;
using namespace CCore::Lang;

/* Main() */

void Main(StrLen ddl_file_name,StrLen class_name,StrLen h_file_name,StrLen cpp_file_name)
 {
  DataMap map(ddl_file_name);

  map.sanity();

  const TypeDef::Lang &lang=map.getLang();

  PrintFile outh(h_file_name);
  PrintFile outc(cpp_file_name);

  // 1

  {
   Printf(outh,"/* enum AtomClass */\n\n");

   Printf(outh,"enum AtomClass\n");
   Printf(outh," {\n");
   Printf(outh,"  AtomNull = 0,\n\n");

   for(const TypeDef::Atom &atom : lang.atoms.getRange() )
     {
      auto ac=atom.index+1;

      Printf(outh,"  Atom_XXX_#; = #;,  /*  #;  */\n",ac,ac,atom.name.getStr());
     }

   Printf(outh," };\n\n");

   Printf(outh,"const char * GetTextDesc(AtomClass ac);\n\n");

   Printf(outh,"/* struct #; */\n\n",class_name);

   Printf(outh,"struct #;\n",class_name);
   Printf(outh," {\n",class_name);

   Printf(outh,"  using Rule = int ;\n\n");

   Printf(outh,"  static const Rule NoRule = -1 ;\n\n");
   Printf(outh,"  static const Rule ShiftRule = 0 ;\n\n");

   Printf(outh,"  using ElementIndex = int ;\n\n");

   Printf(outh,"  using Property = Rule (*)(AtomClass) ;\n\n");

   Printf(outh,"  using Transition = const State * (*)(ElementIndex) ;\n\n");

   Printf(outh,"  static ElementIndex RuleOutput(Rule rule);\n\n");

   Printf(outh,"  static ElementIndex AtomToElement(AtomClass ac) { return ac; }\n\n");

   Printf(outh,"  struct State\n");
   Printf(outh,"   {\n");
   Printf(outh,"    Property prop;\n");
   Printf(outh,"    Transition trans;\n");
   Printf(outh,"   };\n\n");

   Printf(outh,"  static const State *Start;\n");

   Printf(outh," };\n\n");
  }

  // 2

  {
   Printf(outc,"/* enum AtomClass */\n\n");

   Printf(outc,"const char * GetTextDesc(AtomClass ac)\n");
   Printf(outc," {\n");
   Printf(outc,"  switch( ac )\n");
   Printf(outc,"    {\n");
   Printf(outc,"     case 0 : return \"(Null)\";\n");

   for(const TypeDef::Atom &atom : lang.atoms.getRange() )
     {
      auto ac=atom.index+1;

      Printf(outc,"     case #; : return \"#;\";\n",ac,atom.name.getStr());
     }

   Printf(outc,"\n     default: return \"???\";\n");
   Printf(outc,"    }\n");
   Printf(outc," };\n\n");
  }

  // 3

  {
   Printf(outc,"/* struct #; */\n\n",class_name);

   Printf(outc,"auto #;::RuleOutput(Rule rule) -> ElementIndex\n",class_name);
   Printf(outc," {\n");
   Printf(outc,"  switch( rule )\n");
   Printf(outc,"    {\n");

   for(const TypeDef::Rule &rule : lang.rules.getRange() )
     {
      Printf(outc,"     case #; : return #;;\n",rule.index+1,rule.result->element->index+1);
     }

   Printf(outc,"\n     default: return 0;\n");
   Printf(outc,"    }\n");
   Printf(outc," }\n\n");
  }

  // 4

  ulen state_count=lang.states.len;

  {
   for(const TypeDef::Final &final : lang.finals.getRange() )
     {
      Printf(outc,"static #;::Rule Prop#;(AtomClass ac)\n",class_name,final.index);
      Printf(outc," {\n");
      Printf(outc,"  switch( ac )\n");
      Printf(outc,"    {\n");

      for(const TypeDef::Final::Action &action : final.actions.getRange() )
        {
         TypeDef::AtomIndex ac=0;

         if( action.atom ) ac=action.atom->index+1;

         TypeDef::RuleIndex rule=0;

         if( action.rule ) rule=action.rule->index+1;

         Printf(outc,"     case #; : return #;;\n",ac,rule);
        }

      Printf(outc,"\n     default: return -1;\n");
      Printf(outc,"    }\n");
      Printf(outc," }\n\n");
     }

   Printf(outc,"static const #;::State * TransNone(#;::ElementIndex)\n",class_name,class_name);
   Printf(outc," {\n");
   Printf(outc,"  return 0;\n");
   Printf(outc," }\n\n");

   for(const TypeDef::State &state : lang.states.getRange() )
     {
      if( state.transitions.len==0 ) continue;

      Printf(outc,"static const #;::State * Trans#;(#;::ElementIndex ei)\n",class_name,state.index,class_name);
      Printf(outc," {\n");
      Printf(outc,"  switch( ei )\n");
      Printf(outc,"    {\n");

      for(const TypeDef::State::Transition &t : state.transitions.getRange() )
        {
         Printf(outc,"     case #; : return #;::Start+#;;\n",t.element->index+1,class_name,t.state->index);
        }

      Printf(outc,"\n     default: return 0;\n");
      Printf(outc,"    }\n");
      Printf(outc," }\n\n");
     }

   Printf(outc,"static const #;::State StateList[#;]=\n",class_name,state_count);
   Printf(outc," {");

   PrintFirst stem("\n  ",",\n  ");

   for(const TypeDef::State &state : lang.states.getRange() )
     {
      if( state.transitions.len )
        Printf(outc,"#;{Prop#;,Trans#;}",stem,state.final->index,state.index);
      else
        Printf(outc,"#;{Prop#;,TransNone}",stem,state.final->index);
     }

   Printf(outc,"\n };\n\n");

   Printf(outc,"const #;::State * #;::Start=StateList;\n\n",class_name,class_name);
  }
 }

/* main() */

int main(int argc,const char *argv[])
 {
  try
    {
     ReportException report;

     {
      Printf(Con,"--- ParserGen 1.00 ---\n--- Copyright (c) 2017 Sergey Strukov. All rights reserved. ---\n\n");

      if( argc!=5 )
        {
         Printf(Con,"Usage: CCore-ParserGen <Lang.ddl> <ClassName> <Lang.gen.h> <Lang.gen.cpp>\n");

         return 1;
        }

      Main(argv[1],argv[2],argv[3],argv[4]);
     }

     report.guard();

     return 0;
    }
  catch(CatchType)
    {
     return 1;
    }
 }


