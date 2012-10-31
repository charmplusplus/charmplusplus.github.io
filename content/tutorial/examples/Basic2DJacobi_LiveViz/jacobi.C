#include "liveViz.h"

#include "jacobi.decl.h"

#include "jacobi.h"
#include "main.decl.h"
#include "common.h"

#include <stdlib.h>


extern /* readonly */ CProxy_Main mainProxy;
extern /* readonly */ int valRows;
extern /* readonly */ int valCols;
extern /* readonly */ int chareRows;
extern /* readonly */ int chareCols;


Jacobi::Jacobi() {

  // Calculate the number of elements the values matrix has to have.
  // NOTE:  There are two extra rows and two extra columns to hold the incoming
  //   ghost values from neighboring chare objects.
  register int numElements = (valRows + 2) * (valCols + 2);

  // Create the values
  values = new double[numElements];

  for (int i = 0; i < numElements; i++)
    values[i] = 0.5;

  if (thisIndex.y != 0 && values != NULL) {
    if ((thisIndex.x + thisIndex.y) & 0x01) {
      for (int j = 0; j < valCols; j++)
        values[(valCols + 2) + 1 + j] = 1.0;
    } else {
      for (int j = 0; j < valCols; j++)
        values[(valCols + 2) + 1 + j] = 0.0;
    }
  }

  // Set the calculation counter
  calcCounterTarget = 1; // 1 for sending own ghosts to neighbors (must be done before calculation starts)
  if (thisIndex.x > 0) calcCounterTarget++;  // expect a ghost from the west
  if (thisIndex.x < chareCols - 1) calcCounterTarget++;  // expect a ghost from the east
  if (thisIndex.y > 0) calcCounterTarget++;  // expect a ghost from the north
  if (thisIndex.y < chareRows - 1) calcCounterTarget++;  // expect a ghost from the south
  calcCounter = 0;  

  // Create a temp buffer for general use
  tmpBuffer = new double[numElements];

  // If needed, create the buffer for displaying the matrix
  #if DISPLAY_MATRIX != 0
    int displayBufferSize = (18 * (valRows + 4) * (valCols + 4)) + 2048;
    displayBuffer = new char[displayBufferSize];
  #endif
}


// Constructor needed for chare object migration (ignore for now)
// NOTE: This constructor does not need to appear in the ".ci" file
Jacobi::Jacobi(CkMigrateMessage *msg) { }


Jacobi::~Jacobi() {
  if (values != NULL) delete [] values;
  if (tmpBuffer != NULL) delete [] tmpBuffer;
  #if DISPLAY_MATRIX != 0
    if (displayBuffer != NULL) delete [] displayBuffer;
  #endif
}


void Jacobi::startStep() {

  // Send ghost data to neighbors

  // First column to the "thisIndex.x - 1"
  if (thisIndex.x > 0) {
    for (int i = 0; i < valRows; i++)
      tmpBuffer[i] = values[1 + ((valCols + 2) * (i + 1))];
    thisProxy[CkArrayIndex2D(thisIndex.x - 1, thisIndex.y)].eastGhost(valRows, tmpBuffer);
  }

  // Last column to the "thisIndex.x + 1"
  if (thisIndex.x < chareCols - 1) {
    for (int i = 0; i < valRows; i++)
      tmpBuffer[i] = values[valCols + ((valCols + 2) * (i + 1))];
    thisProxy[CkArrayIndex2D(thisIndex.x + 1, thisIndex.y)].westGhost(valRows, tmpBuffer);
  }

  // First row to the "thisIndex.y - 1"
  if (thisIndex.y > 0) {
    double* bufferPtr = &(values[(valCols + 2) + 1]);
    thisProxy[CkArrayIndex2D(thisIndex.x, thisIndex.y - 1)].southGhost(valCols, bufferPtr);
  }
 
  // Last row to the "thisIndex.y + 1"
  if (thisIndex.y < chareRows - 1) {
    double* bufferPtr = &(values[((valCols + 2) * valCols) + 1]);
    thisProxy[CkArrayIndex2D(thisIndex.x, thisIndex.y + 1)].northGhost(valCols, bufferPtr);
  }

  // See if this chare object is now ready to perform the calculation
  attemptCalculation();
}


void Jacobi::eastGhost(int len, double* vals) {

  // Copy the data into the values matrix
  for (int i = 0; i < valRows; i++)
    values[((valCols + 2) * (i + 1)) + valCols + 1] = vals[i];

  // See if this chare object is now ready to perform the calculation
  attemptCalculation();
}


void Jacobi::westGhost(int len, double* vals) {

  // Copy the data into the values matrix
  for (int i = 0; i < valRows; i++)
    values[((valCols + 2) * (i + 1))] = vals[i];

  // See if this chare object is now ready to perform the calculation
  attemptCalculation();
}


void Jacobi::northGhost(int len, double* vals) {

  // Copy the data into the values matrix
  memcpy(&(values[1]), vals, sizeof(double) * len);

  // See if this chare object is now ready to perform the calculation
  attemptCalculation();
}


void Jacobi::southGhost(int len, double* vals) {

  // Copy the data into the values matrix
  memcpy(&(values[((valCols + 2) * (valRows + 1)) + 1]), vals, sizeof(double) * len);

  // See if this chare object is now ready to perform the calculation
  attemptCalculation();
}


void Jacobi::attemptCalculation() {

  // Increment the calculation counter and test it verse the target value
  calcCounter++;
  if (calcCounter >= calcCounterTarget) {

    // Reset the calculation counter
    calcCounter = 0;

    // Perform the calculation
    double maxDiff = doStep();

    // Checkin with the main chare object
    mainProxy.stepCheckin(maxDiff);
  }

}


