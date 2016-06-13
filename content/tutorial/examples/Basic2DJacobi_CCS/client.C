#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "ccs-client.h"
#include "jacobi-CS.h"

int main (int argc, char **argv) {
  if (argc < 3) {
    printf("Usage: %s <hostname> <port>",argv[0]);
    return 1;
  }

  std::vector<char*> vec;

  // Create a CcsServer and connect to the given hostname and port
  CcsServer server;
  char *host=argv[1];
  int port=atoi(argv[2]);

  CcsConnect (&server, host, port, NULL);

  while (true) {

    // Read from the user the list of values (s)he wants to change.
    // The format is "x,y = v" where x and y are the two coordinates inside the
    // global matrix, and v is the new value. An empty line will terminate the
    // input
    char *line = readline("Enter new value with format \"x,y = v\": ");
    while (line != NULL && strlen(line) > 0) {
      vec.push_back(line);
      line = readline("Next: ");
    }

    // If there is no input at all the program will quit after sending an "exit"
    // message to the server, otherwise it will construct a message to send to
    // the "changeValue" CCS handler.
    if (vec.size() > 0) {

      // Allocate the necessary space for the entire message
      int cmdLen = sizeof(ChangeRequest) + (vec.size()-1) * sizeof(SingleValue);
      char *command = (char*)malloc(cmdLen);

      // Parse each line read from the user (and temporarely stored into the
      // std:vector) into the ChangeRequest structure
      ChangeRequest *req = (ChangeRequest *)command;
      req->numEl = vec.size();
      printf("Processing request... %d entries: {\n",req->numEl);
      for (int i=0; i<req->numEl; ++i) {
        SingleValue *sv = &(req->values[i]);
        sscanf(vec[i], "%d,%d = %lf", &sv->x, &sv->y, &sv->newValue);
        free(vec[i]);
        printf("\t%d,%d -> %lf\n", sv->x, sv->y, sv->newValue);
      }
      printf("}\n");
      vec.clear();

      // Send the request to the server at the CCS handler "changeValue"
      CcsSendRequest(&server, "changeValue", 0, cmdLen, command);
      free(command);

      // Receive the reply from the server and print it
      int replySize;
      ChangeRequest *reply;
      CcsRecvResponseMsg(&server, &replySize, (void**)&reply, 120);
      if (reply->numEl == 0) {
        printf("Request process successfully\n");
      } else {
        printf("Request returned %d errors:\n", reply->numEl);
        for (int i=0; i<reply->numEl; ++i) {
          printf("\tinvalid location %d, %d\n", reply->values[i].x, reply->values[i].y);
        }
      }

    } else {
      // If no input is given, send the server a request to the CCS handler
      // "exit"
      CcsSendRequest(&server, "exit", 0, 0, NULL);
      break;
    }
  }
}
