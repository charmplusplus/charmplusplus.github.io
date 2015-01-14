// Each module generates some declarations in <module name>.decl.h. These must
// be included before declarations of the chare classes.
#include "ex1.decl.h"

// Every class declaration of a chare must inherit from the generated base
// class for that chare, CBase_<chare name>.
class Main : public CBase_Main {
  public:
    // This is the main entry point of the program.
    Main(CkArgMsg* m) {
      CkPrintf("Main chare created...\n");
      
      // Here we create a new Greeter chare and send it a message. More explanation
      // will come later in the tutorial.
      CProxy_Greeter greeter = CProxy_Greeter::ckNew();
      greeter.sayHello();
    }
};

class Greeter : public CBase_Greeter {
  public:
    Greeter() {
      CkPrintf("Greeter created...\n");
    }
  
    // This is the definition of the sayHello entry method, which is called remotely
    // by the main chare's constructor.
    void sayHello() {
      // CkPrintf is a Charm++ specific version of printf.
      CkPrintf("Hello world!\n");
      // CkExit() exits the program on every single core it is running on.
      CkExit();
    }
};

#include "ex1.def.h"
