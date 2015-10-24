#include "CssPrint.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#define sstrjoin(...)    _sstrjoin(__VA_ARGS__, NULL)
char *_sstrjoin(char *buf, char *delim, ...)
{
    char *p, *res, *d;
    va_list ap;
    va_start(ap, delim);
    res = buf;
    p = va_arg(ap, char *);
    while(p)
    {
        while(*res++ = *p++)
            /* do nothing */;
        res--;
        if(p = va_arg(ap, char *))
        {
            d = delim;
            while(*res++ = *d++)
                /* do nothing */;
            res--;
        }
    }
    *res = '\0';
    va_end(ap);
    return buf;
}

typedef struct NODE
{   ast_node *data;
    struct NODE *next;
}stack, *LinkStack;

LinkStack Init_LinkStack(){
     return NULL;
}

LinkStack PushS(LinkStack top, ast_node *x)
{
    LinkStack s;
    s=(LinkStack)malloc(sizeof(stack));
    s->data=x;
    s->next=top;
    top=s;
    printf("PushS - INFO : type %d\n", x->type);
    return top;
}

LinkStack PopS(LinkStack top, ast_node **x)
{
    LinkStack p;
    if(top ==NULL)return NULL;
    else
    {
        *x=top->data;
        p=top;
        top=top->next;
        free(p);
       printf("PopS - INFO : type %d\n", (*x)->type);
        return top;
    }
}



void *close_file(const char *filename, FILE *file);
FILE *open_file(const char *filename, const char *opt);
void write_file(FILE *input, const char *content);
void print_node(FILE *input, ast_node *node, LinkStack NestedStack);
void print_content(FILE *input, ast_node *node);

int i, j;                                    //conter of ParentSelectorName
char ParentSelectorName[64][128]={'\0'};            //store combination of parent Name to deal with nested combination in selector name
char TempName[64][128]={'\0'};

int k = 0;                                       //temp conter

int main(){
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


////////////////////////////////////////////////////////////////////////////////////

int CssPrint_main(ast_node *root, const char *filename)
{

    FILE *css_file;
    css_file = open_file(filename, "w");

    LinkStack NestedStack = Init_LinkStack();
    if (root->type == ROOT)
        print_node(css_file, root->child_node, NestedStack);
    else
        print_node(stdout, root, NestedStack);             //if ast_node dont start with 'root' or filename == null, will print cssfile to stdout
    return 0;
}


void print_node(FILE *input, ast_node *node, LinkStack NestedStack)
{
    ast_node *Node;
    ast_node *Next;
    ast_node *Child;
    ast_node *Parent;
    ast_node **Nested = malloc(sizeof(ast_node *));

    Node = node;

    char* buf;
    ast_node *tmp;

    while(Node)
    {
        Next = Node->next_node;
        Child = Node->child_node;
        Parent = Node->parent_node;

        printf("INFO : Entry into type %d\n", Node->type);

        switch(Node->type)
        {
        case RAW_TEXT:
            write_file(input, Node->value);
            break;

        case COMMENT:   // -> only one ' has RAW_TEXT->value
            print_node(input, Child, NestedStack);
            break;

        case SELECTOR:   //  -> has NAME and CONTENT

            printf("Before Push INFO : entry selector and next type is %d\n", Node->child_node->type);
            printf("Before Push INFO : entry selector and next next type is %d\n", Node->child_node->child_node->type);

            NestedStack = PushS(NestedStack, Node);
            break;

        case PROPERTY:   // -> has NAME and RAW_TEXT
            print_node(input, Child, NestedStack);
            write_file(input, ";\n");
            break;

        case NAME:      //  has RAW_TEXT
            if (Parent->type == SELECTOR)
            {
                buf = (char *)malloc(sizeof(char[128]));
                printf("i : %d\n", i);
                for ( ; i > 0 ; --i )
                {
                    sstrjoin(buf, "", buf, ParentSelectorName[i], strcmp(ParentSelectorName[i], "")?" ":"", Child->value);
                    if ( i > 1 )
                         sstrjoin(buf, "", buf, ",\n");
                }
                Child->value = buf;
            }                       //(拼接
            print_node(input, Child, NestedStack);

            if (Next && Next->type == NAME)
                write_file(input, ",\n");
            else if (Parent->type == PROPERTY)
                write_file(input, ":");

            break;

        case CONTENT:   // -> has all other TYPE
            write_file(input, "{\n");
            print_node(input, Child, NestedStack);
            write_file(input, "}\n");
            break;

        default:
            fprintf(stderr, "Error: Unkown Type of Node, %d\n", Node->type);
            break;
        }
        Node = Next;
    }

    while (NestedStack)
    {
        NestedStack = PopS(NestedStack, Nested);

        printf("Nested - INFO : type %d\n", (*Nested)->type);

        for ( i = 0 ; i <= k ; ++i )
        {
            sstrjoin(TempName[i], "", ParentSelectorName[i]);
            printf("TempName  %s:",TempName[i]);
        }

        for ( i = 0, k = 0, tmp = (*Nested)->child_node; tmp && tmp->type == NAME ; ++i, tmp = tmp->next_node)
        {
            for (; j > 0; --j )
            {
                sstrjoin(ParentSelectorName[k], "", TempName[i], tmp->child_node->value);
                printf("%s",ParentSelectorName[k]);
                ++k;
            }
        }
        j = k;
        LinkStack ChildNestedStack = Init_LinkStack();
        print_node(input, (*Nested)->child_node, ChildNestedStack);
    }

}



////////////////////////////////////////////////////////////////////////////////////
void write_file(FILE *input, const char *content)
{
    printf("PRINT:  %s\n",content);
    fprintf(input,"%s",content);
}

FILE *open_file(const char *filename, const char *opt)
{
    FILE *input;
    if (!filename)
         input = stdout;
    else
    {
        input = fopen(filename, opt);
        if (!input) {
            fprintf(stderr, "error: cannot open %s: %s", filename, strerror(errno));
            return NULL;
        }

    }
    return input;
}

void *close_file(const char *filename, FILE *file)
{
    if(file != stdout)
        fclose(file);
}
