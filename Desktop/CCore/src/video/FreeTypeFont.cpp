/* FreeTypeFont.cpp */
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

#include <CCore/inc/video/FreeTypeFont.h>

#include <CCore/inc/video/FreeType.h>

#include <CCore/inc/Exception.h>

namespace CCore {
namespace Video {

/* enum FontHintType */

const char * GetTextDesc(FontHintType fht)
 {
  switch( fht )
    {
     default: return "???";

     case FontHintNone : return "FontHintNone";

     case FontHintNative : return "FontHintNative";

     case FontHintAuto : return "FontHintAuto";
    }
 }

inline FT_Int32 ToFlags(FontHintType fht)
 {
  switch( fht )
    {
     default: return FT_LOAD_NO_HINTING|FT_LOAD_NO_AUTOHINT;

     case FontHintNative : return FT_LOAD_NO_AUTOHINT;

     case FontHintAuto : return FT_LOAD_FORCE_AUTOHINT;
    }
 }

/* enum FontSmoothType */

const char * GetTextDesc(FontSmoothType fsm)
 {
  switch( fsm )
    {
     default: return "???";

     case FontSmoothNone : return "FontSmoothNone";

     case FontSmooth : return "FontSmooth";

     case FontSmoothLCD_RGB : return "FontSmoothLCD_RGB";

     case FontSmoothLCD_BGR : return "FontSmoothLCD_BGR";
    }
 }

inline FT_Render_Mode ToMode(FontSmoothType fsm)
 {
  switch( fsm )
    {
     default: return FT_RENDER_MODE_MONO;

     case FontSmooth : return FT_RENDER_MODE_NORMAL;

     case FontSmoothLCD_RGB :
     case FontSmoothLCD_BGR : return FT_RENDER_MODE_LCD;
    }
 }

/* class FreeTypeFont::Base */

class FreeTypeFont::Base : public FontBase
 {
   mutable FreeType::Face face;
   Config cfg;
   GammaTable gamma_table;

   FontSize font_size;

   struct CharX
    {
     Coord dx;
     Coord dx0;
     Coord dx1;
    };

   CharX char_x[256];

  private:

   using IndexType = FT_UInt ;

   CharX text(char ch)
    {
     auto index=face.getGlyphIndex(Object->map(ch));

     face.loadGlyph(index,ToFlags(cfg.fht));

     if( cfg.strength ) face.emboldenGlyph(cfg.strength);

     face.renderGlyph(ToMode(cfg.fsm));

     auto placement=face.getGlyphPlacement();

     auto &metrics=face.getGlyphMetrics();

     Coord dx0=0;
     Coord dx1=0;

     if( metrics.horiBearingX<0 )
       {
        dx0=FreeType::RoundUp(-metrics.horiBearingX);
       }

     FT_Pos extra=metrics.horiBearingX+metrics.width-metrics.horiAdvance;

     if( extra>0 )
       {
        dx1=FreeType::RoundUp(extra);
       }

     return {placement.getDelta().x,dx0,dx1};
    }

   Coord kerning(IndexType &prev_index,char ch) const
    {
     Coord ret=0;

     auto index=face.getGlyphIndex(Object->map(ch));

     if( prev_index && index )
       {
        FT_Vector delta=face.getKerning(prev_index,index);

        ret=FreeType::Round(delta.x);
       }

     prev_index=index;

     return ret;
    }

   CharX table_text(char ch) const
    {
     return char_x[uint8(ch)];
    }

   CharX table_text(IndexType &prev_index,char ch) const
    {
     CharX ret=table_text(ch);
     Coord delta=kerning(prev_index,ch);

     ret.dx+=delta;

     return ret;
    }

   void updateFontSize()
    {
     if( !face.hasKerning() ) cfg.use_kerning=false;

     font_size.min_dx=MaxCoord;
     font_size.max_dx=0;
     font_size.dx0=0;
     font_size.dx1=0;

     for(int i=-128; i<128 ;i++)
       {
        char ch=char(i);
        CharX cx=text(ch);

        Replace_min(font_size.min_dx,cx.dx);
        Replace_max(font_size.max_dx,cx.dx);
        Replace_max(font_size.dx0,cx.dx0);
        Replace_max(font_size.dx1,cx.dx1);

        char_x[uint8(ch)]=cx;
       }

     Coord dy=FreeType::RoundUp(face.getMetrics().height);
     Coord by=FreeType::RoundUp(face.getMetrics().ascender);
     Coord ey=FreeType::RoundUp(-face.getMetrics().descender);

     if( by+ey>dy )
       {
        font_size.dy=by+ey;
        font_size.by=by;
       }
     else
       {
        font_size.dy=dy;
        font_size.by=by+(dy-by-ey)/2;
       }

     font_size.skew=0;

     if( face.isSFNT() )
       {
        if( const TT_HoriHeader *ptr=face.getTTHoriHeader() )
          {
           MCoord P=ptr->caret_Slope_Run;
           MCoord Q=ptr->caret_Slope_Rise;

           if( P!=0 && Q>0 )
             {
              font_size.skew=Coord( (DCoord(P)*font_size.dy)/Q ); // (P/Q)*font_size.dy
             }
          }
       }
    }

