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
#include <CCore/inc/video/FigureLib.h>

#include <CCore/inc/Array.h>

#include <CCore/inc/Exception.h>

namespace App {

/* class SpaceWindow */

void SpaceWindow::startDrag(Point point)
 {
  if( !drag )
    {
     drag=true;

     drag_base=point;
     space_base=space;

     captureMouse();

     redraw();
    }
 }

void SpaceWindow::dragTo(Point point)
 {
  Point delta=point-drag_base;

  space=space_base+delta;

  redraw();

  changed.assert();
 }

void SpaceWindow::endDrag()
 {
  drag=false;

  releaseMouse();
 }

void SpaceWindow::endDrag(Point point)
 {
  endDrag();

  dragTo(point);
 }

SpaceWindow::SpaceWindow(SubWindowHost &host,const ConfigType &cfg_)
 : SubWindow(host),
   cfg(cfg_)
 {
  space=Point::Diag(+cfg.space_dxy);
 }

SpaceWindow::~SpaceWindow()
 {
 }

 // drawing

bool SpaceWindow::isGoodSize(Point size) const
 {
  return size/2 >= space ;
 }

void SpaceWindow::layout()
 {
  // do nothing
 }

void SpaceWindow::draw(DrawBuf buf,bool) const
 {
  Pane pane(Null,getSize());

  Pane inner=pane.shrink(space);

  if( +inner )
    {
     if( drag )
       {
        MPane p(inner);

        SmoothDrawArt art(buf.cut(pane));

        FigureBox fig(p);

        fig.loop(art,HalfNeg,Fraction(+cfg.border_dxy),+cfg.border);
       }
    }
  else
    {
     SmoothDrawArt art(buf.cut(pane));

     art.erase(+cfg.border);
    }
 }

 // base

void SpaceWindow::open()
 {
  drag=false;
 }

 // keyboard

FocusType SpaceWindow::askFocus() const
 {
  return NoFocus;
 }

 // mouse

void SpaceWindow::looseCapture()
 {
  drag=false;
 }

MouseShape SpaceWindow::getMouseShape(Point,KeyMod) const
 {
  return drag?Mouse_SizeAll:Mouse_Arrow;
 }

 // user input

void SpaceWindow::react(UserAction action)
 {
  action.dispatch(*this);
 }

void SpaceWindow::react_LeftClick(Point point,MouseKey mkey)
 {
  if( mkey&MouseKey_Shift )
    {
     space=point;

     changed.assert();

     redraw();
    }
  else
    {
     startDrag(point);
    }
 }

void SpaceWindow::react_LeftUp(Point point,MouseKey)
 {
  if( drag ) endDrag(point);
 }

void SpaceWindow::react_LeftDClick(Point point,MouseKey mkey)
 {
  react_LeftClick(point,mkey);
 }

void SpaceWindow::react_Move(Point point,MouseKey mkey)
 {
  if( drag )
    {
     if( mkey&MouseKey_Left )
       {
        dragTo(point);
       }
     else
       {
        endDrag();
       }
    }
 }

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

   class TextContourWindow_Left : public TextContourWindow
    {
     public:

      TextContourWindow_Left(SubWindowHost &host,const ConfigType &cfg)
       : TextContourWindow(host,cfg,"Title left"_def,AlignX_Left)
       {
       }
    };

   class TextContourWindow_Right : public TextContourWindow
    {
     public:

      TextContourWindow_Right(SubWindowHost &host,const ConfigType &cfg)
       : TextContourWindow(host,cfg,"Title right"_def,AlignX_Right)
       {
       }
    };

   class TextContourWindow_Center : public TextContourWindow
    {
     public:

      TextContourWindow_Center(SubWindowHost &host,const ConfigType &cfg)
       : TextContourWindow(host,cfg,"Title center"_def,AlignX_Center)
       {
       }
    };

   class ButtonWindow_Button : public ButtonWindow
    {
     public:

      ButtonWindow_Button(SubWindowHost &host,const ConfigType &cfg)
       : ButtonWindow(host,cfg,"Button"_def)
       {
       }
    };

   class KnobWindow_Asterisk : public KnobWindow
    {
     public:

      KnobWindow_Asterisk(SubWindowHost &host,const ConfigType &cfg)
       : KnobWindow(host,cfg,KnobShape::FaceAsterisk)
       {
       }
    };

   class KnobWindow_auto : public KnobWindow
    {
      static KnobShape::FaceType NextFace(KnobShape::FaceType face)
       {
        auto ret=KnobShape::FaceType(face+1);

        if( ret==KnobShape::FaceLim ) return KnobShape::FaceOk;

        return ret;
       }

