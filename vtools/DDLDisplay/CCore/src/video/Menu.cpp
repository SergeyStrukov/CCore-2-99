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
       return dy/10;
      }
    }
 }

void SimpleTopMenuShape::Draw(const DrawBuf &buf,const MenuPoint &point,Font font,VColor vc,bool showhot)
 {
  StrLen str=point.text.str();

  if( ulen s=point.hotindex )
    {
     StrLen str1=str.prefix(s-1);
     StrLen str2=str.part(s);

     font->text(buf,point.place,{AlignX_Center,AlignY_Center},str1,str2,vc);

     if( showhot )
       {
       }
    }
  else
    {
     font->text(buf,point.place,{AlignX_Center,AlignY_Center},str,vc);
    }
 }

void SimpleTopMenuShape::layout()
 {
  Font font=+cfg.font;
  FontSize fs=font->getSize();
  Point space=+cfg.space;

  Coord dy=fs.dy+2*space.y;
  Coordinate dx=0;

  for(MenuPoint &point : data.list )
    {
     Coord len=GetDX(point,font,space.x,dy);

     point.place=Pane(+dx,0,len,dy);

     dx+=len;
    }
 }

Point SimpleTopMenuShape::getMinSize() const
 {
  Font font=+cfg.font;
  FontSize fs=font->getSize();
  Point space=+cfg.space;

  Coord dy=fs.dy+2*space.y;
  Coordinate dx=0;

  for(const MenuPoint &point : data.list )
    {
     dx+=GetDX(point,font,space.x,dy);
    }

  return {+dx,dy};
 }

void SimpleTopMenuShape::draw(const DrawBuf &buf) const
 {
  Font font=+cfg.font;

  SmoothDrawArt art(buf);

  art.erase(+cfg.ground);

  auto r=Range(data.list);

  for(ulen i=0; i<r.len ;i++)
    {
     const MenuPoint &point=r[i];

     switch( point.type )
       {
        case MenuText :
         {
          if( state==MenuHilight && index==i )
            {
             Draw(buf,point,font,+cfg.hilight);
            }
          else if( state==MenuSelect && index==i )
            {
             FigureBox(point.place).loop(art,HalfPos,+cfg.width,+cfg.select);

             Draw(buf,point,font,+cfg.hilight);
            }
          else
            {
             Draw(buf,point,font,+cfg.text);
            }
         }
        break;

        case MenuDisabled :
         {
          Draw(buf,point,font,+cfg.inactive);
         }
        break;

        default: // MenuSeparator
         {
         }
       }
    }
 }

} // namespace Video
} // namespace CCore

