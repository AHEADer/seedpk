/*
 *  author : lkx810
 *  mail : lkx810@gmail.com
 *  time : 2015-10-21
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SRC "../Test/less"
#define CSS "../Test/css"

#define MAXNAMELEN 256

typedef struct Css_Node
{
    char *name;
    char *content;
    struct Css_Node *next;     //
    struct Css_Node *nest;     //descendent selector
} Css_Node;


