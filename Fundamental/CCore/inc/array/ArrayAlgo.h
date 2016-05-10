/* ArrayAlgo.h */
//----------------------------------------------------------------------------------------
//
//  Project: CCore 3.00
//
//  Tag: Fundamental Mini
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2015 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#ifndef CCore_inc_array_ArrayAlgo_h
#define CCore_inc_array_ArrayAlgo_h

#include <CCore/inc/array/Creator.h>

#include <CCore/inc/Move.h>
#include <CCore/inc/MemBase.h>

namespace CCore {

/* functions */

void GuardArrayOverflow(ulen len,ulen maxlen,ulen extra_len);

/* classes */

struct ArrayAlgoMemBase;

template <class T> struct ArrayAlgoBase;

template <class T> struct ArrayAlgoBase_nodtor;

template <class T,class Flags=GetNoThrowFlags<T> > struct ArrayAlgo_mini;

template <class T> struct ArrayAlgo_pod;

template <class T,class Flags=GetNoThrowFlags<T> > struct ArrayAlgo_class;

template <class T,class Flags=GetNoThrowFlags<T> > struct ArrayAlgo;

/* struct ArrayAlgoMemBase */

struct ArrayAlgoMemBase
 {
  static void * MemAlloc(ulen len) { return ::CCore::MemAlloc(len); }

  static bool MemExtend(void *mem,ulen len) { return ::CCore::MemExtend(mem,len); }

  static bool MemShrink(void *mem,ulen len) { return ::CCore::MemShrink(mem,len); }

  static void MemFree(void *mem) { ::CCore::MemFree(mem); }
 };

/* struct ArrayAlgoBase<T> */

template <class T>
struct ArrayAlgoBase : ArrayAlgoMemBase
 {
  //
  //  assume ~T() no-throw
  //

  //
  //  Create() : clean on throw
  //

  class BuildGuard : NoCopy
   {
     Place<void> place;
     T *ptr;
     ulen len;

    public:

     explicit BuildGuard(Place<void> place_) : place(place_),ptr(place_),len(0) {}

     ~BuildGuard() { if( len ) Destroy(ptr,len); }

     Place<void> at() const { return place; }

     void operator ++ () { place+=sizeof (T); len++; }

     PtrLen<T> disarm() { return Range(ptr,Replace_null(len)); }
   };

  class CreateGuard : public BuildGuard
   {
    public:

     CreateGuard(Place<void> place,ulen /*final_len*/) : BuildGuard(place) {}

     ~CreateGuard() {}
   };

  class CreateGuard_nothrow : NoCopy
   {
     Place<void> place;
     T *ptr;
     ulen final_len;

    public:

     CreateGuard_nothrow(Place<void> place_,ulen final_len_) : place(place_),ptr(place_),final_len(final_len_) {}

     Place<void> at() const { return place; }

     void operator ++ () { place+=sizeof (T); }

     PtrLen<T> disarm() { return Range(ptr,final_len); }
   };

  template <bool no_throw,class Base = Meta::Select<no_throw, CreateGuard_nothrow , CreateGuard > >
  class CreateGuardNoThrow : public Base
   {
    public:

     CreateGuardNoThrow(Place<void> place,ulen final_len) : Base(place,final_len) {}

     ~CreateGuardNoThrow() {}
   };

  template <class Creator>
  static PtrLen<T> Create(Place<void> place,ulen len,Creator creator) noexcept( Creator::NoThrow )
   {
    CreateGuardNoThrow<Creator::NoThrow> guard(place,len);

    FunctorTypeOf<Creator> func(creator);

    for(; len ;len--,++guard) func(guard.at());

    return guard.disarm();
   }

  //
  //  single Destroy() : no-throw
  //

  static void Destroy(T *ptr) noexcept
   {
    ptr->~T();
   }

  //
  //  Destroy() : no-throw
  //

  static void Destroy(T *ptr,ulen len) noexcept
   {
    for(; len ;len--,ptr++) Destroy(ptr);
   }
 };

/* struct ArrayAlgoBase_nodtor<T> */

template <class T>
struct ArrayAlgoBase_nodtor : ArrayAlgoMemBase
 {
  //
  //  assume ~T() is trivial
  //

  //
  //  Create()
  //

  class BuildGuard : NoCopy
   {
     Place<void> place;
     T *ptr;
     ulen len;

    public:

     explicit BuildGuard(Place<void> place_) : place(place_),ptr(place_),len(0) {}

