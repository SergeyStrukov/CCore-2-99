/* Parser.cpp */
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

#include <inc/Parser.h>

namespace App {

/* class CharProp */

CharProp::CharProp()
 {
  setSet(GetDigitChars(),Char_Digit);
  setSet(GetCLetterChars(),Char_Letter);
  setSet(GetSpaceChars(),Char_Space);

  setSet(".",Char_Dot);

  setSet("()+-*/=,",Char_Punct);
 }

const CharProp CharProp::Object;

/* class Tokenizer */

ulen Tokenizer::ScanLetterDigit(StrLen text)
 {
  ulen len=text.len;

  for(++text; +text && CharIsLetterDigit(*text) ;++text);

  return len-text.len;
 }

ulen Tokenizer::ScanLetterDigitDot(StrLen text)
 {
  ulen len=text.len;

  for(++text; +text && CharIsLetterDigitDot(*text) ;++text);

  return len-text.len;
 }

ulen Tokenizer::ScanSpace(StrLen text)
 {
  ulen len=text.len;

  for(++text; +text && CharIsSpace(*text) ;++text);

  return len-text.len;
 }

TokenClass Tokenizer::IsNumber(StrLen text)
 {
  TokenClass ret;

  switch( text.back(1) )
    {
     case 'p' : ret=Token_Length; text.len--; break;

     case 'g' : ret=Token_Angle; text.len--; break;

     default: ret=Token_Number;
    }

  bool has_dot=false;
  bool has_digit=false;

  for(; +text ;++text)
    switch( GetCharClass(*text) )
      {
       case Char_Dot : if( has_dot ) return Token_Other; has_dot=true; break;

       case Char_Digit : has_digit=true; break;

       default: return Token_Other;
      }

  return has_digit?ret:Token_Other;
 }

Token Tokenizer::cut(TokenClass tc,ulen len)
 {
  Token ret(tc,pos,text+=len);

  pos+=len;

  return ret;
 }

Token Tokenizer::next_number()
 {
  ulen len=ScanLetterDigitDot(text);

  return cut(IsNumber(text.prefix(len)),len);
 }

Token Tokenizer::next_word()
 {
  return cut(Token_Word,ScanLetterDigit(text));
 }

Token Tokenizer::next_punct()
 {
  return cut(Token_Punct,1);
 }

Token Tokenizer::next_space()
 {
  return cut(Token_Space,ScanSpace(text));
 }

Token Tokenizer::next_other()
 {
  return cut(Token_Other,1);
 }

Token Tokenizer::next()
 {
  switch( GetCharClass(*text) )
    {
     case Char_Dot :
     case Char_Digit  : return next_number();

     case Char_Letter : return next_word();

     case Char_Punct  : return next_punct();

     case Char_Space  : return next_space();

     case Char_Other  : return next_other();
    }

  return Token();
 }

/* class ParserBase */

CharAccent ParserBase::Map(TokenClass tc)
 {
  switch( tc )
    {
     case Token_Number : return CharNumber;

     case Token_Length : return CharLength;

     case Token_Angle : return CharAngle;

     case Token_Word : return CharName;

     case Token_Punct : return CharPunct;

     case Token_Space : return CharNormal;

     default: return CharError;
    }
 }

void ParserBase::paint(Token token,CharAccent acc)
 {
  Range(accent+token.pos,token.str.len).set(acc);
 }

void ParserBase::paint(Token token)
 {
  paint(token,Map(token.tc));
 }

void ParserBase::start()
 {
 }

void ParserBase::next(Token token)
 {
  paint(token);
 }

void ParserBase::stop()
 {
 }

void ParserBase::run()
 {
  start();

  while( +tok ) next(tok.next());

  stop();
 }

} // namespace App

