
#include "PublicStruct.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <stdarg.h>
/*String Concatenation*/
#define sstrjoin(...)    _sstrjoin(__VA_ARGS__, NULL)
char *_sstrjoin(char *buf, char *delim, ...)
{
    char *p, *res, *d;
    int i = 0;
    va_list ap;
    va_start(ap, delim);
    res = buf;
    p = va_arg(ap, char *);
    while(p)
    {
        while(*res++ = *p++)
            ;
        res--;
        if(p = va_arg(ap, char *))
        {
            d = delim;
            while(*res++ = *d++)
                ;
            res--;
        }
    }
    *res = '\0';
    va_end(ap);
    return buf;
}


/*Define a stack to store child selector*/
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

/*File Related Functions*/
void msg_printf(const char *msg, int lever)
{
    if (lever == 0)
        fprintf(stdout, "INFO : %s \n", msg);
    if (lever == -1)
        fprintf(stderr, "ERROR: %s \n", msg);
}

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




