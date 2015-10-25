#include "eval.c"

int main(){
const char *a = "2+3";

printf("%d\n", Compute_int(a));

printf("%f\n", Compute_float(a));
return 0;
}
