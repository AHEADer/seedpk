#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*ID选择器:#定义
 *Class选择器:.定义
 *
 */
//int line = 0, column = 0;

char value[64];
char c , *p = value;

enum Category {
    RAW_TEXT, //0
    SELECTOR_NAME,
    BLOCK_BEGIN,
    BLOCK_END,
    SEPARATOR,
    COMMA,    //5
    VAR_DEFINE,
    ASSIGN,
    INT,
    FLOAT,
    COLOR,     //10
    STRING,
    OPERATOR,//()while calculate
    PROPERTY_NAME,
    FUNC_NAME,
    FUNC_ARGUMENT_BEGIN, //15
    FUNC_ARGUMENT_END,
    COMMENT,             //17
    PX,
    Space,
    DelAll,
};

typedef struct word_list
{
    char content[64];
    enum Category type;
    struct word_list* next;
}WORD_LIST;

//WORD_LIST *head;
WORD_LIST *pp;

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
char* create_string(char* head, char* end)
{
    char a = *end;
    *end = '\0';
    char*b = (char*)malloc(64),*aa = b;
    while(*head!='\0'){
        *b++ = *head++;
    }
    *b++ = a;
    *b = '\0';
    //printf("create_string:%s\n",b );
    return aa;
}

void Store_in_list(enum Category type, const char* value)
{
    while(pp->next != NULL){
        pp = pp->next;
    }
    pp->next = (WORD_LIST*)malloc(sizeof(*pp));
    pp->next->type = type;
    strcpy(pp->next->content,value);
    //pp->next = NULL;
    pp = pp->next;
}

void hanle_varible(char* value_head,int right,int left)
{
    int i,add = 0;
    char *head = value_head;
    char *pp = head;
    char *tmp;
    char have = 'n';
    if (left > right)
    {
        i = left - right;
        while(*pp!='\0'){
            switch(*pp){
                case '@':
                    if (have == 'y')
                    {
                        Store_in_list(OPERATOR,"$");
                        Store_in_list(STRING,create_string(head,pp));
                        //printf("!!!!!!!!!create_string:%s",create_string(head,pp));
                        have = 'n';
                    }
                    add = 1;
                    break;
                case '{':
                    if (add == 1&&i>0)
                    {
                        have = 'y';
                        i--;
                        left--;

                    }
                    break;
                case '\"':
                    if (have == 'y')
                    {
                        Store_in_list(OPERATOR,"$");
                        printf("%s %d 0\n","$", OPERATOR/*, len*/);
                        printf("DEBUG!!!!!!!!\n");
                        pp--;
                        Store_in_list(STRING,create_string(value_head,pp));
                        pp++;
                        printf("%s %d 0\n",create_string(value_head,pp), STRING/*, len*/);
                        //have = 'n';
                    }
                    break;
                default:
                    have = 'y';
                    add = -1;
            }
            pp++;
        }
    }
    else if(left == right)
    {
        while(*pp!='\0'){
            switch(*pp){
                case '@':
                    if (have == 'y')
                    {
                        Store_in_list(OPERATOR,"$");
                        char *store = create_string(value_head,pp);
                        Store_in_list(STRING,store);
                        printf("!!!!!!!!!create_string:%s",store);
                        have = 'n';
                    }
                    add = 1;
                    break;
                case '{':
                    if (add == 1)
                    {
                        Store_in_list(VAR_DEFINE,"@");
                        Store_in_list(BLOCK_BEGIN,"{");
                    }
                    break;
                case '\"':
                    if (have == 'y')
                    {
                        Store_in_list(OPERATOR,"$");
                        printf("%s %d 0\n","$", OPERATOR/*, len*/);
                        pp--;
                        Store_in_list(STRING,create_string(head,pp));
                        pp++;
                        printf("%s %d 0\n",create_string(head,pp), STRING/*, len*/);
                        //have = 'n';
                    }
                    break;
                case '}':
                    if (have == 'y')
                    {
                        tmp = pp;
                        tmp--;
                        for(;*pp!='{';pp--);
                        pp++;
                        Store_in_list(RAW_TEXT,create_string(pp,tmp));
                        printf("%s %d 0\n",create_string(pp,tmp), RAW_TEXT/*, len*/);
                        //printf("!!!!!!!!!create_string:%s!!!!!!!!!!!!!!\n",create_string(pp,tmp));
                        pp = ++tmp;
                        Store_in_list(BLOCK_END,"}");
                        have = 'n';
                    }
                    if (have == 'n')
                    {
                        Store_in_list(BLOCK_END,"}");
                        printf("%s %d 0\n","}", BLOCK_END );
                    }

                    break;
                default:
                    add = -1;
                    have = 'y';
            }
            pp++;
        }
    }
}


