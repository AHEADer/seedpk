/*
 *
 *
 * */

#include "CssPrint.h"
int CssPrint_main(Css_Node *root, const char *filename, const char *head);
void *close_file(const char *filename, FILE *file);
FILE *open_file(const char *filename, const char *opt);
void write_file(FILE *input, const char *content, int opt);
void print_node(FILE *input, Css_Node *node);
void print_content(FILE *input, Css_Node *node);

void main(){}

int CssPrint_main(Css_Node *root, const char *filename, const char *head)
{
    Css_Node *node;
    Css_Node *nest;
    Css_Node *next;
    node = root;

    FILE *css_file;
    css_file = open_file(filename, "w");

    if (head)
        write_file(css_file, head, 0);

    print_node(css_file, root);

}

void print_node(FILE *input, Css_Node *node)
{
    Css_Node *Nest;
    Css_Node *Node;
    char node_name[MAXNAMELEN] = {""};

    Node = node;
    while(Node)
    {
        strcat(node_name, node->name);
        print_content(input, node);

        Nest = Node->nest;
        while(Nest)
        {
            Nest->name = node_name;
            print_node(input, Nest);

            Nest = Nest->nest;
        }

        Node = Node->next;
    }
}


void print_content(FILE *input, Css_Node *node)
{

    if(node->name)
    {
        write_file(input, node->name, 0);
        write_file(input, node->content, 1);

    }
    else
        write_file(input, node->content, 0);
}

void write_file(FILE *input, const char *content, int opt)
{
    if(opt == 0)
    {
        fwrite(content, sizeof(content), 1, input);
    }
    else if(opt == 1)
    {
        fwrite("{\n", 2, 1, input);
        fwrite(content, sizeof(content), 1, input);
        fwrite("\n}\n", 3, 1, input);
    }
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
