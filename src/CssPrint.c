#include "CssPrint.h"

/*Some Fuction in CssPrint.h*/
void *close_file(const char *filename, FILE *file);
FILE *open_file(const char *filename, const char *opt);
void write_file(FILE *input, const char *content);
char *sstrjoin(char *buf, char *delim, ...);
LinkStack PopS(LinkStack top, ast_node **x);
LinkStack PushS(LinkStack top, ast_node *x);
/**/




/*Global variable*/
int i, j;                                           //conter of ParentSelectorName
char ParentSelectorName[64][128]={'\0'};            //store combination of parent Name to deal with nested combination in selector name
char TempName[64][128]={'\0'};
int k = 0;                                          //temp conter


/*Function Entry*/
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

/*Node Recursive Function*/
void print_node(FILE *input, ast_node *node, LinkStack NestedStack)
{
    ast_node *Node;
    ast_node *Next;
    ast_node *Child;
    ast_node *Parent;
    ast_node **Nested;

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
        case RAW_TEXT:      // -> has value. The end of all the nodes
            write_file(input, Node->value);
            break;

        case COMMENT:       // -> only one ' has RAW_TEXT->value
            print_node(input, Child, NestedStack);
            break;

        case SELECTOR:      //  -> has NAME and CONTENT

            printf("Before Push INFO : entry selector and next type is %d\n", Node->child_node->type);
            printf("Before Push INFO : entry selector and next next type is %d\n", Node->child_node->child_node->type);

            NestedStack = PushS(NestedStack, Node);             /*Push Child Seletor*/
            break;

        case PROPERTY:      // -> has NAME and RAW_TEXT
            print_node(input, Child, NestedStack);
            write_file(input, ";\n");
            break;

        case NAME:          //  has RAW_TEXT
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
            }

            print_node(input, Child, NestedStack);

            if (Next && Next->type == NAME)
                write_file(input, ",\n");
            else if (Parent->type == PROPERTY)
                write_file(input, ":");

            break;

        case CONTENT:       // -> has all other TYPE
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

    while (NestedStack)             /*To handle child selector*/
    {
        Nested = malloc(sizeof(ast_node *));
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
