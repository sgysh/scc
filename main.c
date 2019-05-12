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
  parse(tokens);

  // Print the prologue
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  printf("  push rbp\n");
  printf("  mov rbp, rsp\n");
  printf("  sub rsp, 208\n");

  for (int i = 0; code[i]; i++) {
    gen(code[i]);
    printf("  pop rax\n");
  }

  printf("  mov rsp, rbp\n");
  printf("  pop rbp\n");
  printf("  ret\n");
  return 0;
}
