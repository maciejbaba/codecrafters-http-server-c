#include <stdio.h>
#include <string.h>

int main () {
  char str[] = "i am something like a string";

  printf("Received: %s\n", str);
  printf("Splitting the request into tokens...\n");

  char *token = strtok(str, " ");
  
  while (token != NULL) {
    printf("Token: %s\n", token);
    token = strtok(NULL, " ");
  }

}