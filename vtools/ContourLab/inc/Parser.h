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

namespace App {

/* classes */

//enum CharClass;

class CharProp;

struct Token;

class Tokenizer;

class ParserBase;

class PadTextParser;

//enum AtomClass;

class Atom;

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

/* class Atom */

class Atom : Token
 {
  AtomClass ac;

  Atom() noexcept : ac(Atom_Nothing) {}

  Atom(const Token &tok);
 };

} // namespace App

#endif
