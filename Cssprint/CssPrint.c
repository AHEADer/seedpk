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



void *close_file(const char *filename, FILE *file);
FILE *open_file(const char *filename, const char *opt);
void write_file(FILE *input, const char *content);
void print_node(FILE *input, ast_node *node);
void print_content(FILE *input, ast_node *node);




////////////////////////////////////////////define a linkstact store 'Less Nest'(嵌套
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
    s=(LinkStack)malloc(sizeof(LinkStack));
    s->data=x;
    s->next=top;
    top=s;
    return top;
}

LinkStack PopS(LinkStack top, ast_node *x)
{
    LinkStack p;
    if(top ==NULL)return NULL;
    else
    {
        x=top->data;
        p=top;
        top=top->next;
        free(p);
        return top;
    }
}


////////////////////////////////////////////////////////////////////////////////////

int CssPrint_main(ast_node *root, const char *filename)
{

    FILE *css_file;
    css_file = open_file(filename, "w");
    printf("hello");
    if (root->type == ROOT)
        print_node(css_file, root->child_node);
    else
        print_node(stdout, root);             //if ast_node dont start with 'root' or filename == null, will print cssfile to stdout
}

///////////////////////////////////////////////sorry to define two global variable
int i, j, k;                                    //conter of ParentSelectorName
char ParentSelectorName[128][128];               //store combination of parent Name
char TempName[128][128];


typedef struct _NameStack
{   char *str;
    struct _NameStack *next;
    int num;
}namestack, *NameStack;


/*
NameStack Init_NameStack(){
    NameStack s;
    s=(NameStack)malloc(sizeof(NameStack));
    s->str = NULL;
    s->next = NULL;
    s.num = 0;
    return s;
}

NameStack PushName(NameStack top, char *x)
{
    NameStack s;
    s=(NameStack)malloc(sizeof(NameStack));
    s->str=x;
    s->next=top;
    s.num += 1;
    top=s;
    return top;
}

NameStack PopName(NameStack top, char *x)
{
    LinkStack p;
    if(top ==NULL)return NULL;
    else
    {
        x=top->str;
        p=top;
        top=top->next;
        s.num -= 1;
        free(p);
        return top;
    }
}



NameStack namestack = Init_NameStack();
LinkStack Init_LinkStack(){
     return NULL;
}*/


void print_node(FILE *input, ast_node *node)
{
    ast_node *Node;
    ast_node *Next;
    ast_node *Child;
    ast_node *Parent;
    ast_node *Nested;

    Node = node;
    Next = node->next_node;
    Child = node->child_node;

    char* buf;
    ast_node *tmp;

    LinkStack NestedStack = Init_LinkStack();

    int flag;

    while(Node)
    {
        switch(Node->type)
        {
        case RAW_TEXT:
            write_file(input, Node->value);
            break;

        case COMMENT:   // -> only one ' has RAW_TEXT->value
            print_node(input, Child);
            break;

        case SELECTOR:   //  -> has NAME and CONTENT
            NestedStack = PushS(NestedStack, Node);
            break;

        case PROPERTY:   // -> has NAME and RAW_TEXT
            print_node(input, Child);
            fwrite(";\n", 2, 1, input);
            break;

        case NAME:      //  has RAW_TEXT
            if (Parent->type == SELECTOR)
            {
                buf = (char *)malloc(sizeof(char[128]));
                sstrjoin(buf,"",""); //init
                for ( ; i >= 0 ; --i )
                {
                    sstrjoin(buf, "", buf, ParentSelectorName[i], " ", Child->value);
                    if ( i > 0 )
                         sstrjoin(buf, "", buf, ",");
                }
                Child->value = buf;
            }                       //(拼接

            print_node(input, Child);

            if (Next->type == NAME)
                fwrite(",", 1, 1, input);
            else if (Parent->type == PROPERTY)
                fwrite(":", 1, 1, input);

            break;

        case CONTENT:   // -> has all other TYPE
            fwrite("{\n", 2, 1, input);
            print_node(input, Child);
            fwrite("}\n", 2, 1, input);
            break;

        default:
            break;
        }
        Node = Next;
    }

    if (NestedStack)
    {
        NestedStack = PopS(NestedStack, Nested);

        for ( i = 0 ; i <= k; ++i )
        {
            sstrjoin(TempName[i], "", ParentSelectorName[i]);
        }

        for ( i = 0, k = 0, tmp = Nested->child_node; tmp->type == NAME ; ++i, tmp = tmp->next_node)
        {
            for (; j > 0; --j )
                sstrjoin(ParentSelectorName[k], "", TempName[i], tmp->child_node->value);
        }
        j = k;

        print_node(input, Nested->child_node);
    }
}




////////////////////////////////////////////////////////////////////////////////////
void write_file(FILE *input, const char *content)
{
    fwrite(content, sizeof(content), 1, input);
}

FILE *open_file(const char *filename, const char *opt)
{
    FILE *input;
    if (filename)
         input = stdout;
    else
    {
        input = fopen(filename, opt);
        if (input) {
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