void nextchar()
{
    c = getchar();
    *p++ = c;
    //column += 1;
}
void addLink()
{
    printf("addLink: %s %d\n","$",OPERATOR );
    Store_in_list(OPERATOR,"$");
}
void addToken(enum Category type)
{
    //int len = strlen(p);
    if (type == DelAll)
    {
        *p = '\0';
        p = value;
        return;
    } else
    if(type == Space){
        p--;
        return;
    } else
    if (type == FUNC_NAME)
    {
        printf("%s %d 0\n"," ", type/*, len*/);
        printf("function begin!\n");
        Store_in_list(type,"");
        enum Category raw = RAW_TEXT;
        *p = '\0';
        p = value;
        Store_in_list(raw,p);
        printf("%s %d 0\n", p, raw/*, len*/);
    } else
    if (type == PROPERTY_NAME)
    {
        printf("%s %d 0\n"," ", type/*, len*/);
        printf("properity begin!\n");
        Store_in_list(type,"");
        enum Category raw = RAW_TEXT;
        *p = '\0';
        p = value;
        Store_in_list(raw,p);
        printf("%s %d 0\n", p, raw/*, len*/);
    } else
    if (type == COLOR)
    {
        *p = '\0';
        //printf("!!!!color length is :%d\n",strlen(value) );
        if (strlen(value) == 4)
        {
            *p++ = value[2];
            *p++ = value[3];
            *p++ = value[3];
            *p = '\0';
            value[3] = value[2];
            value[2] = value[1];
            p = value;
            Store_in_list(type,p);
            printf("%s %d 0\n", p, type/*, len*/);
        }
        else{
            p =value;
            Store_in_list(type,p);
            printf("%s %d 0\n", p, type/*, len*/);
        }

    }
    else{
       *p = '\0';
        p = value;
        Store_in_list(type,p);
        printf("%s %d 0\n", p, type/*, len*/);
    }
}

void rollback() {
    p--;
    ungetc(c, stdin);
}

void delSpace() {
    *p = '\0';
    p--;
    //column --;
}

void print_list(WORD_LIST **head){
    WORD_LIST *p = (*head)->next;
    printf("what list contain:\n");
    while(p!=NULL)
    {
        printf("content:%s\n",p->content );
        p = p->next;
    }
}

