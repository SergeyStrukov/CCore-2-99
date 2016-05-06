/* PrintTitle.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 2.00
//
//  Tag: Simple Mini
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2015 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_PrintTitle_h
#define CCore_inc_PrintTitle_h

#include <CCore/inc/Printf.h>

namespace CCore {

/* classes */

template <class T> class PrintTitleType;

/* class PrintTitleType<T> */

template <class T>
class PrintTitleType
 {
   const T &obj;

  public:

   explicit PrintTitleType(const T &obj_) : obj(obj_) {}

   using PrintOptType = TitlePrintOpt ;

   template <class P>
   void print(P &out,PrintOptType opt) const
    {
     out.put(opt.border,opt.off);
     out.put(' ');

     PrintCount<P> out_count(out);

     Putobj(out_count,obj);

     out.put(' ');
     out.put(opt.border,PosSub(opt.width,opt.off,out_count.getCount(),2u));
    }
 };

/* PrintTitle() */

template <class T>
PrintTitleType<T> PrintTitle(const T &obj) { return PrintTitleType<T>(obj); }

} // namespace CCore

#endif


