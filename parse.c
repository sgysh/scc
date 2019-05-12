#include "scc.h"

static Vector *tokens;
Node *code[100];

Node *assign();

Token *add_token(Vector *v, int ty, char *input) {
  Token *t = malloc(sizeof(Token));
  t->ty = ty;
  t->input = input;
  vec_push(v, t);
  return t;
}

Vector *tokenize(char *p) {
  Vector *v = new_vector();
  int i = 0;
  while (*p) {
    // Skip whitespace
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (!strncmp(p, "<=", 2)) {
      add_token(v, TK_LE, p);
      i++;
      p += 2;
      continue;
    }

    if (!strncmp(p, ">=", 2)) {
      add_token(v, TK_GE, p);
      i++;
      p += 2;
      continue;
    }

    if (!strncmp(p, "==", 2)) {
      add_token(v, TK_EQ, p);
      i++;
      p += 2;
      continue;
    }

    if (!strncmp(p, "!=", 2)) {
      add_token(v, TK_NE, p);
      i++;
      p += 2;
      continue;
    }

    if (strchr("+-*/()<>;=", *p)) {
      add_token(v, *p, p);
      i++;
      p++;
      continue;
    }

    // Number
    if (isdigit(*p)) {
      Token *t = add_token(v, TK_NUM, p);
      t->val = strtol(p, &p, 10);
      i++;
      continue;
    }

    if ('a' <= *p && *p <= 'z') {
      add_token(v, TK_IDENT, p);
      i++;
      p++;
      continue;
    }

    error("cannot tokenize: %s", p);
    exit(1);
  }

  add_token(v, TK_EOF, p);
  return v;
}

int pos = 0;

int consume(int ty) {
  Token *t = tokens->data[pos];
  if (t->ty != ty)
    return 0;
  pos++;
  return 1;
}

Node *new_node(int ty, Node *lhs, Node *rhs) {
  Node *node = malloc(sizeof(Node));
  node->ty = ty;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = malloc(sizeof(Node));
  node->ty = ND_NUM;
  node->val = val;
  return node;
}

Node *term() {
  Token *t = tokens->data[pos];
  if (consume('(')) {
    Node *node = assign();
    if (!consume(')'))
      error(") expected");
    return node;
  }

  if (t->ty == TK_NUM) {
    pos++;
    return new_node_num(t->val);
  }

  if (t->ty == TK_IDENT) {
    Node *node = malloc(sizeof(Node));
    node->ty = ND_IDENT;
    node->name = *t->input;
    pos++;
    return node;
  }

  error("unexpected token: %s", t->input);
}

Node *unary() {
  if (consume('+'))
    return term();
  if (consume('-'))
    return new_node('-', new_node_num(0), term());
  return term();
}

Node *mul() {
  Node *node = unary();

  for (;;) {
    if (consume('*'))
      node = new_node('*', node, unary());
    else if (consume('/'))
      node = new_node('/', node, unary());
    else
      return node;
  }
}

Node *add() {
  Node *node = mul();

  for (;;) {
    if (consume('+'))
      node = new_node('+', node, mul());
    else if (consume('-'))
      node = new_node('-', node, mul());
    else
      return node;
  }
}

Node *rel() {
  Node *node = add();

  for (;;) {
    if (consume(TK_LE))
      node = new_node(ND_LE, node, add());
    else if (consume(TK_GE))
      node = new_node(ND_LE, add(), node);
    else if (consume('<'))
      node = new_node('<', node, add());
    else if (consume('>'))
      node = new_node('<', add(), node);
    else
      return node;
  }
}

Node *equality() {
  Node *node = rel();

  for (;;) {
    if (consume(TK_EQ))
      node = new_node(ND_EQ, node, rel());
    else if (consume(TK_NE))
      node = new_node(ND_NE, node, rel());
    else
      return node;
  }
}

Node *assign() {
  Node *node = equality();
  if (consume('='))
    node = new_node('=', node, assign());
  return node;
}

Node *stmt() {
  Node *node = assign();
  if (!consume(';'))
    error("; expected");
  return node;
}

void program() {
  int i = 0;
  while (((Token *)tokens->data[pos])->ty != TK_EOF)
    code[i++] = stmt();
  code[i] = NULL;
}

void parse(Vector *v) {
  tokens = v;
  program();;
}
