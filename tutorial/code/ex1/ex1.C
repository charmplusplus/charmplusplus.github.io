#include "ex1.h"

// This is the main entry point of the program.
Main::Main(CkArgMsg* m) {
  CkPrintf("Main chare created...\n");

  // Here we create a new Greeter chare and send it a message. More explanation
  // will come later in the tutorial.
  CProxy_Greeter greeter = CProxy_Greeter::ckNew();
  greeter.sayHello();
}

Greeter::Greeter() {
  CkPrintf("Greeter created...\n");
}

// This is the definition of the sayHello entry method, which is called remotely
// by the main chares constructor.
void Greeter::sayHello() {
  // CkPrintf is a charm specific version of printf.
  CkPrintf("Hello world!\n");
  // CkExit() exits the program on every single core it is running on.
  CkExit();
}

#include "ex1.def.h"
