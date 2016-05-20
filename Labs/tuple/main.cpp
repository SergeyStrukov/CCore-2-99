/* main.cpp */

#include <CCore/inc/Print.h>

namespace App {

/* using */

using namespace CCore;

/* struct PickTypeBox<T> */

template <class T>
struct PickTypeBox
 {
  PickTypeBox<T> operator << (PickTypeBox<AnyType>);

  using Type = T ;
 };

/* struct PickTypeInd<int Ind> */

template <int Ind>
struct PickTypeInd
 {
  PickTypeInd<Ind-1> operator << (AnyType);
 };

template <>
struct PickTypeInd<1>
 {
  template <class T>
  PickTypeBox<T> operator << (PickTypeBox<T>);
 };

/* type PickType<int Ind,TT> */

template <int Ind,class ... TT>
using PickType = typename decltype( ( PickTypeInd<Ind>() << ... << PickTypeBox<TT>() ) )::Type ;

/* struct TypeFactory<TT> */

template <class ... TT>
struct TypeFactory
 {
  template <int Ind>
  using Type = PickType<Ind,TT...> ;
 };

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
  template <int Ind>
  using Type = typename TypeFactory<TT...>::template Type<Ind> ;

  template <int Ind>
  struct Field
   {
    Type<Ind> field;

    Field() {}

    explicit Field(const Type<Ind> &t) : field(t) {}
   };

  template <int ... IList>
  struct Tuple : Field<IList>...
   {
    Tuple() {}

    explicit Tuple(const TT & ... tt) : Field<IList>(tt)... {}

    template <int I>
    auto & ref() { return ((Field<I> *)this)->field; }

    template <int I>
    auto & ref() const { return ((const Field<I> *)this)->field; }

    template <int I>
    auto & ref_const() const { return ((const Field<I> *)this)->field; }

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

/* type Tuple<TT> */

template <class ... TT>
using Tuple = typename TupleFactory<TT...>::Ret ;

} // namespace App

/* main() */

using namespace App;

int main()
 {
  App::Tuple<short,int,long> t(1,2,3);

  //t.ref<1>()=1;
  //t.ref<2>()=2;
  t.ref<3>()=30;

  t.call( [] (short a,int b,long c) { Printf(Con,"#; #; #;\n",a,b,c); } );

  return 0;
 }


