/* Parser.h */
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

#ifndef Parser_h
#define Parser_h

#include <inc/Application.h>

#include <CCore/inc/TextTools.h>
#include <CCore/inc/Array.h>

namespace App {

/* classes */

//enum CharClass;

class CharProp;

struct Token;

class Tokenizer;

class ParserBase;

class PadTextParser;

//enum AtomClass;

struct Atom;

struct FormulaParserData;

template <class Context> class FormulaTextParser;

/* enum CharClass */

enum CharClass
 {
  Char_Digit,
  Char_Letter,
  Char_Dot,
  Char_Punct,
  Char_Space,

  Char_Other
 };

inline bool IsLetterDigit(CharClass cc) { return cc<=Char_Letter; }

inline bool IsLetterDigitDot(CharClass cc) { return cc<=Char_Dot; }

/* class CharProp */

class CharProp : public CharPropTable<CharClass,Char_Other>
 {
  public:

   CharProp();

   static const CharProp Object;
 };

/* char functions */

inline CharClass GetCharClass(char ch) { return CharProp::Object[ch]; }

inline bool CharIsLetterDigit(char ch) { return IsLetterDigit(GetCharClass(ch)); }

inline bool CharIsLetterDigitDot(char ch) { return IsLetterDigitDot(GetCharClass(ch)); }

//---------------------------------
//
// From CharProp.h and TextTools.h
//
//---------------------------------
//
// bool CharIsSpace(char ch);
//
//---------------------------------

/* enum TokenClass */

enum TokenClass
 {
  Token_Number,
  Token_Length,
  Token_Angle,

  Token_Word,
  Token_Punct,
  Token_Space,

  Token_Other
 };

/* struct Token */

struct Token
 {
  TokenClass tc;
  ulen pos;
  StrLen str;

  Token() : tc(Token_Other),pos(0) {}

  Token(TokenClass tc_,ulen pos_,StrLen str_) : tc(tc_),pos(pos_),str(str_) {}

  StrLen getNumber() const;

  bool isName() const
   {
    return tc==Token_Word && !is("angle"_c,"len"_c,"point"_c,"ratio"_c) ;
   }

  bool is(StrLen name) const { return str.equal(name); }

  bool is(TokenClass tc_) const { return tc==tc_; }

  bool is(char ch) const { return tc==Token_Punct && ch==str[0] ; }

  template <class ... TT>
  bool is(TT ... tt) const { return ( ... || is(tt) ); }
 };

/* class Tokenizer */

class Tokenizer : NoCopy
 {
   ulen pos = 0 ;
   StrLen text;

  private:

   static ulen ScanLetterDigit(StrLen text); // >=1

   static ulen ScanLetterDigitDot(StrLen text); // >=1

   static ulen ScanSpace(StrLen text); // >=1

   static TokenClass IsNumber(StrLen text); // >=1

  private:

   Token cut(TokenClass tc,ulen len);

   Token next_number();

   Token next_word();

   Token next_punct();

   Token next_space();

   Token next_other();

  public:

   explicit Tokenizer(StrLen text_) : text(text_) {}

   ulen operator + () const { return text.len; }

   bool operator ! () const { return !text.len; }

   Token next();
 };

/* class ParserBase */

class ParserBase : NoCopy
 {
   Tokenizer tok;
   CharAccent *accent;

  private:

   static CharAccent Map(TokenClass tc);

  protected:

   void paint(Token token,CharAccent acc);

   void paint(Token token);

   virtual void next(Token token);

   virtual void stop();

  public:

   ParserBase(StrLen text,CharAccent *accent_) : tok(text),accent(accent_) {}

   void run();
 };

/* class PadTextParser */

class PadTextParser : public ParserBase
 {
   int state = 0 ;

   Token name;
   Token value;
   Token value2;

  private:

   void paintError(Token token)
    {
     state=-1;

     paint(token,CharError);
    }

   virtual void next(Token token);

   virtual void stop();

  protected:

   virtual bool point(StrLen name,StrLen x,StrLen y);

   virtual bool length(StrLen name,StrLen x);

