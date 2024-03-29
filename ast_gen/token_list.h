#ifndef TOKEN_LIST_H_INCLUDED
#define TOKEN_LIST_H_INCLUDED

#include <cstdlib>
#include <cstring>

using namespace std;

struct token_list_elem
{
    enum TYPE{
        RAW_TEXT,
        SELECTOR_NAME,
        BLOCK_BEGIN,
        BLOCK_END,
        SEPARATOR,
        COMMA,
        VAR_DEFINE,
        ASSIGN,
        INT,
        FLOAT,
        COLOR,
        PX,
        STRING,
        OPERATOR,
        PROPERTY_NAME,
        FUNC_NAME,
        COMMENT,
        FUNC_ARGUMENT_BEGIN,
        FUNC_ARGUMENT_END
    } type;
    char *content;
    struct token_list_elem *next;

};

struct token_list
{
    struct token_list_elem *head;
};

#endif // TOKEN_LIST_H_INCLUDED
