#include "compiler.h"

char code[TMAX];
char strTable[SMAX], *strTableEnd = strTable;
char *tokens[SMAX];
char tokenTop = 0, tokenIdx = 0;
int types[SMAX];
char *typeName[] = { "Id", "Int", "Keyword", "Literal", "Char", "Op" };

int isKeyword(char *s) {
  return strcmp(s, "if") == 0 || strcmp(s, "else") == 0 ||
         strcmp(s, "while") == 0 || strcmp(s, "do") == 0;
}

void lex(char *text) {
  char *p = text;
  while (*p) {
    while (*p == ' ' || *p == '\n' || *p == '\t' || *p == '\r') p++;
    if (*p == '\0') break;

    if (isDigit(*p)) {
      tokens[tokenTop] = strTableEnd;
      types[tokenTop++] = Int;
      while (isDigit(*p)) *strTableEnd++ = *p++;
      *strTableEnd++ = '\0';
    } else if (isAlpha(*p)) {
      tokens[tokenTop] = strTableEnd;
      while (isAlpha(*p) || isDigit(*p)) *strTableEnd++ = *p++;
      *strTableEnd++ = '\0';
      types[tokenTop] = isKeyword(tokens[tokenTop]) ? Keyword : Id;
      tokenTop++;
    } else if (strchr("=<>!&|", *p)) {
      tokens[tokenTop] = strTableEnd;
      *strTableEnd++ = *p;
      if (*(p + 1) == '=' || (*p == '&' && *(p + 1) == '&') || (*p == '|' && *(p + 1) == '|')) {
        *strTableEnd++ = *(p + 1);
        p++;
      }
      *strTableEnd++ = '\0';
      types[tokenTop++] = Op;
      p++;
    } else if (strchr("+-*/;(){}", *p)) {
      tokens[tokenTop] = strTableEnd;
      *strTableEnd++ = *p++;
      *strTableEnd++ = '\0';
      types[tokenTop++] = Op;
    } else {
      printf("Unknown character: %c\n", *p);
      p++;
    }
  }
}
