#include "include/eval.h"

char *_sstrjoin(char *buf, const char *delim, ...)
{
    char *p, *res;
    const char*d;
    va_list ap;
    va_start(ap, delim);
    res = buf;
    p = va_arg(ap, char *);
    while(p)
    {
        while((*res++ = *p++))
            /* do nothing */;
        res--;
        if((p = va_arg(ap, char *)))
        {
            d = delim;
            while((*res++ = *d++))
                /* do nothing */;
            res--;
        }
    }
    *res = '\0';
    va_end(ap);
    return buf;
}


void  DealString(char *OperatorArr, double *DigitalArr, int CurrPosition)
{
    int iFlag = 0;
    int iOperatorArrLen = strlen(OperatorArr);
    for (iFlag = CurrPosition; iFlag < iOperatorArrLen; iFlag ++)   /*将已经运算过的符号，空出来的位置用后面的符号来填充，即把乘和除号的位置用后面的加和减号填充*/
    {
        OperatorArr[iFlag] = OperatorArr[iFlag+1];
        DigitalArr[iFlag] = DigitalArr[iFlag+1];
    }
    DigitalArr[iFlag] = DigitalArr[iFlag+1];
    OperatorArr[iOperatorArrLen-1] = '\0';
}


double DealNumber(const char *Str)
{
    double dValueReturn = 0.0;
    double dFlag = 1.0;
    int    iLoop = 0;
    int    iFloat = 0;
    int    iStrLen = 0;
    char   tempStr[100] = {0};
    int    iTempi = 0;
    int    iStart = 0;
    int    iFlag = 1;            /*正负符号指示器，若为1则为正数，为－1，此数为负数*/

    iStrLen = strlen(Str);

    if (Str[0] == '-')
    {
        iStart = 1;
        iFlag = -1;
    }


    for (iLoop = iStart; iLoop < iStrLen; iLoop ++)
    {
        if (Str[iLoop] == '.')
        {
            iFloat = iLoop;
            break;
        }
        tempStr[iTempi++] = Str[iLoop];  /*将整数部分复制到tempStr[]中*/
    }
    tempStr[iTempi] = '\0';


    if (iFloat != 0)
    {
        for (iLoop = iFloat+1; iLoop < iStrLen; iLoop ++) /*将小数部分计算出来*/
        {
            if (Str[iLoop] == '.')  /*如果有多余的小数点，则表示输入错误*/
            {
                printf("There is more that one dot '.' in number!error!!!\n");
                exit(0);
            }
            dFlag = dFlag * 0.1;
            dValueReturn += (dFlag * (Str[iLoop]-48));
        }
    }


    dFlag = 1.0;


    iStrLen = strlen(tempStr);           /*计算整数部分*/
    for (iLoop = iStrLen-1; iLoop >= 0; iLoop --)
    {
        dValueReturn = dValueReturn + (dFlag * (tempStr[iLoop] - 48));
        dFlag *= 10;
    }


    dValueReturn = dValueReturn * iFlag;
    return dValueReturn;
}