      void nextFace()
       {
        auto face=getFace();

        setFace(NextFace(face));
       }

      SignalConnector<KnobWindow_auto> connector_pressed;

     public:

      KnobWindow_auto(SubWindowHost &host,const ConfigType &cfg)
       : KnobWindow(host,cfg,KnobShape::FaceOk),
         connector_pressed(this,&KnobWindow_auto::nextFace,pressed)
       {
       }
    };

   class LabelWindow_Label : public LabelWindow
    {
     public:

      LabelWindow_Label(SubWindowHost &host,const ConfigType &cfg)
       : LabelWindow(host,cfg,"Label"_def,AlignX_Center,AlignY_Center)
       {
       }
    };

   class TextWindow_SampleText : public TextWindow
    {
     public:

      TextWindow_SampleText(SubWindowHost &host,const ConfigType &cfg)
       : TextWindow(host,cfg,"Sample text line"_def,AlignX_Center,AlignY_Center)
       {
       }
    };

   class TextLineWindow_SampleText : public TextLineWindow
    {
     public:

      TextLineWindow_SampleText(SubWindowHost &host,const ConfigType &cfg)
       : TextLineWindow(host,cfg,"Sample text long line 1234567890"_def)
       {
       }
    };

   class ProgressWindow_Sample : public ProgressWindow
    {
      DeferInput<ProgressWindow_Sample> input;

      DeferTick defer_tick;

      unsigned pos = 0 ;
      unsigned count = 0 ;

     private:

      void tick()
       {
        if( !count )
          {
           count=2_sectick;

           if( pos<=getTotal() )
             setPosPing(pos++);
           else
             defer_tick.stop();
          }

        count--;
       }

     public:

      ProgressWindow_Sample(SubWindowHost &host,const ConfigType &cfg)
       : ProgressWindow(host,cfg),
         input(this)
       {
        defer_tick=input.create(&ProgressWindow_Sample::tick);

        setTotal(100);
       }

      // base

      virtual void open()
       {
        defer_tick.start();
       }

      virtual void close()
       {
        defer_tick.stop();
       }
    };

   class InfoWindow_SampleInfo : public InfoWindow
    {
      static constexpr const char *Sample=
        "type AtomIndex = uint32 ;\r\n"
        "type SyntIndex = uint32 ;\r\n"
        "type KindIndex = uint32 ;\r\n"
        "type ElementIndex = uint32 ;\r\n"
        "type RuleIndex = uint32 ;\r\n"
        "type StateIndex = uint32 ;\r\n"
        "type FinalIndex = uint32 ;\r\n"
        "\r\n"
        "struct Lang\r\n"
        " {\r\n"
        "  Atom[] atoms;\r\n"
        "  Synt[] synts;\r\n"
        "  Synt * [] lang;\r\n"
        "  Element[] elements;\r\n"
        "  Rule[] rules;\r\n"
        "  TopRule[] top_rules;\r\n"
        "  State[] states;\r\n"
        "  Final[] finals;\r\n"
        " };\r\n"
        "\r\n"
        "struct Atom\r\n"
        " {\r\n"
        "  AtomIndex index;\r\n"
        "  text name;\r\n"
        "  \r\n"
        "  Element *element;\r\n"
        " };\r\n"
        "\r\n"
        "struct Synt\r\n"
        " {\r\n"
        "  SyntIndex index;\r\n"
        "  text name;\r\n"
        "  \r\n"
        "  Kind[] kinds;\r\n"
        "  Rule * [] rules;\r\n"
        " };\r\n"
        "\r\n"
        "struct Kind\r\n"
        " {\r\n"
        "  KindIndex kindex; // index among all kinds\r\n"
        "  KindIndex index; // index in synt array\r\n"
        "  text name;\r\n"
        "\r\n"
        "  Synt *synt;\r\n"
        "  \r\n"
        "  Element *element;\r\n"
        "  \r\n"
        "  TopRule * [] rules;\r\n"
        " };\r\n"
        "\r\n"
        "struct Element\r\n"
        " {\r\n"
        "  ElementIndex index;\r\n"
        "  \r\n"
        "  {Atom,Kind} * elem;\r\n"
        " };\r\n"
        "\r\n"
        "struct Rule\r\n"
        " {\r\n"
        "  RuleIndex index;\r\n"
        "  text name;\r\n"
        "\r\n"
        "  Kind *result;\r\n"
        "\r\n"
        "  type Arg = {Atom,Synt} * ;\r\n"
        "\r\n"
        "  Arg[] args;\r\n"
        " };\r\n"
        " \r\n"
        "struct TopRule\r\n"
        " {\r\n"
        "  RuleIndex index;\r\n"
        "  text name;\r\n"
        "  \r\n"
        "  Rule *bottom;\r\n"
        "\r\n"
        "  Kind *result;\r\n"
        "\r\n"
        "  type Arg = {Atom,Kind} * ;\r\n"
        "\r\n"
        "  Arg[] args;\r\n"
        " }; \r\n"
        "\r\n"
        "struct State\r\n"
        " {\r\n"
        "  StateIndex index;\r\n"
        "  Final *final;\r\n"
        "\r\n"
        "  struct Transition\r\n"
        "   {\r\n"
        "    Element *element;\r\n"
        "    State *state;\r\n"
        "   };\r\n"
        "\r\n"
        "  Transition[] transitions;\r\n"
        " };\r\n"
        "\r\n"
        "struct Final\r\n"
        " {\r\n"
        "  FinalIndex index;\r\n"
        "\r\n"
        "  struct Action\r\n"
        "   {\r\n"
        "    Atom *atom; // null for (End)\r\n"
        "    Rule *rule; // null for <- ( STOP if atom is (END) )\r\n"
        "   };\r\n"
        "\r\n"
        "  Action[] actions;\r\n"
        " };\r\n"
        "\r\n";