   bool text(FrameBuf<DesktopColor> &buf,Point &base,char ch,VColor vc) const
    {
     auto index=face.getGlyphIndex(Object->map(ch));

     face.loadGlyph(index,ToFlags(cfg.fht));

     if( cfg.strength ) face.emboldenGlyph(cfg.strength);

     face.renderGlyph(ToMode(cfg.fsm));

     auto placement=face.getGlyphPlacement();

     bool ret=face.drawGlyph(buf,placement.toPos(base),vc,gamma_table.getFunc(),cfg.fsm==FontSmoothLCD_BGR);

     base+=placement.getDelta();

     return ret;
    }

   bool text(FrameBuf<DesktopColor> &buf,Point &base,IndexType &prev_index,char ch,VColor vc) const
    {
     auto index=face.getGlyphIndex(Object->map(ch));

     if( prev_index && index )
       {
        FT_Vector delta=face.getKerning(prev_index,index);

        base+={FreeType::Round(delta.x),FreeType::Round(delta.y)};
       }

     prev_index=index;

     face.loadGlyph(index,ToFlags(cfg.fht));

     if( cfg.strength ) face.emboldenGlyph(cfg.strength);

     face.renderGlyph(ToMode(cfg.fsm));

     auto placement=face.getGlyphPlacement();

     bool ret=face.drawGlyph(buf,placement.toPos(base),vc,gamma_table.getFunc(),cfg.fsm==FontSmoothLCD_BGR);

     base+=placement.getDelta();

     return ret;
    }

  private:

   template <class Func>
   void textRun(AbstractSparseString &str,Func func) const
    {
     if( cfg.use_kerning )
       {
        IndexType index=0;

        str.applyChar( [this,&index,func] (char ch) { func( table_text(index,ch) ); } );
       }
     else
       {
        str.applyChar( [this,func] (char ch) { func( table_text(ch) ); } );
       }
    }

   template <class Func>
   void textRun(AbstractSparseString &str,ulen pos,Func func) const
    {
     if( cfg.use_kerning )
       {
        IndexType index=0;

        str.applyCharWhile( [this,&index,&pos,func] (char ch)
                                                    {
                                                     if( !pos ) return false;

                                                     pos--;

                                                     func( table_text(index,ch) );

                                                     return true;

                                                    } );
       }
     else
       {
        str.applyCharWhile( [this,&pos,func] (char ch)
                                             {
                                              if( !pos ) return false;

                                              pos--;

                                              func( table_text(ch) );

                                              return true;

                                             } );
       }
    }

   template <class Func>
   void textRunWhile(AbstractSparseString &str,Func func) const
    {
     if( cfg.use_kerning )
       {
        IndexType index=0;

        str.applyCharWhile( [this,&index,func] (char ch) { return func( table_text(index,ch) ); } );
       }
     else
       {
        str.applyCharWhile( [this,func] (char ch) { return func( table_text(ch) ); } );
       }
    }

   Coord toBaseline(Point point) const
    {
     if( font_size.skew )
       {
        Coord delta=Coord( (font_size.skew*MCoord(point.y))/font_size.dy ); // (skew/dy)*y

        return point.x-delta;
       }
     else
       {
        return point.x;
       }
    }

   ulen fitBase(AbstractSparseString &str,Coord base_dx) const
    {
     ulen ret=0;

     textRunWhile(str,[&] (CharX cx)
                          {
                           base_dx-=cx.dx;

                           if( base_dx<0 ) return false;

                           ret++;

                           return true;

                          } );

     return ret;
    }

   MCoord text_dx(AbstractSparseString &str) const
    {
     MCoord ret=0;

     textRun(str, [&] (CharX cx) { ret+=cx.dx; } );

     return ret;
    }

