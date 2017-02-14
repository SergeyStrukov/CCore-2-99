/* MCoordEdit.h */
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
//  Copyright (c) 2017 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_video_pref_MCoordEdit_h
#define CCore_inc_video_pref_MCoordEdit_h

#include <CCore/inc/video/WindowLib.h>

namespace CCore {
namespace Video {

/* classes */

class MCoordEditWindow;

/* class MCoordEditWindow */

class MCoordEditWindow : public ComboWindow
 {
  public:

   struct Config
    {
     RefVal<unsigned> precision = 4 ;

     CtorRefVal<KnobWindow::ConfigType> knob_cfg;
     CtorRefVal<TextWindow::ConfigType> text_cfg;

     Config() noexcept {}

     template <class Bag,class Proxy>
     void bind(const Bag &bag,Proxy proxy)
      {
       precision.bind(bag.mcoord_edit_precision);

       knob_cfg.bind(proxy);
       text_cfg.bind(proxy);
      }
    };

   using ConfigType = Config ;

  private:

   const Config &cfg;

   MCoord value = 0 ;

   KnobWindow plus;
   KnobWindow minus;
   KnobWindow small_plus;
   KnobWindow small_minus;
   TextWindow text;

  private:

   void showVal();

   void plusVal();

   void minusVal();

   void plusSmallVal();

   void minusSmallVal();

   SignalConnector<MCoordEditWindow> connector_plus_pressed;
   SignalConnector<MCoordEditWindow> connector_minus_pressed;

   SignalConnector<MCoordEditWindow> connector_small_plus_pressed;
   SignalConnector<MCoordEditWindow> connector_small_minus_pressed;

  public:

   MCoordEditWindow(SubWindowHost &host,const ConfigType &cfg);

   virtual ~MCoordEditWindow();

   // methods

   Point getMinSize() const;

   MCoord getMCoord() const { return value; }

   void setMCoord(MCoord value);

   // drawing

   virtual void layout();

   // user input

   virtual void react(UserAction action);

   void react_Char(char ch);

   // signals

   Signal<MCoord> changed; // value
 };

} // namespace Video
} // namespace CCore

#endif

