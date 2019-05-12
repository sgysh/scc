#include "scc.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: scc <code>\n");
    return 1;
  }

  if (!strcmp(argv[1], "-test")) {
    runtest();
    return 0;
  }

  Vector *tokens = tokenize(argv[1]);
  Node *node = parse(tokens);

  // Print the prologue
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  gen(node);

  printf("  pop rax\n");
  printf("  ret\n");
  return 0;
}
