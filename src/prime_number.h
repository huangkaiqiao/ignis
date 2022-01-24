#include <stdio.h>

void calPrimeNumber()
{
  int count = 0;
  for(int i=2;i<1000;i++)
  {
    bool isPrime = true;
    for(int j=2;j<i/2;j++)
    {
      if(i%j==0){
        isPrime = false;
      }
    }
    if (isPrime) {
      printf("%d, ", i);
      count +=1;
    }
  }
  printf("\n%d\n", count);
}