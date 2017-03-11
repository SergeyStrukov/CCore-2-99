//#include <Aspect.ddl>

Aspect Data=
 {
  ".",
  {
   "",
   New,
   {
    &Dir0,
    &Dir1,
    &Dir2,
    &Dir3
   },
   {
    { "Makefile.s-files" , New },
    { "Makefile.files" , New },
    { "Makefile.cpp-files" , New },
    { "Makefile" , New }
   }
  }
 };

Dir Dir0=
  {
   "src",
   New,
   {
   },
   {
    { "main.cpp" , New },
    { "ErrorText.cpp" , New },
    { "Client.cpp" , New },
    { "AspectData.cpp" , New },
    { "Aspect.TypeSet.gen.h" , New },
    { "Aspect.TypeDef.gen.h" , New },
    { "Aspect.ddl" , New },
    { "Aspect.cpp" , New },
    { "Application.cpp" , New }
   }
  };

Dir Dir1=
  {
   "obj",
   New,
   {
   },
   {
    { "Makefile.dep" , New },
    { "main.o" , New },
    { "main.dep" , New },
    { "ErrorText.o" , New },
    { "ErrorText.dep" , New },
    { "empty" , New },
    { "Client.o" , New },
    { "Client.dep" , New },
    { "AspectData.o" , New },
    { "AspectData.dep" , New },
    { "Aspect.o" , New },
    { "Aspect.dep" , New },
    { "Application.o" , New },
    { "Application.dep" , New }
   }
  };

Dir Dir2=
  {
   "inc",
   New,
   {
   },
   {
    { "ErrorText.h" , New },
    { "Client.h" , New },
    { "AspectData.h" , New },
    { "Aspect.h" , New },
    { "Application.h" , New }
   }
  };

Dir Dir3=
  {
   "docs",
   New,
   {
   },
   {
    { "test.aspect.ddl" , New }
   }
  };

