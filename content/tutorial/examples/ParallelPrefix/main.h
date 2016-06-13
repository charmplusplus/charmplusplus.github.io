#ifndef __MAIN_H__
#define __MAIN_H__


#include "prefix.decl.h"


class Main : public CBase_Main {

 private:

  /// Member Variables (Object State) ///
  int appStage;     // State the application is in
  int stepCount;    // Number of steps completed
  int checkInCount; // Number of objects finished with step
  CProxy_Prefix prefixArray;  // Prefix array proxy

  /// Private Member Functions ///
  void startNextStage();

 public:

  /// Constructors ///
  Main(CkArgMsg* msg);
  Main(CkMigrateMessage* msg);

  /// Entry Methods ///
  void valuesDisplayed();
  void stepCheckIn();

};


#endif //__MAIN_H__
