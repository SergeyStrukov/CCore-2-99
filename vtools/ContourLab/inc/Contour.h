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
#include <CCore/inc/Array.h>
#include <CCore/inc/CompactMap.h>
#include <CCore/inc/StrKey.h>

namespace App {

/* functions */

void GuardTypeMismatch();

inline void GuardType(int t1,int t2) { if( t1!=t2 ) GuardTypeMismatch(); }

/* classes */

template <class S> struct UnusedPad;

struct Formular;

struct Label;

class Contour;

/* struct UnusedPad<S> */

template <class S>
struct UnusedPad
 {
  static S pad;
 };

template <class S>
S UnusedPad<S>::pad{};

/* struct Formular */

struct Formular : Geometry
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

        // default object

        static Base * GetDefault()
         {
          Base *ret=&Default;

          ret->incRef();

          return ret;
         }

        static Base Default;
      };

     template <class S>
     struct NodeInterface : Base
      {
       virtual S get() const =0;

       virtual S & ref() =0;
      };

     template <class T,class S=typename T::RetType>
     struct Node : NodeInterface<S>
      {
       T obj;

       template <class ... SS>
       explicit Node(SS && ... ss) : obj( std::forward<SS>(ss)... ) {}

       virtual S get() const { return obj(); }

       virtual S & ref() { return obj.pad; }
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

     Object() noexcept : ptr(Base::GetDefault()),type_id(0) {}

     template <class T,class ... SS>
     Object(Tag<T>,SS && ... ss) : ptr(new Node<T>( std::forward<SS>(ss)... )),type_id(T::TypeId) {}

     ~Object() {}

     int getTypeId() const { return type_id&(PadFlag-1); }

     int isPad() const { return type_id&PadFlag; }

     StrLen getTypeName() const { return TypeName(getTypeId()); }

     template <class S>
     S get() const
      {
       GuardType(getTypeId(),S::TypeId);

       return static_cast<NodeInterface<S> *>(ptr.getPtr())->get();
      }

     template <class Func>
     auto call(Func func) const
      {
       return TypeSwitch(getTypeId(),CallFunc<Func>(func,*this));
      }

     template <class S>
     S & ref() const
      {
       GuardType(type_id,S::TypeId|PadFlag);

       return static_cast<NodeInterface<S> *>(ptr.getPtr())->ref();
      }

     template <class Func>
     auto callRef(Func func) const
      {
       return TypeSwitch(getTypeId(),CallRefFunc<Func>(func,*this));
      }
   };

  template <class T,class ... SS>
  static Object CreateObject(SS && ... ss) { return Object(Object::Tag<T>(), std::forward<SS>(ss)... ); }

  // Function binder

  template <class S>
  using ToObject = Object ;

  template <class S,class IList,class ... SS> struct FormulaFactory;

  template <class S,class ... SS,int ... IList>
  struct FormulaFactory<S,Meta::IndexListBox<IList...>,SS...>
   {
    template <S Func(SS...)>
    struct Pack : UnusedPad<S>
     {
      static const int TypeId = S::TypeId ;

      using RetType = S ;

      Object args[sizeof ... (SS)];

      explicit Pack(const ToObject<SS> & ... ss) : args{ss...} {}

      static S SafeCall(SS ... ss)
       {
        try
          {
           AssertValid(ss...);

           return Func(ss...);
          }
        catch(RealException rex)
          {
           return S(rex);
          }
       }

      S operator () () const { return SafeCall( args[IList-1].template get<SS>()... ); }
     };

    template <S Func(SS...)>
    static Object Create(const ToObject<SS> & ... ss) { return CreateObject<Pack<Func> >(ss...); }

    static bool TestType(const Object *list,const int *type,int ind) { return list[ind].getTypeId()!=type[ind]; }

    template <S Func(SS...)>
    static bool SafeCreate(Object &ret,PtrLen<const Object> list)
     {
      if( list.len!=sizeof ... (SS) ) return false;

      int temp[]={ SS::TypeId ... };

      if( ( TestType(list.ptr,temp,IList-1) || ... ) ) return false;

      ret=CreateObject<Pack<Func> >( list[IList-1]... );

      return true;
     }
   };

  template <class S,class ... SS>
  using FormulaAlias = FormulaFactory<S, Meta::IndexList<SS...> ,SS...> ;

  template <class S,class ... SS>
  struct Formula : FormulaAlias<S,SS...>
   {
   };

  template <class S>
  struct Formula<S>
   {
    template <S Func()>
    struct Pack : UnusedPad<S>
     {
      static const int TypeId = S::TypeId ;

      using RetType = S ;

      Pack() {}

      static S SafeCall()
       {
        try
          {
           return Func();
          }
        catch(RealException rex)
          {
           return S(rex);
          }
       }

      S operator () () const { return SafeCall(); }
     };

    template <S Func()>
    static Object Create() { return CreateObject<Pack<Func> >(); }
   };

  template <class Sig> struct FormulaTypeCtor;

  template <class S,class ... SS>
  struct FormulaTypeCtor<S (SS...)>
   {
    using RetType = Formula<S,SS...> ;
   };

  template <class Sig>
  using FormulaType = typename FormulaTypeCtor<Sig>::RetType ;

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

/* struct Label */

