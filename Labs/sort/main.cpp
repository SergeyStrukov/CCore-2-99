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

/* IndexSmartInsSort() */

template <class Ind,class Len,class Func>
void IndexSmartInsSort2(Ind *ind,Len len,Ind *spare,Func less)
 {
  // TODO
 }

template <class Ind,class Len,class Func>
void TopSmartIns(Ind *ind,Len len,Len top,Func less)
 {
  // TODO
 }

template <class Ind,class Len,class Func>
void IndexSmartInsSort(Ind *ind,Len len,Func less)
 {
  Len len2=len/2;

  // 1

  for(Len i=0; i<len2 ;i+=2)
    {
     if( less(ind[i+1],ind[i]) ) Swap(ind[i],ind[i+1]);
    }

  // 2

  IndexSmartInsSort2(ind,len2,ind+len,less);

  // 3

  Len total=5,top=1,eps=0;

  while( total<=len )
    {
     TopSmartIns(ind,total,top,less);

     top=(2*top-1)+eps;

     eps=4-eps;

     total+=2*top+2;
    }

  Len delta=(total-len+1)/2;

  if( top>=delta )
    {
     top-=delta;
     total-=2*delta;

     TopSmartIns(ind,total,top,less);

     if( total<len )
       {
        TopSmartIns(ind,len,Len(0),less);
       }
    }
 }

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
  IndexSmartInsSort(ind,len, [a,ctx] (Ind i,Ind j) { return ctx.less(a+i,a+j); } );

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


