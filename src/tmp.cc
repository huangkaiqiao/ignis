/**
 * @file tmp.cc
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-08
 * 
 * @copyright Copyright (c) 2022
 * 
 * 大数操作类，默认十六进制
 */

#include <iostream>
// #include <algorithm>
#include <cstring>
#include <string_view>
#include <sstream>
#include <string>

// #include "BigInt.hpp"
void load_levels_example();
#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h" // support for loading levels from the environment variable

using namespace std;

void load_levels_example()
{
    spdlog::cfg::load_env_levels();
    // spdlog::set_pattern("*** [%Y-%m-%d %H:%M:%S %e %z] [thread %t] %v ***");
    spdlog::set_pattern("*** %+ ***");
}

typedef unsigned char byte;  // 0~255
typedef const char* String;
typedef unsigned long int u64;

/**
 * 32 字节的数字包装类 
 */
class Num
{
  private:
    byte _value[32]; // 十六进制字符串
    int _length;
  public:
    Num(const char *str);
    void copy(byte *target, byte *source);
    void decStrToBytes(const char *str, byte *target);
    byte* getBytes( void );
    string getDecimalString( void );
    byte hexToDecimal(byte *ptr);
    byte length( void );
    // Num operator+(const Num& n);
    Num operator+(Num& n);
};

/**
 * 构造函数， 参数是如同 "0xFF" 的十六进制字符串
 */
Num::Num(const char *_str){
  string str = _str;
  bool isDecimal = false;
  bool isHexadecimal = false;
  // 判断参数
  if (str.rfind("0x", 0) == 0){
    // throw "Illegal arguement";
    isHexadecimal = true;
  } else {
    spdlog::warn("use recommand input like 0xFF...");
    isDecimal = true;    
  }
  // 1.输入十六进制的情况
  // 两位两位截取
  // 赋值给 _bytes
  int len = str.length();
  if (isHexadecimal) {
    len += len % 2;
    for (int i=len-2, j=0; i>0; i-=2, j++){
      byte dec = this->hexToDecimal((byte *)&str.at(i));
    }
  }

  // 2.输入十进制的情况
  if (isDecimal) {
    byte value[32] = {};
    this->decStrToBytes(_str, value);
    this->copy(this->_value, value);
    // _bytes = const_cast<unsigned char *>(bytes);
    // length = strlen(_bytes);
  }
  _length = len;
}

void Num::copy(byte* target, byte* source){
  for(byte i=0; i<32; i++){
    *(target+i) = *(source+i);
  }
}

// byte *Num::decStrToBytes(const char *decStr){
void Num::decStrToBytes(const char *decStr, byte *target){
  // byte value[32] = {};
  u64 n = 0ll;
  byte len = strlen(decStr);
  for(byte i=0; i<len; i++){
    n *= 10;
    n += *(decStr+i) - '0';
    // SPDLOG_INFO("decStrToBytes n:{}", n);
  }
  byte i=0;
  while(n>0){
    // SPDLOG_INFO("n:{}", n);
    target[i++] = n & 0xff;
    n = n >> 8;
  }
  // return value;
} 

byte Num::hexToDecimal(byte* ptr){
  // SPDLOG_INFO("byte:{:c}{:c}", *ptr, *(ptr+1));
  byte dec = 0;
  for(byte i=0; i<2; i++){
    byte c = *(ptr+i);
    // char c1 = *(ptr+1);
    if ('0' <= c && c <= '9') {
      dec += c - '0';
    } else if ('A' <= c && c <= 'Z') {
      dec += c - 'A' + 0xa;
    } else if ('a' <= c && c <= 'z') {
      dec += c - 'a' + 0xa;
    }
    dec *= 0xf - i*0xe;
    // SPDLOG_INFO("hexToDecimal: {:X}", dec);
  }
  // SPDLOG_INFO("hexToDecimal: {:X}", dec);
  return dec;
}

byte* Num::getBytes( void )
{
  return _value;
}

string Num::getDecimalString( void )
{
  // string s = "";
  int i = 32;
  while(_value[--i]==0){}
  u64 tmp = 0;
  for(; i>=0; i--){
    // SPDLOG_INFO("value.{}: {}, tmp:{}", i, _value[i], tmp);
    // s = s + (char)((_value[i] >> 4) + '0') + (char)((_value[i] & 0x0f + '0'));
    tmp <<= 8;
    tmp |= _value[i];
  }
  ostringstream s;
  s << tmp;
  return s.str();
}

// Num operator+(Num& n){
//   // Num num;
//   byte length = max(this.length(), n.length());
// }

template <> struct fmt::formatter<Num>: formatter<string> {
  // parse is inherited from formatter<string_view>.
  template <typename FormatContext>
  auto format(Num a, FormatContext& ctx) {
    // string message = reinterpret_cast<char *>(a.getBytes());
    // string_view name = "not found";
    string message = a.getDecimalString();
    return formatter<string>::format(message, ctx);
  }
};

const byte max(byte a, byte b)
{
  return (a<b) ? b : a;
}

byte Num::length( void ){
  return _length;
}

Num Num::operator+(Num& n){
      // Num num;
      byte length = max(this->length(), n.length());
      byte carry = 0;
      byte *val0 = this->getBytes();
      byte *val1 = n.getBytes();
      Num sum = Num("0");
      byte *value = sum.getBytes();
      for(int i=0; i<length; i++){
        byte a = *(val0+i);
        byte b = *(val1+i);
        byte byte_sum = a + b;
        *(value+i) = byte_sum + carry;
        carry = byte_sum < a || byte_sum < b;
      }
      if(carry == 1) {
        SPDLOG_INFO("digits overflow");
      }
      return sum;
    }

int main() {
  load_levels_example();
  Num a = Num("9876543210");
  spdlog::info("a: {}", a);
  Num b = Num("0123456789");
  spdlog::info("b: {}", b);
  Num c = a + b;
  spdlog::info("sum: {}", a + b);
  return 0;
}