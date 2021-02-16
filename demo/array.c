#include <stdio.h>

int a;
int b[25];

int main() {
  b[3]= 31;
  a= b[3];
  printf("%d\n", a);
  return(0);
}
