#include "include/CssPrint.h"
#include "include/ast.h"
#include "include/eval.h"


/*Global variable*/
int i, j = 0;                                           //counter of ParentSelectorName
char ParentSelectorName[64][128]= {'\0'};           //store combination of parent Name to deal with nested combination in selector name
char TempName[64][128]= {'\0'};
int k = 0;                                          //temp counter
int m = 0;
int first = 1;


/*Function Entry*/
int CssPrint_main(ast_node *root, const char *filename)
{

    FILE *css_file;
    css_file = open_file(filename, "w");

    if (root->type == ast_node::TYPE::ROOT)
    {
        print_node(css_file, root->child_node, "");
        if (root->child_node)
            write_file(css_file, "}\n");
    }
    else
    {
        print_node(stdout, root, "");
        write_file(stdout, "}\n");
    }             //if ast_node dont start with 'root' or filename == null, will print cssfile to stdout
    return 0;
}

/*Node Recursive Function*/
void print_node(FILE *input, ast_node *node, const char *now_name)
{
    if (!node)
    {
        return;
    }


    switch(node->type)
    {
    case ast_node::TYPE::RAW_TEXT:      // -> has value. The end of all the nodes

        write_file(input, node->value);
        print_node(input, node->next_node, now_name);

        break;

    case ast_node::TYPE::COMMENT:       // -> only one ' has ast_node::TYPE::RAW_TEXT->value
        print_node(input, node->child_node, now_name);
        print_node(input, node->next_node, now_name);
        break;

    case ast_node::TYPE::SELECTOR:      //  -> has ast_node::TYPE::NAME and ast_node::TYPE::CONTENT
    {
        if (node->parent_node->type != ast_node::ROOT)
            print_node(input, node->next_node, now_name);
        ast_node *name_node;
        int c = 1;
        char *new_name = new char[1024];
        strcpy(new_name, now_name);


        for (name_node = node->child_node; name_node; name_node = name_node->next_node)
        {
            if (name_node->type == ast_node::NAME)
            {
                if (c)
                {
                    sstrjoin(new_name,"", new_name, name_node->child_node->value);
                    c=0;
                }
                else
                    sstrjoin(new_name, ",\n", new_name, name_node->child_node->value);
            }
        }
        if (node->parent_node->type != ast_node::TYPE::ROOT)
            write_file(input, "}\n");


        if (node->parent_node->type == ast_node::TYPE::ROOT && node->previous_node && node->previous_node->type==ast_node::TYPE::SELECTOR)
            write_file(input, "}\n");

        write_file(input, new_name);
        print_node(input, node->child_node, new_name);
        delete new_name;
        if (node->parent_node->type == ast_node::ROOT)
            print_node(input, node->next_node, now_name);
    }
    break;

    case ast_node::TYPE::PROPERTY:      // -> has ast_node::TYPE::NAME and ast_node::TYPE::RAW_TEXT
        write_file(input ,"   ");
        print_node(input, node->child_node, now_name);
        write_file(input, ";\n");
        print_node(input, node->next_node, now_name);
        break;

    case ast_node::TYPE::NAME:          //  has ast_node::TYPE::RAW_TEXT

        if (node->parent_node->type == ast_node::TYPE::SELECTOR)
        {
            print_node(input, node->next_node, now_name);
            break;
        }
        print_node(input, node->child_node, now_name);
        if (node->parent_node->type == ast_node::TYPE::PROPERTY)
            write_file(input, ":");
        print_node(input, node->next_node, now_name);
        break;

    case ast_node::TYPE::CONTENT:       // -> has all other TYPE
        write_file(input, "{\n");
        print_node(input, node->child_node, now_name);
        break;

    default:
        fprintf(stderr, "Error: Unkown Type of Node, %d\n", node->type);
        break;
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

