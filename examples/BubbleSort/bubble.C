#include "bubble.decl.h"

#include "bubble.h"
#include "main.decl.h"

#include <stdlib.h>


extern /* readonly */ CProxy_Main mainProxy;
extern /* readonly */ int numElements;


Bubble::Bubble() {

  // Init local variables
  myValue = rand() % 10;
}


// Constructor needed for chare object migration (ignore for now)
// NOTE: This constructor does not need to appear in the ".ci" file
Bubble::Bubble(CkMigrateMessage *msg) { }


void Bubble::startPhase(int phaseCounter) {

  // Depending on the phase (0 or 1), send a message to the chare object
  //   at 'thisIndex + 1' to request a swap.  Otherwise, send a message
  //   back to the main chare (since this object isn't initiating anything).
  int targetIndex = thisIndex + 1;
  if ((targetIndex < numElements) && ((thisIndex % 2) == phaseCounter)) {
    thisProxy[targetIndex].requestSwap(myValue);
  } else {
    mainProxy.phaseCheckin(0); // Send a '0' since no swap initiated
  }
}


void Bubble::requestSwap(int neighborValue) {

  // Compare the neighbor's value with own.  If the neighbor's value is
  //   greater than own, then swap the values (since swaps are always
  //   requested by lower indexes to higher indexes).  Otherwise,
  //   deny the swap.
  int neighborIndex = thisIndex - 1;
  if (neighborValue > myValue) {
    thisProxy[neighborIndex].acceptSwap(myValue);
    myValue = neighborValue;
  } else {
    thisProxy[neighborIndex].denySwap();
  }
}


void Bubble::acceptSwap(int neighborValue) {

  // Replace myValue with neighborValue
  myValue = neighborValue;

  // Check-in with the main chare object
  mainProxy.phaseCheckin(1);  // Send a '1' since a swap occured
}


void Bubble::denySwap() {

  // Check-in with the main chare object
  mainProxy.phaseCheckin(0);  // Send a '0' since no swap occured
}


void Bubble::displayValue(int prefixLen, char* prefix) {

  // Display the value
  CkPrintf("%s%sbubble[%d]::myValue = %d\n",
           ((prefix == NULL) ? ("") : (prefix)), ((prefix == NULL) ? ("") : (" ")),
           thisIndex, myValue
          );

  // Tell the next or main if last
  if (thisIndex < numElements - 1) {
    thisProxy[thisIndex + 1].displayValue(prefixLen, prefix);
  } else {
    mainProxy.arrayDisplayFinished();
  }
}


#include "bubble.def.h"
