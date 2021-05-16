#ifndef _NODE_H_
#define _NODE_H_
#include "square.h"

#define RUN_OK 0
#define RUN_NG 1

typedef enum {
  NODE_ARGS,
  NODE_PAIR,
  NODE_VALUE,
  NODE_CFUNC,
  NODE_BLOCK,
  NODE_IDENT,
  NODE_LOOP,
  NODE_LET,
  NODE_IF,
  NODE_FDEF,
  NODE_RETURN,
  NODE_LAMBDA,
  NODE_BREAK,
  NODE_VAR,
  NODE_CONST,
  NODE_OP,
  NODE_CALL,
  NODE_IMPORT,
  NODE_PRINT,
} node_type;

typedef struct {
  node_type type;
  squ_value value;
} node;

typedef struct {
  node_type type;
  node* key;
  node* value;
} node_pair;

typedef squ_array node_array;

typedef struct {
  node_type type;
  node* cond;
  node* stmt_seq;
  node* opt_else;
} node_if;

typedef struct{
  node_type type;
  node* stmt_seq;
  node* cond;
}node_loop;


typedef struct {
  node_type type;
  node* lhs;
  node* rhs;
} node_let;

typedef struct {
  node_type type;
  node* lhs;
  char* op;
  node* rhs;
} node_op;

typedef struct {
  node_type type;
  node* args;
  node* stmt_seq;
} node_block;

typedef struct {
  node_type type;
  node* cond;
  node* ident;
  node* args;
  node* blk;
} node_call;

typedef struct{
  node_type type;
  node* args;
  node* body;
  node* args_value;
  node* ret;
}node_lambda;

typedef struct squ_lambda{
  node* body;
  node* args;
  node* args_value;
  node* ret;
}squ_lambda;

typedef struct {
  node_type type;
  node* ident;
  node* args;
  node* blk;
  node* ret;
}node_fdef;

typedef struct {
  node_type type;
  node* rv;
} node_return;

typedef struct {
  node_type type;
  squ_id name;
} node_import;

typedef struct
{
  node_type type;
  node* args;
}node_print;


extern node* node_value_new(node*);
extern node* node_array_new();
extern node* node_array_of(node*);
extern void node_array_add(node*, node*);
extern void node_array_free(node*);
extern node* node_pair_new(node*, node*);
extern node* node_let_new(node*, node*);
extern node* node_op_new(char*, node*, node*);
extern node* node_block_new(node*, node*);
extern node* node_call_new(node*, node*, node*, node*);
extern node* node_lambda_new(node*, node*, node*, node*);
extern node* node_fdef_new(node*, node*, node*, node*);
extern node* node_double_new(squ_double);
extern node* node_int_new(squ_int);
extern node* node_string_new(squ_string);
extern node* node_string_len_new(squ_string, size_t);
extern node* node_if_new(node*, node*, node*);
extern node* node_loop_new(node*, node*);
extern node* node_return_new(node*);
extern node* node_import_new(squ_id);
extern node* node_break_new();
extern node* node_ident_new(squ_string);
extern squ_id node_ident_of(squ_string);
extern node* node_null();
extern node* node_true();
extern node* node_false();
extern void node_free(node*);
extern node* node_print_new(node*);

squ_lambda* lambda;

#endif /* _NODE_H_ */