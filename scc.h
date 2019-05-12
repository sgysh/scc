#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  void **data;
  int capacity;
  int len;
} Vector;

enum {
  TK_NUM = 256, // Number literal
  TK_EQ,
  TK_NE,
  TK_LE,
  TK_GE,
  TK_EOF,       // End marker
};

// Token type
typedef struct {
  int ty;      // Token type
  int val;     // Number literal
  char *input; // Token string (for error reporting)
} Token;

enum {
  ND_NUM = 256,  // Number literal
  ND_EQ,
  ND_NE,
  ND_LE,
};

typedef struct Node {
  int ty;           // Node type
  struct Node *lhs; // left-hand side
  struct Node *rhs; // right-hand side
  int val;          // Number literal
} Node;

Vector *new_vector();
void vec_push(Vector *vec, void *elem);
void error(char *fmt, ...);
Node *parse(Vector *v);
void gen(Node *node);
void runtest();
Vector *tokenize(char *p);