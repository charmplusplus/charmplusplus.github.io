#ifndef __BUBBLE_H__
#define __BUBBLE_H__


class Bubble : public CBase_Bubble {

 private:

  /// Member Variables (Object State) ///
  int myValue;

  /// Member Functions (private) ///

 public:

  /// Constructors ///
  Bubble();
  Bubble(CkMigrateMessage *msg);

  /// Entry Methods ///
  void startPhase(int phaseCounter);
  void requestSwap(int neighborValue);
  void acceptSwap(int neighborValue);
  void denySwap();
  void displayValue(int prefixLen, char* prefix);
};


#endif //__BUBBLE_H__
