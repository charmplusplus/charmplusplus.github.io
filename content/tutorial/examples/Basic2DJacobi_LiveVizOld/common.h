#ifndef __COMMON_H__
#define __COMMON_H__


// Set this to a non-zero value to have the program
//   display the output of the calculation during the
//   jacobi calculation.
#define DISPLAY_MATRIX  0

// Default values for some variables
#define DEFAULT_VALUE_ROWS  5
#define DEFAULT_VALUE_COLS  5
#define DEFAULT_CHARE_ROWS  3
#define DEFAULT_CHARE_COLS  3
#define DEFAULT_THRESHOLD   0.01

// Values for the amount of time the program pauses to allow the liveViz
// client to connect.
#define PAUSE_TIME        15.0
#define PAUSE_PRINT_TIME   3.0

#endif //__COMMON_H__
