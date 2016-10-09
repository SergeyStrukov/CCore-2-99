/* Menu.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 3.00
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

#include <CCore/inc/video/Menu.h>

#include <CCore/inc/video/FigureLib.h>

namespace CCore {
namespace Video {

/* struct MenuPoint */

void MenuPoint::pickhot()
 {
  StrLen str=text.str();

  if( str.len )
    for(ulen i=0,lim=str.len-1; i<lim ;i++)
      {
       if( str[i]==Hot )
         {
          hotindex=i+1;
          hotkey=str[hotindex];

          return;
         }
      }

  hotindex=0;
  hotkey=0;
 }

/* struct MenuData */

auto MenuData::find(char ch) const -> FindResult
 {
  auto r=Range(list);

  for(ulen i=0; i<r.len ;i++) if( r[i].test(ch) ) return {i,true};

  return {0,false};
 }

auto MenuData::find(Point point) const -> FindResult
 {
  auto r=Range(list);

  for(ulen i=0; i<r.len ;i++) if( r[i].test(point) ) return {i,true};

  return {0,false};
 }

auto MenuData::findDown(ulen index) const -> FindResult
 {
  auto r=Range(list);

  Replace_min(index,r.len);

  while( index-- ) if( r[index].type==MenuText ) return {index,true};

  return {0,false};
 }

auto MenuData::findUp(ulen index) const -> FindResult
 {
  auto r=Range(list);

  for(index++; index<r.len ;index++) if( r[index].type==MenuText ) return {index,true};

  return {0,false};
 }

/* class SimpleTopMenuShape */

Coord SimpleTopMenuShape::GetDX(const MenuPoint &point,Font font,Coord space,Coord dy)
 {
  switch( point.type )
    {
     case MenuText :
     case MenuDisabled :
      {
       StrLen str=point.text.str();

       if( ulen s=point.hotindex )
         {
          StrLen str1=str.prefix(s-1);
          StrLen str2=str.part(s);

          TextSize ts=font->text(str1,str2);

          Coordinate dx(ts.full_dx);

          dx+=2*space;

          return +dx;
         }
       else
         {
          TextSize ts=font->text(str);

          Coordinate dx(ts.full_dx);

          dx+=2*space;

          return +dx;
         }
      }
     break;

     case MenuHidden :
      {
       return 0;
      }

     default: // MenuSeparator
      {
       return dy/5;
      }
    }
 }

VColor SimpleTopMenuShape::PlaceFunc::place(ulen index_,char,Point base_,Point delta_)
 {
  if( index==index_ )
    {
     base=base_;
     delta=delta_;
    }

  return vc;
 }

VColor SimpleTopMenuShape::HotFunc::hot(ulen index_,char,Point,Point)
 {
  if( index==index_ ) return hotc;

  return vc;
 }

void SimpleTopMenuShape::Draw(const DrawBuf &buf,const MenuPoint &point,Pane pane,Font font,VColor vc,const Config &cfg,bool showhot)
 {
  StrLen str=point.text.str();

  if( ulen s=point.hotindex )
    {
     StrLen str1=str.prefix(s-1);
     StrLen str2=str.part(s);

     if( showhot )
       {
        if( +cfg.use_hotcolor )
          {
           HotFunc func(vc,str1.len,+cfg.hot);

           font->text(buf,pane,{AlignX_Center,AlignY_Center},str1,str2,func.function_hot());
          }
        else
          {
           PlaceFunc func(vc,str1.len);

           font->text(buf,pane,{AlignX_Center,AlignY_Center},str1,str2,func.function_place());

           Point base=pane.getBase()+func.base;
           MCoord width=+cfg.width;

           base=base.addY(RoundUpLen(width));

           SmoothDrawArt art(buf);

           art.path(HalfNeg,width,+cfg.select,base,base+func.delta);
          }
       }
     else
       {
        font->text(buf,pane,{AlignX_Center,AlignY_Center},str1,str2,vc);
       }
    }
  else
    {
     font->text(buf,pane,{AlignX_Center,AlignY_Center},str,vc);
    }
 }

void SimpleTopMenuShape::Draw(const DrawBuf &buf,Pane pane,const Config &cfg)
 {
  SmoothDrawArt art(buf);

  MPane p(pane);
  TwoField field(p.getTopLeft(),+cfg.left,p.getTopRight(),+cfg.right);

  FigureBox(p).solid(art,field);
 }

