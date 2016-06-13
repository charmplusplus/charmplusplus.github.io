#include "prefix.decl.h"

#include "prefix.h"
#include "main.decl.h"

#include <stdlib.h>


extern /* readonly */ CProxy_Main mainProxy;
extern /* readonly */ int numElements;


Prefix::Prefix() {

  // Initialize the value member variable to some random value
  value = rand() % 10;
}


// Constructor needed for chare object migration (ignore for now)
// NOTE: This constructor does not need to appear in the ".ci" file
Prefix::Prefix(CkMigrateMessage *msg) { }


void Prefix::displayValue(char* prefix, int len) {

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


void Prefix::doStep(int stepNum) {

  // Start by passing own value to neighbor (Note: stepNum is
  //   zero-based).  Note: If this chare object is not passing
  //   a value, then there is no receiving chare object to
  //   check in with the main chare object.  Therefor, this
  //   chare object should do it.  This will mean that some
  //   chare objects will check in twice and some will not check
  //   in at all.  However, the total number of check-ins will
  //   still sum to numElements.
  int targetIndex = thisIndex + (1 << stepNum);
  if (targetIndex < numElements)
    thisProxy[targetIndex].passValue(value);
  else
    mainProxy.stepCheckIn();
}

void Prefix::passValue(unsigned int incomingValue) {

  // Add the incomingValue to own value
  value += incomingValue;

  // Check in with the main chare
  mainProxy.stepCheckIn();
}


#include "prefix.def.h"
