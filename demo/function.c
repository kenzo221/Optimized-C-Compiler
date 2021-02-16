#include <stdio.h>
void fred();

void main()
{
  int i;
  for (i= 1; i <= 10; i= i + 1) {
    printf("%d\n", i);
  
  }
    fred();
}

void fred()
{
  int a; int b;
  a= 12; b= 3;
  if (a >= b) { printf("%d\n", 2 * b - a); }
}
