#include "include/CssPrint.h"
#include "include/ast.h"
#include "include/eval.h"


/*Global variable*/
int i, j = 0;                                           //counter of ParentSelectorName
char ParentSelectorName[64][128]= {'\0'};           //store combination of parent Name to deal with nested combination in selector name
char TempName[64][128]= {'\0'};
int k = 0;                                          //temp counter
int m = 0;


/*Function Entry*/
int CssPrint_main(ast_node *root, const char *filename)
{

    FILE *css_file;
    css_file = open_file(filename, "w");

    LinkStack NestedStack = Init_LinkStack();
    if (root->type == ast_node::TYPE::ROOT)
        print_node(css_file, root->child_node, NestedStack);
    else
        print_node(stdout, root, NestedStack);             //if ast_node dont start with 'root' or filename == null, will print cssfile to stdout
    return 0;
}

/*Node Recursive Function*/
void print_node(FILE *input, ast_node *node, LinkStack NestedStack)
{
    ast_node *Node;
    ast_node *Next;
    ast_node *Child;
    ast_node *Parent;
    ast_node **Nested;

    Node = node;

    LinkStack ChildNestedStack = Init_LinkStack();
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
        case ast_node::TYPE::RAW_TEXT:      // -> has value. The end of all the nodes
            write_file(input, Node->value);
            break;

        case ast_node::TYPE::COMMENT:       // -> only one ' has ast_node::TYPE::RAW_TEXT->value
            print_node(input, Child, NestedStack);
            break;

        case ast_node::TYPE::SELECTOR:      //  -> has ast_node::TYPE::NAME and ast_node::TYPE::CONTENT

            printf("Before Push INFO : entry selector and next type is %d\n", Node->child_node->type);
            printf("Before Push INFO : entry selector and next next type is %d\n", Node->child_node->child_node->type);

            NestedStack = PushS(NestedStack, Node);             /*Push Child Seletor*/
            if (Next)
                print_node(input, Next, ChildNestedStack);
            while (NestedStack)             /*To handle child selector*/
            {
                Nested = (ast_node **) malloc(sizeof(ast_node *));
                NestedStack = PopS(NestedStack, Nested);

                printf("Nested - INFO : type %d\n", (*Nested)->type);

                for ( i = 0 ; i < k ; ++i )
                {
                    sstrjoin(ParentSelectorName[i], "", TempName[i]);
                    printf("TempName  %s:",TempName[i]);
                }
                m = i;

                for ( i = 0, k = 0, tmp = (*Nested)->child_node; tmp && tmp->type == ast_node::TYPE::NAME ; ++i, tmp = tmp->next_node)
                {
                    for (; j >= 0; --j )
                    {
                        sstrjoin(TempName[k], "", ParentSelectorName[i], tmp->child_node->value);
                        printf("%s",TempName[k]);
                        ++k;
                    }
                }
                j = k;

                print_node(input, (*Nested)->child_node, ChildNestedStack);
            }
            break;

        case ast_node::TYPE::PROPERTY:      // -> has ast_node::TYPE::NAME and ast_node::TYPE::RAW_TEXT
            print_node(input, Child, NestedStack);
            write_file(input, ";\n");
            break;

        case ast_node::TYPE::NAME:          //  has ast_node::TYPE::RAW_TEXT
            if (Parent->type == ast_node::TYPE::SELECTOR)
            {
                buf = (char *)malloc(sizeof(char[128]));
                sstrjoin(buf, "", "");
                for ( ; m >= 0 ; --m )
                {
                    sstrjoin(buf, "", ParentSelectorName[m], strcmp(ParentSelectorName[m], "")?" ":"", Child->value);
                    if ( i > 1 )
                        sstrjoin(buf, "", buf, ",\n");
                }
                Child->value = buf;
            }

            print_node(input, Child, NestedStack);

            if (Next && Next->type == ast_node::TYPE::NAME)
                write_file(input, ",\n");
            else if (Parent->type == ast_node::TYPE::PROPERTY)
                write_file(input, ":");

            break;

        case ast_node::TYPE::CONTENT:       // -> has all other TYPE
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

}
LinkStack Init_LinkStack()
{
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
        if (!input)
        {
            fprintf(stderr, "error: cannot open %s: %s", filename, strerror(errno));
            return NULL;
        }

    }
    return input;
}

void close_file(const char *filename, FILE *file)
{
    if(file != stdout)
        fclose(file);
}




