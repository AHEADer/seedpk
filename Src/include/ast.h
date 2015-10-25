#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

#include "token_list.h"

struct str_ref
{
    char *str;
    str_ref *next;
    ~str_ref()
    {
        delete str;
    }
};

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
        PX,
        VAR_CALL,
        STRING,
        RAW_TEXT,
        OPERATOR,
        /*TEMP USE ONLY*/
        UNDECIDED
    } type;
    union
    {
        int i_val;
        float f_val;
        _var_call *var;
        char *s_string;
        char *text;
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
        FLOAT,
        COLOR,
        PX,
        STRING,
        FUNC,
        RAW_TEXT,
        UNDECIDED //use before calculate
    } type;
    char *name;
    union
    {
        char * v_string;
        char *v_text;

    };
    union
    {
        _operation *op_list;
        ast_node *func;
    };
    struct _var *next;
    _var(TYPE t):
        type(t),
        name(nullptr),
        v_string(nullptr),
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

struct _ret_with_type
{
    char *s_result;
    union
    {
        int i_result;
        float f_result;
    };
    _var::TYPE t;
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
        FUNC_DEFINE,
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
    void cp(ast_node *src);
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

    str_ref *str_list;
    ast_node *func_node;

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
        state(STATE_NORMAL),
        str_list(nullptr),
        func_node(nullptr)
    {

    }

    ~ast_gen()
    {
        if (str_list)
        {
            str_ref *t1, *t2;
            for (t1 = str_list, t2 = str_list->next; t2; t1 = t2, t2 = t2->next)
            {
                delete t1;
            }
            delete t1;
        }
    }

    void find_merge(ast_node *node);

    int first_step();
    int check_arg(ast_node *func, ast_node *call);
    void add_new_name(_var *dst, _var *src);
    void copy_child(ast_node *dst, ast_node *src, _var *var_list);
    void extend_with_var_list(ast_node *node, _var *var_list, ast_node *func);
    void end_block(ast_node *content_node);
    _ret_with_type var_call_in(_var_call *call, _var *var_list, int mask_num, char **mask);
    _ret_with_type cal_var(char *var_name, _var *var_list, int mask_num, char **mask);
    _ret_with_type cal_op_string(_operation *op, _var *var_list, int mask_num, char **mask);
    void op_extend(ast_node *node, int mask_num, char **mask);
    int var_extend(ast_node *node, int mask_num, char **mask);
    int property_extend(ast_node *node, int mask_num, char **mask);
    int func_call_extend(ast_node *node);
    char *new_str_ref(int len);
    ast_node *get_ast()
    {
        return root_node;
    }
};

#endif // AST_H_INCLUDED
