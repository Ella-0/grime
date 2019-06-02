enum TokenType {
  TEOF,
  TTEXT,
  TSYMBOL
};

struct Token {
  enum TokenType type;
  char *data;
};
