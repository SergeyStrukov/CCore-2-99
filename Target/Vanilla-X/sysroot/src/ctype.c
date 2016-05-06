/* ctype.c */ 
/*----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: Target/BeagleBoneBlack
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003 
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//--------------------------------------------------------------------------------------*/

#include <sysroot/usr/include/ctype.h>

const unsigned short __std_ctype_flags[257]=
 {
  0,
  /* code 0 */  0|__std_ctype_iscntrl,
  /* code 1 */  0|__std_ctype_iscntrl,
  /* code 2 */  0|__std_ctype_iscntrl,
  /* code 3 */  0|__std_ctype_iscntrl,
  /* code 4 */  0|__std_ctype_iscntrl,
  /* code 5 */  0|__std_ctype_iscntrl,
  /* code 6 */  0|__std_ctype_iscntrl,
  /* code 7 */  0|__std_ctype_iscntrl,
  /* code 8 */  0|__std_ctype_iscntrl,
  /* code 9 */  0|__std_ctype_iscntrl|__std_ctype_isspace|__std_ctype_isblank,
  /* code 10 */  0|__std_ctype_iscntrl|__std_ctype_isspace,
  /* code 11 */  0|__std_ctype_iscntrl|__std_ctype_isspace,
  /* code 12 */  0|__std_ctype_iscntrl|__std_ctype_isspace,
  /* code 13 */  0|__std_ctype_iscntrl|__std_ctype_isspace,
  /* code 14 */  0|__std_ctype_iscntrl,
  /* code 15 */  0|__std_ctype_iscntrl,
  /* code 16 */  0|__std_ctype_iscntrl,
  /* code 17 */  0|__std_ctype_iscntrl,
  /* code 18 */  0|__std_ctype_iscntrl,
  /* code 19 */  0|__std_ctype_iscntrl,
  /* code 20 */  0|__std_ctype_iscntrl,
  /* code 21 */  0|__std_ctype_iscntrl,
  /* code 22 */  0|__std_ctype_iscntrl,
  /* code 23 */  0|__std_ctype_iscntrl,
  /* code 24 */  0|__std_ctype_iscntrl,
  /* code 25 */  0|__std_ctype_iscntrl,
  /* code 26 */  0|__std_ctype_iscntrl,
  /* code 27 */  0|__std_ctype_iscntrl,
  /* code 28 */  0|__std_ctype_iscntrl,
  /* code 29 */  0|__std_ctype_iscntrl,
  /* code 30 */  0|__std_ctype_iscntrl,
  /* code 31 */  0|__std_ctype_iscntrl,
  /*   */  0|__std_ctype_isprint|__std_ctype_isspace|__std_ctype_isblank,
  /* ! */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* " */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* # */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* $ */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* % */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* & */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* ' */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* ( */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* ) */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* * */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* + */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* , */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* - */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* . */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* / */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* 0 */  0|__std_ctype_isprint|__std_ctype_isdigit|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* 1 */  0|__std_ctype_isprint|__std_ctype_isdigit|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* 2 */  0|__std_ctype_isprint|__std_ctype_isdigit|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* 3 */  0|__std_ctype_isprint|__std_ctype_isdigit|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* 4 */  0|__std_ctype_isprint|__std_ctype_isdigit|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* 5 */  0|__std_ctype_isprint|__std_ctype_isdigit|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* 6 */  0|__std_ctype_isprint|__std_ctype_isdigit|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* 7 */  0|__std_ctype_isprint|__std_ctype_isdigit|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* 8 */  0|__std_ctype_isprint|__std_ctype_isdigit|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* 9 */  0|__std_ctype_isprint|__std_ctype_isdigit|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* : */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* ; */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* < */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* = */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* > */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* ? */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* @ */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* A */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* B */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* C */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* D */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* E */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* F */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* G */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* H */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* I */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* J */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* K */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* L */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* M */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* N */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* O */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* P */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* Q */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* R */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* S */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* T */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* U */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* V */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* W */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* X */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* Y */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* Z */  0|__std_ctype_isprint|__std_ctype_isupper|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* [ */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* \ */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* ] */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* ^ */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* _ */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* ` */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* a */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* b */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* c */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* d */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* e */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* f */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isxdigit|__std_ctype_isgraph,
  /* g */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* h */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* i */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* j */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* k */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* l */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* m */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* n */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* o */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* p */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* q */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* r */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* s */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* t */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* u */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* v */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* w */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* x */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* y */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* z */  0|__std_ctype_isprint|__std_ctype_islower|__std_ctype_isalpha|__std_ctype_isalnum|__std_ctype_isgraph,
  /* { */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* | */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* } */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* ~ */  0|__std_ctype_isprint|__std_ctype_ispunct|__std_ctype_isgraph,
  /* code 127 */  0|__std_ctype_iscntrl,
  /* code 128 */  0,
  /* code 129 */  0,
  /* code 130 */  0,
  /* code 131 */  0,
  /* code 132 */  0,
  /* code 133 */  0,
  /* code 134 */  0,
  /* code 135 */  0,
  /* code 136 */  0,
  /* code 137 */  0,
  /* code 138 */  0,
  /* code 139 */  0,
  /* code 140 */  0,
  /* code 141 */  0,
  /* code 142 */  0,
  /* code 143 */  0,
  /* code 144 */  0,
  /* code 145 */  0,
  /* code 146 */  0,
  /* code 147 */  0,
  /* code 148 */  0,
  /* code 149 */  0,
  /* code 150 */  0,
  /* code 151 */  0,
  /* code 152 */  0,
  /* code 153 */  0,
  /* code 154 */  0,
  /* code 155 */  0,
  /* code 156 */  0,
  /* code 157 */  0,
  /* code 158 */  0,
  /* code 159 */  0,
  /* code 160 */  0,
  /* code 161 */  0,
  /* code 162 */  0,
  /* code 163 */  0,
  /* code 164 */  0,
  /* code 165 */  0,
  /* code 166 */  0,
  /* code 167 */  0,
  /* code 168 */  0,
  /* code 169 */  0,
  /* code 170 */  0,
  /* code 171 */  0,
  /* code 172 */  0,
  /* code 173 */  0,
  /* code 174 */  0,
  /* code 175 */  0,
  /* code 176 */  0,
  /* code 177 */  0,
  /* code 178 */  0,
  /* code 179 */  0,
  /* code 180 */  0,
  /* code 181 */  0,
  /* code 182 */  0,
  /* code 183 */  0,
  /* code 184 */  0,
  /* code 185 */  0,
  /* code 186 */  0,
  /* code 187 */  0,
  /* code 188 */  0,
  /* code 189 */  0,
  /* code 190 */  0,
  /* code 191 */  0,
  /* code 192 */  0,
  /* code 193 */  0,
  /* code 194 */  0,
  /* code 195 */  0,
  /* code 196 */  0,
  /* code 197 */  0,
  /* code 198 */  0,
  /* code 199 */  0,
  /* code 200 */  0,
  /* code 201 */  0,
  /* code 202 */  0,
  /* code 203 */  0,
  /* code 204 */  0,
  /* code 205 */  0,
  /* code 206 */  0,
  /* code 207 */  0,
  /* code 208 */  0,
  /* code 209 */  0,
  /* code 210 */  0,
  /* code 211 */  0,
  /* code 212 */  0,
  /* code 213 */  0,
  /* code 214 */  0,
  /* code 215 */  0,
  /* code 216 */  0,
  /* code 217 */  0,
  /* code 218 */  0,
  /* code 219 */  0,
  /* code 220 */  0,
  /* code 221 */  0,
  /* code 222 */  0,
  /* code 223 */  0,
  /* code 224 */  0,
  /* code 225 */  0,
  /* code 226 */  0,
  /* code 227 */  0,
  /* code 228 */  0,
  /* code 229 */  0,
  /* code 230 */  0,
  /* code 231 */  0,
  /* code 232 */  0,
  /* code 233 */  0,
  /* code 234 */  0,
  /* code 235 */  0,
  /* code 236 */  0,
  /* code 237 */  0,
  /* code 238 */  0,
  /* code 239 */  0,
  /* code 240 */  0,
  /* code 241 */  0,
  /* code 242 */  0,
  /* code 243 */  0,
  /* code 244 */  0,
  /* code 245 */  0,
  /* code 246 */  0,
  /* code 247 */  0,
  /* code 248 */  0,
  /* code 249 */  0,
  /* code 250 */  0,
  /* code 251 */  0,
  /* code 252 */  0,
  /* code 253 */  0,
  /* code 254 */  0,
  /* code 255 */  0
 };