   ulen skipCount(AbstractSparseString &str,MCoord &x) const // return suffix length
    {
     ulen ret=0;

     if( cfg.use_kerning )
       {
        IndexType index=0;

        str.apply( [&] (PtrLen<const char> str)
                       {
                        if( ret )
                          {
                           ret+=str.len;
                          }
                        else
                          {
                           for(; +str ;++str)
                             {
                              char ch=*str;
                              Coord delta=kerning(index,ch);
                              CharX cx=table_text(ch);

                              if( x+cx.dx+cx.dx1<0 )
                                {
                                 x+=delta+cx.dx;
                                }
                              else
                                {
                                 ret=str.len;

                                 break;
                                }
                             }
                          }

                       } );
       }
     else
       {
        str.apply( [&] (PtrLen<const char> str)
                       {
                        if( ret )
                          {
                           ret+=str.len;
                          }
                        else
                          {
                           for(; +str ;++str)
                             {
                              CharX cx=table_text(*str);

                              if( x+cx.dx+cx.dx1<0 )
                                {
                                 x+=cx.dx;
                                }
                              else
                                {
                                 ret=str.len;

                                 break;
                                }
                             }
                          }

                       } );
       }

     return ret;
    }

   Coord skip(AbstractSparseString &str,MCoord x) const
    {
     if( x>=0 ) return Coord(x);

     ulen len=skipCount(str,x);

     str.cutSuffix(len);

     return Coord(x);
    }

   Point prepare(Coord dx,Coord dy,TextPlace place,AbstractSparseString &str) const
    {
     Coord y;

     switch( place.align_y )
       {
        case AlignY_Top : y=font_size.by; break;

        case AlignY_Center : y=(dy-font_size.dy)/2+font_size.by; break;

        case AlignY_Bottom : y=dy-font_size.dy+font_size.by; break;

        default: y=place.y;
       }

     Coord x;

     switch( place.align_x )
       {
        case AlignX_Left : x=font_size.dx0; break;

        case AlignX_Right :
         {
          MCoord tdx=text_dx(str);

          x=skip(str,dx-font_size.dx1-tdx);
         }
        break;

        case AlignX_Center :
         {
          MCoord tdx=text_dx(str);

          x=skip(str,(dx-tdx)/2);
         }
        break;

        default: x=place.x;
       }

     return {x,y};
    }

   void text(FrameBuf<DesktopColor> &buf,Point base,AbstractSparseString &str,VColor vc) const
    {
     if( base.y<font_size.by-font_size.dy || base.y>buf.dY()+font_size.by ) return;

     if( cfg.use_kerning )
       {
        IndexType index=0;

        str.applyCharWhile( [&] (char ch) { return text(buf,base,index,ch,vc); } );
       }
     else
       {
        str.applyCharWhile( [&] (char ch) { return text(buf,base,ch,vc); } );
       }
    }

   void text(DrawBuf buf,Coord dx,Coord dy,TextPlace place,AbstractSparseString &str,VColor vc) const
    {
     if( !buf ) return;

     Point base=prepare(dx,dy,place,str);

     text(buf,buf.map(base),str,vc);
    }

  private:

   struct Global
    {
     Mutex mutex;
     FreeType::Lib lib;
     CharMapTable map;

     Global() : lib(FT_LCD_FILTER_DEFAULT) {}
    };

   static InitExitObject<Global> Object;

  public:

   static void Init() { Object.init(); }

   static void Exit() { Object.exit(); }

   // constructors

   explicit Base(StrLen file_name)
    : face(Object->lib,Object->mutex,file_name)
    {
     updateFontSize();
    }

   Base(StrLen file_name,bool &is_font)
    : face(Object->lib,Object->mutex,file_name,is_font)
    {
     if( is_font )
       {
        try
          {
           updateFontSize();
          }
        catch(...)
          {
           Printf(NoException,"file_name #;",file_name);

           is_font=false;
          }
       }
    }

   Base(StrLen dir,StrLen file_name)
    : face(Object->lib,Object->mutex,dir,file_name)
    {
     updateFontSize();
    }

   virtual ~Base() {}

   // AbstractFont

   virtual FontSize getSize() const
    {
     return font_size;
    }

   virtual TextSize text(AbstractSparseString &str) const
    {
     TextSize ret;

     ret.dy=font_size.dy;
     ret.by=font_size.by;
     ret.dx0=0;
     ret.dx1=0;
     ret.skew=font_size.skew;

     CoordAcc acc;
     bool first=true;

     textRun(str, [&] (CharX cx)
                      {
                       acc.add(cx.dx);

                       if( first ) first=false,ret.dx0=cx.dx0;

                       ret.dx1=cx.dx1;

                      } );

     ret.dx=acc.value;

     if( ret.dx<0 ) ret.dx=0;

     acc.add(font_size.dx0);
     acc.add(font_size.dx1);

     ret.full_dx=acc.value;
     ret.overflow=acc.overflow;

     return ret;
    }

