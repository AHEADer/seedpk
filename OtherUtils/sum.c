#include<iostream>
using namespace std;
#include<stdarg.h>

#include<stdio.h>
int sum(int n,...)
{
	int i , sum = 0;
	va_list vap;
	va_start(vap , n);     //指向可变参数表中的第一个参数
	for(i = 0 ; i < n ; ++i)
		sum += va_arg(vap , int);     //取出可变参数表中的参数，并修改参数指针vap使其增加以指向表中下一个参数
	va_end(vap);    //把指针vap赋值为0
	return sum;
}
int main(void)
{
    int k=8;
	int m =sum(4, k, k*(k-15), 27, (k*k)/30); 
	cout<<m<<endl;
    cout<<2/3<<endl;


    printf("%d",2.3333);
	return 0;
}

