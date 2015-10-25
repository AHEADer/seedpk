#include "include/advance.h"
#include "include/ast.h"
#include "include/CssPrint.h"
#include "include/token_list.h"

using namespace std;

extern token_list_elem *pp;

int main(int argc, char **argv)
{
    if(argc<2)
    {
        printf("input error!\n");
        exit(-1);
    }

    pp = NULL;
    token_list_elem *init = new token_list_elem();
    //token_list_elem *init = (token_list_elem*)malloc(sizeof(token_list_elem));
    pp = init;
    init->next = NULL;
    spilt(argv[1]);
    //init = init->next;
    print_list(&init);

    return 0;
}
