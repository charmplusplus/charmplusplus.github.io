#ifndef __PREFIX_H__
#define __PREFIX_H__


class Prefix : public CBase_Prefix {

 private:

  /// Member Variables (Object State) ///
  unsigned int value;  // Chare object's value

 public:

  /// Constructors ///
  Prefix();
  Prefix(CkMigrateMessage *msg);

  /// Entry Methods ///
  void displayValue(char* prefix, int len);
  void doStep(int stepNum);
  void passValue(unsigned int incomingValue);

};


#endif //__HELLO_H__
