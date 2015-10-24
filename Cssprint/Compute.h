#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*以下为本程序涉及到的函数的声明*/
int CheckString(const char *Str);   /*检查字符串中有否除了0-9,+,-,*,/,(,),之外的其他字符*/

void  DealString(char *OperatorArr, double *DigitalArr, int CurrPosition); /*将当前已经完成运算的运算符消去，同时将数值数组的位置调整以进行下一次运算。*/

double DealNumber(const char *Str);  /*对于输入的字符串，先将其小数点以前的部分复制到tempStr[]数组中*/

double ComputeString(const char *Str); /*对输入的字符串进行处理，是本程序最重要的一个函数*/

/*以下为本程序中各函数的实现*/
/**************************************************************************
* 函数名：  int CheckString(char *Str)
* 输入参数：char *Str: 输入的字符串
* 返回参数：
            0：字符串中有不符合规定的字符
            1: 字符串字符符合规定，没有不符合规定的字符.
* 功能：
            检查字符串中有否除了0-9,+,-,*,/,(,),之外的其他字符，
            如果有，则返回0, 表示出现错误。
            若没有，则返回1,表式字符串符合规定。
**************************************************************************/
int CheckString(const char *Str)
{
int iFlag         = 0;      //k用于检查括号是否配对
int iCharacterNum = 0;      //iCharacterNum用于统计输入的字符的个数
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


/*****************************************************************************************************************
* 函数名： void  DealString(char *OperatorArr, double *DigitalArr, int CurrPosition)
* 输入参数：
           char *OperatorArr :  运算符数组
           double *DigitalArr:  数值数组
           int CurrPosition:    当前运算符数组位置
* 返回参数:无
* 功能：
           将当前已经完成运算的运算符消去，同时将数值数组的位置调整以进行下一次运算。
           传入值CurrPosition若为3，则当前符号的数组位置为3.
           OperatorArr[3]=OperatorArr[3+1].......OperatorArr[len-2]=OperatorArr[len-1]  OperatorArr[len-1]='\0';
           DigitalArr[i]=DigitalArr[i+1].......DigitalArr[len-1]=DigitalArr[len]  因为数值比运算符多一个。
******************************************************************************************************************/
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


/************************************************************************************
* 函数名：  double DealNumber(char *Str)
* 输入参数：char *Str :由数字和小数点组成的字符，用以转换成double型的数值。
* 返回参数：dValueReturn:返回转换好的值。
* 功能：
            对于输入的字符串，先将其小数点以前的部分复制到tempStr[]数组中，
            若有小数点，则将将小数点之后的数值，也就是小数部分先进行计算,值存入dValueReturn中
            计算完成后，再对整数部分进行计算，值加上小数部分的值，存入dValueReturn中。
*************************************************************************************/
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


/************************************************************************************
* 函数名： double ComputeString(char *Str)
* 输入参数：
           char *Str: 即将进行运算的字符串型数学表达式，如3.5+(2*3/5)
* 返回参数：
           dTotalNum[0]:计算结果将放入dTotalNum[0]中
* 功能：
           将输入的字符串中的数字分别调用DealNumber(char *Str)函数进行数值变换，再将其依
           次存入doulbe dTotalNum[i]中，将加减乘除运算符依次存入字符串符号数组中，
           然后如果遇到括号，则将括号内的字符串存入另一字符数组中，然后用此
           ComputeString(char *Str) 递归函数进行递归运算。 然后根据先乘除，后加减的顺序对已
           存入数组的数值根 据存入字符串符号数组的运算符进行运算。结果存入dTotalNum[0]中。
           返回最终结果。
*************************************************************************************/
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