   virtual bool angle(StrLen name,StrLen x);

   virtual bool ratio(StrLen name,StrLen x);

  public:

   explicit PadTextParser(StrLen text,CharAccent *accent=0) : ParserBase(text,accent) {}

   operator bool() const { return state!=-1; }
 };

/* enum AtomClass */

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

/* struct Atom */

struct Atom : Token
 {
  AtomClass ac;

  Atom() noexcept : ac(Atom_Nothing) {}

  Atom(const Token &token);
 };

/* struct FormulaParserData */

struct FormulaParserData
 {
  static int Property(int state);

  static int Rule(int prop,AtomClass ac); // -1 no rule , 0 <- OR STOP , ac = Atom_Nothing (End)

  static int ElemRule(int rule); // production kind number

  static int ElemAtom(AtomClass ac);

  static int Transition(int state,int element);
 };

/* class FormulaTextParser<Context> */

template <class Context>
class FormulaTextParser : public ParserBase , FormulaParserData
 {
   struct Element : MemBase
    {
     virtual ~Element() {}
    };

   struct ElementAtom : Element
    {
     Atom atom;

     explicit ElementAtom(const Atom &atom_) : atom(atom_) {}
    };

   struct Element_EXPR;

   struct Element_FORMULA;

   struct Element_EXPR_LIST;

   struct Element_FORMULA : Element
    {
     bool set(Context &ctx,ElementAtom *name,ElementAtom *,Element_EXPR *expr)
      {
       return ctx.set(name->atom.str,expr->value);
      }
    };

   struct Element_EXPR : Element
    {
     typename Context::ExprType value;

     bool add(Context &ctx,Element_EXPR *a,ElementAtom *,Element_EXPR *b) { return ctx.add(value,a->value,b->value); }

     bool sub(Context &ctx,Element_EXPR *a,ElementAtom *,Element_EXPR *b) { return ctx.sub(value,a->value,b->value); }

     bool mul(Context &ctx,Element_EXPR *a,ElementAtom *,Element_EXPR *b) { return ctx.mul(value,a->value,b->value); }

     bool div(Context &ctx,Element_EXPR *a,ElementAtom *,Element_EXPR *b) { return ctx.div(value,a->value,b->value); }

     bool neg(Context &ctx,ElementAtom *,Element_EXPR *a) { return ctx.neg(value,a->value); }

     bool brace(Context &,ElementAtom *,Element_EXPR *a,ElementAtom *) { value=a->value; return true; }

     bool func(Context &ctx,ElementAtom *name,ElementAtom *,Element_EXPR_LIST *list,ElementAtom *)
      {
       return ctx.func(value,name->atom.str,Range(list->values));
      }

     bool arg(Context &ctx,ElementAtom *name) { return ctx.arg(value,name->atom.str); }

     bool number(Context &ctx,ElementAtom *number) { return ctx.number(value,number->atom.getNumber()); }

     bool angle(Context &ctx,ElementAtom *angle) { return ctx.angle(value,angle->atom.getNumber()); }

     bool length(Context &ctx,ElementAtom *length) { return ctx.angle(value,length->atom.getNumber()); }

     bool point(Context &ctx,ElementAtom *,ElementAtom *x,ElementAtom *,ElementAtom *y,ElementAtom *)
      {
       return ctx.point(value,x->atom.getNumber(),y->atom.getNumber());
      }
    };

   struct Element_EXPR_LIST : Element
    {
     RefArray<typename Context::ExprType> values;

     bool empty(Context &) { return true; }

     bool one(Context &,Element_EXPR *expr) { values.reserve(10); values.append_copy(expr->value); return true; }

     bool ext(Context &,Element_EXPR_LIST *list,ElementAtom *,Element_EXPR *expr)
      {
       values=list->values;

       values.append_copy(expr->value);

       return true;
      }
    };

   struct State
    {
     int state = 0 ;
     Element *element = 0 ;

     State() noexcept {}

     State(int state_,Element *element_) : state(state_),element(element_) {}

     template <class E>
     operator E * () const { return static_cast<E *>(element); }
    };

   Context &ctx;

