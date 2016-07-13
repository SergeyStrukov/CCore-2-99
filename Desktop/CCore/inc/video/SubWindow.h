/* SubWindow.h */
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

#ifndef CCore_inc_video_SubWindow_h
#define CCore_inc_video_SubWindow_h

#include <CCore/inc/video/FrameWindow.h>
#include <CCore/inc/video/DrawBuf.h>

#include <CCore/inc/InterfaceHost.h>
#include <CCore/inc/List.h>

namespace CCore {
namespace Video {

/* consts */

enum FocusType
 {
  NoFocus = 0,
  FocusOk,
  FocusTab
 };

/* classes */

struct SubWindowHost;

class SubWindow;

class WindowList;

class ComboWindow;

/* struct SubWindowHost */

struct SubWindowHost
 {
  virtual FrameWindow * getFrame()=0;

  virtual Point getScreenOrigin()=0;

  virtual void redraw(Pane pane)=0; // relative host coords

  virtual void setFocus(SubWindow *sub_win)=0;

  virtual void captureMouse(SubWindow *sub_win)=0;

  virtual void releaseMouse(SubWindow *sub_win)=0;
 };

/* class SubWindow */

class SubWindow : public NoCopyBase<MemBase,UserInput,InterfaceHost>
 {
   DLink<SubWindow> link;
   WindowList *list = 0 ;

   Pane place; // relative host coords

   SubWindowHost &host;

   friend class SubWindowHost;
   friend class WindowList;

  public:

   explicit SubWindow(SubWindowHost &host_) : host(host_) {}

   virtual ~SubWindow();

   // methods

   Pane getPlace() const { return place; }

   Point getSize() const { return place.getSize(); }

   FrameWindow * getFrame() const { return host.getFrame(); }

   WindowHost * getWindowHost() const { return host.getFrame()->getHost(); }

   Desktop * getDesktop() const { return host.getFrame()->getDesktop(); }

   Point getScreenOrigin() const { return host.getScreenOrigin()+place.getBase(); }

   template <class T>
   T toScreen(T obj) const { return obj+getScreenOrigin(); }

   template <class T>
   T fromScreen(T obj) const { return obj-getScreenOrigin(); }

   void setPlace(Pane place_)
    {
     place=place_;

     layout();
    }

   void redraw() { host.redraw(place); }

   void redraw(Pane pane) { host.redraw(Inner(place,pane)); }

   void setFocus() { host.setFocus(this); }

   void captureMouse() { host.captureMouse(this); }

   void releaseMouse() { host.releaseMouse(this); }

   void disableFrameReact() { getFrame()->disableReact(); }

   void enableFrameReact() { getFrame()->enableReact(); }

   // drawing

   virtual void layout()
    {
     // do nothing
    }

   virtual void draw(DrawBuf buf,bool drag_active) const
    {
     buf.erase(drag_active?Gray:Black);
    }

   virtual void draw(DrawBuf buf,Pane pane,bool drag_active) const
    {
     Used(pane);

     draw(buf,drag_active);
    }

   // base

   virtual void open()
    {
     // do nothing
    }

   virtual void close()
    {
     // do nothing
    }

   // keyboard

   virtual FocusType askFocus() const
    {
     return FocusOk;
    }

   virtual void gainFocus()
    {
     // do nothing
    }

   virtual void looseFocus()
    {
     // do nothing
    }

   // tab focus

   virtual void topTabFocus()
    {
    }

   virtual bool nextTabFocus()
    {
     return false;
    }

   virtual void bottomTabFocus()
    {
    }

   virtual bool prevTabFocus()
    {
     return false;
    }

   // mouse

   virtual void looseCapture()
    {
     // do nothing
    }

   virtual MouseShape getMouseShape(Point point,KeyMod kmod) const
    {
     Used(point);
     Used(kmod);

     return Mouse_Arrow;
    }

   // forward

   void forward_draw(const DrawBuf &buf,bool drag_active) const
    {
     draw(buf.cutRebase(place),drag_active);
    }

