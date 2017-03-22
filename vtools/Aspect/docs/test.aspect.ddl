//#include <Aspect.ddl>

Aspect Data=
 {
  "..",
  {
   "",
   Yellow,
   {
    &Dir0,
    &Dir1,
    &Dir2,
    &Dir3
   },
   {
    { "Makefile" , Ignore },
    { "Makefile.cpp-files" , Ignore },
    { "Makefile.files" , Ignore },
    { "Makefile.s-files" , Ignore }
   }
  }
 };

Dir Dir0=
  {
   "docs",
   Ignore,
   {
   },
   {
    { "html.aspect.ddl" , Ignore },
    { "test.aspect.ddl" , Ignore }
   }
  };

Dir Dir1=
  {
   "inc",
   Yellow,
   {
   },
   {
    { "Application.h" , Green },
    { "Aspect.h" , Yellow },
    { "AspectData.h" , Green },
    { "Client.h" , Green },
    { "ErrorText.h" , Green }
   }
  };

Dir Dir2=
  {
   "obj",
   Ignore,
   {
   },
   {
    { "empty" , Ignore },
    { "Application.dep" , Ignore },
    { "Aspect.dep" , Ignore },
    { "AspectData.dep" , Ignore },
    { "Client.dep" , Ignore },
    { "ErrorText.dep" , Ignore },
    { "main.dep" , Ignore },
    { "Makefile.dep" , Ignore },
    { "Application.o" , Ignore },
    { "Aspect.o" , Ignore },
    { "AspectData.o" , Ignore },
    { "Client.o" , Ignore },
    { "ErrorText.o" , Ignore },
    { "main.o" , Ignore }
   }
  };

Dir Dir3=
  {
   "src",
   Yellow,
   {
   },
   {
    { "Application.cpp" , Green },
    { "Aspect.cpp" , Yellow },
    { "AspectData.cpp" , Green },
    { "Client.cpp" , Green },
    { "ErrorText.cpp" , Green },
    { "main.cpp" , Green },
    { "Aspect.ddl" , Green },
    { "Aspect.TypeDef.gen.h" , Ignore },
    { "Aspect.TypeSet.gen.h" , Ignore }
   }
  };

