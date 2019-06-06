int isInteger(struct Token token) {
  for (char *c = token.data; c; c++) {
    if ('0' > *c || *c > '9') {
      if (*c != '\0') {
        return 0;
      } else {
        return 1;
      }
    }
  }
}
