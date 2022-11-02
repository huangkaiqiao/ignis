// #include "cstdio"
#include <iostream>

using namespace std;

int gcd(int a, int b)
{
  if (b<a) {
    int tmp = 0;
    tmp = a;
    a = b;
    b = tmp;
  }
  if (a%b==0) {
    return b;
  }
  return gcd(b, a%b);
}


/**
 * 拓展欧几里德算法 
 * q: quotient, r: reminder
 * e=17 lambda=780， 780÷17＝45...15, 17÷15=1...2, 15÷2=7...1
 * 17x % 780y == 1
 */
// struct EX_GCD {
//   int s;  // a 
//   int t;
//   int gcd;  // greatest common divisor
// };
// struct EX_GCD extended_euclidean(int a, int b)
// {
//   if(a < b)
//     return extended_euclidean(b, a);
//   struct EX_GCD ex_gcd; 
//   if(b == 0){
//     ex_gcd.s = 1;
//     ex_gcd.t = 0;
//     ex_gcd.gcd = 0;
//   }
//   int _r = a, r = b;
//   int _s = 0, s = 1;
//   int _t = 1, t = 0;
//   int q, tmp;
//   int count = 0;
//   while(r!=0 && count++<10){
//     q = _r / r;
//     tmp = _r % r;
//     _r = r;
//     r = tmp;
//     tmp = _s - q * s;
//     _s = s;
//     s = tmp;
//     tmp = _t - q * t;
//     _t = t;
//     t = tmp;
//     std::cout << q << " " <<  r << " " << s << " " << t << std::endl;
//   }
//   ex_gcd.s = _s;
//   ex_gcd.t = _t;
//   ex_gcd.gcd = _r;
//   return ex_gcd;
// }

// int main(void) {
// 	int a, b;
// 	printf("Please input two integers divided by a space.\n");
// 	scanf("%d%d", &a, &b);
// 	struct EX_GCD solution = extended_euclidean(a, b);
// 	printf("%d*%d+%d*%d=%d\n", solution.s, a, solution.t, b, solution.gcd);
// 	printf("Press any key to exit.\n");
// 	getchar();
// 	getchar();
// 	return 0;
// }

/**
 * 扩展欧几里德算法
 */ 
// int extended_euclidean(int a, int b, int &x, int &y) {
//   if (a % b == 0) {
//     // return q / r;
//     return a / b;
//   }
  
//   return extended_euclidean(q, r, _t, t);
// }

/**
 * 模乘逆元
 */
// int modular_multiplicative_inverse(int a, int b) {
//   if (a < b){
//     int tmp = a;
//     a = b;
//     b = tmp;
//   }
//   int mmi = extended_euclidean(a, b, 0, 1);
//   return mmi;
// }

//C++:
inline long long extend_gcd(long long a, long long b, long long &x, long long &y)
{
    if (a == 0 && b == 0)
        return -1ll;
    if (b == 0)
    {
        x = 1ll;
        y = 0ll;
        std::cout << "a=" << a << ", b=" << b << ", x=" << x << ", y=" << y << ", res=" << a << std::endl;
        return a;
    }
    long long d = extend_gcd(b, a % b, y, x);
    y -= a / b * x;
    std::cout << "a=" << a << ", b=" << b << ", x=" << x << ", y=" << y << ", res=" << d << std::endl;
    return d;
}
inline long long mod_reverse(long long a, long long n)
{
    long long x, y, d = extend_gcd(a, n, x, y);
    if (d == 1)
    {
        if (x % n <= 0)
            return x % n + n;
        else
            return x % n;
    }
    else
        return -1ll;
}

// int main(void) {
//   std::cout << "math/mod.cc" << std::endl;
//   std::cout << mod_reverse(17ll, 780ll) << std::endl;
//   std::cout << mod_reverse(780ll, 17ll) << std::endl;
//   std::cout << mod_reverse(46ll, 240ll) << std::endl;
// }
int extend_euclid(int a, int b, int &x, int &y) {
  // cout << "0a:" << a << " b:" << b << " x:" << x << " y:" << y << endl;
  if (a==0 && b==0) {
    return -1;
  }
  if (b==0) {
    x = 1;
    y = 0;
    return a;
  }
  int d = extend_euclid(b, a % b, y, x);
  y -= a / b * x;
  cout << "1a:" << a << " b:" << b << " x:" << x << " y:" << y << endl;
  return d;
}

int mod_inverse(int e, int n){
  if (e > n) {
    int tmp = e;
    e = n;
    n = tmp;
  }
  int x, y, d = extend_euclid(n, e, x, y);
  if (d != 1) {
    return -1;
  }
  if (y < 0){
    y = n + y; 
  }
  return y;
} 

// int main() {
//   int mmi = mod_inverse(17, 780);
//   cout << "modular multiplicative inverse:" << mmi << endl;
//   return (0);
// }