#include "prefix.decl.h"

#include "prefix.h"
#include "main.decl.h"

#include <stdlib.h>


extern /* readonly */ CProxy_Main mainProxy;
extern /* readonly */ int numElements;


Prefix::Prefix() {

  // Initialize the local variables
  stepCount = 0;            // First step (0)
  bufferedMessages = NULL;  // Empty
  value = rand() % 10;      // Random positive int between 0 and 9 (inclusive)
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


void Prefix::startPrefixCalculation() {

  // Send message for step 0
  sendMessageForStep(0);

  // Check to see if this chare object has finished receiving values
  checkFinished();
}


void Prefix::passValue(unsigned int incomingValue, int stepNum) {

  // Check to see if this message has arrived out of order.  If so,
  //   buffer it.  Otherwise, if it is in order, begin processing.
  if (stepNum != stepCount) {  // Out-of-Order Message Detected

    // Create a buffer to hold the message data and link it into
    //   the bufferedMessages linked list.  NOTE:  Hopefully this
    //   list will not grown very much in practice so don't worry
    //   about the ording of entries in the list.
    MsgBuffer* msgBuf = new MsgBuffer;
    msgBuf->stepNum = stepNum;
    msgBuf->value = incomingValue;
    msgBuf->prev = NULL;
    if (bufferedMessages == NULL) {
      msgBuf->next = NULL;
    } else {
      msgBuf->next = bufferedMessages;
      bufferedMessages->prev = msgBuf;
    }
    bufferedMessages = msgBuf;

  } else {

    // Process the message that just arrived
    value += incomingValue;
    stepCount++;
    // NOTE: By just sending here, an extra message might be sent (i.e
    //   this chare sends a message for an extra step).  However, this
    //   will target an element past the end of the array and the check
    //   in sendMessageForStep() will catch it so there is no need to
    //   check if a message should or should not be sent here.  Same
    //   holds true for processing buffered messages below.
    sendMessageForStep(stepCount);

    // Process any bufferedMessages that need to be processed now
    int msgProcessedFlag = 1;
    while (msgProcessedFlag) {

      // Clear the msgProcessedFlag
      msgProcessedFlag = 0;

      // Seached the bufferedMessages for a stepNum == stepCount
      MsgBuffer* msg = bufferedMessages;
      while (msg != NULL) {

        if (msg->stepNum == stepCount) {

          // Remove msg from the linked list of bufferedMessages
          if (msg->prev != NULL) msg->prev->next = msg->next;
          if (msg->next != NULL) msg->next->prev = msg->prev;
          bufferedMessages = ((msg->prev == NULL) ? (msg->next) : (msg->prev));

          // Process msg
          value += msg->value;
          stepCount++;
          sendMessageForStep(stepCount);

          // No long need the message buffer, so delete it
          delete msg;

          // Set the msgProcessedFlag
          msgProcessedFlag = 1;

          // Found it, so stop searching the bufferedMessages list
          break;

	} else {

          // Move onto the next buffered message
          msg = msg->next;
	}
      }
    }

    // Check to see if this chare object is finished receiving values
    checkFinished();
  }
}


void Prefix::checkFinished() {

  // Check the stepCount, if this chare object has received all of the
  //   passValue() messages that it is going to receive, send the remaining
  //   passValue() messages.
  if ((1 << stepCount) > thisIndex) {

    // Increment stepCount (the send for this step should have already
    //   taken place so begin by checking the next step
    stepCount++;

    // Send the remaining passValue() messages
    while ((1 << stepCount) < numElements) {

      // Send the message for this step
      sendMessageForStep(stepCount);

      // Move onto the next step
      stepCount++;
    }

    // Notify the main chare that this chare object has completed
    mainProxy.checkIn();
  }  
}


void Prefix::sendMessageForStep(int stepNum) {

  int targetIndex = thisIndex + (1 << stepNum);
  if (targetIndex < numElements) {
    thisProxy[targetIndex].passValue(value, stepNum);
  }
}


#include "prefix.def.h"
