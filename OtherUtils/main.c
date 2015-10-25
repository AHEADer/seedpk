#include "CssPrint.c"




int main(){

    printf("----------------------Begin--------------------");

    ast_node *root;
    ast_node *comment;
    ast_node *raw_text;
    ast_node *selctor;
    ast_node *name;
    ast_node *raw_text0;
    ast_node * content;

    root = malloc(sizeof(ast_node));
    comment = malloc(sizeof(ast_node));
    raw_text = malloc(sizeof(ast_node));
    selctor = malloc(sizeof(ast_node));
    name = malloc(sizeof(ast_node));
    raw_text0 = malloc(sizeof(ast_node));

    content = malloc(sizeof(ast_node));

    root->type = ROOT;
    root->value = NULL;
    root->next_node=NULL;
    root->child_node=comment;
    root->previous_node=NULL;
    root->parent_node=root;

    comment->type = COMMENT;
    comment->value = NULL;
    comment->next_node=selctor;
    comment->child_node=raw_text;
    comment->previous_node=NULL;
    comment->parent_node=root;

    raw_text->type = RAW_TEXT;
    raw_text->value = "/*hello world*/\n";
    raw_text->next_node=NULL;
    raw_text->child_node=NULL;
    raw_text->previous_node=NULL;
    raw_text->parent_node=comment;

    selctor->type = SELECTOR;
    selctor->value = NULL;
    selctor->next_node=NULL;
    selctor->child_node=name;
    selctor->previous_node=comment;
    selctor->parent_node=root;


    name->type = NAME;
    name->value = NULL;
    name->next_node=content;
    name->child_node=raw_text0;
    name->previous_node=comment;
    name->parent_node=selctor;

    raw_text0->type = RAW_TEXT;
    raw_text0->value = "#hehe0";
    raw_text0->next_node=NULL;
    raw_text0->child_node=NULL;
    raw_text0->previous_node=comment;
    raw_text0->parent_node=root;

    content->type = CONTENT;
    content->value = NULL;
    content->next_node=NULL;
    content->child_node=NULL;
    content->previous_node=NULL;
    content->parent_node=selctor;



    CssPrint_main(root,"hellowolrd");
return 0;}
