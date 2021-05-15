#include "square.h"
#include "node.h"
#include "parse.tab.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SQU_ERROR_RUNTIME 0
#define SQU_ERROR_RETURN 1

static char*
strdup0(const char *s)
{
  size_t len = strlen(s);
  char *p;

  p = (char*)malloc(len+1);
  if (p) {
    strcpy(p, s);
  }
  return p;
}

static char*
strndup0(const char *s, size_t n)
{
  size_t i;
  const char *p = s;
  char *new;

  for (i=0; i<n && *p; i++,p++)
    ;
  new = (char*)malloc(i+1);
  if (new) {
    memcpy(new, s, i);
    new[i] = '\0';
  }
  return new;
}

node*
node_value_new(node* v)
{
  /* TODO */
  return NULL;
}

squ_array*
squ_array_new() {
  /* TODO: error check */
  squ_array* arr = (squ_array*)malloc(sizeof(squ_array));
  arr->len = 0;
  arr->max = 0;
  arr->data = NULL;
  return arr;
}

void
squ_array_add(squ_array* arr, void* data) {
  if (arr->len == arr->max) {
    arr->max = arr->len + 10;
    arr->data = realloc(arr->data, sizeof(void*) * arr->max);
  }
  /* TODO: error check */
  arr->data[arr->len] = data;
  arr->len++;
}

node*
node_array_new()
{
  /* TODO: error check */
  node* np = malloc(sizeof(node));
  np->type = NODE_VALUE;
  np->value.t = SQU_VALUE_ARRAY;
  np->value.v.p = squ_array_new();
  return np;
}

node*
node_array_of(node* np)
{
  if (np == NULL)
    np = node_array_new();
  return np;
}

void
node_array_add(node* arr, node* np)
{
  node_array* arr0 = arr->value.v.p;
  squ_array_add(arr0, np);
}

void
node_array_free(node* np)
{
  int i;
  node_array* arr = np->value.v.p;
  for (i = 0; i < arr->len; i++)
  {
    node_free(&arr->data[i]);
  }
  free(np);
}

node*
node_pair_new(node* key, node* value)
{
  node_pair* npair = malloc(sizeof(node_pair));
  npair->key = key;
  npair->value = value;

  node* np = malloc(sizeof(node));
  np->type = NODE_PAIR;
  np->value.t = SQU_VALUE_USER;
  np->value.v.p = npair;
  return np;
}

node*
node_let_new(node* lhs, node* rhs)
{
  node_let* nlet = malloc(sizeof(node_let));
  nlet->lhs = lhs;
  nlet->rhs = rhs;

  node* np = malloc(sizeof(node));
  np->type = NODE_LET;
  np->value.t = SQU_VALUE_USER;
  np->value.v.p = nlet;
  return np;
}

node*
node_op_new(char* op, node* lhs, node* rhs)
{
  node_op* nop = malloc(sizeof(node_op));
  nop->lhs = lhs;
  nop->op = op;
  nop->rhs = rhs;

  node* np = malloc(sizeof(node));
  np->type = NODE_OP;
  np->value.t = SQU_VALUE_USER;
  np->value.v.p = nop;
  return np;
}

node*
node_print_new(node* args)
{
  node_print* nprint = malloc(sizeof(node_print));
  nprint->args = args;

  node* np = malloc(sizeof(node));
  np->type = NODE_PRINT;
  np->value.t = SQU_VALUE_USER;
  np->value.v.p = nprint;
  return np;
}

node*
node_block_new(node* args, node* stmt_seq)
{
  node_block* block = malloc(sizeof(node_block));
  block->args = args;
  block->stmt_seq = stmt_seq;

  node* np = malloc(sizeof(node));
  np->type = NODE_BLOCK;
  np->value.t = SQU_VALUE_USER;
  np->value.v.p = block;
  return np;
}

node*
node_call_new(node* cond, node* ident, node* args, node* blk)
{
  node_call* ncall = malloc(sizeof(node_call));
  ncall->cond = cond;
  ncall->ident = ident;
  ncall->args = args;
  ncall->blk = blk;

  node* np = malloc(sizeof(node));
  np->type = NODE_CALL;
  np->value.t = SQU_VALUE_USER;
  np->value.v.p = ncall;
  return np;
}

