#ifndef __MAIN_H__
#define __MAIN_H__


#include "bubble.decl.h"


class Main : public CBase_Main {

 private:

  /// Member Variables (Object State) ///
  int phaseCounter;
  int checkinCounter;
  int swapFlag;
  int numConPhasesWithNoSwap;
  void (Main::*postDisplayFunc)(void);
  CProxy_Bubble bubbleArray;

  /// Private Member Functions ///
  void startArrayDisplay(void (Main::*cbFunc)(void), char* prefix);
  void startNextPhase();
  void exit();

 public:

  /// Constructors ///
  Main(CkArgMsg* msg);
  Main(CkMigrateMessage* msg);

  /// Entry Methods ///
  void phaseCheckin(int didSwapFlag);
  void arrayDisplayFinished();
};


#endif //__MAIN_H__