struct Label
 {
  // property

  String name;

  bool show = true ;
  bool gray = true ;
  bool show_name = true ;

  // work

  MPoint pos;
  bool pos_ok = false ;

  Label() noexcept {}

  explicit Label(const String &name_) noexcept : name(name_) {}

  bool test()
   {
    pos_ok=false;

    return show;
   }

  void setPos(MPoint pos_)
   {
    pos=pos_;
    pos_ok=true;
   }
 };

/* struct ErrorText */

struct ErrorText : NoCopy
 {
  char buf[TextBufLen];
  ulen len = 0 ;

  // methods

  bool operator + () const { return len==0; }

  bool operator ! () const { return len!=0; }

  StrLen getText() const { return Range(buf,len); }

  void setText(StrLen str)
   {
    len=Min(DimOf(buf),str.len);

    Range(buf,len).copy(str.ptr);
   }
 };

/* class Contour */

class Contour : public Formular
 {
   struct Item
    {
     mutable Label label;
     Object obj;
    };

   struct Key : CmpComparable<Key>
    {
     StrKey::HashType hash;
     String name;

     explicit Key(StrKey k) : hash(k.hash),name(k.str) {}

     CmpResult objCmp(const Key &obj) const
      {
       if( CmpResult ret=LessCmp(hash,obj.hash) ) return ret;

       return StrCmp(Range(name),Range(obj.name));
      }
    };

   friend CmpResult Cmp(StrKey a,const Key &b)
    {
     if( CmpResult ret=LessCmp(a.hash,b.hash) ) return ret;

     return StrCmp(a.str,Range(b.name));
    }

   DynArray<Item> pads;
   DynArray<Item> formulas;

   CompactRBTreeMap<Key,Object,StrKey> map;

  private:

   class ItemInfo : public ComboInfo
    {
      class Base;

     public:

      explicit ItemInfo(const DynArray<Item> &data);

      ~ItemInfo();
    };

   static bool UpItem(DynArray<AnyType> &a,ulen index)
    {
     auto r=Range(a);

     if( index>0 && index<r.len )
       {
        Swap(r[index],r[index-1]);

        return true;
       }

     return false;
    }

   static bool DownItem(DynArray<AnyType> &a,ulen index)
    {
     auto r=Range(a);

     if( r.len && index<r.len-1 )
       {
        Swap(r[index],r[index+1]);

        return true;
       }

     return false;
    }

  private:

   template <class S>
   bool addPad(ulen index,StrLen name,S s)
    {
     pads.reserve(1);

     StrKey k(name);

     Object obj=Pad<S>::Create(s);

     auto result=map.find_or_add(k,obj);

     if( !result.new_flag ) return false;

     Label label(result.key->name);

     Item item{label,obj};

     ArrayCopyIns(pads,index,item);

     return true;
    }

   template <class ... OO>
   bool addFormula(ulen index,StrLen name,Object obj)
    {
     formulas.reserve(1);

     StrKey k(name);

     auto result=map.find_or_add(k,obj);

     if( !result.new_flag ) return false;

     Label label(result.key->name);

     Item item{label,obj};

     ArrayCopyIns(formulas,index,item);

     return true;
    }

   bool testName(StrLen name) const;

   bool delItem(DynArray<Item> &a,ulen index);

   class PadTestParser;

   class PadAddParser;

   class FormulaTestContext;

   class FormulaAddContext;

   template <class Func>
   struct Bind
    {
     Func func;
     bool selected;
     Label &label;

     void operator () (AnyType s) { if( !s.rex ) func(label,selected,s); }

     void operator () () {}
    };

   template <class Func>
   struct BindRef
    {
     Func func;
     Label &label;
     const Object &obj;

     void operator () (AnyType &s) { func(label,obj,s); }

     void operator () () {}
    };

  public:

   Contour();

   ~Contour();

   // pad list

   ComboInfo getPadInfo();

   bool padUp(ulen index);

   bool padDown(ulen index);

   bool padDel(ulen index);

   bool padAddTest(StrLen text,CharAccent *accent) const;

   bool padAdd(ulen index,StrLen text);

   // formula list

   ComboInfo getFormulaInfo();

   bool formulaUp(ulen index);

   bool formulaDown(ulen index);

   bool formulaDel(ulen index);

   bool formulaAddTest(StrLen text,CharAccent *accent) const;

   bool formulaAdd(ulen index,StrLen text);

   // elements

   Label & refPadLabel(ulen index) { return pads.at(index).label; }

   Label & refFormulaLabel(ulen index) { return formulas.at(index).label; }

   template <class Func>
   void pad(ulen index,Func func)
    {
     Item &item=pads.at(index);

     item.obj.callRef( BindRef<Func>{func,item.label,item.obj} );
    }

   template <class Func>
   void apply(ulen pad_ind,ulen formula_ind,Func func) const
    {
     for(ulen i=0,len=pads.getLen(); i<len ;i++)
       {
        const Item &item=pads[i];

        item.obj.call( Bind<Func>{func,i==pad_ind,item.label} );
       }

     for(ulen i=0,len=formulas.getLen(); i<len ;i++)
       {
        const Item &item=formulas[i];

        item.obj.call( Bind<Func>{func,i==formula_ind,item.label} );
       }
    }

   template <class Func>
   void applyRef(Func func)
    {
     auto temp = [=] (Item &item) { item.obj.callRef( BindRef<Func>{func,item.label,item.obj} ); } ;

     pads.apply(temp);
    }

   // save/load

   void erase();

   void save(StrLen file_name,ErrorText &etext) const;

   void load(StrLen file_name,ErrorText &etext);
 };

} // namespace App

#endif
