#include "main.decl.h"

#include "main.h"
#include "prefix.decl.h"

#include <stdlib.h>
#include <time.h>


/* readonly */ CProxy_Main mainProxy;
/* readonly */ int numElements;


// Entry point of Charm++ application
Main::Main(CkArgMsg* msg) {

  // Initialize the local member variables
  appStage = 0;

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

  // Start the next stage
  startNextStage();
}


// Constructor needed for chare object migration (ignore for now)
// NOTE: This constructor does not need to appear in the ".ci" file
Main::Main(CkMigrateMessage* msg) { }


void Main::startNextStage() {

  // Temp variable used for invoking Prefix::displayValue()
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
      checkInCount = 0;
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


void Main::valuesDisplayed() {

  // Displaying the values of the array is a stage in the calculation
  //   so start the next stage.
  startNextStage();
}


void Main::checkIn() {

  // Increment the count of chare objects that have checked in.  Once all
  //   of them have checked in, move on to the next stage.
  checkInCount++;
  if (checkInCount >= numElements) {
    startNextStage();
  }
}


// Because this function is declared in a ".ci" file (main.ci in this
//   case) with the "initproc" keyword, it will be called once at startup
//   (before Main::Main()) on each physical processor.  In this case,
//   it is being used to seed the random number generator on each processor.
void processorStartupFunc() {

  // Seed rand() with a different value for each processor
  srand(time(NULL) + CkMyPe());
}


#include "main.def.h"