     Place<void> at() const { return place; }

     void operator ++ () { place+=sizeof (T); len++; }

     PtrLen<T> disarm() { return Range(ptr,len); }
   };

  class CreateGuard : NoCopy
   {
     Place<void> place;
     T *ptr;
     ulen final_len;

    public:

     CreateGuard(Place<void> place_,ulen final_len_) : place(place_),ptr(place_),final_len(final_len_) {}

     Place<void> at() const { return place; }

     void operator ++ () { place+=sizeof (T); }

     PtrLen<T> disarm() { return Range(ptr,final_len); }
   };

  using CreateGuard_nothrow = CreateGuard ;

  template <bool no_throw>
  class CreateGuardNoThrow : public CreateGuard
   {
    public:

     CreateGuardNoThrow(Place<void> place,ulen final_len) : CreateGuard(place,final_len) {}

     ~CreateGuardNoThrow() {}
   };

  template <class Creator>
  static PtrLen<T> Create(Place<void> place,ulen len,Creator creator) noexcept( Creator::NoThrow )
   {
    PtrLen<T> ret(place,len);

    FunctorTypeOf<Creator> func(creator);

    for(; len ;len--,place+=sizeof (T)) func(place);

    return ret;
   }

  //
  //  single Destroy() : empty
  //

  static void Destroy(T *)
   {
   }

  //
  //  Destroy() : empty
  //

  static void Destroy(T *,ulen)
   {
   }
 };

/* type ArrayAlgoBase_class<T> */

template <class T>
using ArrayAlgoBase_class = Meta::Select< Meta::HasTrivDtor<T> , ArrayAlgoBase_nodtor<T> , ArrayAlgoBase<T> > ;

/* struct ArrayAlgo_mini<T,Flags> */

template <class T,class Flags>
struct ArrayAlgo_mini : ArrayAlgoBase<T>
 {
  //
  //  assume ~T() no-throw
  //

  using ArrayAlgoBase<T>::Create;

  //
  //  Create...() : clean on throw
  //

  enum PropFlagType
   {
    Default_no_throw = Flags::Default_no_throw,
    Copy_no_throw = Flags::Copy_no_throw,

    MoveTo_exist = false
   };

  static PtrLen<T> Create_raw(Place<void> place,ulen len)
   {
    return Create(place,len,Creator_default<T,Default_no_throw>());
   }

  static PtrLen<T> Create_default(Place<void> place,ulen len)
   {
    return Create(place,len,Creator_default<T,Default_no_throw>());
   }

  template <class ... SS>
  static PtrLen<T> Create_fill(Place<void> place,ulen len,SS && ... ss)
   {
    return Create(place,len,Creator_fill<T,SS...>( std::forward<SS>(ss)... ));
   }

  static PtrLen<T> Create_copy(Place<void> place,ulen len,const T src[])
   {
    return Create(place,len,Creator_copy<T,Copy_no_throw>(src));
   }

  template <class S>
  static PtrLen<T> Create_cast(Place<void> place,ulen len,const S src[])
   {
    return Create(place,len,Creator_cast<T,S>(src));
   }
 };

/* struct ArrayAlgo_pod<T> */

template <class T>
struct ArrayAlgo_pod : ArrayAlgoBase_nodtor<T>
 {
  using ArrayAlgoBase_nodtor<T>::Create;

  //
  //  Create...()
  //

  enum PropFlagType
   {
    Default_no_throw = true,
    Copy_no_throw = true,

    MoveTo_exist = true
   };

  static PtrLen<T> Create_raw(Place<void> place,ulen len)
   {
    PtrLen<T> ret(place,len);

    return ret;
   }

  static PtrLen<T> Create_default(Place<void> place,ulen len)
   {
    PtrLen<T> ret(place,len);

    ret.set_null();

    return ret;
   }

  template <class ... SS>
  static PtrLen<T> Create_fill(Place<void> place,ulen len,SS && ... ss)
   {
    return Create(place,len,Creator_fill<T,SS...>( std::forward<SS>(ss)... ));
   }

  static PtrLen<T> Create_copy(Place<void> place,ulen len,const T src[])
   {
    PtrLen<T> ret(place,len);

    ret.copy(src);

    return ret;
   }

  template <class S>
  static PtrLen<T> Create_cast(Place<void> place,ulen len,const S src[])
   {
    return Create(place,len,Creator_cast<T,S>(src));
   }

