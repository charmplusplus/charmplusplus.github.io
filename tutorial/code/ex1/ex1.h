// Each module generates some declarations in <module name>.decl.h. These must
// be included before declarations of the chare classes.
#include "ex1.decl.h"

// Every class declaration of a chare must inherit from the generated base
// class for that chare, CBase_<chare name>.
class Main : public CBase_Main {
 public:
  Main(CkArgMsg*);
};

class Greeter : public CBase_Greeter {
 public:
  Greeter();
  void sayHello();
};
