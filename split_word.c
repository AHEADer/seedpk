#include <stdio.h>
#include <stdlib.h>
/*ID选择器:#定义
 *Class选择器:.定义
 *
 */
int line = 0, column = 0;
char c;

enum Category {
    Sentence,
    ID_selector,
    Class_selector,
    Symbol,
};

enum State {
    BeginState,

};

void nextchar()
{
    c = getchar();
    column += 1;
}

void addToken(enum Category type)
{

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
                        }
                }
        }
		
	}

}

