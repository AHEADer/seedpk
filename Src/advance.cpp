#include "include/advance.h"

char value[64];
char c , *p = value;

token_list_elem *pp;


char* create_string(char* head, char* end)
{
    char a = *end;
    *end = '\0';
    char b[64],*aa = b;
    strcpy(b,head);
    int i = strlen(b);
    b[++i] = a;
    b[i] = '\0';
    //printf("create_string:%s\n",b );
    return aa;
}

void Store_in_list(token_list_elem::TYPE type, const char* value)
{
    while(pp->next != NULL){
        pp = pp->next;
    }
    pp->next = (token_list_elem*)malloc(sizeof(*pp));
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
    char have = 'n';
    if (left > right)
    {
        i = left - right;
        while(*pp!='\0'){
            switch(*pp){
                case '@':
                    if (have == 'y')
                    {
                        Store_in_list(token_list_elem::token_list_elem::OPERATOR,"$");
                        Store_in_list(token_list_elem::token_list_elem::STRING,create_string(value,pp));
                    }
                    add = 1;
                    break;
                case '{':
                    if (add == 1&&i>0)
                    {
                        have = 'y';
                        i--;

                    }
                    break;
                case '\"':
                    if (have == 'y')
                    {
                        Store_in_list(token_list_elem::token_list_elem::OPERATOR,"$");
                        printf("%s %d 0\n","$", token_list_elem::token_list_elem::OPERATOR/*, len*/);
                        Store_in_list(token_list_elem::token_list_elem::STRING,create_string(value,pp));
                        printf("%s %d 0\n",create_string(value,pp), token_list_elem::token_list_elem::STRING/*, len*/);
                    }
                    break;
                default:
                    add = -1;
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
    printf("addLink: %s %d\n","$",token_list_elem::token_list_elem::OPERATOR );
    Store_in_list(token_list_elem::token_list_elem::OPERATOR,"$");
}
void addToken(token_list_elem::TYPE type)
{
    //int len = strlen(p);
    if (type == token_list_elem::token_list_elem::DelAll)
    {
        *p = '\0';
        p = value;
        return;
    } else
    if(type == token_list_elem::token_list_elem::Space){
        p--;
        return;
    } else
    if (type == token_list_elem::token_list_elem::FUNC_NAME)
    {
        printf("%s %d 0\n"," ", type/*, len*/);
        printf("function begin!\n");
        Store_in_list(type,"");
        token_list_elem::TYPE raw = token_list_elem::token_list_elem::RAW_TEXT;
        *p = '\0';
        p = value;
        Store_in_list(raw,p);
        printf("%s %d 0\n", p, raw/*, len*/);
    } else
    if (type == token_list_elem::token_list_elem::PROPERTY_NAME)
    {
        printf("%s %d 0\n"," ", type/*, len*/);
        printf("properity begin!\n");
        Store_in_list(type,"");
        token_list_elem::TYPE raw = token_list_elem::token_list_elem::RAW_TEXT;
        *p = '\0';
        p = value;
        Store_in_list(raw,p);
        printf("%s %d 0\n", p, raw/*, len*/);
    } else
    if (type == token_list_elem::token_list_elem::COLOR)
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

void print_list(token_list_elem *head){
    token_list_elem *p = head->next;
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
                                addToken(token_list_elem::token_list_elem::OPERATOR);
                        }
                        break;
                    case '@':
                        s = VariableState;
                        addToken(token_list_elem::token_list_elem::VAR_DEFINE);
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
                        addToken(token_list_elem::token_list_elem::BLOCK_BEGIN);
                        //s = ElementState;
                        break;
                    case '}':
                        addToken(token_list_elem::BLOCK_END);
                        //nextchar();
                        //s = BeginState;
                        break;
                    case '\n': case '\r':
                        addToken(token_list_elem::token_list_elem::DelAll);
                        //nextchar();
                        //s = BeginState;
                        break;
                    case ' ':
                        addToken(token_list_elem::token_list_elem::Space);
                        //nextchar();
                        break;
                    case '+':case '*':
                        addToken(token_list_elem::token_list_elem::OPERATOR);
                        //nextchar();
                        break;
                    case '-':
                        nextchar();
                        switch(c) {
                            case ' ':
                                rollback();
                                addToken(token_list_elem::token_list_elem::OPERATOR);
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
                        addToken(token_list_elem::token_list_elem::PROPERTY_NAME);
                        nextchar();
                        addToken(token_list_elem::token_list_elem::ASSIGN);
                        nextchar();
                        s = ValueState;
                        break;
                    case '(':
                        if(value[0]=='('){
                            addToken(token_list_elem::token_list_elem::OPERATOR);
                        }
                        else{
                            rollback();
                            addToken(token_list_elem::token_list_elem::FUNC_NAME);
                            nextchar();
                            addToken(token_list_elem::token_list_elem::FUNC_ARGUMENT_BEGIN);
                            s = ArgumentState;
                        }
                        break;
                    case ')':
                        if(value[0]==')'){
                            addToken(token_list_elem::token_list_elem::FUNC_ARGUMENT_END);
                        }
                        else{
                            rollback();
                            addToken(token_list_elem::token_list_elem::RAW_TEXT);
                            nextchar();
                            addToken(token_list_elem::token_list_elem::OPERATOR);
                            switch(c){
                            case ';':
                                addToken(token_list_elem::token_list_elem::SEPARATOR);
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
                        addToken(token_list_elem::token_list_elem::SEPARATOR);
                        break;
                    default:
                        //nextchar();
                        //addToken(token_list_elem::Space);
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
                                addToken(token_list_elem::COMMENT);
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
                        addToken(token_list_elem::token_list_elem::RAW_TEXT);
                        nextchar();
                        addToken(token_list_elem::token_list_elem::ASSIGN);
                        nextchar();
                        s = ValueState;
                        break;
                    case ' ':
                        addToken(token_list_elem::token_list_elem::Space);
                        nextchar();
                        break;
                    case '@':
                        addToken(token_list_elem::token_list_elem::VAR_DEFINE);
                        nextchar();
                        break;
                    case '{':
                        addToken(token_list_elem::token_list_elem::BLOCK_BEGIN);
                        nextchar();
                        break;
                    case '}':
                        rollback();
                        addToken(token_list_elem::token_list_elem::RAW_TEXT);
                        nextchar();
                        addToken(token_list_elem::BLOCK_END);
                        nextchar();
                        s = BeginState;
                    case ';':
                        rollback();
                        addToken(token_list_elem::token_list_elem::RAW_TEXT);
                        nextchar();
                        addToken(token_list_elem::token_list_elem::SEPARATOR);
                        s = BeginState;
                        break;
                    //test!!!!!!!!!!
                    case '+':case '*':
                        if (value[0]=='+'||value[0]=='*')
                        {
                            addToken(token_list_elem::token_list_elem::OPERATOR);
                        }
                        else if(value[0]<='9'&&value[0]>='0'){
                            rollback();
                            if (is_float == 'y')
                            {
                                is_float = 'n';
                                addToken(token_list_elem::FLOAT);
                            }
                            else
                                addToken(token_list_elem::INT);
                            nextchar();
                            addToken(token_list_elem::OPERATOR);
                        }
                        else{
                            rollback();
                            addToken(token_list_elem::token_list_elem::RAW_TEXT);
                            nextchar();
                            addToken(token_list_elem::OPERATOR);
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
                            addToken(token_list_elem::token_list_elem::RAW_TEXT);
                            s = BeginState;
                        }
                        else{
                            if (is_float == 'e')
                            {
                                rollback();
                                addToken(token_list_elem::token_list_elem::RAW_TEXT);
                                nextchar();
                                addToken(token_list_elem::token_list_elem::RAW_TEXT);
                                s = BeginState;
                            }
                            else
                                nextchar();
                        }
                        break;
                    case ',':
                        rollback();
                        addToken(token_list_elem::token_list_elem::RAW_TEXT);
                        nextchar();
                        addToken(token_list_elem::COMMA);
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
                        addToken(token_list_elem::Space);
                        nextchar();
                        //s = ValueState;
                        break;
                    case ';':
                        rollback();
                        if(value[0]!=' '&&p!=value){
                            //printf("value is %s",value);
                            addToken(token_list_elem::token_list_elem::RAW_TEXT);
                        }
                        else
                            addToken(token_list_elem::DelAll);
                        nextchar();
                        addToken(token_list_elem::SEPARATOR);
                        s = BeginState;
                        break;
                    case '+':case '*':
                        if (value[0]=='+'||value[0]=='*')
                        {
                            addToken(token_list_elem::OPERATOR);
                        }
                        else if(value[0]<='9'&&value[0]>='0'){
                            rollback();
                            if (is_float == 'y')
                            {
                                addToken(token_list_elem::FLOAT);
                            }
                            else
                                addToken(token_list_elem::INT);
                            nextchar();
                            addToken(token_list_elem::OPERATOR);
                        }
                        else{
                            rollback();
                            addToken(token_list_elem::token_list_elem::RAW_TEXT);
                            nextchar();
                            addToken(token_list_elem::OPERATOR);
                        }
                        nextchar();
                        break;
                    case '-':
                        nextchar();
                        switch(c) {
                            case ' ':
                                rollback();
                                addToken(token_list_elem::OPERATOR);
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
                            addToken(token_list_elem::Space);
                            nextchar();
                        }
                        break;
                    /*
                    case '(':
                        if(value[0]=='('){
                            addToken(token_list_elem::OPERATOR);
                            s = BeginState;
                        }
                        else{
                            rollback();
                            addToken(token_list_elem::FUNC_NAME);
                            nextchar();
                            addToken(Block);
                            s = BeginState;
                        }
                        break;
                        */
                    case ')':
                        if(value[0]==')'){
                            addToken(token_list_elem::token_list_elem::RAW_TEXT);
                            s = BeginState;
                        }
                        else{
                            rollback();
                            addToken(token_list_elem::RAW_TEXT);
                            nextchar();
                            addToken(token_list_elem::token_list_elem::RAW_TEXT);
                            s = BeginState;
                        }
                        break;

                    case ',':
                        rollback();
                        addToken(token_list_elem::token_list_elem::RAW_TEXT);                //not sure,test,then
                        nextchar();
                        addToken(token_list_elem::COMMA);
                        s = BeginState;
                        break;
                    case '@':
                        s = VariableState;
                        is_float = 'e';
                        rollback();
                        if (p != value)
                        {
                            addToken(token_list_elem::token_list_elem::RAW_TEXT);
                            nextchar();
                        }
                        else{
                            nextchar();
                        }
                        addToken(token_list_elem::VAR_DEFINE);
                        nextchar();
                        break;
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
                                addToken(token_list_elem::SELECTOR_NAME);
                                nextchar();
                                addToken(token_list_elem::BLOCK_BEGIN);
                                s = BeginState;
                                break;
                            case '(':
                                rollback();
                                p--;
                                while(*p == ' '){
                                    p--;
                                }
                                p++;
                                addToken(token_list_elem::FUNC_NAME);
                                nextchar();
                                addToken(token_list_elem::FUNC_ARGUMENT_BEGIN);
                                s = ArgumentState;
                                break;
                            default:
                                nextchar();
                        }
                        break;
                    case '{':
                        rollback();
                        addToken(token_list_elem::SELECTOR_NAME);
                        s = BeginState;
                        break;
                    case ';':
                        rollback();
                        addToken(token_list_elem::SELECTOR_NAME);
                        nextchar();
                        addToken(token_list_elem::SEPARATOR);
                        s = BeginState;
                        break;
                    case '(':
                        rollback();
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
                        addToken(token_list_elem::SELECTOR_NAME);
                        nextchar();
                        addToken(token_list_elem::Space);
                        s = BeginState;
                        break;
                    case '{':
                        rollback();
                        addToken(token_list_elem::SELECTOR_NAME);
                        s = BeginState;
                        break;
                    default:
                        nextchar();
                }
                break;
            case AnnotationDelState:
                if(c == '\n' || c == '\r'){
                    addToken(token_list_elem::DelAll);
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
                            addToken(token_list_elem::PROPERTY_NAME);
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
                        addToken(token_list_elem::PROPERTY_NAME);
                        nextchar();
                        nextchar();
                        s = KeyState;
                        break;
                    default:
                            rollback();
                            if(c==' '||c=='\n'||c=='\r')
                                addToken(token_list_elem::Space);
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
                            addToken(token_list_elem::FLOAT);
                        }
                        else
                            addToken(token_list_elem::INT);
                        nextchar();
                        addToken(token_list_elem::DelAll);
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
                                addToken(token_list_elem::FLOAT);
                                is_float = 'n';
                            }
                            else
                            addToken(token_list_elem::INT);
                        }
                        nextchar();
                        addToken(token_list_elem::SEPARATOR);
                        s = BeginState;
                        break;
                    case ',':
                        rollback();
                        if (is_float == 'y')
                        {
                            addToken(token_list_elem::FLOAT);
                            is_float = 'n';
                        }
                        else
                            addToken(token_list_elem::INT);
                        nextchar();
                        addToken(token_list_elem::COMMA);
                        nextchar();
                        s = ArgumentState;
                        break;
                    case ')':
                        if(value[0]==')'){
                            addToken(token_list_elem::FUNC_ARGUMENT_END);
                            s = BeginState;
                        }
                        else{
                            rollback();
                            if (is_float == 'y')
                            {
                                addToken(token_list_elem::FLOAT);
                                is_float = 'n';
                            }
                            else
                                addToken(token_list_elem::INT);
                            nextchar();
                            addToken(token_list_elem::FUNC_ARGUMENT_END);
                            s = BeginState;
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
                            addToken(token_list_elem::OPERATOR);
                        }
                        else if(value[0]<='9'&&value[0]>='0'){
                            rollback();
                            if (is_float == 'y')
                            {
                                addToken(token_list_elem::FLOAT);
                            }
                            else
                                addToken(token_list_elem::INT);
                            nextchar();
                            addToken(token_list_elem::OPERATOR);
                        }
                        else{
                            rollback();
                            addToken(token_list_elem::token_list_elem::RAW_TEXT);
                            nextchar();
                            addToken(token_list_elem::OPERATOR);
                        }
                        nextchar();
                        break;
                    case '-':
                        nextchar();
                        switch(c) {
                            case ' ':
                                rollback();
                                addToken(token_list_elem::OPERATOR);
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
                            addToken(token_list_elem::PX);
                            nextchar();
                            nextchar();
                            addToken(token_list_elem::DelAll);
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
                        addToken(token_list_elem::COLOR);
                        nextchar();
                        addToken(token_list_elem::SEPARATOR);
                        s = BeginState;
                        break;
                    case ' ':
                        addToken(token_list_elem::Space);
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
                        addToken(token_list_elem::token_list_elem::RAW_TEXT);                     //not sure,for test
                        nextchar();
                        addToken(token_list_elem::FUNC_ARGUMENT_END);
                        nextchar();
                        switch(c){
                            case ';':
                                addToken(token_list_elem::SEPARATOR);
                                break;
                            case ' ':
                                addToken(token_list_elem::DelAll);
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
                            addToken(token_list_elem::FLOAT);
                            is_float = 'n';
                        }
                        else
                            addToken(token_list_elem::INT);
                        }
                        else{
                            addToken(token_list_elem::token_list_elem::RAW_TEXT);
                        }
                            nextchar();

                        nextchar();
                        addToken(token_list_elem::COMMA);
                        nextchar();
                        break;
                    case '@':
                        addToken(token_list_elem::VAR_DEFINE);
                        nextchar();
                        break;
                    case ' ':
                        addToken(token_list_elem::Space);
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
                        addToken(token_list_elem::token_list_elem::RAW_TEXT);
                        nextchar();
                        addToken(token_list_elem::ASSIGN);
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
                            addToken(token_list_elem::STRING);
                            nextchar();
                            addToken(token_list_elem::DelAll);
                            s = ValueState;
                        }
                        else{
                            nextchar();
                        }
                        break;
                    case '\"':
                        rollback();
                        addToken(token_list_elem::STRING);
                        nextchar();
                        addToken(token_list_elem::DelAll);
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
                                addToken(token_list_elem::STRING);
                                printf("predict\n");
                                ungetc('@',stdin);
                                nextchar();
                                nextchar();
                                s = QuoteState;
                                //addToken(token_list_elem::STRING);
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
                        hanle_varible(value,right,left);
                        s = ValueState;
                        break;
                    case '}':
                        right++;
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

