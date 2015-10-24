/*
 *  author : lkx810
 *  mail : lkx810@gmail.com
 *  time : 2015-10-21
 * */


typedef struct ast_node
{
    enum TYPE
    {
        ROOT, //No value, has CONTENT
        RAW_TEXT,
        COMMENT,
        SELECTOR,  //No value, has NAME and CONTENT
        PROPERTY,  //No value, has NAME and VALUE
        NAME,      //No value, has RAW_TEXT and OPERATION in step one and only VALUE in step two
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
    char *value;
    struct ast_node *next_node;
    struct ast_node *child_node;
    struct ast_node *previous_node;
    struct ast_node *parent_node;
} ast_node;


int CssPrint_main(ast_node *root, const char *filename);
