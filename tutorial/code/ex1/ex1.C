#include "ex1.h"

Main::Main(CkArgMsg* m) {
  CkPrintf("Main chare created...\n");
  CProxy_Greeter greeter = CProxy_Greeter::ckNew();
  greeter.sayHello();
}

Greeter::Greeter() {
  CkPrintf("Greeter created...\n");
}

void Greeter::sayHello() {
  CkPrintf("Hello world!\n");
  CkExit();
}

#include "ex1.def.h"
