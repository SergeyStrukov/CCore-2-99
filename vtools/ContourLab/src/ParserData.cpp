/* ParserData.cpp */
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

/* struct FormulaParserData */

int FormulaParserData::Property(int state)
 {
  switch( state )
    {
     case 0 : return 37;
     case 1 : return 1;
     case 2 : return 2;
     case 3 : return 2;
     case 4 : return 2;
     case 5 : return 3;
     case 6 : return 3;
     case 7 : return 3;
     case 8 : return 3;
     case 9 : return 3;
     case 10 : return 3;
     case 11 : return 3;
     case 12 : return 3;
     case 13 : return 3;
     case 14 : return 3;
     case 15 : return 3;
     case 16 : return 3;
     case 17 : return 3;
     case 18 : return 3;
     case 19 : return 3;
     case 20 : return 3;
     case 21 : return 3;
     case 22 : return 4;
     case 23 : return 4;
     case 24 : return 4;
     case 25 : return 5;
     case 26 : return 6;
     case 27 : return 7;
     case 28 : return 8;
     case 29 : return 9;
     case 30 : return 10;
     case 31 : return 11;
     case 32 : return 12;
     case 33 : return 13;
     case 34 : return 14;
     case 35 : return 15;
     case 36 : return 16;
     case 37 : return 17;
     case 38 : return 18;
     case 39 : return 19;
     case 40 : return 20;
     case 41 : return 20;
     case 42 : return 20;
     case 43 : return 21;
     case 44 : return 22;
     case 45 : return 23;
     case 46 : return 24;
     case 47 : return 25;
     case 48 : return 26;
     case 49 : return 27;
     case 50 : return 28;
     case 51 : return 29;
     case 52 : return 30;
     case 53 : return 31;
     case 54 : return 32;
     case 55 : return 32;
     case 56 : return 32;
     case 57 : return 32;
     case 58 : return 32;
     case 59 : return 32;
     case 60 : return 33;
     case 61 : return 33;
     case 62 : return 33;
     case 63 : return 34;
     case 64 : return 34;
     case 65 : return 34;
     case 66 : return 35;
     case 67 : return 35;
     case 68 : return 35;
     case 69 : return 36;
     case 70 : return 0;
     case 71 : return 38;
     case 72 : return 38;
     case 73 : return 38;
     case 74 : return 39;
     case 75 : return 40;
     case 76 : return 41;
     case 77 : return 42;
     case 78 : return 43;
     case 79 : return 44;
     case 80 : return 45;
     case 81 : return 46;
     case 82 : return 47;
     case 83 : return 48;
     case 84 : return 49;
     case 85 : return 50;
     case 86 : return 51;
     case 87 : return 52;
     case 88 : return 53;
     default: return -1;
    }
 }

