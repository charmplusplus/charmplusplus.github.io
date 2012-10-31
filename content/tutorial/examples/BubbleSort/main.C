#include "main.decl.h"

#include "main.h"
#include "bubble.decl.h"

#include <stdlib.h>
#include <time.h>


/* readonly */ CProxy_Main mainProxy;
/* readonly */ int numElements;


// Entry point of Charm++ application
Main::Main(CkArgMsg* msg) {

  // Initialize member variables
  phaseCounter = 0;
  checkinCounter = 0;
  numConPhasesWithNoSwap = 0;

  // Read in any command-line arguments
  numElements = 5;
  if (msg->argc > 1)
    numElements = atoi(msg->argv[1]);

  // We are done with msg so delete it.
  delete msg;

  // Display some info about this execution
  //   for the user.
  CkPrintf("Running \"Bubble Sort\" with %d elements using %d processors.\n",
           numElements, CkNumPes()
          );

  // Set the mainProxy readonly to point to a
  //   proxy for the Main chare object (this
  //   chare object).
  mainProxy = thisProxy;

  // Create the array of Jacobi chare objects.
  bubbleArray = CProxy_Bubble::ckNew(numElements);

  // Display the array and then start the first phase
  startArrayDisplay(&Main::startNextPhase, "Before");
}


// Constructor needed for chare object migration (ignore for now)
// NOTE: This constructor does not need to appear in the ".ci" file
Main::Main(CkMigrateMessage* msg) { }


void Main::startNextPhase() {

  // Clear the appropriate member variables
  checkinCounter = 0;
  swapFlag = 0;

  // Broadcast a message to start the phase to all the bubble array elements
  bubbleArray.startPhase(phaseCounter);
}


void Main::phaseCheckin(int didSwapFlag) {

  // Combined the given didSwapFlag with the current swapFlag
  swapFlag |= didSwapFlag;

  // Increment the checkinCounter.  If all chare objects have checked-in,
  //   complete the phase.
  checkinCounter++;
  if (checkinCounter >= numElements) {

    int doneFlag = 0;

    // Check to see if everything is sorted (i.e. no swaps occured
    //   during this phase and previous phase).
    if (swapFlag == 0) {

      // Increment the number of continuous phases without swap counter
      //   and check to see if no swaps have occured for two phases.
      //   If so, the program has finished (set doneFlag so next phase
      //   doesn't start).
      numConPhasesWithNoSwap++;
      if (numConPhasesWithNoSwap >= 2) {
        // Display the array and exit (set doneFlag)
        startArrayDisplay(&Main::exit, "After");
        doneFlag = 1;
      }

    } else {

      // At least one swap occured to reset the number of
      //   continuous phases without swap to 0.
      numConPhasesWithNoSwap = 0;
    }

    // Toggle the phaseCounter (between 0 and 1) and
    //   start the next phase.  NOTE: Only do this if
    //   another phase is needed
    if (!doneFlag) {
      phaseCounter = ((phaseCounter == 0) ? (1) : (0));
      startNextPhase();
    }
  }
}


void Main::startArrayDisplay(void (Main::*cbFunc)(void), char* prefix) {

  // Set the function to execute when the display process is over
  //   (postDisplayFunc) and start displaying the values.
  postDisplayFunc = cbFunc;
  bubbleArray[0].displayValue(strlen(prefix) + 1, prefix);
}


void Main::arrayDisplayFinished() {

  // If there is a post-display function to call, call it.
  if (postDisplayFunc != NULL) (this->*postDisplayFunc)();
}


void Main::exit() {

  // Exit the program
  CkExit();
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
