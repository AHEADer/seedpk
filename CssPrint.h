/*
 *  author : lkx810
 *  mail : lkx810@gmail.com
 *  time : 2015-10-21
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SRC "../Test/less"
#define CSS "../Test/css"

#define MAXNAMELEN 256

//typedef struct Css_Node
//{
//    char *name;
//    char *content;
//    struct Css_Node *next;     //
//    struct Css_Node *nest;     //descendent selector
//} Css_Node;

struct ast_node
{
    enum TYPE
    {
        ROOT, //No value, has CONTENT
        RAW_TEXT,
        COMMENT,
        SELECTOR,  //No value, has NAME and CONTENT
        PROPERTY,  //No value, has NAME and VALUE
        NAME,      //No value, has VALUE and OPERATION in step one and only VALUE in step two
        CONTENT, //No value, has all other TYPE
        /*TEMP TYPE USED IN STEP ONE*/
        RESERVERED,
        FUNC_DEFINE, //No value
        FUNC_CALL, //No value
        FUNC_ARGUMENT, //No value
        VAR_DEFINE, //No value
        STRING,
        OPERATION,
    } type;
    union
    {
        char *value;
        _operation *op;
    };
    ast_node *next_node;
    ast_node *child_node;
    ast_node *previous_node;
    ast_node *parent_node;
    _var *name_list;

    ast_node(TYPE t, ast_node *previous_node, ast_node *parent_node, _var *name_list):
        type(t),
        value(nullptr),
        next_node(nullptr),
        child_node(nullptr),
        previous_node(previous_node),
        parent_node(parent_node),
        name_list(name_list)
    {
        if (previous_node)
            previous_node->next_node = this;
    }
};


