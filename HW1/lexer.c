#include "compiler.h"

#define LMAX 100

char *typeName[6] = {"Id", "Int", "Keyword", "Literal", "Char", "Op"};
char code[TMAX], *p;
char strTable[TMAX], *strTableEnd = strTable;
char *tokens[TMAX];
char tokenTop = 0, tokenIdx = 0;
char token[LMAX];

int isKeyword(char *s) {
  return strcmp(s, "if") == 0 || strcmp(s, "else") == 0 ||
         strcmp(s, "while") == 0 || strcmp(s, "do") == 0;
}

char *scan() {
  while (isspace(*p)) p++;
  if (*p == '\0') return NULL;

  char *start = p;
  int type;

  if (*p == '"') { 
    p++;
    while (*p != '"' && *p != '\0') p++;
    if (*p == '"') p++;
    type = Literal;
  } else if (isDigit(*p)) { 
    while (isDigit(*p)) p++;
    type = Int;
  } else if (isAlpha(*p) || *p == '_') { 
    while (isAlpha(*p) || isDigit(*p) || *p == '_') p++;
    type = Id;
  } else if (strchr("+-*/%&|<>!=", *p)) { 
    char c = *p++;
    if (*p == '=') p++; // eg. +=, ==, !=, <=
    else if ((c == '+' || c == '-' || c == '&' || c == '|') && *p == c) p++; // eg. ++, --, &&, ||
    type = Op;
  } else { 
    p++;
    type = Char;
  }

  int len = p - start;
  strncpy(token, start, len);
  token[len] = '\0';
  return token;
}

void lex(char *text) {
  printf("========== lex ==============\n");
  p = text;
  tokenTop = 0;
  strTableEnd = strTable;

  while (1) {
    char *tok = scan();
    if (tok == NULL) break;

    strcpy(strTableEnd, tok);
    tokens[tokenTop++] = strTableEnd;
    strTableEnd += strlen(tok) + 1;

    printf("token=%s\n", tok);
  }
}
