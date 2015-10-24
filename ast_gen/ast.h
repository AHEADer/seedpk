#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

#include "token_list.h"

struct _var_call
{
    int undef_times;
    char *name;
};

struct _operation
{
    enum TYPE
    {
        INT,
        FLOAT,
        COLOR,
        VAR_CALL,
        STRING,
        OPERATOR,
    } type;
    union {
        int i_val;
        float f_val;
        _var_call *var;
        char *s_string;
        char op;
    };
    _operation *next;
};

struct ast_node;

struct _var
{
    enum TYPE
    {
        NEW_FLAG,
        INT,
        FLAOT,
        COLOR,
        STRING,
        FUNC,
        RAW_TEXT,
        UNDECIDED //use before calculate
    } type;
    char *name;
    union
    {
        int v_int;
        float v_float;
        char * v_string;
        char *v_text;
        ast_node *func;

    };
    union
    {
        _operation *op_list;
        char *string_with_var;
        ast_node *name_node;
    };
    struct _var *next;
    _var(TYPE t):
        type(t),
        name(nullptr),
        v_int(0),
        op_list(nullptr),
        next(nullptr)
    {

    }
    ~_var()
    {
        if (type != STRING)
            delete op_list;
    }
};

struct ast_node
{
    enum TYPE
    {
        ROOT, //No value, has CONTENT
        RAW_TEXT,
        COMMENT,   //No value, has RAW_TEXT
        SELECTOR,  //No value, has NAME and CONTENT
        PROPERTY,  //No value, has NAME and VALUE
        NAME,      //No value, has RAW_TEXT and VAR_CALL in step one and only RAW_TEXT in step two
        CONTENT, //No value, has all other TYPE
        /*TEMP TYPE USED IN STEP ONE*/
        FUNC_DEFINE;
        FUNC_CALL, //No value
        FUNC_ARGUMENT, //No value
        VAR_CALL,
        STRING,
        OPERATION,
    } type;
    union
    {
        char *value;
        _operation *op;
        _var_call *var_call;
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

class ast_gen
{
private:
    ast_node *root_node;
    token_list tokens;
    token_list_elem *parsing;

    enum STATE
    {
        STATE_NORMAL,
        MIXIN
    } state;

    ast_node *make_node(ast_node *current_node, ast_node *content_node, ast_node::TYPE t);
    ast_node *make_name(ast_node *current_node, ast_node *parent_node);
    _operation *make_operation();
    _var_call *make_var_call();
    int sub_block_parser(ast_node *content_node);

public:
    ast_gen(token_list& tokenlist):
        root_node(new ast_node(ast_node::TYPE::ROOT, nullptr, nullptr, new _var(_var::NEW_FLAG))),
        tokens(tokenlist),
        parsing(tokens.head),
        state(STATE_NORMAL)
    {

    }
    int first_step();
    int second_step();
    const ast_node *get_ast()
    {
        return root_node;
    }
};

#endif // AST_H_INCLUDED