void SimpleTopMenuShape::layout()
 {
  Font font=+cfg.font;
  FontSize fs=font->getSize();
  Point space=+cfg.space;

  Coord dy=fs.dy+2*space.y;
  Coordinate x=0;

  for(MenuPoint &point : data.list )
    {
     Coord dx=GetDX(point,font,space.x,dy);

     point.place=Pane(+x,0,dx,dy)+pane.getBase();

     x+=dx;
    }

  max_off=+x;

  if( max_off>pane.dx ) max_off-=pane.dx; else max_off=0;

  Replace_min(off,max_off);
 }

Point SimpleTopMenuShape::getMinSize() const
 {
  Font font=+cfg.font;
  FontSize fs=font->getSize();
  Point space=+cfg.space;

  Coord dy=fs.dy+2*space.y;
  Coordinate x=0;

  for(const MenuPoint &point : data.list )
    {
     x+=GetDX(point,font,space.x,dy);
    }

  return {+x,dy};
 }

void SimpleTopMenuShape::draw(const DrawBuf &buf) const
 {
  Font font=+cfg.font;

  SmoothDrawArt art(buf);

  art.block(pane,+cfg.ground);

  auto r=Range(data.list);

  for(ulen i=0; i<r.len ;i++)
    {
     const MenuPoint &point=r[i];

     Pane pane=point.place-Point(off,0);

     switch( point.type )
       {
        case MenuText :
         {
          if( state==MenuHilight && index==i )
            {
             Draw(buf,point,pane,font,+cfg.hilight,cfg,focus);
            }
          else if( state==MenuSelect && index==i )
            {
             FigureBox(pane).loop(art,HalfPos,+cfg.width,+cfg.select);

             Draw(buf,point,pane,font,+cfg.hilight,cfg,focus);
            }
          else
            {
             Draw(buf,point,pane,font,+cfg.text,cfg,focus);
            }
         }
        break;

        case MenuDisabled :
         {
          Draw(buf,point,pane,font,+cfg.inactive,cfg);
         }
        break;

        case MenuHidden :
         {
          // do nothing
         }
        break;

        case MenuSeparator :
         {
          Draw(buf,pane,cfg);
         }
       }
    }

  if( off>0 )
    {
     Coord len=pane.dy/3;

     Pane p(pane.x,pane.y+pane.dy-len,len,len);

     FigureLeftArrow(p).curveSolid(art,+cfg.hilight);
    }

  if( off<max_off )
    {
     Coord len=pane.dy/3;

     Pane p(pane.x+pane.dx-len,pane.y+pane.dy-len,len,len);

     FigureRightArrow(p).curveSolid(art,+cfg.hilight);
    }
 }

/* class SimpleCascadeMenuShape */

Coord SimpleCascadeMenuShape::GetDX(const MenuPoint &point,Font font,Coord space)
 {
  switch( point.type )
    {
     case MenuText :
     case MenuDisabled :
      {
       StrLen str=point.text.str();

       if( ulen s=point.hotindex )
         {
          StrLen str1=str.prefix(s-1);
          StrLen str2=str.part(s);

          TextSize ts=font->text(str1,str2);

          Coordinate dx(ts.full_dx);

          dx+=2*space;

          return +dx;
         }
       else
         {
          TextSize ts=font->text(str);

          Coordinate dx(ts.full_dx);

          dx+=2*space;

          return +dx;
         }
      }
     break;

     default: // MenuHidden || MenuSeparator
      {
       return 0;
      }
    }
 }

VColor SimpleCascadeMenuShape::PlaceFunc::place(ulen index_,char,Point base_,Point delta_)
 {
  if( index==index_ )
    {
     base=base_;
     delta=delta_;
    }

  return vc;
 }

VColor SimpleCascadeMenuShape::HotFunc::hot(ulen index_,char,Point,Point)
 {
  if( index==index_ ) return hotc;

  return vc;
 }

