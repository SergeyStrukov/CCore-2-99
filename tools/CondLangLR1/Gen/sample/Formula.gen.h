/* enum AtomClass */

enum AtomClass
 {
  AtomNull = 0,

  Atom_XXX_1 = 1,  /*  (  */
  Atom_XXX_2 = 2,  /*  )  */
  Atom_XXX_3 = 3,  /*  *  */
  Atom_XXX_4 = 4,  /*  +  */
  Atom_XXX_5 = 5,  /*  ,  */
  Atom_XXX_6 = 6,  /*  -  */
  Atom_XXX_7 = 7,  /*  /  */
  Atom_XXX_8 = 8,  /*  =  */
  Atom_XXX_9 = 9,  /*  Angle  */
  Atom_XXX_10 = 10,  /*  Length  */
  Atom_XXX_11 = 11,  /*  Name  */
  Atom_XXX_12 = 12,  /*  Number  */
 };

const char * GetTextDesc(AtomClass ac);

/* struct FormulaParserData */

struct FormulaParserData
 {
  using Rule = int ;

  static const Rule NoRule = -1 ;

  static const Rule ShiftRule = 0 ;

  using ElementIndex = int ;

  using Property = Rule (*)(AtomClass) ;

  using Transition = const State * (*)(ElementIndex) ;

  static ElementIndex RuleOutput(Rule rule);

  static ElementIndex AtomToElement(AtomClass ac) { return ac; }

  struct State
   {
    Property prop;
    Transition trans;
   };

  static const State *Start;
 };

