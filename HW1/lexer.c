#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "compiler.h"

char *tokens[MAXTOKEN];
int tokenTop = 0;
int tokenIdx = 0;

void tokenize(char *code) {
  char *p = strtok(code, " \t\r\n");
  while (p != NULL) {
    
    if (strchr("(){};=+-*/<>!&|", *p) && strlen(p) > 1) {
      for (int i = 0; i < strlen(p); i++) {
        char *t = (char *)malloc(2);
        t[0] = p[i];
        t[1] = '\0';
        tokens[tokenTop++] = t;
      }
    } else {
      tokens[tokenTop++] = strdup(p);
    }
    p = strtok(NULL, " \t\r\n");
  }
}
