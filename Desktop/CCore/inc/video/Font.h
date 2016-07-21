/* Font.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: Desktop
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_video_Font_h
#define CCore_inc_video_Font_h

#include <CCore/inc/video/DrawBuf.h>

#include <CCore/inc/RefObjectBase.h>
#include <CCore/inc/algon/ApplyToRange.h>

namespace CCore {
namespace Video {

/* consts */

enum AlignX
 {
  AlignX_Left,
  AlignX_Center,
  AlignX_Right,
  AlignX_Given
 };

enum AlignY
 {
  AlignY_Top,
  AlignY_Center,
  AlignY_Bottom,
  AlignY_Given
 };

/* classes */

struct FontSize;

struct TextSize;

struct TextPlace;

struct AbstractSparseString;

class SingleString;

class DoubleString;

struct AbstractFont;

class Font;

/* struct FontSize */

struct FontSize
 {
  Coord min_dx;
  Coord max_dx;
  Coord dy;
  Coord by;
  Coord dx0;
  Coord dx1;
  Coord skew;

  Coord medDx() const { return min_dx+(max_dx-min_dx)/2; }
 };

/* struct TextSize */

struct TextSize
 {
  Coord dx;      // MaxCoord if overflowed
  Coord dy;
  Coord by;
  Coord dx0;
  Coord dx1;
  Coord skew;
  Coord full_dx; // dx+font_size.dx0+font_size.dx1 , MaxCoord if overflowed
  bool overflow; // full_dx overflowed
 };

/* struct TextPlace */

struct TextPlace
 {
  AlignX align_x;
  AlignY align_y;

  Coord x = 0 ; // relative pane
  Coord y = 0 ;

  TextPlace(AlignX align_x_,AlignY align_y_) : align_x(align_x_),align_y(align_y_) {}

  TextPlace(AlignX align_x_,Coord y_) : align_x(align_x_),align_y(AlignY_Given),y(y_) {}

  TextPlace(Coord x_,AlignY align_y_) : align_x(AlignX_Given),align_y(align_y_),x(x_) {}

  TextPlace(Coord x_,Coord y_) : align_x(AlignX_Given),align_y(AlignY_Given),x(x_),y(y_) {}

  TextPlace(Point pos) : TextPlace(pos.x,pos.y) {}
 };

/* struct AbstractSparseString */

struct AbstractSparseString
 {
  // abstract

  virtual void restart()=0;

  virtual StrLen next()=0;

  virtual void cutSuffix(ulen len)=0;

  virtual bool cutCenter(ulen len)=0; // true if extra character is appended, len+1 result is returned

  // helper

  template <FuncInitArgType<StrLen> FuncInit>
  auto apply(FuncInit func_init)
   {
    restart();

    FunctorTypeOf<FuncInit> func(func_init);

    for(;;)
      {
       StrLen r=next();

       if( !r ) break;

       func(r);
      }

    return Algon::GetResult(func);
   }

  template <FuncInitArgType<char> FuncInit>
  auto applyChar(FuncInit func_init)
   {
    restart();

    FunctorTypeOf<FuncInit> func(func_init);

    for(;;)
      {
       StrLen r=next();

       if( !r ) break;

       for(char ch : r ) func(ch);
      }

    return Algon::GetResult(func);
   }

  template <FuncInitType<bool,StrLen> FuncInit>
  auto apply(FuncInit func_init)
   {
    restart();

    FunctorTypeOf<FuncInit> func(func_init);

    for(;;)
      {
       StrLen r=next();

       if( !r ) break;

       if( !func(r) ) break;
      }

    return Algon::GetResult(func);
   }

  template <FuncInitType<bool,char> FuncInit>
  auto applyChar(FuncInit func_init)
   {
    restart();

    FunctorTypeOf<FuncInit> func(func_init);

    for(;;)
      {
       StrLen r=next();

       if( !r ) break;

       for(char ch : r ) if( !func(ch) ) return Algon::GetResult(func);
      }

    return Algon::GetResult(func);
   }

  ULenSat countLen();
 };

/* class SingleString */

class SingleString : public AbstractSparseString
 {
   StrLen str;
   bool first = true ;

  public:

   explicit SingleString(StrLen str_) : str(str_) {}

   ~SingleString() {}

   // AbstractSparseString

   virtual void restart();

   virtual StrLen next();

   virtual void cutSuffix(ulen len);

   virtual bool cutCenter(ulen len);
 };

/* class DoubleString */

class DoubleString : public AbstractSparseString
 {
   StrLen str1;
   StrLen str2;
   unsigned ind = 1 ;

  public:

   DoubleString(StrLen str1_,StrLen str2_);

   ~DoubleString() {}

   // AbstractSparseString

   virtual void restart();

   virtual StrLen next();

   virtual void cutSuffix(ulen len);

   virtual bool cutCenter(ulen len);
 };

/* struct AbstractFont */

struct AbstractFont
 {
  // abstract

  virtual FontSize getSize() const =0;

  virtual TextSize text(AbstractSparseString &str) const =0;

  virtual ulen position(AbstractSparseString &str,Point point) const =0; // 1-based cell index, 0 -- before the first

  virtual TextSize text(AbstractSparseString &str,ulen pos) const =0; // 0-based, implementation-defined if out of range

  virtual ulen fit(AbstractSparseString &str,Coord full_dx) const =0; // 0-based

  virtual void text(DrawBuf buf,Pane pane,TextPlace place,AbstractSparseString &str,VColor vc) const =0;

  // helpers

  // single

  TextSize text(StrLen str) const
   {
    SingleString obj(str);

    return text(obj);
   }

  ulen position(StrLen str,Point point) const
   {
    SingleString obj(str);

    return position(obj,point);
   }

  TextSize text(StrLen str,ulen pos) const
   {
    SingleString obj(str);

    return text(obj,pos);
   }

  ulen fit(StrLen str,Coord full_dx) const
   {
    SingleString obj(str);

    return fit(obj,full_dx);
   }

  void text(DrawBuf buf,Pane pane,TextPlace place,StrLen str,VColor vc) const
   {
    SingleString obj(str);

    text(buf,pane,place,obj,vc);
   }

  // double

  TextSize text(StrLen str1,StrLen str2) const
   {
    DoubleString obj(str1,str2);

    return text(obj);
   }

  ulen position(StrLen str1,StrLen str2,Point point) const
   {
    DoubleString obj(str1,str2);

    return position(obj,point);
   }

  TextSize text(StrLen str1,StrLen str2,ulen pos) const
   {
    DoubleString obj(str1,str2);

    return text(obj,pos);
   }

  ulen fit(StrLen str1,StrLen str2,Coord full_dx) const
   {
    DoubleString obj(str1,str2);

    return fit(obj,full_dx);
   }

  void text(DrawBuf buf,Pane pane,TextPlace place,StrLen str1,StrLen str2,VColor vc) const
   {
    DoubleString obj(str1,str2);

    text(buf,pane,place,obj,vc);
   }
 };

/* type FontBase */

using FontBase = RefObjectBase<AbstractFont> ;

/* class Font */

class Font
 {
   RefPtr<FontBase> ptr;

  protected:

   explicit Font(FontBase *font) : ptr(font) {}

   template <class T>
   T * castPtr() const { return dynamic_cast<T *>(ptr.getPtr()); }

  public:

   Font(); // default font

   ~Font() {}

   const AbstractFont * getPtr() const { return ptr.getPtr(); }

   const AbstractFont * operator -> () const { return ptr.getPtr(); }
 };

} // namespace Video
} // namespace CCore

#endif


