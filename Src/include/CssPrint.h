#ifndef CSSPRINT_H_INCLUDED
#define CSSPRINT_H_INCLUDED

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>

#include <stdarg.h>

#include "ast.h"
#include "eval.h"


/*Define a stack to store child selector*/
typedef struct NODE
{   ast_node *data;
    struct NODE *next;
}stack, *LinkStack;

/*String Concatenation*/
void print_node(FILE *input, ast_node *node, LinkStack NestedStack);
void close_file(const char *filename, FILE *file);
FILE *open_file(const char *filename, const char *opt);
void write_file(FILE *input, const char *content);
/*char *sstrjoin(char *buf, char *delim, ...);*/
LinkStack PopS(LinkStack top, ast_node **x);
LinkStack PushS(LinkStack top, ast_node *x);
LinkStack Init_LinkStack();

#endif // CSSPRINT_H_INCLUDED
