#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*ID选择器:#定义
 *Class选择器:.定义
 *
 */

int line = 0, column = 0;
char value[64];
char c , *p = value;

enum Category {
    Sentence,
    ID_selector,
    Class_selector,
    Symbol,
    Block,
    Variable,
    Value,
};

enum State {
    BeginState,
    AnnotationState,
    VariableState,
    ClassState,
    IDState,
    ValueState,
};

void nextchar()
{
    c = getchar();
    *p++ = c;
    column += 1;
}

void addToken(enum Category type)
{
    *p = '\0';
    p = value;
    int len = strlen(p);
        printf("%s %d 0\n", p, type);
}

void rollback() {
    p--;
    ungetc(c, stdin);
}

void delSpace() {
    *p = '\0';
    p--;
    column --;
}


int spilt(const char* _argv);

int main(int argc, char const *argv[])
{
    if(argc<2){
        printf("input error!\n");
        exit(-1);
    }
    spilt(argv[1]);
    
    return 0;

}

int spilt(const char* _argv)
{
    if(!freopen(_argv,"r",stdin)){
        printf("cannot read file!");
        return -1;
    }
    enum State s = BeginState;
	while(c!=EOF){
		nextchar();
        switch(s){
            case BeginState:
                switch(c){
                    case '/':
                        nextchar();
                        switch(c) {
                            case '*':
                                s = AnnotationState;
                                nextchar();
                                break;
                            case '/':
                                c = getchar();
                                while(c!='\n'&&c!=EOF){
                                    c = getchar();
                                }
                                c = getchar();
                                s = BeginState;
                                break;
                        }
                    case '@':
                        s = VariableState;
                        break;
                    case '#':
                        s = IDState;
                        break;
                    case '.':
                        s = ClassState;
                        break;
                    default:
                        break;
                }
                break;
            case AnnotationState:
                switch(c){
                    case '*':
                        switch(c){
                            case '/':
                                addToken(Sentence);
                                s = BeginState;
                                break;
                            default:
                                nextchar();
                                s = AnnotationState;
                        }

                    default:
                        break;
                }
                break;
            case VariableState:
                switch(c){
                    case ':':
                        rollback();
                        addToken(Variable);
                        nextchar();
                        addToken(Block);
                        nextchar();
                        s = ValueState;
                        break;

                    default:
                        nextchar();
                        s = VariableState;        
                }
                break;
            case ValueState:
                switch(c){
                    case ' ':
                        //delSpace();
                        nextchar();
                        printf("haha\n");
                        s = ValueState;
                        break;
                    case ';':
                        addToken(Value);
                        s = BeginState;
                        break;
                    default:
                        nextchar();
                        s = ValueState;
                }
                break;
        }
		
	}

}

