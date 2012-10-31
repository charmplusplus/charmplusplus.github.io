#ifndef __PREFIX_H__
#define __PREFIX_H__


typedef struct __msg_buffer {
  int stepNum;
  unsigned int value;
  __msg_buffer* next;
  __msg_buffer* prev;
} MsgBuffer;


class Prefix : public CBase_Prefix {

 private:

  /// Member Variables (Object State) ///
  MsgBuffer* bufferedMessages;
  int stepCount;
  unsigned int value;  // Chare object's value

  /// Member Functions (private) ///
  void checkFinished();
  void sendMessageForStep(int stepNum);

 public:

  /// Constructors ///
  Prefix();
  Prefix(CkMigrateMessage *msg);

  /// Entry Methods ///
  void displayValue(char* prefix, int len);
  void startPrefixCalculation();
  void passValue(unsigned int incomingValue, int stepNum);

};


#endif //__HELLO_H__
