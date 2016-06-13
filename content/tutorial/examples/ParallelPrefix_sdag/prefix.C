#include "prefix.decl.h"

#include <stdlib.h>
#include <time.h>


/* readonly */ CProxy_Main mainProxy;
/* readonly */ int numElements;

class Main : public CBase_Main {
  public:

  // Entry point of Charm++ application
  Main(CkArgMsg* msg) {

    // There should be 0 or 1 command line arguements.
    //   If there is one, it is the number of "Hello"
    //   chares that should be created.
    numElements = 5;  // Default numElements to 5
    if (msg->argc > 1)
      numElements = atoi(msg->argv[1]);

    // We are done with msg so delete it.
    delete msg;

    // Display some info about this execution
    //   for the user.
    CkPrintf("Running \"Parallel Prefix\" with %d elements "
        "using %d processors.\n",
        numElements, CkNumPes());

    // Set the mainProxy readonly to point to a
    //   proxy for the Main chare object (this
    //   chare object).
    mainProxy = thisProxy;

    // Create the array of Hello chare objects.
    prefixArray = CProxy_Prefix::ckNew(numElements);

    appStage = 0;
    startNextStage();
  }


  // Constructor needed for chare object migration (ignore for now)
  // NOTE: This constructor does not need to appear in the ".ci" file
  Main(CkMigrateMessage* msg) { }

  void startNextStage() {
    char* str;

    // Increment the appStage counter
    appStage++;

    // Start the next stage based on the appStage counter's value
    switch (appStage) {

      // Stage 1: Initial display of values
      case 1:
        str = "Before: ";
        prefixArray[0].displayValue(str, strlen(str));
        break;

        // Stage 2: Prefix Calculation
      case 2:
        prefixArray.startPrefixCalculation();
        break;

        // Stage 3: Final display of values
      case 3:
        str = "After: ";
        prefixArray[0].displayValue(str, strlen(str));
        break;

        // Stage 4: Program has finished, exit
      case 4:
        CkExit();
        break;

        // Default: Should never be reached !!!
      default:
        CkPrintf("ERROR: default case reached in Main::startNextStage() !!!\n");
        CkExit();
        break;
    }

  }

  void valuesDisplayed() {
    startNextStage();
  }

  void checkIn() {
    startNextStage();
  }

 private:
  CProxy_Prefix prefixArray;
  int appStage;
};

class Prefix : public CBase_Prefix {
  Prefix_SDAG_CODE

  public:
    int stepCount;
    int istep;
    int targetIndex;
    int value;

    Prefix() {
      srand(time(NULL) + thisIndex);
      value = rand() % 10;      // Random positive int between 0 and 9 (inclusive)
    }


    // Constructor needed for chare object migration (ignore for now)
    // NOTE: This constructor does not need to appear in the ".ci" file
    Prefix(CkMigrateMessage *msg) { }

    void displayValue(char* prefix, int len) {

      // Make sure prefix points to something.
      const char* prefixStr = ((prefix == NULL) ? ("") : (prefix));

      // Display the value of this chare object.
      CkPrintf("%sPrefix[%d].value = %u\n", prefixStr, thisIndex, value);

      // Tell the next chare object in this chare array to also display
      //   its value.  If this is the last element in the array, then
      //   notify the main chare object that all values have been
      //   displayed.
      if (thisIndex < (numElements - 1))
        thisProxy[thisIndex + 1].displayValue(prefix, strlen(prefix));
      else
        mainProxy.valuesDisplayed();
    }



};


#include "prefix.def.h"