void spilt(const char* _argv)
{
    if(!freopen(_argv,"r",stdin)){
        printf("cannot read file!");
        return ;
    }
    char is_float = 'n';
    int left = 1;
    int right = 0;
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
                            case ' ':
                                rollback();
                                addToken(OPERATOR);
                        }
                        break;
                    case '@':
                        s = VariableState;
                        if (value[0]=='@')
                        {
                            addToken(VAR_DEFINE);
                        }
                        else{
                            rollback();
                            addToken(RAW_TEXT);
                            nextchar();
                            addToken(VAR_DEFINE);
                        }

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
                        if (value[0]=='{')
                        {
                            addToken(BLOCK_BEGIN);
                        }
                        else{
                            rollback();
                            addToken(RAW_TEXT);
                            nextchar();
                            addToken(BLOCK_BEGIN);
                        }
                        //s = ElementState;
                        break;
                    case '}':
                        addToken(BLOCK_END);
                        //nextchar();
                        //s = BeginState;
                        break;
                    case '\n': case '\r':
                        addToken(DelAll);
                        //nextchar();
                        //s = BeginState;
                        break;
                    case ' ':
                        addToken(Space);
                        //nextchar();
                        break;
                    case '+':case '*':
                        addToken(OPERATOR);
                        //nextchar();
                        break;
                    case '-':
                        nextchar();
                        switch(c) {
                            case ' ':
                                rollback();
                                addToken(OPERATOR);
                                //nextchar();
                                break;
                            default:
                                break;
                            /*case '/':
                                nextchar();
                                s = AnnotationDelState;
                                break;*/
                            }
                            break;
                    case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
                        s = NumState;
                        break;
                    case ':':
                        rollback();
                        addToken(PROPERTY_NAME);
                        nextchar();
                        addToken(ASSIGN);
                        nextchar();
                        s = ValueState;
                        break;
                    case '(':
                        if(value[0]=='('){
                            addToken(OPERATOR);
                        }
                        else{
                            rollback();
                            addToken(FUNC_NAME);
                            nextchar();
                            addToken(FUNC_ARGUMENT_BEGIN);
                            s = ArgumentState;
                        }
                        break;
                    case ')':
                        if(value[0]==')'){
                            addToken(FUNC_ARGUMENT_END);
                        }
                        else{
                            rollback();
                            addToken(RAW_TEXT);
                            nextchar();
                            addToken(OPERATOR);
                            switch(c){
                            case ';':
                                addToken(SEPARATOR);
                                break;
                            default:
                                break;
                        }
                        }
                        break;
                        /*
                    case '=':
                        rollback();
                        addToken(Argument_name);
                        nextchar();
                        addToken(Block);
                        nextchar();
                        s = ArgumentState;
                        break;
                        */
                    case ';':
                        addToken(SEPARATOR);
                        break;
                    default:
                        //nextchar();
                        //addToken(Space);
                        //s = BeginState;
                        break;
                }
                break;
            case AnnotationState:
                switch(c){
                    case '*':
                        nextchar();
                        switch(c){
                            case '/':
                                addToken(COMMENT);
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
                        addToken(RAW_TEXT);
                        nextchar();
                        addToken(ASSIGN);
                        nextchar();
                        s = ValueState;
                        break;
                    case ' ':
                        addToken(Space);
                        nextchar();
                        break;
                    case '@':
                        if (value[0]=='@')
                        {
                            addToken(VAR_DEFINE);
                            nextchar();
                        }
                        else{
                            rollback();
                            addToken(RAW_TEXT);
                            nextchar();
                            addToken(VAR_DEFINE);
                            nextchar();
                        }
                        break;
                    case '{':
                        addToken(BLOCK_BEGIN);
                        nextchar();
                        break;
                    case '}':
                        rollback();
                        addToken(RAW_TEXT);
                        nextchar();
                        addToken(BLOCK_END);
                        nextchar();
                        break;
                        //s = BeginState;
                    case ';':
                        rollback();
                        addToken(RAW_TEXT);
                        nextchar();
                        addToken(SEPARATOR);
                        s = BeginState;
                        break;
                    //test!!!!!!!!!!
                    case '+':case '*':
                        if (value[0]=='+'||value[0]=='*')
                        {
                            addToken(OPERATOR);
                        }
                        else if(value[0]<='9'&&value[0]>='0'){
                            rollback();
                            if (is_float == 'y')
                            {
                                is_float = 'n';
                                addToken(FLOAT);
                            }
                            else
                                addToken(INT);
                            nextchar();
                            addToken(OPERATOR);
                        }
                        else{
                            rollback();
                            addToken(RAW_TEXT);
                            nextchar();
                            addToken(OPERATOR);
                        }
                        nextchar();
                        break;
                    //test!!!!!!!!!!!!!
                    case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
                        if(value[0]>='0'&&value[0]<='9')
                            s = NumState;
                        else
                            nextchar();
                        break;
                    case ')':
                        if(value[0]==')'){
                            addToken(RAW_TEXT);
                            s = BeginState;
                        }
                        else{
                            if (is_float == 'e')
                            {
                                rollback();
                                addToken(RAW_TEXT);
                                nextchar();
                                addToken(RAW_TEXT);
                                s = BeginState;
                            }
                            else
                                nextchar();
                        }
                        break;
                    case ',':
                        rollback();
                        addToken(RAW_TEXT);
                        nextchar();
                        addToken(COMMA);
                        nextchar();
                        s  = ArgumentState;         //test!!!
                        break;
                    default:
                        nextchar();
                }
                break;
            case ValueState:
                switch(c){
                    case ' ':
                        addToken(Space);
                        nextchar();
                        //s = ValueState;
                        break;
                    case ';':
                        rollback();
                        if(value[0]!=' '&&p!=value){
                            //printf("value is %s",value);
                            addToken(RAW_TEXT);
                        }
                        else
                            addToken(DelAll);
                        nextchar();
                        addToken(SEPARATOR);
                        s = BeginState;
                        break;
                    case '+':case '*':
                        if (value[0]=='+'||value[0]=='*')
                        {
                            addToken(OPERATOR);
                        }
                        else if(value[0]<='9'&&value[0]>='0'){
                            rollback();
                            if (is_float == 'y')
                            {
                                addToken(FLOAT);
                            }
                            else
                                addToken(INT);
                            nextchar();
                            addToken(OPERATOR);
                        }
                        else{
                            rollback();
                            addToken(RAW_TEXT);
                            nextchar();
                            addToken(OPERATOR);
                        }
                        nextchar();
                        break;
                    case '-':
                        nextchar();
                        switch(c) {
                            case ' ':
                                rollback();
                                addToken(OPERATOR);
                                //nextchar();
                                break;
                            default:
                                break;
                            }
                            break;
                    case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
                        if (value[0]>='0'&&value[0]<='9')
                        {
                            s = NumState;
                        }
                        else
                            nextchar();
                        break;
                    case '#':
                        s = ColorState;
                        break;

                    case '\"':
                        if ((value[0] >='a'&&value[0]<='z')||(value[0]>='A'&&value[0]<='Z')||value[0]=='_')
                        {
                            nextchar();
                        }
                        else{
                            s = StringState;
                            addToken(Space);
                            nextchar();
                        }
                        break;
                    /*
                    case '(':
                        if(value[0]=='('){
                            addToken(OPERATOR);
                            s = BeginState;
                        }
                        else{
                            rollback();
                            addToken(FUNC_NAME);
                            nextchar();
                            addToken(Block);
                            s = BeginState;
                        }
                        break;
                        */
                    case ')':
                        if(value[0]==')'){
                            addToken(RAW_TEXT);
                            s = BeginState;
                        }
                        else{
                            rollback();
                            addToken(RAW_TEXT);
                            nextchar();
                            addToken(RAW_TEXT);
                            s = BeginState;
                        }
                        break;

                    case ',':
                        rollback();
                        addToken(RAW_TEXT);                //not sure,test,then
                        nextchar();
                        addToken(COMMA);
                        nextchar();                        //test for 7.less from gyz
                        break;
                    case '@':
                        s = VariableState;
                        is_float = 'e';
                        rollback();
                        if (p != value)
                        {
                            addToken(RAW_TEXT);
                            nextchar();
                        }
                        else{
                            nextchar();
                        }
                        addToken(VAR_DEFINE);
                        nextchar();
                        break;
                    case '}':
                        rollback();
                        addToken(RAW_TEXT);
                        nextchar();
                        addToken(BLOCK_END);
                    default:
                        nextchar();
                        //s = ValueState;
                }
                break;
            case ClassState:
                switch(c){
                    case ' ':
                        while(c == ' ')
                        {
                            nextchar();
                        }
                        switch(c){
                            case '{':
                                rollback();
                                p--;
                                while(*p == ' '){
                                    p--;
                                }
                                p++;
                                addToken(SELECTOR_NAME);
                                nextchar();
                                addToken(BLOCK_BEGIN);
                                s = BeginState;
                                break;
                            case '(':
                                rollback();
                                p--;
                                while(*p == ' '){
                                    p--;
                                }
                                p++;
                                addToken(FUNC_NAME);
                                nextchar();
                                addToken(FUNC_ARGUMENT_BEGIN);
                                s = ArgumentState;
                                break;
                            default:
                                nextchar();
                        }
                        break;
                    case '{':
                        rollback();
                        addToken(SELECTOR_NAME);
                        s = BeginState;
                        break;
                    case ';':
                        rollback();
                        addToken(SELECTOR_NAME);
                        nextchar();
                        addToken(SEPARATOR);
                        s = BeginState;
                        break;
                    case '(':
                        rollback();
                        s = BeginState;
                        break;
                    case ',':
                        rollback();
                        addToken(SELECTOR_NAME);
                        nextchar();
                        addToken(COMMA);
                        s = BeginState;
                        break;
                    default:
                        nextchar();
                    }
                break;
            case IDState:
                switch(c){
                    case ' ':
                        rollback();
                        addToken(SELECTOR_NAME);
                        nextchar();
                        addToken(Space);
                        s = BeginState;
                        break;
                    case ';':
                        rollback();
                        addToken(SELECTOR_NAME);
                        nextchar();
                        addToken(SEPARATOR);
                        s = BeginState;
                        break;
                    case '{':
                        rollback();
                        addToken(SELECTOR_NAME);
                        s = BeginState;
                        break;
                    case ',':
                        rollback();
                        addToken(SELECTOR_NAME);
                        nextchar();
                        addToken(COMMA);
                        s = BeginState;
                        break;
                    default:
                        nextchar();
                }
                break;
            case AnnotationDelState:
                if(c == '\n' || c == '\r'){
                    addToken(DelAll);
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
                            addToken(PROPERTY_NAME);
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
                        addToken(PROPERTY_NAME);
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
                            //s = ElementState;
                }
                break;

            case KeyState:
                break;

            case NumState:
                switch(c){
                    case ' ':
                        rollback();
                        if(is_float == 'y'){
                            addToken(FLOAT);
                        }
                        else
                            addToken(INT);
                        nextchar();
                        addToken(DelAll);
                        s = ValueState;
                        break;
                    case ';':
                        rollback();
                        if (p==value)
                        {
                            s = BeginState;
                        }
                        else{
                            if (is_float == 'y')
                            {
                                addToken(FLOAT);
                                is_float = 'n';
                            }
                            else
                            addToken(INT);
                        }
                        nextchar();
                        addToken(SEPARATOR);
                        s = BeginState;
                        break;
                    case ',':
                        rollback();
                        if (is_float == 'y')
                        {
                            addToken(FLOAT);
                            is_float = 'n';
                        }
                        else
                            addToken(INT);
                        nextchar();
                        addToken(COMMA);
                        nextchar();
                        s = ArgumentState;
                        break;
                    case ')':
                        if(value[0]==')'){
                            addToken(FUNC_ARGUMENT_END);
                            s = BeginState;
                        }
                        else{
                            rollback();
                            if (is_float == 'y')
                            {
                                addToken(FLOAT);
                                is_float = 'n';
                            }
                            else
                                addToken(INT);
                            nextchar();
                            addToken(FUNC_ARGUMENT_END);
                            s = BeginState;
                        }
                        break;
                    case '@':
                        if (value[0]=='@')
                        {
                            addToken(VAR_DEFINE);
                            nextchar();
                        }
                        else{
                            rollback();
                            addToken(RAW_TEXT);
                            nextchar();
                            addToken(VAR_DEFINE);
                            nextchar();
                        }
                        break;
                    case '.':
                        is_float = 'y';
                        nextchar();
                        break;
                    case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
                        nextchar();
                        break;
                    //!!!!!!test
                    case '+':case '*':
                        if (value[0]=='+'||value[0]=='*')
                        {
                            addToken(OPERATOR);
                        }
                        else if(value[0]<='9'&&value[0]>='0'){
                            rollback();
                            if (is_float == 'y')
                            {
                                addToken(FLOAT);
                            }
                            else
                                addToken(INT);
                            nextchar();
                            addToken(OPERATOR);
                        }
                        else{
                            rollback();
                            addToken(RAW_TEXT);
                            nextchar();
                            addToken(OPERATOR);
                        }
                        nextchar();
                        break;
                    case '-':
                        nextchar();
                        switch(c) {
                            case ' ':
                                rollback();
                                addToken(OPERATOR);
                                //nextchar();
                                break;
                            default:
                                break;
                            }
                            break;
                    //!!!!!!test
                    //handle px:
                    case 'p':
                        if (value[0]>='0'&&value[0]<='9')
                        {
                            rollback();
                            addToken(PX);
                            nextchar();
                            nextchar();
                            addToken(DelAll);
                            nextchar();
                        }
                        break;
                    default:
                        nextchar();
                        s = ValueState;
                }
                break;
            case ColorState:
                switch(c){
                    case ';':
                        rollback();
                        addToken(COLOR);
                        nextchar();
                        addToken(SEPARATOR);
                        s = BeginState;
                        break;
                    case ' ':
                        addToken(Space);
                        nextchar();
                        break;
                    default:
                        nextchar();
                }
                break;
            case ArgumentState:
                switch(c){
                    case ')':
                        rollback();
                        addToken(RAW_TEXT);                     //not sure,for test
                        nextchar();
                        addToken(FUNC_ARGUMENT_END);
                        nextchar();
                        switch(c){
                            case ';':
                                addToken(SEPARATOR);
                                break;
                            case ' ':
                                addToken(DelAll);
                            default:
                                break;
                        }
                        s = BeginState;
                        break;
                    case ';':
                    case ',':
                        rollback();
                        if ((value[0]>='0'&&value[0]<='9')||value[0] == '.')
                        {
                            if (is_float == 'y')
                        {
                            addToken(FLOAT);
                            is_float = 'n';
                        }
                        else
                            addToken(INT);
                        }
                        else{
                            addToken(RAW_TEXT);
                        }
                            nextchar();

                        nextchar();
                        addToken(COMMA);
                        nextchar();
                        break;
                    case '@':
                        addToken(VAR_DEFINE);
                        nextchar();
                        break;
                    case ' ':
                        addToken(Space);
                        nextchar();
                        break;
                    case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
                        if (value[0]>='0'&&value[0]<='9')
                        {
                            s = NumState;
                        }
                        else
                            nextchar();
                        break;
                    case ':':
                        rollback();
                        addToken(RAW_TEXT);
                        nextchar();
                        addToken(ASSIGN);
                        nextchar();
                        s = ValueState;
                        break;
                    default:
                        nextchar();
                }
                break;
            case FuncState:
                switch(c){

                }
            case StringState:
                switch(c){
                    case '\\':
                        nextchar();
                        if (c == '\"')
                        {
                            rollback();
                            addToken(STRING);
                            nextchar();
                            addToken(DelAll);
                            s = ValueState;
                        }
                        else{
                            nextchar();
                        }
                        break;
                    case '\"':
                        rollback();
                        addToken(STRING);
                        nextchar();
                        addToken(DelAll);
                        nextchar();
                        s = ValueState;
                        break;
                    case '@':
                        nextchar();
                        switch(c){
                            case '{':
                                rollback();
                                //ungetc('@',stdin);
                                p--;
                                addToken(STRING);
                                //printf("predict\n");
                                ungetc('@',stdin);
                                nextchar();
                                nextchar();
                                s = QuoteState;
                                //addToken(STRING);
                                //printf("haha\n");
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        nextchar();
                }
                break;
            case QuoteState:
                switch(c){
                    case '\"':
                        //p--;
                        *p = '\0';
                        p = value;
                        printf("at this point right is%d,left is %d\n",right,left);
                        hanle_varible(p,right,left);
                        right = 0;
                        left = 1;
                        *p = '\0';
                        nextchar();
                        s = ValueState;
                        break;
                    case '}':
                        right++;
                        if(right==1 && left == 2)
                            printf("bingo!\n");
                        nextchar();
                        break;
                    case '@':
                        nextchar();
                        switch(c){
                            case '{':
                                left++;
                                nextchar();
                                break;
                            default:
                                nextchar();
                        }
                        break;
                    default:
                        nextchar();
                }
                break;
            default:
                nextchar();
        }

    }

}