double ComputeString(const char *Str)           /*可递归函数*/
{                                /*取得数值字符串，并调用DealNumber转换成double*/
    char cStr[100] = {'\0'}, cTotalChar[30] = {'\0'};   /*cStr保存当前的表达式串,cTotalChar保存一个数的所有字符*/
    char cStack[80] = {'\0'};                  /*保存所有的符号的堆栈*/
    int iCharPos = 0;                   /*保存符号的位置指针*/
    double dTotalNum[80] = {0.0};     /*保存当前所有的数的一个堆栈*/
    int iDigitalPos = 0;              /*保存数字位置指针*/
    int iBracketFlag = 0;             /*若iBracketFlag=1则表示有一对括号*/
    int iCharInBracketNum = 0, iCharInNumberNum = 0;  /*iCharInBracketNum保存新括号内的字符数, iCharInNumberNum保存number里的字符位置*/
    int iNumOfMulAndDiv = 0;                         /*乘除符号数量*/
    int iNumOfAddAndDec = 0;                         /*加减符号数量*/


    while ((*Str) != '\0')           /*当p==1 和k==0时，表示已经把括号里的内容全部复制到g[100]中了*/
    {
        iBracketFlag = 0;
        iCharInBracketNum = 0;

        switch (*Str)
        {
            case '+':            /*当前字符为加减乘除时则表示*/
            case '-':
            case '*':
            case '/':
                {
                    cStack[iCharPos++] = (*Str);
                    if(((*Str) == '*') || ((*Str) == '/'))
                    {
                        iNumOfMulAndDiv ++;
                    }
                    else
                    {
                        iNumOfAddAndDec ++;
                    }
                    if((*(Str-1)) != ')')
                    {
                        cTotalChar[iCharInNumberNum] = '\0';
                        iCharInNumberNum = 0;             /*完成一个数字的复制，其位置指针i=0*/
                        dTotalNum[iDigitalPos++] = DealNumber(cTotalChar);
                    }
                    break;
                }


            case '(':   /*有括号，则将当前括号作用范围内的全部字符保存，作为*/
                {           /*一个新的字符表达式进行递归调用ComputeString函数计算。*/
                    iBracketFlag ++;
                    while(iBracketFlag > 0)
                    {
                        Str ++;
                        cStr[iCharInBracketNum] = (*Str);
                        iCharInBracketNum ++;
                        if((*Str) == ')')
                        {
                            iBracketFlag--;
                        }
                        else if((*Str) == '(')
                        {
                            iBracketFlag ++;
                        }
                    }
                    cStr[iCharInBracketNum-1] = '\0';
                    iCharInBracketNum = 0;         /*完成一个括号内容的复制，其位置指针num=0*/
                    dTotalNum[iDigitalPos++] = ComputeString(cStr);
                    break;
                }


            default:
                {
                    cTotalChar[iCharInNumberNum++] = (*Str);
                    if((*(Str+1)) == '\0')
                    {
                        cTotalChar[iCharInNumberNum] = '\0';
                        dTotalNum[iDigitalPos++] = DealNumber(cTotalChar);
                    }
                    break;
                }
        }

        Str ++;
    }


    cStack[iCharPos] = '\0';


    iCharInNumberNum = 0;


    while (iNumOfMulAndDiv > 0)
    {
        switch (cStack[iCharInNumberNum])
        {
            case '*':
                {
                    iNumOfMulAndDiv --;
                    dTotalNum[iCharInNumberNum+1] = dTotalNum[iCharInNumberNum] * dTotalNum[iCharInNumberNum+1];
                    DealString(cStack,dTotalNum, iCharInNumberNum);
                    break;
                }


            case '/':
                {
                    iNumOfMulAndDiv --;
                    dTotalNum[iCharInNumberNum+1] = dTotalNum[iCharInNumberNum] / (float)dTotalNum[iCharInNumberNum+1];
                    DealString(cStack, dTotalNum, iCharInNumberNum);
                    break;
                }


            default:
                {
                    iCharInNumberNum ++;
                    break;
                }
        }
    }


    iCharInNumberNum = 0;
    while (iNumOfAddAndDec > 0)
    {
        switch(cStack[iCharInNumberNum])
        {
            case '+':
                {
                    dTotalNum[iCharInNumberNum+1] = dTotalNum[iCharInNumberNum] + dTotalNum[iCharInNumberNum+1];
                    iNumOfAddAndDec --;
                    DealString(cStack,dTotalNum, iCharInNumberNum);
                    break;
                }


            case '-':
                {
                    dTotalNum[iCharInNumberNum+1] = dTotalNum[iCharInNumberNum] - dTotalNum[iCharInNumberNum+1];
                    iNumOfAddAndDec --;
                    DealString(cStack,dTotalNum,iCharInNumberNum);
                    break;
                }


            default:
                {
                    printf("operator error!");
                    break;
                }
        }

    }

    return dTotalNum[0];
}