   virtual ulen position(AbstractSparseString &str,Point point) const
    {
     Coord x=toBaseline(point);

     if( x<0 ) return 0;

     return 1+fitBase(str,x);
    }

   virtual TextSize text(AbstractSparseString &str,ulen pos) const
    {
     TextSize ret;

     ret.dy=font_size.dy;
     ret.by=font_size.by;
     ret.dx0=0;
     ret.dx1=0;
     ret.skew=font_size.skew;

     CoordAcc acc;
     bool first=true;

     textRun(str,pos, [&] (CharX cx)
                          {
                           acc.add(cx.dx);

                           if( first ) first=false,ret.dx0=cx.dx0;

                           ret.dx1=cx.dx1;

                          } );

     ret.dx=acc.value;

     if( ret.dx<0 ) ret.dx=0;

     acc.add(font_size.dx0);
     acc.add(font_size.dx1);

     ret.full_dx=acc.value;
     ret.overflow=acc.overflow;

     return ret;
    }

   virtual ulen fit(AbstractSparseString &str,Coord full_dx) const
    {
     return fitBase(str,full_dx-font_size.dx0-font_size.dx1);
    }

   virtual void text(DrawBuf buf,Pane pane,TextPlace place,AbstractSparseString &str,VColor vc) const
    {
     text(buf.cutRebase(pane),pane.dx,pane.dy,place,str,vc);
    }

   // set params

   void setSize(Coord dx,Coord dy)
    {
     face.setPixelSize(dx,dy);

     updateFontSize();
    }

   void setFixedSize(ulen index)
    {
     face.setFixedSize((FT_Int)index);

     updateFontSize();
    }

   void setConfig(const Config &cfg_)
    {
     cfg=cfg_;

     updateFontSize();

     if( cfg.gamma_order!=gamma_table.getOrder() ) gamma_table.fill(cfg.gamma_order);
    }

   // get props

   StyleFlags getStyleFlags() const
    {
     StyleFlags ret;

     ret.scalable=face.isScalable();
     ret.monospace=face.isMonospace();
     ret.italic=face.isItalic();
     ret.bold=face.isBold();

     return ret;
    }

   Config getConfig() const
    {
     return cfg;
    }

   StrLen getFamily() const
    {
     return face.getFamily();
    }

   StrLen getStyle() const
    {
     return face.getStyle();
    }

   void getSizeList(Function<void (Coord dx,Coord dy)> func) const
    {
     for(auto size : face.getSizeList() ) func(size.width,size.height);
    }
 };

InitExitObject<FreeTypeFont::Base::Global> FreeTypeFont::Base::Object{};

/* class FreeTypeFont::InitExit */

FreeTypeFont::InitExit::InitExit()
 {
  FreeTypeFont::Base::Init();
 }

FreeTypeFont::InitExit::~InitExit()
 {
  FreeTypeFont::Base::Exit();
 }

/* class FreeTypeFont */

auto FreeTypeFont::getBase() const -> Base *
 {
  return castPtr<Base>();
 }

 // constructors

FreeTypeFont::FreeTypeFont(StrLen file_name)
 : Font(new Base(file_name))
 {
 }

FreeTypeFont::FreeTypeFont(StrLen file_name,bool &is_font)
 : Font(new Base(file_name,is_font))
 {
 }

FreeTypeFont::FreeTypeFont(StrLen dir,StrLen file_name)
 : Font(new Base(dir,file_name))
 {
 }

 // set params

void FreeTypeFont::setSize(Coord dx,Coord dy)
 {
  if( Base *base=getBase() ) base->setSize(dx,dy);
 }

void FreeTypeFont::setFixedSize(ulen index)
 {
  if( Base *base=getBase() ) base->setFixedSize(index);
 }

void FreeTypeFont::setConfig(const Config &cfg)
 {
  if( Base *base=getBase() ) base->setConfig(cfg);
 }

 // get props

auto FreeTypeFont::getStyleFlags() const -> StyleFlags
 {
  if( Base *base=getBase() ) return base->getStyleFlags();

  return {};
 }

auto FreeTypeFont::getConfig() const -> Config
 {
  if( Base *base=getBase() ) return base->getConfig();

  return {};
 }

StrLen FreeTypeFont::getFamily() const
 {
  if( Base *base=getBase() ) return base->getFamily();

  return "Unknown";
 }

StrLen FreeTypeFont::getStyle() const
 {
  if( Base *base=getBase() ) return base->getStyle();

  return Null;
 }

void FreeTypeFont::getSizeList(Function<void (Coord dx,Coord dy)> func) const
 {
  if( Base *base=getBase() ) base->getSizeList(func);
 }

} // namespace Video
} // namespace CCore

