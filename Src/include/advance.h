#ifndef ADVANCD_H_INCLUDED
#define ADVANCD_H_INCLUDED


#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "token_list.h"

/*ID选择器:#定义
 *Class选择器:.定义
 *
 */
//int line = 0, column = 0;

enum State {
    BeginState,
    AnnotationState,
    AnnotationDelState,
    VariableState,
    ClassState,
    IDState,
    ValueState,
    ElementState,
    KeyState,
    NumState,
    ColorState,
    ArgumentState,
    FuncState,
    StringState,
    QuoteState,
};
char* create_string(char* head, char* end);
void Store_in_list(token_list_elem::TYPE type, const char* value);
void hanle_varible(char* value_head,int right,int left);
void nextchar();
void addLink();
void addToken(token_list_elem::TYPE type);
void rollback();
void delSpace();
void print_list(token_list_elem *head);
void spilt(const char* _argv);

#endif
