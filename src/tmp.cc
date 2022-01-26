#include <iostream>

#include "BigInt.hpp"

using namespace std;

int main() {
  BigInt big1 = 1234567890, big2;
  big2 = "987654321012345678909876543210123456789098765432101234567890";
  std::cout << big1 * big2 * 123456 << "\n";
}