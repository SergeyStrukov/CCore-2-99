/* Client.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: SubLab 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2016 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <inc/Client.h>

#include <CCore/inc/video/Layout.h>

#include <CCore/inc/Array.h>

#include <CCore/inc/Exception.h>

namespace App {

/* class ClientWindow::TypeInfo::Base */

class ClientWindow::TypeInfo::Base : public ComboInfoBase
 {
   struct Item
    {
     ComboInfoType type;
     DefString name;
     SubFactory factory;

     explicit Item(const DefString &title)
      : type(ComboInfoTitle),
        name(title),
        factory(0)
      {
      }

     Item(const DefString &name_,SubFactory factory_)
      : type(ComboInfoText),
        name(name_),
        factory(factory_)
      {
      }

     operator ComboInfoItem() const
      {
       return ComboInfoItem(type,name.str());
      }
    };

   DynArray<Item> list;

  private:

   void add(const DefString &title)
    {
     list.append_fill(title);
    }

   void add(const DefString &name,SubFactory factory)
    {
     list.append_fill(name,factory);
    }

   template <class W>
   static SubWindow * Create(SubWindowHost &host,const UserPreference &pref)
    {
     return new W(host,pref.getSmartConfig());
    }

  public:

   Base() // TODO
    {
     add("Decorative"_def);

       add("XSingleLine"_def,Create<XSingleLineWindow>);
       add("YSingleLine"_def,Create<YSingleLineWindow>);
       add("XDoubleLine"_def,Create<XDoubleLineWindow>);
       add("YDoubleLine"_def,Create<YDoubleLineWindow>);
       add("Contour"_def,Create<ContourWindow>);
    }

   virtual ~Base()
    {
    }

   // methods

   SubFactory getFactory(ulen index) const
    {
     SubFactory ret=list.at(index).factory;

     if( !ret )
       {
        Printf(Exception,"App::ClientWindow::TypeInfo::Base::getFactory(#;) : title index",index);
       }

     return ret;
    }

   // AbstractComboInfo

   virtual ulen getLineCount() const
    {
     return list.getLen();
    }

   virtual ComboInfoItem getLine(ulen index) const
    {
     return list.at(index);
    }
 };

/* class ClientWindow::TypeInfo */

ClientWindow::TypeInfo::TypeInfo()
 : ComboInfo(new Base)
 {
 }

ClientWindow::TypeInfo::~TypeInfo()
 {
 }

auto ClientWindow::TypeInfo::getFactory(ulen index) const -> SubFactory
 {
  return castPtr<Base>()->getFactory(index);
 }

/* class ClientWindow */

void ClientWindow::wheat_changed(bool)
 {
  redraw();
 }

void ClientWindow::type_selected(ulen index)
 {
  if( Change(cur_index,index) )
    {
     // delete old

     cur.set(0);

     // create new

     cur.set( info.getFactory(index)(wlist,pref) );

     wlist.insBottom(*cur);

     // update

     layout();
     redraw();
    }
 }

ClientWindow::ClientWindow(SubWindowHost &host,const Config &cfg_,const UserPreference &pref_)
 : ComboWindow(host),
   cfg(cfg_),
   pref(pref_),

   check_wheat(wlist,cfg.check_cfg,false),
   label_wheat(wlist,cfg.label_cfg,"Wheat"_def),
   list_type(wlist,cfg.list_cfg,info),

   connector_wheat_changed(this,&ClientWindow::wheat_changed,check_wheat.changed),
   connector_type_selected(this,&ClientWindow::type_selected,list_type.selected)
 {
  wlist.insTop(check_wheat,label_wheat,list_type);
 }

ClientWindow::~ClientWindow()
 {
 }

 // drawing

void ClientWindow::layout()
 {
  PaneCut pane(getSize(),+cfg.space_dxy);

  pane.shrink();

  // check_wheat , label_wheat , list_type

  {
   auto check__wheat=CutBox(check_wheat);
   auto label__wheat=CutPoint(label_wheat);
   auto list__type=CutPoint(list_type);

   Coordinate len=Coordinate(check__wheat.getExt())+label__wheat.getMinSize().x;

   Coord dx=Sup(list__type.getMinSize().x,+len);

   PaneCut p=pane.cutLeft(dx);

   Coord dy=SupDY(check__wheat,label__wheat);

   PaneCut q=p.cutTop(dy);

   q.place_cutLeft(check__wheat).placeMin(label__wheat);

   p.place(list__type);
  }

  // cur

  if( +cur )
    {
     Pane p=pane;

     cur->setPlace(p.shrink(+cfg.spaceInner_dxy));
    }
 }

void ClientWindow::draw(DrawBuf buf,bool drag_active) const
 {
  buf.erase(getBack());

  wlist.draw(buf,drag_active);
 }

void ClientWindow::draw(DrawBuf buf,Pane pane,bool drag_active) const
 {
  buf.block_safe(pane,getBack());

  wlist.draw(buf,pane,drag_active);
 }

 // base

void ClientWindow::open()
 {
  wlist.open();
 }

} // namespace App

