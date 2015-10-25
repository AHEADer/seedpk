#include <stdio.h>
#include <string.h>


typedef struct var_list
{
    char *begin;
    char *end;
    struct var_list *next;
} var_list;

var_list *Init_var_list(){
    return NULL;
}

var_list *PushV(var_list *top, char *begin, char *end)
{
    var_list *s;
    s = (var_list*)malloc(sizeof(var_list));
    s->begin = begin;
    s->end = end;
    s->next = top;
    top = s;
    return top;
}

var_list *PopV(var_list *top, char *begin, char


var_list *string_var(const char *str)
{
    var_list *top;
    int n;

}



char *_string_judge(const char *str, var_list *pt, int n)
{
    switch (*str)
    {
        case '@':
            if (*(str+1) == '{')
                
            break;
        case '{':
            
            break;
        case '}':

            break;
        case '\0':
            break;
        default"
            break;
    }

}