int FormulaParserData::Rule(int prop,AtomClass ac)
 {
  switch( prop )
   {
    case 0 :
     switch( ac )
       {
        case 5 : return 0;
        case 6 : return 9;
        case 7 : return 9;
        case 9 : return 9;
        case 11 : return 9;
        case 10 : return 9;
        case 8 : return 9;
        default: return -1;
       }
    break;

    case 1 :
     switch( ac )
       {
        case 5 : return 0;
        case 6 : return 9;
        case 7 : return 9;
        case 9 : return 9;
        case 10 : return 9;
        case 8 : return 9;
        default: return -1;
       }
    break;

    case 2 :
     switch( ac )
       {
        case 5 : return 0;
        case 6 : return 14;
        case 10 : return 0;
        case 3 : return 0;
        case 4 : return 0;
        case 1 : return 0;
        case 2 : return 0;
        default: return -1;
       }
    break;

    case 3 :
     switch( ac )
       {
        case 5 : return 0;
        case 10 : return 0;
        case 3 : return 0;
        case 4 : return 0;
        case 1 : return 0;
        case 2 : return 0;
        default: return -1;
       }
    break;

    case 4 :
     switch( ac )
       {
        case 5 : return 0;
        case 3 : return 0;
        case 4 : return 0;
        case 1 : return 0;
        case 2 : return 0;
        default: return -1;
       }
    break;

    case 5 :
     switch( ac )
       {
        case 6 : return 2;
        case 7 : return 0;
        case 9 : return 2;
        case 11 : return 2;
        case 10 : return 2;
        case 8 : return 0;
        default: return -1;
       }
    break;

    case 6 :
     switch( ac )
       {
        case 6 : return 2;
        case 7 : return 0;
        case 9 : return 2;
        case 10 : return 2;
        case 8 : return 0;
        default: return -1;
       }
    break;

    case 7 :
     switch( ac )
       {
        case 6 : return 3;
        case 7 : return 0;
        case 9 : return 3;
        case 11 : return 3;
        case 10 : return 3;
        case 8 : return 0;
        default: return -1;
       }
    break;

    case 8 :
     switch( ac )
       {
        case 6 : return 3;
        case 7 : return 0;
        case 9 : return 3;
        case 10 : return 3;
        case 8 : return 0;
        default: return -1;
       }
    break;

    case 9 :
     switch( ac )
       {
        case 6 : return 4;
        case 7 : return 4;
        case 9 : return 4;
        case 11 : return 4;
        case 10 : return 4;
        case 8 : return 4;
        default: return -1;
       }
    break;

    case 10 :
     switch( ac )
       {
        case 6 : return 4;
        case 7 : return 4;
        case 9 : return 4;
        case 10 : return 4;
        case 8 : return 4;
        default: return -1;
       }
    break;

    case 11 :
     switch( ac )
       {
        case 6 : return 5;
        case 7 : return 5;
        case 9 : return 5;
        case 11 : return 5;
        case 10 : return 5;
        case 8 : return 5;
        default: return -1;
       }
    break;

    case 12 :
     switch( ac )
       {
        case 6 : return 5;
        case 7 : return 5;
        case 9 : return 5;
        case 10 : return 5;
        case 8 : return 5;
        default: return -1;
       }
    break;

    case 13 :
     switch( ac )
       {
        case 6 : return 6;
        case 7 : return 6;
        case 9 : return 6;
        case 11 : return 6;
        case 10 : return 6;
        case 8 : return 6;
        default: return -1;
       }
    break;

    case 14 :
     switch( ac )
       {
        case 6 : return 6;
        case 7 : return 6;
        case 9 : return 6;
        case 10 : return 6;
        case 8 : return 6;
        default: return -1;
       }
    break;

    case 15 :
     switch( ac )
       {
        case 6 : return 7;
        case 7 : return 7;
        case 9 : return 7;
        case 11 : return 7;
        case 10 : return 7;
        case 8 : return 7;
        default: return -1;
       }
    break;

    case 16 :
     switch( ac )
       {
        case 6 : return 7;
        case 7 : return 7;
        case 9 : return 7;
        case 10 : return 7;
        case 8 : return 7;
        default: return -1;
       }
    break;

    case 17 :
     switch( ac )
       {
        case 6 : return 8;
        case 7 : return 8;
        case 9 : return 8;
        case 11 : return 8;
        case 10 : return 8;
        case 8 : return 8;
        default: return -1;
       }
    break;

    case 18 :
     switch( ac )
       {
        case 6 : return 8;
        case 7 : return 8;
        case 9 : return 8;
        case 10 : return 8;
        case 8 : return 8;
        default: return -1;
       }
    break;

    case 19 :
     switch( ac )
       {
        case 6 : return 10;
        case 7 : return 10;
        case 9 : return 10;
        case 11 : return 10;
        case 10 : return 10;
        case 8 : return 10;
        default: return -1;
       }
    break;

    case 20 :
     switch( ac )
       {
        case 6 : return 10;
        case 7 : return 10;
        case 9 : return 10;
        case 11 : return 0;
        case 10 : return 10;
        case 8 : return 10;
        default: return -1;
       }
    break;

    case 21 :
     switch( ac )
       {
        case 6 : return 10;
        case 7 : return 10;
        case 9 : return 10;
        case 10 : return 10;
        case 8 : return 10;
        default: return -1;
       }
    break;

    case 22 :
     switch( ac )
       {
        case 6 : return 11;
        case 7 : return 11;
        case 9 : return 11;
        case 11 : return 11;
        case 10 : return 11;
        case 8 : return 11;
        default: return -1;
       }
    break;

    case 23 :
     switch( ac )
       {
        case 6 : return 11;
        case 7 : return 11;
        case 9 : return 11;
        case 10 : return 11;
        case 8 : return 11;
        default: return -1;
       }
    break;

    case 24 :
     switch( ac )
       {
        case 6 : return 12;
        case 7 : return 12;
        case 9 : return 12;
        case 11 : return 12;
        case 10 : return 12;
        case 8 : return 12;
        default: return -1;
       }
    break;

    case 25 :
     switch( ac )
       {
        case 6 : return 12;
        case 7 : return 12;
        case 9 : return 12;
        case 10 : return 12;
        case 8 : return 12;
        default: return -1;
       }
    break;

    case 26 :
     switch( ac )
       {
        case 6 : return 13;
        case 7 : return 13;
        case 9 : return 13;
        case 11 : return 13;
        case 10 : return 13;
        case 8 : return 13;
        default: return -1;
       }
    break;

    case 27 :
     switch( ac )
       {
        case 6 : return 13;
        case 7 : return 13;
        case 9 : return 13;
        case 10 : return 13;
        case 8 : return 13;
        default: return -1;
       }
    break;

    case 28 :
     switch( ac )
       {
        case 6 : return 15;
        case 7 : return 0;
        case 9 : return 0;
        case 11 : return 15;
        case 10 : return 0;
        case 8 : return 0;
        default: return -1;
       }
    break;

    case 29 :
     switch( ac )
       {
        case 6 : return 15;
        case 9 : return 0;
        case 11 : return 15;
        case 10 : return 0;
        default: return -1;
       }
    break;

    case 30 :
     switch( ac )
       {
        case 6 : return 16;
        case 7 : return 0;
        case 9 : return 0;
        case 11 : return 16;
        case 10 : return 0;
        case 8 : return 0;
        default: return -1;
       }
    break;

    case 31 :
     switch( ac )
       {
        case 6 : return 16;
        case 9 : return 0;
        case 11 : return 16;
        case 10 : return 0;
        default: return -1;
       }
    break;

    case 32 :
     switch( ac )
       {
        case 6 : return 0;
        default: return -1;
       }
    break;

    case 33 :
     switch( ac )
       {
        case 6 : return 0;
        case 7 : return 0;
        case 9 : return 0;
        case 10 : return 0;
        case 8 : return 0;
        default: return -1;
       }
    break;

    case 34 :
     switch( ac )
       {
        case 6 : return 0;
        case 9 : return 0;
        case 10 : return 0;
        default: return -1;
       }
    break;

    case 35 :
     switch( ac )
       {
        case 6 : return 0;
        case 11 : return 0;
        default: return -1;
       }
    break;

    case 36 :
     switch( ac )
       {
        case 12 : return 0;
        default: return -1;
       }
    break;

    case 37 :
     switch( ac )
       {
        case 1 : return 0;
        default: return -1;
       }
    break;

    case 38 :
     switch( ac )
       {
        case 2 : return 0;
        default: return -1;
       }
    break;

    case 39 :
     switch( ac )
       {
        case 0 : return 1;
        case 7 : return 0;
        case 9 : return 0;
        case 10 : return 0;
        case 8 : return 0;
        default: return -1;
       }
    break;

    case 40 :
     switch( ac )
       {
        case 0 : return 1;
        case 9 : return 0;
        case 10 : return 0;
        default: return -1;
       }
    break;

    case 41 :
     switch( ac )
       {
        case 0 : return 2;
        case 7 : return 0;
        case 9 : return 2;
        case 10 : return 2;
        case 8 : return 0;
        default: return -1;
       }
    break;

    case 42 :
     switch( ac )
       {
        case 0 : return 3;
        case 7 : return 0;
        case 9 : return 3;
        case 10 : return 3;
        case 8 : return 0;
        default: return -1;
       }
    break;

    case 43 :
     switch( ac )
       {
        case 0 : return 4;
        case 7 : return 4;
        case 9 : return 4;
        case 10 : return 4;
        case 8 : return 4;
        default: return -1;
       }
    break;

    case 44 :
     switch( ac )
       {
        case 0 : return 5;
        case 7 : return 5;
        case 9 : return 5;
        case 10 : return 5;
        case 8 : return 5;
        default: return -1;
       }
    break;

    case 45 :
     switch( ac )
       {
        case 0 : return 6;
        case 7 : return 6;
        case 9 : return 6;
        case 10 : return 6;
        case 8 : return 6;
        default: return -1;
       }
    break;

    case 46 :
     switch( ac )
       {
        case 0 : return 7;
        case 7 : return 7;
        case 9 : return 7;
        case 10 : return 7;
        case 8 : return 7;
        default: return -1;
       }
    break;

    case 47 :
     switch( ac )
       {
        case 0 : return 8;
        case 7 : return 8;
        case 9 : return 8;
        case 10 : return 8;
        case 8 : return 8;
        default: return -1;
       }
    break;

    case 48 :
     switch( ac )
       {
        case 0 : return 9;
        case 5 : return 0;
        case 7 : return 9;
        case 9 : return 9;
        case 10 : return 9;
        case 8 : return 9;
        default: return -1;
       }
    break;

    case 49 :
     switch( ac )
       {
        case 0 : return 10;
        case 7 : return 10;
        case 9 : return 10;
        case 10 : return 10;
        case 8 : return 10;
        default: return -1;
       }
    break;

    case 50 :
     switch( ac )
       {
        case 0 : return 11;
        case 7 : return 11;
        case 9 : return 11;
        case 10 : return 11;
        case 8 : return 11;
        default: return -1;
       }
    break;

    case 51 :
     switch( ac )
       {
        case 0 : return 12;
        case 7 : return 12;
        case 9 : return 12;
        case 10 : return 12;
        case 8 : return 12;
        default: return -1;
       }
    break;

    case 52 :
     switch( ac )
       {
        case 0 : return 13;
        case 7 : return 13;
        case 9 : return 13;
        case 10 : return 13;
        case 8 : return 13;
        default: return -1;
       }
    break;

    case 53 :
     switch( ac )
       {
        case 0 : return 0;
        default: return -1;
       }
    break;

    default: return -1;
   }
 }