  static PtrLen<T> Create_swap(Place<void> place,ulen len,T objs[])
   {
    PtrLen<T> ret(place,len);

    ret.copy(objs);

    Range(objs,len).set_null(); // may be excessive

    return ret;
   }

  //
  //  Single : no-throw
  //

  static T * Create_swap(Place<void> place,T &obj) noexcept
   {
    T *ret=new(place) T(obj);

    obj=T(); // may be excessive

    return ret;
   }

  //
  //  ProvideLen() : double extension
  //

  static ulen ProvideLen(ulen len,ulen maxlen,ulen extra_len)
   {
    return Max<ulen>(LenAdd(len,extra_len),2*maxlen+100u);
   }

  //
  //  MoveTo() : no-throw
  //

  static PtrLen<T> MoveTo(T *ptr,ulen len,Place<void> place) noexcept
   {
    PtrLen<T> ret(place,len);

    ret.copy(ptr);

    return ret;
   }
 };

/* struct ArrayAlgo_class<T,Flags> */

template <class T,class Flags>
struct ArrayAlgo_class : ArrayAlgoBase_class<T>
 {
  //
  //  assume ~T() , Move(T *,Place<void>) , Swap(T &,T &) no-throw
  //

  using ArrayAlgoBase_class<T>::Create;

  //
  //  Create...() : clean on throw
  //

  enum PropFlagType
   {
    Default_no_throw = Flags::Default_no_throw,
    Copy_no_throw = Flags::Copy_no_throw,

    MoveTo_exist = true
   };

  static PtrLen<T> Create_raw(Place<void> place,ulen len)
   {
    return Create(place,len,Creator_default<T,Default_no_throw>());
   }

  static PtrLen<T> Create_default(Place<void> place,ulen len)
   {
    return Create(place,len,Creator_default<T,Default_no_throw>());
   }

  template <class ... SS>
  static PtrLen<T> Create_fill(Place<void> place,ulen len,SS && ... ss)
   {
    return Create(place,len,Creator_fill<T,SS...>( std::forward<SS>(ss)... ));
   }

  static PtrLen<T> Create_copy(Place<void> place,ulen len,const T src[])
   {
    return Create(place,len,Creator_copy<T,Copy_no_throw>(src));
   }

  template <class S>
  static PtrLen<T> Create_cast(Place<void> place,ulen len,const S src[])
   {
    return Create(place,len,Creator_cast<T,S>(src));
   }

  static PtrLen<T> Create_swap(Place<void> place,ulen len,T objs[])
   {
    return Create(place,len,Creator_swap<T,ArrayAlgo_class<T,Flags> >(objs));
   }

  //
  //  Single
  //

  static T * Create_swap(Place<void> place,T &obj) noexcept( Default_no_throw )
   {
    T *ret=new(place) T();

    Swap(*ret,obj);

    return ret;
   }

  //
  //  ProvideLen() : double extension
  //

  static ulen ProvideLen(ulen len,ulen maxlen,ulen extra_len)
   {
    return Max<ulen>(LenAdd(len,extra_len),2*maxlen+100u);
   }

  //
  //  MoveTo() : no-throw
  //

  static PtrLen<T> MoveTo(T *ptr,ulen len,Place<void> place) noexcept
   {
    PtrLen<T> ret(place,len);

    for(; len ;len--,ptr++,place+=sizeof (T)) Move(ptr,place);

    return ret;
   }
 };

/* concept Has_ArrayAlgoType<T> */

template <class T>
concept bool Has_ArrayAlgoType = requires()
 {
  typename T::ArrayAlgoType;
 } ;

/* concept No_ArrayAlgoType<T> */

template <class T>
concept bool No_ArrayAlgoType = !Has_ArrayAlgoType<T> ;

/* struct ArrayAlgo<T,Flags> */

template <Has_ArrayAlgoType T,class Flags>
struct ArrayAlgo<T,Flags> : T::ArrayAlgoType {};

template <No_ArrayAlgoType T,class Flags> requires ( PODType<T> && NothrowCopyableType<T> && NothrowDefaultCtorType<T> )
struct ArrayAlgo<T,Flags> : ArrayAlgo_pod<T> {};

template <No_ArrayAlgoType T,class Flags> requires !( PODType<T> && NothrowCopyableType<T> && NothrowDefaultCtorType<T> )
struct ArrayAlgo<T,Flags> : ArrayAlgo_class<T,Flags> {};

} // namespace CCore

#endif


