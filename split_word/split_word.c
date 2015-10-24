#include "split_word.h"
int main(int argc, char const *argv[])
{
    if(argc<2){
        printf("input error!\n");
        exit(-1);
    }

    pp = NULL;
    WORD_LIST *init = (WORD_LIST*)malloc(sizeof(*init));
    pp = init;
    init->next = NULL;
    spilt(argv[1]);
    //init = init->next;
    print_list(&init);

    return 0;

}
