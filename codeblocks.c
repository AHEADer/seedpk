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
    Space,
    Element,
    Key,
};

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
    if(type == Space)
        return;
    else
    printf("%s %d %d 0\n", p, type, len);
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


void spilt(const char* _argv);

int main(int argc, char const *argv[])
{
    if(argc<2){
        printf("input error!\n");
        exit(-1);
    }
    spilt(argv[1]);

    return 0;

}

void spilt(const char* _argv)
{
    if(!freopen(_argv,"r",stdin)){
        printf("cannot read file!");
        return ;
    }
    enum State s = BeginState;
	while(c!=EOF){
        switch(s){
            case BeginState:
                nextchar();
                switch(c){
                    case '/':
                        nextchar();
                        switch(c) {
                            case '*':
                                s = AnnotationState;
                                nextchar();
                                break;
                            case '/':
                                nextchar();
                                s = AnnotationDelState;
                                break;
                        }
                        break;
                    case '@':
                        s = VariableState;
                        nextchar();
                        break;
                    case '#':
                        nextchar();
                        s = IDState;
                        break;
                    case '.':
                        nextchar();
                        s = ClassState;
                        break;
                    case '{':
                        addToken(Block);
                        s = ElementState;
                        break;
                    case '}':
                        addToken(Block);
                        s = BeginState;
                        break;
                    case '\n': case '\r':
                        addToken(Space);
                        s = BeginState;
                        break;
                    default:
                        addToken(Space);
                        s = BeginState;
                        break;
                }
                break;
            case AnnotationState:
                switch(c){
                    case '*':
                        nextchar();
                        switch(c){
                            case '/':
                                addToken(Sentence);
                                s = BeginState;
                                break;
                            default:
                                nextchar();
                                s = AnnotationState;
                        }
                        break;

                    default:
                        //printf("%c\n",c);
                        nextchar();
                        s = AnnotationState;
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
                    case ' ':
                        rollback();
                        break;
                    default:
                        nextchar();
                        s = VariableState;
                }
                break;
            case ValueState:
                switch(c){
                    case ' ':
                        addToken(Space);
                        nextchar();
                        s = ValueState;
                        break;
                    case ';':
                        rollback();
                        addToken(Value);
                        nextchar();
                        addToken(Block);
                        s = BeginState;
                        break;
                    default:
                        nextchar();
                        s = ValueState;
                }
                break;
            case ClassState:
                nextchar();
                break;
            case IDState:
                switch(c){
                    case ' ':
                        rollback();
                        addToken(ID_selector);
                        nextchar();
                        addToken(Space);
                        s = BeginState;
                        break;
                    case '{':
                        rollback();
                        addToken(ID_selector);
                        s = BeginState;
                        break;
                    default:
                        nextchar();
                }
                break;
            case AnnotationDelState:
                if(c == '\n' || c == '\r'){
                    addToken(Space);
                    s = BeginState;
                }
                else{
                    nextchar();
                    s = AnnotationDelState;
                }
                break;

            case ElementState:
                switch(c){
                    case ' ':
                        rollback();
                        if(c!=' '&&c!='{'){
                            addToken(Element);
                            nextchar();
                            s = KeyState;
                        }
                        else{
                        nextchar();
                        s = ElementState;
                        }
                        break;
                    case ':':
                        rollback();
                        if(c!=' ')
                            addToken(Element);
                        nextchar();
                        nextchar();
                        s = KeyState;
                        break;
                    default:
                            rollback();
                            if(c==' '||c=='\n'||c=='\r')
                                addToken(Space);
                            nextchar();
                            nextchar();
                            s = ElementState;
                }
                break;

            case KeyState:
                break;

            default:
                nextchar();
        }

	}

}

