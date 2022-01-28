#include <iostream>
#include <cstring>

// #include "BigInt.hpp"
void load_levels_example();
#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h" // support for loading levels from the environment variable

using namespace std;

void load_levels_example()
{
    spdlog::cfg::load_env_levels();
}
/**
 * 32 字节的数字包装类 
 */
class Num
{
  private:
    unsigned char _bytes[32];
    int length;
  public:
    Num(const char *str);
    char* getBytes( void );

};

/**
 * 构造函数， 参数是如同 "0xFF" 的十六进制字符串
 */
Num::Num(const char* str){
  // 判断参数
  if (str.rfind("0x", 0) != 0){
    throw "Illegal arguement";
  }
  // 两位两位截取
  // 赋值给 _bytes
  int len = strlen(str);
  len += len % 2;
  for (int i=len-1, j=0; i>0; i-=2, j++){
    int dec = this.hexToDecimal(str+i)
  } 
  _bytes = (char *)bytes;
  // _bytes = const_cast<unsigned char *>(bytes);
  length = strlen(_bytes);
}

unsigned char Num::hexToDecimal(char* ptr){
  char c0 = *ptr;
  char c1 = *(ptr+1);
  int dec = 0;
  if ('0' <= c2 && c2 <= '9') {
    dec += c2 - '0';
  } else if ('A' <= c2 && c2 <= 'Z') {
    dec += c2 - 'A';
  } else if ('a' <= c2 && c2 <= 'z')
}

char* Num::getBytes( void )
{
  return _bytes;
}

template <> struct fmt::formatter<Int>: formatter<string_view> {
  // parse is inherited from formatter<string_view>.
  template <typename FormatContext>
  auto format(Int a, FormatContext& ctx) {
    string_view name = a.getBytes();
    // string_view name = "not found";
    return formatter<string_view>::format(name, ctx);
  }
};


int main() {
  load_levels_example();
  Int a = Int("9876543210");
  spdlog::info("a: {}", a);
  return 0;
}