node*
node_lambda_new(node* args, node* body, node* args_value)
{
  node_lambda* nlambda = malloc(sizeof(node_lambda));
  nlambda->args = args;
  nlambda->body = body;
  nlambda->args_value = args_value;

  node* np = malloc(sizeof(node));
  np->type = NODE_LAMBDA;
  np->value.t = SQU_VALUE_USER;
  np->value.v.p = nlambda;
  return np;
}

node*
node_fdef_new(node* ident, node* args, node* blk)
{
  node_fdef* nfdef = malloc(sizeof(node_fdef));
  nfdef->ident = ident;
  nfdef->args = args;
  nfdef->blk = blk;

  node* np = malloc(sizeof(node));
  np->type = NODE_FDEF;
  np->value.t = SQU_VALUE_USER;
  np->value.v.p = nfdef;
  return np;
}

node*
node_double_new(squ_double d)
{
  node* np = malloc(sizeof(node));

  np->type = NODE_VALUE;
  np->value.t = SQU_VALUE_DOUBLE;
  np->value.v.d = d;
  return np;
}

node*
node_int_new(squ_int i)
{
  node* np = malloc(sizeof(node));

  np->type = NODE_VALUE;
  np->value.t = SQU_VALUE_INT;
  np->value.v.i = i;
  return np;
}

node*
node_string_new(squ_string s)
{
  node* np = malloc(sizeof(node));

  np->type = NODE_VALUE;
  np->value.t = SQU_VALUE_STRING;
  np->value.v.s = strdup0(s);
  return np;
}

node*
node_string_len_new(squ_string s, size_t l)
{
  node* np = malloc(sizeof(node));

  np->type = NODE_VALUE;
  np->value.t = SQU_VALUE_STRING;
  np->value.v.s = strndup0(s, l);
  return np;
}

node*
node_ident_new(squ_string id)
{
  node* np = malloc(sizeof(node));

  np->type = NODE_IDENT;
  np->value.t = SQU_VALUE_IDENT;
  np->value.v.id = id;
  return np;
}

squ_id
node_ident_of(char* s)
{
  /* TODO: get id of the identifier which named as s */
  return (squ_id)strdup0(s);
}

node*
node_null()
{
  static node nd = { NODE_VALUE, { SQU_VALUE_NULL, {0} } };
  return &nd;
}

node*
node_true()
{
  static node nd = { NODE_VALUE, { SQU_VALUE_BOOL, {true} } };
  return &nd;
}

node*
node_false()
{
  static node nd = { NODE_VALUE, { SQU_VALUE_BOOL, {false} } };
  return &nd;
}

node*
node_if_new(node* cond, node* stmt_seq, node* opt_else)
{
  node_if* nif = malloc(sizeof(node_if));
  nif->cond = cond;
  nif->stmt_seq = stmt_seq;
  nif->opt_else = opt_else;

  node* np = malloc(sizeof(node));
  np->type = NODE_IF;
  np->value.t = SQU_VALUE_USER;
  np->value.v.p = nif;
  return np;
}

node*
node_loop_new(node* stmt_seq, node* cond)
{
  node_loop* nloop = malloc(sizeof(node_loop));
  nloop -> stmt_seq = stmt_seq;
  nloop -> cond = cond;

  node* np = malloc(sizeof(node));
  np -> type = NODE_LOOP;
  np -> value.t = SQU_VALUE_USER;
  np -> value.v.p = nloop;
  return np;
}

node*
node_return_new(node* value)
{
  node* np = malloc(sizeof(node));
  np->type = NODE_RETURN;
  np->value.t = SQU_VALUE_USER;
  np->value.v.p = value;
  return np;
}

node*
node_import_new(squ_id name)
{
  node_import* nimp = malloc(sizeof(nimp));
  nimp -> name = name;
  
  node* np = malloc(sizeof(node));
  np -> type = NODE_IMPORT;
  np -> value.t = SQU_VALUE_USER;
  np -> value.v.p = nimp;
  np -> value.v.id = nimp -> name;

  return np;
}

node*
node_break_new(node* value)
{
  static node nd = { NODE_BREAK };
  return &nd;
}