   DynArray<State> stack;
   bool ok = true ;

  private:

   void paintError(Token token)
    {
     ok=false;

     paint(token,CharError);
    }

   State & top()
    {
     ulen len=stack.getLen();

     return stack[len-1];
    }

   void push(const Atom &atom)
    {
     stack.reserve(1);

     int state=top().state;

     stack.append_fill(Transition(state,ElemAtom(atom.ac)),new ElementAtom(atom));
    }

   void pop(ulen count)
    {
     for(State &state : Range(stack).suffix(count) ) delete state.element;

     stack.shrink(count);
    }

   template <class T>
   void paintAtom(T) {}

   void paintAtom(ElementAtom *element) { paintError(element->atom); }

   template <class ... TT>
   void paintAtoms(TT ... tt)
    {
     ( ... , paintAtom(tt) );
    }

   template <class E,class ... TT,int ... IList>
   bool applyRule(bool (E::*method)(Context &,TT...),int elem,Meta::IndexListBox<IList...>)
    {
     stack.reserve(1);

     ulen len=stack.getLen();
     State *base=stack.getPtr()+(len-sizeof ... (TT));

     E *element=new E{};

     bool ok=(element->*method)(ctx,base[IList]...);

     if( !ok )
       {
        paintAtoms<TT...>(base[IList]...);

        delete element;

        return false;
       }

     pop(sizeof ... (TT));

     int state=top().state;

     stack.append_fill(Transition(state,elem),element);

     return true;
    }

   template <class E,class ... TT>
   bool applyRule(bool (E::*method)(Context &,TT...),int elem)
    {
     return applyRule(method,elem, Meta::IndexList<0,TT...>() );
    }

   bool applyRule(int rule)
    {
     int elem=ElemRule(rule);

     switch( rule )
       {
        case 1 : return applyRule(&Element_FORMULA::set,elem);

        case 2 : return applyRule(&Element_EXPR::add,elem);

        case 3 : return applyRule(&Element_EXPR::sub,elem);

        case 4 : return applyRule(&Element_EXPR::mul,elem);

        case 5 : return applyRule(&Element_EXPR::div,elem);

        case 6 : return applyRule(&Element_EXPR::neg,elem);

        case 7 : return applyRule(&Element_EXPR::brace,elem);

        case 8 : return applyRule(&Element_EXPR::func,elem);

        case 9 : return applyRule(&Element_EXPR::arg,elem);

        case 10 : return applyRule(&Element_EXPR::number,elem);

        case 11 : return applyRule(&Element_EXPR::angle,elem);

        case 12 : return applyRule(&Element_EXPR::length,elem);

        case 13 : return applyRule(&Element_EXPR::point,elem);

        case 14 : return applyRule(&Element_EXPR_LIST::empty,elem);

        case 15 : return applyRule(&Element_EXPR_LIST::one,elem);

        case 16 : return applyRule(&Element_EXPR_LIST::ext,elem);

        default: return false;
       }
    }

   virtual void next(Token token)
    {
     paint(token);

     if( !ok ) return;

     if( token.is(Token_Space) ) return;

     if( token.is(Token_Other) )
       {
        ok=false;

        return;
       }

     Atom atom(token);

     for(;;)
       switch( int rule=Rule(Property(top().state),atom.ac) )
         {
          case -1 : paintError(token); return;

          case 0 : push(atom); return;

          default: if( !applyRule(rule) ) { ok=false; return; }
         }
    }

   virtual void stop()
    {
     if( !ok ) return;

     for(;;)
       switch( int rule=Rule(Property(top().state),Atom_Nothing) )
         {
          case -1 : ok=false; return;

          case 0 : return;

          default: if( !applyRule(rule) ) { ok=false; return; }
         }
    }

  public:

   FormulaTextParser(Context &ctx_,StrLen text,CharAccent *accent=0)
    : ParserBase(text,accent),
      ctx(ctx_)
    {
     stack.append_default();
    }

   ~FormulaTextParser()
    {
     ulen len=stack.getLen();

     pop(len-1);
    }

   operator bool() const { return ok; }
 };

} // namespace App

#endif
