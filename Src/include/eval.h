#ifndef EVAL_H_INCLUDED
#define EVAL_H_INCLUDED

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#define sstrjoin(...)    _sstrjoin(__VA_ARGS__, NULL)
char *_sstrjoin(char *buf, const char *delim, ...);
float Compute_float(const char *Str);
int Compute_int(const char *Str);


/*************************************************************************/
int CheckString(const char *Str);
void  DealString(char *OperatorArr, double *DigitalArr, int CurrPosition);
double DealNumber(const char *Str);
double ComputeString(const char *Str);


/*************************************************************************/
int CheckString(const char *Str);
void  DealString(char *OperatorArr, double *DigitalArr, int CurrPosition);
double DealNumber(const char *Str);
double ComputeString(const char *Str);           /*可递归函数*/
int color_calc(int a, int b, int op);


float Compute_float(const char *Str);
int Compute_int(const char *Str);
int ComputeColor(const char *Str);

#endif // EVAL_H_INCLUDED