int displayMatrix(double* matrix, int rows, int cols, char* prefixStr, char* displayBuffer) {

  char* buffer = displayBuffer;

  // Start matrix
  if (prefixStr != NULL)
    buffer += sprintf(buffer, "%s :: matrix = {\n              ", ((prefixStr == NULL) ? ("") : (prefixStr)));
  else
    buffer += sprintf(buffer, "matrix = {\n              ");

  // First row + upper line of box
  for (int j = 1; j < cols - 1; j++)
    buffer += sprintf(buffer, "%1.6lf ", matrix[j]);
  buffer += sprintf(buffer, "\n            /-");
  for (int j = 1; j < cols - 1; j++)
    buffer += sprintf(buffer, "---------");
  buffer += sprintf(buffer, "\\\n");

  // Internal rows
  for (int i = 1; i < rows - 1; i++) {
    buffer += sprintf(buffer, "   ");
    for (int j = 0; j < cols; j++) {
      if (j == 1) buffer += sprintf(buffer, "| ");
      buffer += sprintf(buffer, "%1.6lf ", matrix[(i * cols) + j]);
      if (j == cols - 2) buffer += sprintf(buffer, "| ");
    }
    buffer += sprintf(buffer, "\n");
  }

  // lower line of box + last row
  buffer += sprintf(buffer, "            \\-");
  for (int j = 1; j < cols - 1; j++)
    buffer += sprintf(buffer, "---------");
  buffer += sprintf(buffer, "/\n              ");
  for (int j = 1; j < cols - 1; j++)
    buffer += sprintf(buffer, "%1.6lf ", matrix[j]);
  buffer += sprintf(buffer, "\n");

  // End matrix
  buffer += sprintf(buffer, "}\n");

  return (buffer - displayBuffer);
}


double Jacobi::doStep() {

  // Zero out the tmpBuffer
  memset(tmpBuffer, 0, sizeof(double) * ((valCols + 2) * (valRows + 2)));

  // Display the matrix before the calculation
  #if DISPLAY_MATRIX != 0
    char* buffer = displayBuffer;
    displayBuffer[0] = '\0';
    int displayLen = 0;
    buffer += sprintf(buffer, "Jacobi[%d,%d]::doStep() - Called...\n", thisIndex.x, thisIndex.y);
    buffer += displayMatrix(values, valRows + 2, valCols + 2, "Before", buffer);
  #endif

  register int startX = 1;
  if (thisIndex.y != 0) {
    startX = valCols;
    if ((thisIndex.x + thisIndex.y) & 0x01) {
      for (int j = 0; j < valCols; j++)
        tmpBuffer[(valCols + 2) + 1 + j] = 1.0;
    } else {
      for (int j = 0; j < valCols; j++)
        tmpBuffer[(valCols + 2) + 1 + j] = 0.0;
    }
  }

  // Do the calculation
  double maxDiff = 0.0;
  for (int y = 1; y <= valRows; y++) {
    for (int x = startX; x <= valCols; x++) {

      // Calculate the index of the element
      int index = (y * (valCols + 2)) + x;

      // Calculate the new value for this element
      double newVal = values[index];
      newVal += values[index - 1];
      newVal += values[index + 1];
      newVal += values[index - (valCols + 2)];
      newVal += values[index + (valCols + 2)];
      tmpBuffer[index] = newVal / 5.0;

      // Calculate the difference for the element and increase
      //   maxDiff if need be.
      double diff = tmpBuffer[index] - values[index];
      if (diff < 0) diff *= -1.0;
      maxDiff = ((maxDiff > diff) ? (maxDiff) : (diff));
    }
    startX = 1;
  }

  // Swap the buffers
  double* tmpPtr = values;
  values = tmpBuffer;
  tmpBuffer = tmpPtr;

  // Display the matrix after the calculation
  #if DISPLAY_MATRIX != 0
    buffer += displayMatrix(values, valRows + 2, valCols + 2, "After", buffer);
    buffer += sprintf(buffer, "local maxDiff = %lf\n", maxDiff);
    CkPrintf(displayBuffer);
  #endif

  return maxDiff;
}


void Jacobi::liveVizFunc(liveVizRequestMsg* msg) {

  // Fill in the tmpBuffer with the image data
  typedef struct __rbg_color { byte r, g, b; } RBGColor;
  RBGColor* imgPtr = (RBGColor*)tmpBuffer;

  CkAssert(sizeof(RBGColor) == 3);

  // Copy in the data from the values array
  for (int x = 0; x < valCols; x++) {
    for (int y = 0; y < valRows; y++) {

      register int valIndex = ((y + 1) * (valCols + 2)) + (x + 1);
      register int imgIndex = (y * valCols) + x;

      register float fVal = values[valIndex];

      RBGColor* c = &(imgPtr[imgIndex]);
      c->r = ((fVal > 0.5) ? ((int)(255.0 * ((fVal - 0.5) * 2.0))) : (0));
      c->g = 0;
      c->b = ((fVal < 0.5) ? ((int)(128.0 * ((0.5 - fVal) * 2.0))) : (0));

    }
  }
  
  // Calculate the offset of this chare's portion of the image in
  //   the overall image
  register int xOffset = thisIndex.x * valCols;
  register int yOffset = thisIndex.y * valRows;

  // Deposit this chare's portion of the image to liveViz
  liveVizDeposit(msg, xOffset, yOffset, valCols, valRows, (byte*)imgPtr, this);
}


#include "jacobi.def.h"
