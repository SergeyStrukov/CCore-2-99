/* Contour.h */
//----------------------------------------------------------------------------------------
//
//  Project: ContourLab 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2017 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef Contour_h
#define Contour_h

#include <inc/Geometry.h>

#include <CCore/inc/MemBase.h>
#include <CCore/inc/RefPtr.h>
#include <CCore/inc/String.h>

namespace App {

/* functions */

void GuardTypeMismatch();

inline void GuardType(int t1,int t2) { if( t1!=t2 ) GuardTypeMismatch(); }

/* classes */

template <class S> struct UnusedPad;

struct Formula;

class Contour;

/* struct UnusedPad<S> */

template <class S>
struct UnusedPad
 {
  static S pad;
 };

template <class S>
S UnusedPad<S>::pad;

/* struct Formula */

struct Formula : Geometry
 {
  // Object

  static const int PadFlag = 1<<10 ;

  class Object
   {
     class Base : MemBase_nocopy
      {
        ulen ref_count = 1 ;

       public:

        Base() {}

        virtual ~Base() {}

        void incRef() { ref_count++; }

        bool decRef() { return !--ref_count; }

        void destroy() noexcept { delete this; }
      };

     template <class S>
     struct NodeInterface : Base
      {
       virtual S get() const =0;

       virtual S & ref() const =0;
      };

     template <class T,class S=typename T::RetType>
     struct Node : NodeInterface<S>
      {
       T obj;

       template <class ... SS>
       explicit Node(SS && ... ss) : obj( std::forward<SS>(ss)... ) {}

       virtual S get() const { return obj(); }

       virtual S & ref() const { return obj.pad; }
      };

     RefPtr<Base> ptr;
     int type_id;

    private:

     template <class Func>
     struct CallFunc
      {
       Func &func;
       const Object &arg;

       CallFunc(Func &func_,const Object &arg_) : func(func_),arg(arg_) {}

       template <class S>
       auto doIt() { return func(arg.get<S>()); }

       auto doOther() { return func(); }
      };

     template <class Func>
     struct CallRefFunc
      {
       Func &func;
       const Object &arg;

       CallRefFunc(Func &func_,const Object &arg_) : func(func_),arg(arg_) {}

       template <class S>
       auto doIt() { return func(arg.ref<S>()); }

       auto doOther() { return func(); }
      };

    public:

     template <class T> struct Tag {};

     template <class T,class ... SS>
     Object(Tag<T>,SS && ... ss) : ptr(new Node<T>( std::forward<SS>(ss)... )),type_id(T::TypeId) {}

     ~Object() {}

     int getTypeId() const { return type_id&(PadFlag-1); }

     int isPad() const { return type_id&PadFlag; }

     StrLen getTypeName() const { return TypeName(getTypeId()); }

     template <class S>
     S get() const
      {
       GuardType(type_id,S::TypeId);

       return static_cast<NodeInterface<S> *>(ptr.getPtr())->get();
      }

     template <class Func>
     auto call(Func func)
      {
       return TypeSwitch(type_id,CallFunc<Func>(func,*this));
      }

     template <class S>
     S & ref() const
      {
       GuardType(type_id,S::TypeId|PadFlag);

       return static_cast<NodeInterface<S> *>(ptr.getPtr())->ref();
      }

     template <class Func>
     auto callRef(Func func)
      {
       return TypeSwitch(type_id,CallRefFunc<Func>(func,*this));
      }
   };

  template <class T,class ... SS>
  static Object CreateObject(SS && ... ss) { return Object(Object::Tag<T>(), std::forward<SS>(ss)... ); }

  // Function binder

  template <class S>
  using ToObject = Object ;

  template <class S,class IList,class ... SS> struct BinderFactory;

  template <class S,class ... SS,int ... IList>
  struct BinderFactory<S,Meta::IndexListBox<IList...>,SS...>
   {
    template <S Func(SS...)>
    struct Pack : UnusedPad<S>
     {
      static const int TypeId = S::TypeId ;

      using RetType = S ;

      Object args[sizeof ... (SS)];

      explicit Pack(const ToObject<SS> & ... ss) : args{ss...} {}

      S operator () () const { return Func( args[IList-1].get()... ); }
     };

    template <S Func(SS...)>
    static Object Create(const ToObject<SS> & ... ss) { return CreateObject<Pack<Func> >(ss...); }
   };

  template <class S,class ... SS>
  using BinderAlias = BinderFactory<S, Meta::IndexList<SS...> ,SS...> ;

  template <class S,class ... SS>
  struct Binder : BinderAlias<S,SS...>
   {
   };

  template <class S>
  struct Binder<S>
   {
    template <S Func()>
    struct Pack : UnusedPad<S>
     {
      static const int TypeId = S::TypeId ;

      using RetType = S ;

      Pack() {}

      S operator () () const { return Func(); }
     };

    template <S Func()>
    static Object Create() { return CreateObject<Pack<Func> >(); }
   };

  // Pad

  template <class S>
  struct Pad
   {
    static const int TypeId = S::TypeId|PadFlag ;

    using RetType = S ;

    S pad;

    explicit Pad(const S &obj) : pad(obj) {}

    S operator () () const { return pad; }

    static Object Create(const S &obj) { return CreateObject<Pad>(obj); }
   };
 };

/* class Contour */

class Contour : public Formula
 {
 };

} // namespace App

#endif
