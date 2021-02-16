#include <stdio.h>
char  c;
char *str;
void display();

int main() {
  c= '\n'; printf("%d\n", c);

  for (str= "Hello world\n"; *str != 0; str= str + 1) {
    printf("%c", *str);
 
  }
  
  display();
  return(0);
}
