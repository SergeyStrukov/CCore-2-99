/* Info.h */
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

#ifndef CCore_inc_video_Info_h
#define CCore_inc_video_Info_h

#include <CCore/inc/String.h>

#include <CCore/inc/RefObjectBase.h>

namespace CCore {
namespace Video {

/* classes */

struct AbstractInfo;

class Info;

class InfoFromString;

/* struct AbstractInfo */

struct AbstractInfo
 {
  virtual ulen getLineCount() const =0;

  virtual StrLen getLine(ulen index) const =0;
 };

/* type InfoBase */

using InfoBase = RefObjectBase<AbstractInfo> ;

/* class Info */

class Info
 {
   RefPtr<InfoBase> ptr;

  protected:

   explicit Info(InfoBase *info) : ptr(info) {}

   template <class T>
   T * castPtr() const { return dynamic_cast<T *>(ptr.getPtr()); }

  public:

   Info();

   ~Info() {}

   const AbstractInfo * getPtr() const { return ptr.getPtr(); }

   const AbstractInfo * operator -> () const { return ptr.getPtr(); }
 };

/* class InfoFromString */

class InfoFromString : public Info
 {
   class StringSet : public InfoBase
    {
      DynArray<String> buf;

     public:

      explicit StringSet(StrLen str);

      virtual ~StringSet();

      // AbstractInfo

      virtual ulen getLineCount() const;

      virtual StrLen getLine(ulen index) const;
    };

  public:

   explicit InfoFromString(StrLen str);

   ~InfoFromString();
 };

} // namespace Video
} // namespace CCore

#endif


