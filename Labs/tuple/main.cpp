/* main.cpp */

#include <CCore/inc/Print.h>

namespace App {

/* using */

using namespace CCore;

/* type EraseType<T> */

template <class T>
using EraseType = int ;

/* struct TupleIndexList<int ... IList> */

template <int ... IList>
struct TupleIndexList
 {
  TupleIndexList<IList...,1+sizeof ... (IList)> operator + (int);

  template <class Factory>
  typename Factory::template Tuple<IList...> apply();
 };

/* struct TupleFactory<TT> */

template <class ... TT>
struct TupleFactory
 {
  template <int Ind,class T>
  struct Field
   {
    T field;

    Field() {}

    explicit Field(const T &t) : field(t) {}
   };

  template <int I,class T>
  static Field<I,T> * Cast(Field<I,T> *ptr) { return ptr; }

  template <int I,class T>
  static const Field<I,T> * Cast(const Field<I,T> *ptr) { return ptr; }

  template <int ... IList>
  struct Tuple : Field<IList,TT>...
   {
    Tuple() {}

    explicit Tuple(const TT & ... tt) : Field<IList,TT>(tt)... {}

    template <int I>
    auto & ref() { return Cast<I>(this)->field; }

    template <int I>
    auto & ref() const { return Cast<I>(this)->field; }

    template <int I>
    auto & ref_const() const { return Cast<I>(this)->field; }

    template <FuncInitArgType<TT & ...> FuncInit>
    auto call(FuncInit func_init)
     {
      FunctorTypeOf<FuncInit> func(func_init);

      return func( ref<IList>()... );
     }

    template <FuncInitArgType<const TT & ...> FuncInit>
    auto call(FuncInit func_init) const
     {
      FunctorTypeOf<FuncInit> func(func_init);

      return func( ref<IList>()... );
     }
   };

  using Ret = decltype( ( TupleIndexList<>() + ... + EraseType<TT>() ).template apply<TupleFactory>() ) ;
 };

/* struct Tuple<TT> */

template <class ... TT>
struct Tuple : TupleFactory<TT...>::Ret
 {
  using TupleFactory<TT...>::Ret::Ret;
 };

} // namespace App

/* main() */

using namespace App;

int main()
 {
  App::Tuple<short,int,long> t(1,2,3);

  //t.ref<1>()=1;
  //t.ref<2>()=2;
  t.ref<3>() *= 10 ;

  t.call( [] (short a,int b,long c) { Printf(Con,"#; #; #;\n",a,b,c); } );

  return 0;
 }