int FormulaParserData::ElemRule(int rule)
 {
  switch( rule )
    {
     case 1 : return 12;
     case 2 : return 13;
     case 3 : return 13;
     case 4 : return 14;
     case 5 : return 14;
     case 6 : return 15;
     case 7 : return 16;
     case 8 : return 16;
     case 9 : return 16;
     case 10 : return 16;
     case 11 : return 16;
     case 12 : return 16;
     case 13 : return 16;
     case 14 : return 17;
     case 15 : return 18;
     case 16 : return 18;
     default: return -1;
    }
 }

int FormulaParserData::ElemAtom(AtomClass ac)
 {
  switch( ac )
    {
     case 5 : return 0;
     case 6 : return 1;
     case 7 : return 2;
     case 9 : return 3;
     case 11 : return 4;
     case 10 : return 5;
     case 8 : return 6;
     case 12 : return 7;
     case 3 : return 8;
     case 4 : return 9;
     case 1 : return 10;
     case 2 : return 11;
     default: return -1;
    }
 }

int FormulaParserData::Transition(int state,int element)
 {
  switch( state )
    {
     case 0 :
      switch( element )
        {
         case 10 : return 69;
         case 12 : return 88;
         default: return -1;
        }
     break;

     case 1 :
      switch( element )
        {
         case 0 : return 3;
         default: return -1;
        }
     break;

     case 2 :
      switch( element )
        {
         case 0 : return 10;
         case 5 : return 22;
         case 8 : return 44;
         case 9 : return 46;
         case 10 : return 70;
         case 11 : return 39;
         case 13 : return 51;
         case 14 : return 50;
         case 15 : return 50;
         case 16 : return 50;
         case 17 : return 54;
         case 18 : return 66;
         default: return -1;
        }
     break;

     case 3 :
      switch( element )
        {
         case 0 : return 10;
         case 5 : return 22;
         case 8 : return 44;
         case 9 : return 46;
         case 10 : return 70;
         case 11 : return 39;
         case 13 : return 51;
         case 14 : return 50;
         case 15 : return 50;
         case 16 : return 50;
         case 17 : return 55;
         case 18 : return 67;
         default: return -1;
        }
     break;

     case 4 :
      switch( element )
        {
         case 0 : return 10;
         case 5 : return 22;
         case 8 : return 44;
         case 9 : return 46;
         case 10 : return 70;
         case 11 : return 39;
         case 13 : return 51;
         case 14 : return 50;
         case 15 : return 50;
         case 16 : return 50;
         case 17 : return 58;
         case 18 : return 68;
         default: return -1;
        }
     break;

     case 5 :
      switch( element )
        {
         case 0 : return 10;
         case 5 : return 22;
         case 8 : return 44;
         case 9 : return 46;
         case 10 : return 70;
         case 11 : return 39;
         case 13 : return 53;
         case 14 : return 52;
         case 15 : return 52;
         case 16 : return 52;
         default: return -1;
        }
     break;

     case 6 :
      switch( element )
        {
         case 0 : return 10;
         case 5 : return 22;
         case 8 : return 44;
         case 9 : return 46;
         case 10 : return 70;
         case 11 : return 39;
         case 14 : return 25;
         case 15 : return 25;
         case 16 : return 25;
         default: return -1;
        }
     break;

     case 7 :
      switch( element )
        {
         case 0 : return 10;
         case 5 : return 22;
         case 8 : return 44;
         case 9 : return 46;
         case 10 : return 70;
         case 11 : return 39;
         case 14 : return 27;
         case 15 : return 27;
         case 16 : return 27;
         default: return -1;
        }
     break;

     case 8 :
      switch( element )
        {
         case 0 : return 10;
         case 5 : return 22;
         case 8 : return 44;
         case 9 : return 46;
         case 10 : return 70;
         case 11 : return 39;
         case 15 : return 29;
         case 16 : return 29;
         default: return -1;
        }
     break;

     case 9 :
      switch( element )
        {
         case 0 : return 10;
         case 5 : return 22;
         case 8 : return 44;
         case 9 : return 46;
         case 10 : return 70;
         case 11 : return 39;
         case 15 : return 31;
         case 16 : return 31;
         default: return -1;
        }
     break;

     case 10 :
      switch( element )
        {
         case 0 : return 11;
         case 5 : return 23;
         case 8 : return 45;
         case 9 : return 47;
         case 10 : return 1;
         case 11 : return 40;
         case 13 : return 63;
         case 14 : return 60;
         case 15 : return 60;
         case 16 : return 60;
         default: return -1;
        }
     break;

     case 11 :
      switch( element )
        {
         case 0 : return 11;
         case 5 : return 23;
         case 8 : return 45;
         case 9 : return 47;
         case 10 : return 1;
         case 11 : return 41;
         case 13 : return 64;
         case 14 : return 61;
         case 15 : return 61;
         case 16 : return 61;
         default: return -1;
        }
     break;

     case 12 :
      switch( element )
        {
         case 0 : return 11;
         case 5 : return 23;
         case 8 : return 45;
         case 9 : return 47;
         case 10 : return 1;
         case 11 : return 42;
         case 13 : return 65;
         case 14 : return 62;
         case 15 : return 62;
         case 16 : return 62;
         default: return -1;
        }
     break;

     case 13 :
      switch( element )
        {
         case 0 : return 11;
         case 5 : return 23;
         case 8 : return 45;
         case 9 : return 47;
         case 10 : return 1;
         case 11 : return 43;
         case 14 : return 26;
         case 15 : return 26;
         case 16 : return 26;
         default: return -1;
        }
     break;

     case 14 :
      switch( element )
        {
         case 0 : return 11;
         case 5 : return 23;
         case 8 : return 45;
         case 9 : return 47;
         case 10 : return 1;
         case 11 : return 43;
         case 14 : return 28;
         case 15 : return 28;
         case 16 : return 28;
         default: return -1;
        }
     break;

     case 15 :
      switch( element )
        {
         case 0 : return 11;
         case 5 : return 23;
         case 8 : return 45;
         case 9 : return 47;
         case 10 : return 1;
         case 11 : return 43;
         case 15 : return 30;
         case 16 : return 30;
         default: return -1;
        }
     break;

     case 16 :
      switch( element )
        {
         case 0 : return 11;
         case 5 : return 23;
         case 8 : return 45;
         case 9 : return 47;
         case 10 : return 1;
         case 11 : return 43;
         case 15 : return 32;
         case 16 : return 32;
         default: return -1;
        }
     break;

     case 17 :
      switch( element )
        {
         case 0 : return 12;
         case 5 : return 24;
         case 8 : return 85;
         case 9 : return 86;
         case 10 : return 83;
         case 11 : return 84;
         case 13 : return 75;
         case 14 : return 74;
         case 15 : return 74;
         case 16 : return 74;
         default: return -1;
        }
     break;

     case 18 :
      switch( element )
        {
         case 0 : return 12;
         case 5 : return 24;
         case 8 : return 85;
         case 9 : return 86;
         case 10 : return 83;
         case 11 : return 84;
         case 14 : return 76;
         case 15 : return 76;
         case 16 : return 76;
         default: return -1;
        }
     break;

     case 19 :
      switch( element )
        {
         case 0 : return 12;
         case 5 : return 24;
         case 8 : return 85;
         case 9 : return 86;
         case 10 : return 83;
         case 11 : return 84;
         case 14 : return 77;
         case 15 : return 77;
         case 16 : return 77;
         default: return -1;
        }
     break;

     case 20 :
      switch( element )
        {
         case 0 : return 12;
         case 5 : return 24;
         case 8 : return 85;
         case 9 : return 86;
         case 10 : return 83;
         case 11 : return 84;
         case 15 : return 78;
         case 16 : return 78;
         default: return -1;
        }
     break;

     case 21 :
      switch( element )
        {
         case 0 : return 12;
         case 5 : return 24;
         case 8 : return 85;
         case 9 : return 86;
         case 10 : return 83;
         case 11 : return 84;
         case 15 : return 79;
         case 16 : return 79;
         default: return -1;
        }
     break;

     case 22 :
      switch( element )
        {
         case 0 : return 10;
         case 8 : return 44;
         case 9 : return 46;
         case 10 : return 70;
         case 11 : return 39;
         case 16 : return 33;
         default: return -1;
        }
     break;

     case 23 :
      switch( element )
        {
         case 0 : return 11;
         case 8 : return 45;
         case 9 : return 47;
         case 10 : return 1;
         case 11 : return 43;
         case 16 : return 34;
         default: return -1;
        }
     break;

     case 24 :
      switch( element )
        {
         case 0 : return 12;
         case 8 : return 85;
         case 9 : return 86;
         case 10 : return 83;
         case 11 : return 84;
         case 16 : return 80;
         default: return -1;
        }
     break;

     case 25 :
      switch( element )
        {
         case 2 : return 8;
         case 6 : return 9;
         default: return -1;
        }
     break;

     case 26 :
      switch( element )
        {
         case 2 : return 15;
         case 6 : return 16;
         default: return -1;
        }
     break;

     case 27 :
      switch( element )
        {
         case 2 : return 8;
         case 6 : return 9;
         default: return -1;
        }
     break;

     case 28 :
      switch( element )
        {
         case 2 : return 15;
         case 6 : return 16;
         default: return -1;
        }
     break;

     case 29 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 30 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 31 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 32 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 33 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 34 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 35 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 36 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 37 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 38 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 39 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 40 :
      switch( element )
        {
         case 4 : return 71;
         default: return -1;
        }
     break;

     case 41 :
      switch( element )
        {
         case 4 : return 72;
         default: return -1;
        }
     break;

     case 42 :
      switch( element )
        {
         case 4 : return 73;
         default: return -1;
        }
     break;

     case 43 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 44 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 45 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 46 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 47 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 48 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 49 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 50 :
      switch( element )
        {
         case 2 : return 8;
         case 3 : return 6;
         case 5 : return 7;
         case 6 : return 9;
         default: return -1;
        }
     break;

     case 51 :
      switch( element )
        {
         case 3 : return 6;
         case 5 : return 7;
         default: return -1;
        }
     break;

     case 52 :
      switch( element )
        {
         case 2 : return 8;
         case 3 : return 6;
         case 5 : return 7;
         case 6 : return 9;
         default: return -1;
        }
     break;

     case 53 :
      switch( element )
        {
         case 3 : return 6;
         case 5 : return 7;
         default: return -1;
        }
     break;

     case 54 :
      switch( element )
        {
         case 1 : return 37;
         default: return -1;
        }
     break;

     case 55 :
      switch( element )
        {
         case 1 : return 38;
         default: return -1;
        }
     break;

     case 56 :
      switch( element )
        {
         case 1 : return 48;
         default: return -1;
        }
     break;

     case 57 :
      switch( element )
        {
         case 1 : return 49;
         default: return -1;
        }
     break;

     case 58 :
      switch( element )
        {
         case 1 : return 82;
         default: return -1;
        }
     break;

     case 59 :
      switch( element )
        {
         case 1 : return 87;
         default: return -1;
        }
     break;

     case 60 :
      switch( element )
        {
         case 1 : return 35;
         case 2 : return 15;
         case 3 : return 13;
         case 5 : return 14;
         case 6 : return 16;
         default: return -1;
        }
     break;

     case 61 :
      switch( element )
        {
         case 1 : return 36;
         case 2 : return 15;
         case 3 : return 13;
         case 5 : return 14;
         case 6 : return 16;
         default: return -1;
        }
     break;

     case 62 :
      switch( element )
        {
         case 1 : return 81;
         case 2 : return 15;
         case 3 : return 13;
         case 5 : return 14;
         case 6 : return 16;
         default: return -1;
        }
     break;

     case 63 :
      switch( element )
        {
         case 1 : return 35;
         case 3 : return 13;
         case 5 : return 14;
         default: return -1;
        }
     break;

     case 64 :
      switch( element )
        {
         case 1 : return 36;
         case 3 : return 13;
         case 5 : return 14;
         default: return -1;
        }
     break;

     case 65 :
      switch( element )
        {
         case 1 : return 81;
         case 3 : return 13;
         case 5 : return 14;
         default: return -1;
        }
     break;

     case 66 :
      switch( element )
        {
         case 1 : return 37;
         case 4 : return 5;
         default: return -1;
        }
     break;

     case 67 :
      switch( element )
        {
         case 1 : return 38;
         case 4 : return 5;
         default: return -1;
        }
     break;

     case 68 :
      switch( element )
        {
         case 1 : return 82;
         case 4 : return 5;
         default: return -1;
        }
     break;

     case 69 :
      switch( element )
        {
         case 7 : return 17;
         default: return -1;
        }
     break;

     case 70 :
      switch( element )
        {
         case 0 : return 2;
         default: return -1;
        }
     break;

     case 71 :
      switch( element )
        {
         case 11 : return 56;
         default: return -1;
        }
     break;

     case 72 :
      switch( element )
        {
         case 11 : return 57;
         default: return -1;
        }
     break;

     case 73 :
      switch( element )
        {
         case 11 : return 59;
         default: return -1;
        }
     break;

     case 74 :
      switch( element )
        {
         case 2 : return 20;
         case 3 : return 18;
         case 5 : return 19;
         case 6 : return 21;
         default: return -1;
        }
     break;

     case 75 :
      switch( element )
        {
         case 3 : return 18;
         case 5 : return 19;
         default: return -1;
        }
     break;

     case 76 :
      switch( element )
        {
         case 2 : return 20;
         case 6 : return 21;
         default: return -1;
        }
     break;

     case 77 :
      switch( element )
        {
         case 2 : return 20;
         case 6 : return 21;
         default: return -1;
        }
     break;

     case 78 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 79 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 80 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 81 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 82 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 83 :
      switch( element )
        {
         case 0 : return 4;
         default: return -1;
        }
     break;

     case 84 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 85 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 86 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 87 :
      switch( element )
        {
         default: return -1;
        }
     break;

     case 88 :
      switch( element )
        {
         default: return -1;
        }
     break;

     default: return -1;
    }
 }

} // namespace App