#ifdef __cplusplus

#error this must be C file

#else

int isupper(int c) { return __std_ctype_flags[c+1]&__std_ctype_isupper; }
int islower(int c) { return __std_ctype_flags[c+1]&__std_ctype_islower; }
int isdigit(int c) { return __std_ctype_flags[c+1]&__std_ctype_isdigit; }
int isxdigit(int c) { return __std_ctype_flags[c+1]&__std_ctype_isxdigit; }
 
int isalpha(int c) { return __std_ctype_flags[c+1]&__std_ctype_isalpha; }
int isalnum(int c) { return __std_ctype_flags[c+1]&__std_ctype_isalnum; }
 
int isblank(int c) { return __std_ctype_flags[c+1]&__std_ctype_isblank; }
int isspace(int c) { return __std_ctype_flags[c+1]&__std_ctype_isspace; }

int iscntrl(int c) { return __std_ctype_flags[c+1]&__std_ctype_iscntrl; }
int isprint(int c) { return __std_ctype_flags[c+1]&__std_ctype_isprint; }

int isgraph(int c) { return __std_ctype_flags[c+1]&__std_ctype_isgraph; }
int ispunct(int c) { return __std_ctype_flags[c+1]&__std_ctype_ispunct; }
 
int tolower(int c) { return (c<'A')?c:( (c>'Z')?c:( (c-'A')+'a' ) ); }
int toupper(int c) { return (c<'a')?c:( (c>'z')?c:( (c-'a')+'A' ) ); }
 
#endif