   void forward_draw(const DrawBuf &buf,Pane pane,bool drag_active) const
    {
     pane=Inf(place,pane);

     if( +pane ) draw(buf.cutRebase(place),pane-place.getBase(),drag_active);
    }

   void forward_react(UserAction action)
    {
     action.rebase(place.getBase());

     react(action);
    }

   struct Forwarder : UserInput
    {
     SubWindow *obj;

     explicit Forwarder(SubWindow *obj_) : obj(obj_) {}

     virtual void react(UserAction action)
      {
       obj->forward_react(action);
      }
    };

   Forwarder forward()
    {
     return Forwarder(this);
    }

   MouseShape forward_getMouseShape(Point point,KeyMod kmod) const { return getMouseShape(point-place.getBase(),kmod); }
 };

/* class WindowList */

class WindowList : public NoCopyBase<SubWindowHost,UserInput>
 {
   SubWindow &parent;

   SubWindow *focus = 0 ;
   SubWindow *capture = 0 ;
   SubWindow *enter = 0 ;

   bool has_focus = false ;
   bool is_opened = false ;

   bool enable_tab = false ;
   bool enable_click = false ;

   using Algo = DLink<SubWindow>::LinearAlgo<&SubWindow::link> ;

   Algo::FirstLast list;

  private:

   SubWindow * find(Point point) const;

   SubWindow * pick(Point point) const;

   SubWindow * next(SubWindow *sub_win);

   SubWindow * prev(SubWindow *sub_win);

  public:

   explicit WindowList(SubWindow &parent_) : parent(parent_) {}

   ~WindowList();

   // methods

   void insTop(SubWindow *sub_win);

   void insBottom(SubWindow *sub_win);

   void del(SubWindow *sub_win);

   void moveTop(SubWindow *sub_win);

   void moveBottom(SubWindow *sub_win);


   void insTop(SubWindow &sub_win) { insTop(&sub_win); }

   void insBottom(SubWindow &sub_win) { insBottom(&sub_win); }

   void del(SubWindow &sub_win) { del(&sub_win); }

   void moveTop(SubWindow &sub_win) { moveTop(&sub_win); }

   void moveBottom(SubWindow &sub_win) { moveBottom(&sub_win); }

   void delAll();

   // multiple insert

   template <class ... TT>
   void insTop(SubWindow *sub_win,TT * ... tt)
    {
     insTop(tt...);

     insTop(sub_win);
    }

   template <class ... TT>
   void insTop(SubWindow &sub_win,TT & ... tt)
    {
     insTop(tt...);

     insTop(sub_win);
    }

   template <class ... TT>
   void insBottom(SubWindow *sub_win,TT * ... tt)
    {
     insBottom(sub_win);

     insBottom(tt...);
    }

   template <class ... TT>
   void insBottom(SubWindow &sub_win,TT & ... tt)
    {
     insBottom(sub_win);

     insBottom(tt...);
    }

   // focus

   SubWindow * getFocus() const { return has_focus?focus:0; }

   void focusTop();

   void focusBottom();

   void focusNext();

   void focusPrev();

   void focusOn(Point point);

   void enableTabFocus(bool enable_tab_=true) { enable_tab=enable_tab_; }

   void enableClickFocus(bool enable_click_=true) { enable_click=enable_click_; }

   // draw

   void draw(const DrawBuf &buf,bool drag_active) const;

   void draw(const DrawBuf &buf,Pane pane,bool drag_active) const;

   // SubWindowHost

   virtual FrameWindow * getFrame();

   virtual Point getScreenOrigin();

   virtual void redraw(Pane pane);

   virtual void setFocus(SubWindow *sub_win);

   virtual void captureMouse(SubWindow *sub_win);

   virtual void releaseMouse(SubWindow *sub_win);

   // base

   void open();

   void close();

   // keyboard

   void gainFocus();

   void looseFocus();

   // tab focus

   void topTabFocus();

   bool nextTabFocus();

   void bottomTabFocus();

   bool prevTabFocus();

   // mouse

   void looseCapture();

   MouseShape getMouseShape(Point point,KeyMod kmod,MouseShape def_shape=Mouse_Arrow) const;

   // user input

   void react(UserAction action,FuncArgType<UserAction> func);