     public:

      InfoWindow_SampleInfo(SubWindowHost &host,const ConfigType &cfg)
       : InfoWindow(host,cfg,InfoFromString(Sample))
       {
       }
    };

  public:

   Base() // TODO
    {
     add("Decorative"_def);

       add("XSingleLine"_def,Create<XSingleLineWindow>);
       add("YSingleLine"_def,Create<YSingleLineWindow>);
       add("XDoubleLine"_def,Create<XDoubleLineWindow>);
       add("YDoubleLine"_def,Create<YDoubleLineWindow>);
       add("Contour"_def,Create<ContourWindow>);
       add("TextContour left"_def,Create<TextContourWindow_Left>);
       add("TextContour right"_def,Create<TextContourWindow_Right>);
       add("TextContour center"_def,Create<TextContourWindow_Center>);

     add("Button"_def);

       add("Button"_def,Create<ButtonWindow_Button>);
       add("Knob"_def,Create<KnobWindow_Asterisk>);
       add("Knob auto"_def,Create<KnobWindow_auto>);

     add("Box"_def);

       add("Check"_def,Create<CheckWindow>);
       add("Switch"_def,Create<SwitchWindow>);

       // Window.Check  AltWindow

       // Window.Radio
       // Window.Light

     add("Line"_def);

       add("Label"_def,Create<LabelWindow_Label>); // Window.Text
       add("Text"_def,Create<TextWindow_SampleText>); // Window.Text
       add("TextLine"_def,Create<TextLineWindow_SampleText>);
       add("Progress"_def,Create<ProgressWindow_Sample>);

     add("Pane"_def);

       add("Info"_def,Create<InfoWindow_SampleInfo>);

       //add(""_def,Create<>);
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

     wlist.insTop(*cur);

     // update

     layout();
     redraw();
    }
 }

void ClientWindow::space_changed()
 {
  if( +cur )
    {
     cur->setPlace(space.getInner());

     cur->redraw();
    }
 }

ClientWindow::ClientWindow(SubWindowHost &host,const Config &cfg_,const UserPreference &pref_)
 : ComboWindow(host),
   cfg(cfg_),
   pref(pref_),

   check_wheat(wlist,cfg.check_cfg,false),
   label_wheat(wlist,cfg.label_cfg,"Wheat"_def),
   list_type(wlist,cfg.list_cfg,info),
   space(wlist,cfg.space_cfg),

   connector_wheat_changed(this,&ClientWindow::wheat_changed,check_wheat.changed),
   connector_type_selected(this,&ClientWindow::type_selected,list_type.selected),
   connector_space_changed(this,&ClientWindow::space_changed,space.changed)
 {
  wlist.insTop(check_wheat,label_wheat,list_type,space);
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

  // space

  pane.place(space);

  // cur

  if( +cur )
    {
     cur->setPlace(space.getInner());
    }
 }

void ClientWindow::draw(DrawBuf buf,bool drag_active) const
 {
  buf.erase(getBack());

  wlist.draw(buf,drag_active);
 }

void ClientWindow::draw(DrawBuf buf,Pane pane,bool drag_active) const
 {
  buf.erase(pane,getBack());

  wlist.draw(buf,pane,drag_active);
 }

 // base

void ClientWindow::open()
 {
  wlist.open();
 }

} // namespace App

