/*
 *
 *
 * */

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

#include "CssPrint.h"
int CssPrint_main(ast_node *root, const char *filename, const char *head);
void *close_file(const char *filename, FILE *file);
FILE *open_file(const char *filename, const char *opt);
void write_file(FILE *input, const char *content, int opt);
void print_node(FILE *input, ast_node *node);
void print_content(FILE *input, ast_node *node);

void main(){}

typedef struct NODE
{   ast_node *node;
    struct NODE *next;
}stack, *LinkStact;

LinkStact Init_LinkStack(){
     return NULL;
}

LinkStack PushS(LinkStack top, ast_node x)
{
    StackNode *s;
    s=malloc( sizeof(StackNode) );
    s->data=x;
    s->next=top;
    top=s;
    return top;
}

LinkStack PopS(LinkStack top, ast_node *x)
{
    StackNode *p;
    if （top= =NULL） return NULL;
    else
    {
        *x = top->data;
        p = top;
        top = top->next;
        free (p);
        return top;
    }
}

////////////////////////////////////////////////////////////////////////////////////

int CssPrint_main(ast_node *root, const char *filename)
{
    ast_node *node;
    ast_node *nest;
    ast_node *next;
    node = root;

    char* ParentSelectorName[128];

    FILE *css_file;
    css_file = open_file(filename, "w");

    if (root.type == ROOT)
        print_node(input, root->child_node);
    else
        print_node(stdout, root);

}

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

    char buf[128];
    int i;
    ast_node *tmp;

    LinkStact NestedStack = Init_LinkStack();

    int flag;

    while(Node)
    {
        if (NestedStack)
        {
            NestedStack = PopS(NestedStack, Nested);
            for ( i = 0, tmp = Nested->child; tmp.type == NAME ; ++i, tmp = tmp->Nested)
            {
                ParentSelectorName[i] = tmp->children->value;
            }
            print_node(input, Nested);
        }

        swtich(Node.type)
        {
        case RAW_TEXT:
            write_file(input, Node->value);
            break;

        case COMMENT:   // 评论 has RAW_TEXT->value
            print_node(input, child);
            break;

        case SELECTOR:   // 选择器 has NAME and CONTENT
            NestedStack = PushS(NestedStack, Node);
            break;

        case PROPERTY:   // 属性 has NAME and RAW_TEXT
            print_node(input, Child);
            fwrite(";\n", 2, 1, input);
            break;

        case NAME:      // 名字 has RAW_TEXT
            if (Parent.type == SELECTOR)
            {
                sstrjoin(bufi,"",""); //init
                for ( ; i >= 0 ; --i )
                {
                    sstrjoin(buf, "", buf, ParentSelectorName[i], " ", Child->value);
                    if ( i > 0 )
                         sstrjoin(buf, "", buf, ",");
                }
                Child->value = buf;
            }

            print_node(input, Child);
            if (Next.type == NAME)
                fwrite(",", 1, 1, input);
            else if (Parent.type == PROPERTY)
                fwrite(":", 1, 1, input);

            break;

        case CONTENT:   // 子块 has all other TYPE
            fwrite("{\n", 2, 1, input);
            print_node(input, Child);
            fwrite("}\n", 2, 1, input);
            break;
        }
        Node = Next;
    }
}

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