   void react_Keyboard(UserAction action,FuncArgType<UserAction> func);

   void react_Mouse(UserAction action,FuncArgType<UserAction> func);

   void react_Move(Point point,MouseKey mkey,FuncArgType<UserAction> func);

   void react_Leave(FuncArgType<UserAction> func);

   virtual void react(UserAction action);
 };

template <FuncArgType<UserAction> Func>
void WindowList::react(UserAction action,Func func)
 {
  if( action.fromKeyboard() )
    {
     react_Keyboard(action,func);
    }
  else
    {
     struct React
      {
       WindowList *list;
       const Func &func;

       void react_Move(Point point,MouseKey mkey)
        {
         list->react_Move(point,mkey,func);
        }

       void react_Leave()
        {
         list->react_Leave(func);
        }

       void react_other(UserAction action)
        {
         list->react_Mouse(action,func);
        }
      };

     React obj{this,func};

     action.dispatch(obj);
    }
 }

void WindowList::react_Keyboard(UserAction action,FuncArgType<UserAction> func)
 {
  if( focus )
    {
     if( enable_tab )
       {
        struct React
         {
          WindowList *list;
          SubWindow *sub_win;

          void react_Key(VKey vkey,KeyMod kmod)
           {
            if( vkey==VKey_Tab )
              {
               if( kmod&KeyMod_Shift )
                 {
                  list->focusPrev();
                 }
               else
                 {
                  list->focusNext();
                 }
              }
            else
              {
               sub_win->put_Key(vkey,kmod);
              }
           }

          void react_other(UserAction action)
           {
            sub_win->react(action);
           }
         };

        React obj{this,focus};

        action.dispatch(obj);
       }
     else
       {
        focus->react(action);
       }
    }
  else
    {
     func(action);
    }
 }

void WindowList::react_Mouse(UserAction action,FuncArgType<UserAction> func)
 {
  if( enable_click )
    {
     struct React
      {
       WindowList *list;

       void react_LeftClick(Point point,MouseKey)
        {
         list->focusOn(point);
        }

       void react_LeftDClick(Point point,MouseKey)
        {
         list->focusOn(point);
        }

       void react_RightClick(Point point,MouseKey)
        {
         list->focusOn(point);
        }

       void react_RightDClick(Point point,MouseKey)
        {
         list->focusOn(point);
        }
      };

     React obj{this};

     action.dispatch(obj);
    }

  if( SubWindow *sub_win=pick(action.getPoint()) )
    {
     sub_win->forward_react(action);
    }
  else
    {
     func(action);
    }
 }

void WindowList::react_Move(Point point,MouseKey mkey,FuncArgType<UserAction> func)
 {
  if( SubWindow *sub_win=find(point) )
    {
     if( enter!=sub_win )
       {
        if( enter )
          Replace(enter,sub_win)->put_Leave();
        else
          enter=sub_win;
       }

     if( capture )
       capture->forward().put_Move(point,mkey);
     else
       sub_win->forward().put_Move(point,mkey);
    }
  else
    {
     if( enter ) Replace_null(enter)->put_Leave();

     if( capture )
       {
        capture->forward().put_Move(point,mkey);
       }
     else
       {
        UserInputFunc(func).put_Move(point,mkey);
       }
    }
 }

void WindowList::react_Leave(FuncArgType<UserAction> func)
 {
  if( enter )
    {
     Replace_null(enter)->put_Leave();
    }
  else
    {
     UserInputFunc(func).put_Leave();
    }
 }

/* class SomeWindow */

#if 0

class SomeWindow : public SubWindow
 {
   WindowList wlist;

  public:

   explicit SomeWindow(SubWindowHost &host);

   virtual ~SomeWindow();

   // drawing

   virtual void layout();

   virtual void draw(DrawBuf buf,bool drag_active) const;

   virtual void draw(DrawBuf buf,Pane pane,bool drag_active) const;

   // base

   virtual void open();

   virtual void close();

   // keyboard

   virtual FocusType askFocus() const;

   virtual void gainFocus();

   virtual void looseFocus();

   // tab focus

   virtual void topTabFocus();

