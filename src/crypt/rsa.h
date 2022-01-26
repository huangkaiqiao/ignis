// #include <stdlib>

#include "spdlog/spdlog.h"

#include "math/mod.h"

int primeNumberArr[169] = {2, 3, 4, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997};
int encrypt(int plaintext, int n, int e);
int decrypt(int ciphertext, int n, int d);

void testRsa()
{
  time_t t;
  /* Initializes random number generator */
  srand((unsigned) time(&t));

  // int p = rand() % 100;
  int p = 61;
  // int q = rand() % 100;
  int q = 53;
  int n = p*q;
  int lambda = 780;
  int e = 17;
  int d = mod_reverse(e, lambda);
  int m = 65;
  int ciphertext = encrypt(m, n, e);
  int plaintext = decrypt(ciphertext, n, d);
  spdlog::info("TestRsa ciphertext:{}, plaintext:{}", ciphertext, plaintext);
}

int encrypt(int m, int n, int e) {
  int c = 1;
  for(int i=0;i<e;i++){
    c *= m;
    c %= n;
  }
  return c;
}

int decrypt(int c, int n, int d) {
  int m = 1;
  for(int i=0;i<d;i++){
    m *= c;
    m %= n;
  }
  return m;
}