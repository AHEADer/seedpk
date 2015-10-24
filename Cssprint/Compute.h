#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int CheckString(const char *Str);   

void  DealString(char *OperatorArr, double *DigitalArr, int CurrPosition); 

double DealNumber(const char *Str);  

double ComputeString(const char *Str); 

int CheckString(const char *Str)
{
    int iFlag         = 0;      
    int iCharacterNum = 0;     
    while ((*Str) != '\0')
    {
        if (((*Str)>='0' && (*Str)<='9') || ((*Str)=='+') ||
                ((*Str)=='-') || ((*Str)=='*') || ((*Str)=='/') ||
                ((*Str)=='.') || ((*Str)=='(') ||  ((*Str)==')') )
        {
            iCharacterNum ++;
        }
        else
        {
            printf("Input error, there have the char not the math expression char!\n");
            return 0;
        }

        if ((*Str) == '(')
        {
            iFlag++;
        }
        else if ((*Str) ==')')
        {
            iFlag--;
        }
        Str++;
    }


    if (iFlag != 0)
    {
        printf("Input error, there is not have correct bracket '()'!\n");
        return 0;
    }
    printf("You input %d characters in total!\n", iCharacterNum);


    return 1;
}


void  DealString(char *OperatorArr, double *DigitalArr, int CurrPosition)
{
    int iFlag = 0;
    int iOperatorArrLen = strlen(OperatorArr);
    for (iFlag = CurrPosition; iFlag < iOperatorArrLen; iFlag ++) 
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
        tempStr[iTempi++] = Str[iLoop];  
    }
    tempStr[iTempi] = '\0';


    if (iFloat != 0)
    {
        for (iLoop = iFloat+1; iLoop < iStrLen; iLoop ++) 
        {
            if (Str[iLoop] == '.')  
            {
                printf("There is more that one dot '.' in number!error!!!\n");
                exit(0);
            }
            dFlag = dFlag * 0.1;
            dValueReturn += (dFlag * (Str[iLoop]-48));
        }
    }


    dFlag = 1.0;


    iStrLen = strlen(tempStr);        
    for (iLoop = iStrLen-1; iLoop >= 0; iLoop --)
    {
        dValueReturn = dValueReturn + (dFlag * (tempStr[iLoop] - 48));
        dFlag *= 10;
    }


    dValueReturn = dValueReturn * iFlag;
    return dValueReturn;
}


double ComputeString(const char *Str)
{                                
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