   virtual bool nextTabFocus();

   virtual void bottomTabFocus();

   virtual bool prevTabFocus();

   // mouse

   virtual void looseCapture();

   virtual MouseShape getMouseShape(Point point,KeyMod kmod) const;

   // user input

   virtual void react(UserAction action);
 };

SomeWindow::SomeWindow(SubWindowHost &host)
 : SubWindow(host),
   wlist(*this)
 {
  // TODO
 }

SomeWindow::~SomeWindow()
 {
 }

 // drawing

void SomeWindow::layout()
 {
  // TODO
 }

void SomeWindow::draw(DrawBuf buf,bool drag_active) const
 {
  // TODO

  wlist.draw(buf,drag_active);
 }

void SomeWindow::draw(DrawBuf buf,Pane pane,bool drag_active) const
 {
  // TODO

  wlist.draw(buf,pane,drag_active);
 }

 // base

void SomeWindow::open()
 {
  // TODO

  wlist.open();
 }

void SomeWindow::close()
 {
  // TODO

  wlist.close();
 }

 // keyboard

FocusType SomeWindow::askFocus() const
 {
  return FocusTab;
 }

void SomeWindow::gainFocus()
 {
  // TODO

  wlist.gainFocus();
 }

void SomeWindow::looseFocus()
 {
  // TODO

  wlist.looseFocus();
 }

 // tab focus

void SomeWindow::topTabFocus()
 {
  wlist.topTabFocus();
 }

bool SomeWindow::nextTabFocus()
 {
  return wlist.nextTabFocus();
 }

void SomeWindow::bottomTabFocus()
 {
  wlist.bottomTabFocus();
 }

bool SomeWindow::prevTabFocus()
 {
  return wlist.prevTabFocus();
 }

 // mouse

void SomeWindow::looseCapture()
 {
  // TODO

  wlist.looseCapture();
 }

MouseShape SomeWindow::getMouseShape(Point point,KeyMod kmod) const
 {
  // TODO

  return wlist.getMouseShape(point,kmod,Mouse_Arrow);
 }

 // user input

void SomeWindow::react(UserAction action)
 {
  action.dispatch(*this, [this] (UserAction action) { wlist.react(action); } );

  /* OR */

  wlist.react(action, [this] (UserAction action) { action.dispatch(*this); } );
 }

#endif

/* class ComboWindow */

class ComboWindow : public SubWindow
 {
  protected:

   WindowList wlist;

  protected:

   explicit ComboWindow(SubWindowHost &host)
    : SubWindow(host),
      wlist(*this)
    {
     wlist.enableTabFocus();
     wlist.enableClickFocus();
    }

   virtual ~ComboWindow()
    {
    }

  public:

   // drawing

   virtual void draw(DrawBuf buf,bool drag_active) const
    {
     wlist.draw(buf,drag_active);
    }

   virtual void draw(DrawBuf buf,Pane pane,bool drag_active) const
    {
     wlist.draw(buf,pane,drag_active);
    }

   // base

   virtual void open()
    {
     wlist.open();

     wlist.focusTop();
    }

   virtual void close()
    {
     wlist.close();
    }

   // keyboard

   virtual FocusType askFocus() const
    {
     return FocusTab;
    }

   virtual void gainFocus()
    {
     wlist.gainFocus();
    }

   virtual void looseFocus()
    {
     wlist.looseFocus();
    }

   // tab focus

   virtual void topTabFocus()
    {
     wlist.topTabFocus();
    }

   virtual bool nextTabFocus()
    {
     return wlist.nextTabFocus();
    }

   virtual void bottomTabFocus()
    {
     wlist.bottomTabFocus();
    }

   virtual bool prevTabFocus()
    {
     return wlist.prevTabFocus();
    }

   // mouse

   virtual void looseCapture()
    {
     wlist.looseCapture();
    }

   virtual MouseShape getMouseShape(Point point,KeyMod kmod) const
    {
     return wlist.getMouseShape(point,kmod);
    }

   // user input

   virtual void react(UserAction action)
    {
     wlist.react(action);
    }
 };

} // namespace Video
} // namespace CCore

#endif



