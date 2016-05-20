/* main.cpp */

#include <CCore/inc/Print.h>
#include <CCore/inc/Sort.h>
#include <CCore/inc/Array.h>
#include <CCore/inc/Random.h>
#include <CCore/inc/PrintSet.h>
#include <CCore/inc/Exception.h>

namespace App {

/* using */

using namespace CCore;

/* classes */

template <RanType Ran,SortContextType<Ran> Ctx=SortCtx<Ran> > struct SmartInsSort;

/**/

template <RanType Ran,SortContextType<Ran> Ctx>
struct SmartInsIndexCtx
 {
  Ran base;
  Ctx ctx;
  ulen delta;

  SmartInsIndexCtx(Ran base_,Ctx ctx_,ulen delta_) : base(base_),ctx(ctx_),delta(delta_) {}

  template <class ExtCtx>
  SmartInsIndexCtx(Ran base_,ExtCtx extctx,ulen delta_) : base(base_),ctx(extctx.ctx),delta(delta_) {}

  template <class Ind>
  void swap(Ind *a,Ind *b) { CopySwap(*a,*b); CopySwap(a[delta],b[delta]); }

  template <class Ind>
  bool less(const Ind *a,const Ind *b) const { return ctx.less(base+(*a),base+(*b)); }
 };

template <class Ran,class Ctx>
struct SmartInsPickCtxCtor
 {
  using Ret = SmartInsIndexCtx<Ran,Ctx> ;
 };

template <class Ran,class Ctx>
struct SmartInsPickCtxCtor<Ran,SmartInsIndexCtx<Ran,Ctx> >
 {
  using Ret = SmartInsIndexCtx<Ran,Ctx> ;
 };

template <class Ran,class Ctx>
using SmartInsPickCtx = typename SmartInsPickCtxCtor<Ran,Ctx>::Ret ;

/* class SmartInsSort */

template <RanType Ran,SortContextType<Ran> Ctx>
struct SmartInsSort
 {
  using Ind = uint8 ;

  static const ulen MedLen = Ind(-1)+ulen(1) ;

  template <ULenType Len>
  static void Permutate(Ran a,Len len,Ctx ctx,Ind ind[]);


  template <ULenType Len>
  static bool Sort(Ran a,Len len,Ctx ctx,Ind ind[]);

  template <ULenType Len>
  static bool Sort(Ran a,Len len,Ctx ctx);

  template <ULenType Len>
  static bool Sort(Ran a,Len len) requires ( DefaultCtorType<Ctx> ) { return Sort(a,len,Ctx()); }
 };

template <RanType Ran,SortContextType<Ran> Ctx>
template <ULenType Len>
void SmartInsSort<Ran,Ctx>::Permutate(Ran a,Len len,Ctx ctx,Ind ind[])
 {
  Ind *inv=ind+len;

  for(Len i=0; i<len ;i++)
    {
     inv[ind[i]]=Ind(i);
    }

  for(Len i=0; i<len ;i++)
    {
     Ind j=ind[i];

     if( j!=i )
       {
        ctx.swap(a+i,a+j);

        Ind k=inv[i];

        ind[k]=j;
        inv[j]=k;
       }
    }
 }

template <RanType Ran,SortContextType<Ran> Ctx>
template <ULenType Len>
bool SmartInsSort<Ran,Ctx>::Sort(Ran a,Len len,Ctx ctx,Ind ind[])
 {
  Len len2=len/2;

  for(Len i=0; i<len2 ;i++)
    {
     if( ctx.less(a+ind[i],a+ind[i+len2]) ) CopySwap(ind[i],ind[i+len2]);
    }

  SmartInsSort<Ind *,SmartInsPickCtx<Ran,Ctx> >::Sort(ind,len2,{a,ctx,len2});

  // TODO

  return true;
 }

template <RanType Ran,SortContextType<Ran> Ctx>
template <ULenType Len>
bool SmartInsSort<Ran,Ctx>::Sort(Ran a,Len len,Ctx ctx)
 {
  if( ShortSort<Ran,Ctx>::Sort(a,len,ctx) ) return true;

  if( len>MedLen ) return false;

  Ind ind[2*MedLen];

  for(Len i=0; i<len ;i++) ind[i]=Ind(i);

  Sort(a,len,ctx,ind);

  Permutate(a,len,ctx,ind);

  return true;
 }

/*-------------------------------------------------------------------------------------*/

/* consts */

const ulen Count  = 1000  ;
const ulen MaxLen = 10000 ;

/* functions */

inline ulen IncLen(ulen len)
 {
  if( len<100 ) return 1;
  if( len<1000 ) return 10;
  if( len<10000 ) return 100;

  return 1000;
 }

/* class Engine<Algo> */

template <template <class Ran,class Ctx> class Algo>
class Engine
 {
   using Type = unsigned ;

   Random random;

  private:

   Type next() { return random.select(100,200); }

   void fill(Type *ptr,ulen len)
    {
     for(; len ;len--) *(ptr++)=next();
    }

   bool sort(Type *ptr,ulen len)
    {
     return Algo<Type *,SortCtx<Type *> >::Sort(ptr,len);
    }

   void test(const Type *ptr,ulen len)
    {
     for(; len>1 ;len--,ptr++)
       if( ptr[0]>ptr[1] )
         Printf(Exception,"test failed");
    }

   void show(const Type *ptr,ulen len)
    {
     Printf(Con,"#;\n",PrintSet(Range(ptr,len)));
    }

  public:

   Engine() {}

   ~Engine() {}

   bool run(ulen len)
    {
     if( len>MaxLen ) return false;

     if( len%10==0 )
       Printf(Con,"len = #;\n",len);

     SimpleArray<Type> a(len);

     Type *ptr=a.getPtr();

     for(ulen cnt=Count; cnt ;cnt--)
       {
        fill(ptr,len);

        if( cnt==Count && len==10 ) show(ptr,len);

        if( !sort(ptr,len) ) return false;

        if( cnt==Count && len==10 ) show(ptr,len);

        test(ptr,len);
       }

     return true;
    }
 };

} // namespace App

/* main() */

using namespace App;

int main()
 {
  try
    {
     Engine<SmartInsSort> engine;

     for(ulen len=0; engine.run(len) ;len+=IncLen(len));

     return 0;
    }
  catch(...)
    {
     return 1;
    }
 }


