#include "token.c"
#include <string.h>

char* strAppend(char* str, char a) {
  size_t len = strlen(str);
  char *out = malloc((len + 2) * sizeof(char));
  strcpy(out, str);
  out[len] = a;
  out[len + 1] = '\0';
  return out;
}

void tryReduction(struct Token *tokens, struct Token *building, enum TokenType type, int *offset) {
  if (building->type != type && building->type != TEOF) {
    printf("creating token: %s of type: %d\n", building->data, building->type);
    tokens[*offset] = *building;
    building->type = type;
    building->data = "";
    (*offset)++;
  }
}

struct Token *lex(char *src) {
  struct Token *tokens = malloc(strlen(src) * sizeof(struct Token *));
  int offset = 0;
  struct Token building;
  building.type = TEOF;
  building.data = "";
  for(; *src; src++) {
    switch (*src) {
      case '{':
      case '}':
      case '(':
      case ')':
      case '[':
      case ']':
        tryReduction(tokens, &building, TEOF, &offset);
        struct Token token;
        token.type = TSYMBOL;
        token.data = strAppend("", *src);
        tryReduction(tokens, &token, TEOF, &offset);
        break;

      case '+':
      case '-':
      case '*':
      case '/':
      case '=':
      case '>':
      case '<':
      case ':':
        tryReduction(tokens, &building, TSYMBOL, &offset);
        building.type = TSYMBOL;
        building.data = strAppend(building.data, *src);
        break;

      case '\n':
      case '\t':
      case ' ':
        tryReduction(tokens, &building, TEOF, &offset);
        break;

      default:
        tryReduction(tokens, &building, TTEXT, &offset);
        building.type = TTEXT;
        building.data = strAppend(building.data, *src);
        break;
    }
  }
  tryReduction(tokens, &building, TEOF, &offset);
  tokens[offset] = building;
  return tokens;
}