void SimpleCascadeMenuShape::Draw(const DrawBuf &buf,const MenuPoint &point,Pane pane,Font font,VColor vc,const Config &cfg,bool showhot)
 {
  StrLen str=point.text.str();

  if( ulen s=point.hotindex )
    {
     StrLen str1=str.prefix(s-1);
     StrLen str2=str.part(s);

     if( showhot )
       {
        if( +cfg.use_hotcolor )
          {
           HotFunc func(vc,str1.len,+cfg.hot);

           font->text(buf,pane,{AlignX_Center,AlignY_Center},str1,str2,func.function_hot());
          }
        else
          {
           PlaceFunc func(vc,str1.len);

           font->text(buf,pane,{AlignX_Center,AlignY_Center},str1,str2,func.function_place());

           Point base=pane.getBase()+func.base;
           MCoord width=+cfg.width;

           base=base.addY(RoundUpLen(width));

           SmoothDrawArt art(buf);

           art.path(HalfNeg,width,+cfg.select,base,base+func.delta);
          }
       }
     else
       {
        font->text(buf,pane,{AlignX_Center,AlignY_Center},str1,str2,vc);
       }
    }
  else
    {
     font->text(buf,pane,{AlignX_Center,AlignY_Center},str,vc);
    }
 }

void SimpleCascadeMenuShape::Draw(const DrawBuf &buf,Pane pane,const Config &cfg)
 {
  SmoothDrawArt art(buf);

  MPane p(pane);
  TwoField field(p.getTopLeft(),+cfg.top,p.getBottomLeft(),+cfg.bottom);

  FigureBox(p).solid(art,field);
 }

void SimpleCascadeMenuShape::layout()
 {
  Font font=+cfg.font;
  FontSize fs=font->getSize();
  Point space=+cfg.space;

  Coord dy=fs.dy+2*space.y;
  Coordinate y=0;

  for(MenuPoint &point : data.list )
    switch( point.type )
      {
       case MenuText :
       case MenuDisabled :
        {
         Coord dx=GetDX(point,font,space.x);

         point.place=Pane(0,+y,dx,dy)+pane.getBase();

         y+=dy;
        }
       break;

       case MenuHidden :
        {
         point.place=Empty;
        }
       break;

       case MenuSeparator :
        {
         Coord sdy=dy/5;

         point.place=Pane(0,+y,pane.dx,sdy)+pane.getBase();

         y+=sdy;
        }
       break;
      }

  max_off=+y;

  if( max_off>pane.dy ) max_off-=pane.dy; else max_off=0;

  Replace_min(off,max_off);
 }

Point SimpleCascadeMenuShape::getMinSize() const
 {
  Font font=+cfg.font;
  FontSize fs=font->getSize();
  Point space=+cfg.space;

  Coord dy=fs.dy+2*space.y;
  Coord len=dy;
  Coordinate y=0;

  for(const MenuPoint &point : data.list )
    switch( point.type )
      {
       case MenuText :
       case MenuDisabled :
        {
         Coord dx=GetDX(point,font,space.x);

         Replace_max(len,dx);

         y+=dy;
        }
       break;

       case MenuSeparator :
        {
         Coord sdy=dy/5;

         y+=sdy;
        }
       break;
      }

  return Point(len,+y);
 }

void SimpleCascadeMenuShape::draw(const DrawBuf &buf) const
 {
  Font font=+cfg.font;

  SmoothDrawArt art(buf);

  art.block(pane,+cfg.ground);

  auto r=Range(data.list);

  for(ulen i=0; i<r.len ;i++)
    {
     const MenuPoint &point=r[i];

     Pane pane=point.place-Point(0,off);

     switch( point.type )
       {
        case MenuText :
         {
          if( state==MenuHilight && index==i )
            {
             Draw(buf,point,pane,font,+cfg.hilight,cfg,focus);
            }
          else if( state==MenuSelect && index==i )
            {
             FigureBox(pane).loop(art,HalfPos,+cfg.width,+cfg.select);

             Draw(buf,point,pane,font,+cfg.hilight,cfg,focus);
            }
          else
            {
             Draw(buf,point,pane,font,+cfg.text,cfg,focus);
            }
         }
        break;

        case MenuDisabled :
         {
          Draw(buf,point,pane,font,+cfg.inactive,cfg);
         }
        break;

        case MenuHidden :
         {
          // do nothing
         }
        break;

        case MenuSeparator :
         {
          Draw(buf,pane,cfg);
         }
       }
    }

  if( off>0 )
    {
     Coord len=pane.dy/3;

     Pane p(pane.x+pane.dx-len,pane.y,len,len);

     FigureUpArrow(p).curveSolid(art,+cfg.hilight);
    }

  if( off<max_off )
    {
     Coord len=pane.dy/3;

     Pane p(pane.x+pane.dx-len,pane.y+pane.dy-len,len,len);

     FigureDownArrow(p).curveSolid(art,+cfg.hilight);
    }
 }

} // namespace Video
} // namespace CCore