int ComputeColor(const char *Str)           /*可递归函数*/
{                                /*取得数值字符串，并调用DealNumber转换成double*/
    char cStr[100] = {'\0'}, cTotalChar[30] = {'\0'};   /*cStr保存当前的表达式串,cTotalChar保存一个数的所有字符*/
    char cStack[80] = {'\0'};                  /*保存所有的符号的堆栈*/
    int iCharPos = 0;                   /*保存符号的位置指针*/
    double dTotalNum[80] = {0.0};     /*保存当前所有的数的一个堆栈*/
    int iDigitalPos = 0;              /*保存数字位置指针*/
    int iBracketFlag = 0;             /*若iBracketFlag=1则表示有一对括号*/
    int iCharInBracketNum = 0, iCharInNumberNum = 0;  /*iCharInBracketNum保存新括号内的字符数, iCharInNumberNum保存number里的字符位置*/
    int iNumOfMulAndDiv = 0;                         /*乘除符号数量*/
    int iNumOfAddAndDec = 0;                         /*加减符号数量*/


    while ((*Str) != '\0')           /*当p==1 和k==0时，表示已经把括号里的内容全部复制到g[100]中了*/
    {
        iBracketFlag = 0;
        iCharInBracketNum = 0;

        switch (*Str)
        {
            case '+':            /*当前字符为加减乘除时则表示*/
            case '-':
            case '*':
            case '/':
                {
                    cStack[iCharPos++] = (*Str);
                    if(((*Str) == '*') || ((*Str) == '/'))
                    {
                        iNumOfMulAndDiv ++;
                    }
                    else
                    {
                        iNumOfAddAndDec ++;
                    }
                    if((*(Str-1)) != ')')
                    {
                        cTotalChar[iCharInNumberNum] = '\0';
                        iCharInNumberNum = 0;             /*完成一个数字的复制，其位置指针i=0*/
                        dTotalNum[iDigitalPos++] = DealNumber(cTotalChar);
                    }
                    break;
                }


            case '(':   /*有括号，则将当前括号作用范围内的全部字符保存，作为*/
                {           /*一个新的字符表达式进行递归调用ComputeString函数计算。*/
                    iBracketFlag ++;
                    while(iBracketFlag > 0)
                    {
                        Str ++;
                        cStr[iCharInBracketNum] = (*Str);
                        iCharInBracketNum ++;
                        if((*Str) == ')')
                        {
                            iBracketFlag--;
                        }
                        else if((*Str) == '(')
                        {
                            iBracketFlag ++;
                        }
                    }
                    cStr[iCharInBracketNum-1] = '\0';
                    iCharInBracketNum = 0;         /*完成一个括号内容的复制，其位置指针num=0*/
                    dTotalNum[iDigitalPos++] = ComputeString(cStr);
                    break;
                }


            default:
                {
                    cTotalChar[iCharInNumberNum++] = (*Str);
                    if((*(Str+1)) == '\0')
                    {
                        cTotalChar[iCharInNumberNum] = '\0';
                        dTotalNum[iDigitalPos++] = DealNumber(cTotalChar);
                    }
                    break;
                }
        }

        Str ++;
    }


    cStack[iCharPos] = '\0';


    iCharInNumberNum = 0;


    while (iNumOfMulAndDiv > 0)
    {
        switch (cStack[iCharInNumberNum])
        {
            case '*':
                {
                    iNumOfMulAndDiv --;
                    dTotalNum[iCharInNumberNum+1] = dTotalNum[iCharInNumberNum] * dTotalNum[iCharInNumberNum+1];
                    DealString(cStack,dTotalNum, iCharInNumberNum);
                    break;
                }


            case '/':
                {
                    iNumOfMulAndDiv --;
                    dTotalNum[iCharInNumberNum+1] = dTotalNum[iCharInNumberNum] / (float)dTotalNum[iCharInNumberNum+1];
                    DealString(cStack, dTotalNum, iCharInNumberNum);
                    break;
                }


            default:
                {
                    iCharInNumberNum ++;
                    break;
                }
        }
    }


    iCharInNumberNum = 0;
    while (iNumOfAddAndDec > 0)
    {
        switch(cStack[iCharInNumberNum])
        {
            case '+':
                {
                    dTotalNum[iCharInNumberNum+1] = (float)color_calc((int)dTotalNum[iCharInNumberNum], (int)dTotalNum[iCharInNumberNum+1], 0);
                    iNumOfAddAndDec --;
                    DealString(cStack,dTotalNum, iCharInNumberNum);
                    break;
                }


            case '-':
                {
                    dTotalNum[iCharInNumberNum+1] = (float)color_calc((int)dTotalNum[iCharInNumberNum], (int)dTotalNum[iCharInNumberNum+1], 1);
                    iNumOfAddAndDec --;
                    DealString(cStack,dTotalNum,iCharInNumberNum);
                    break;
                }


            default:
                {
                    printf("operator error!");
                    break;
                }
        }

    }

    return (int)dTotalNum[0];
}




int color_calc(int a, int b, int op)
{
    int low1, med1, high1;
    int low2, med2, high2;
    int low, med, high;


    low1 = a & 0xFF;    med1 = ( a >> 8 ) & 0xFF;   high1 = ( a >> 16 ) & 0xFF;
    low2 = b & 0xFF;    med2 = ( b >> 8 ) & 0xFF;   high2 = ( b >> 16 ) & 0xFF;

    if (op == 0)
    {
        low = low1 + low2;
        med = med1 + med2;
        high = high1 + high2;
    }
    else if (op == 1)
    {
        low = low1 - low2;
        med = med1 - med2;
        high = high1 - high2;
    }

    low = (low < 0) ? 0 : low;
    med = (med < 0) ? 0 : med;
    high = (high < 0) ? 0 : high;

    low = (low > 255) ? 255 : low;
    med = (med > 255) ? 255 : med;
    high = (high > 255) ? 255 : high;

    return low + (med << 8)  + (high << 16);
}




float Compute_float(const char *Str)
{
    return (float)ComputeString(Str);
}

int Compute_int(const char *Str)
{
    return (int)ComputeString(Str);
}
