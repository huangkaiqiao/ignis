     
#include <stdio.h>
  
int main() {
  
    int  x  =  1;
    if(*(char  *)&x  ==  1)
        printf("低字节在前\n");
    else
        printf("高字节在前\n");
  
    return 0;
}