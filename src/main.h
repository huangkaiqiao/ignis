#include <iostream>
#include <string>
#include <stdarg.h>
#include <stdint.h>
#include "spdlog/spdlog.h"

using std::hex;
using std::string;
using std::stringstream;
using std::vector;
std::stringstream SS;

typedef unsigned int uint32;
typedef unsigned char u8;
typedef unsigned u32;

int factorial(int number) { return number <= 1 ? number : factorial(number - 1) * number; }

string rand_str(const int len)  /*参数为字符串的长度*/
{
    /*初始化*/
    string str;                 /*声明用来保存随机字符串的str*/
    char c;                     /*声明字符c，用来保存随机生成的字符*/
    int idx;                    /*用来循环的变量*/
    /*循环向字符串中添加随机生成的字符*/
    for(idx = 0;idx < len;idx ++)
    {
        /*rand()%26是取余，余数为0~25加上'a',就是字母a~z,详见asc码表*/
        c = 'a' + rand()%26;
        str.push_back(c);       /*push_back()是string类尾插函数。这里插入随机字符c*/
    }
    return str;                 /*返回生成的随机字符串*/
}

// hex array to string
// std::string HexToString(const unsigned char* data,int len)
std::string HexToString(string h)
{
    const unsigned char* data = (unsigned char *)h.data();
    int len = h.size();
    const std::string hexme = "0123456789abcdef";
    std::string ret="";
    for(int i =0;i<len;i++)
    {
       ret.push_back(hexme[(data[i]&0xF0)>>4]);
       ret.push_back(hexme[data[i]&0x0F]);
       ret.push_back(' ');
    }
    return ret;
}

// string to hex
// std::string StringToHex(const unsigned char* pData,size_t nSize)
std::string StringToHex(string s)
{
    const unsigned char* pData = (unsigned char *)s.data();
    int nSize = s.size();
    // spdlog::info("StringToHex: nSize={}", nSize);
    std::string str;
    char szBuf[3] = "";
    for(size_t i = 0;i<nSize;i++)
    {
        std::snprintf(szBuf,3,"%02x",*(pData+i));
        str +=szBuf;
    }
    return str;
}

/**
 * num 表示消息的字节长度
*/
string Uint64ToString(uint64_t num) {
    // unsigned char arr[8];
    num = num*8;
    stringstream ss;
    // ss.str("");
    ss << (unsigned char)((num >> 56) & 0xFF); 
    ss << (unsigned char)((num >> 48) & 0xFF); 
    ss << (unsigned char)((num >> 40) & 0xFF);
    ss << (unsigned char)((num >> 32) & 0xFF); 
    ss << (unsigned char)((num >> 24) & 0xFF); // 0x12 
    ss << (unsigned char)((num >> 16) & 0xFF); // 0x34
    ss << (unsigned char)((num >> 8) & 0xFF); // 0x56
    ss << (unsigned char)(num & 0xFF); // 0x78
    return ss.str();
}

string U32ToString(u32 num) {
    stringstream ss;
    ss << u8((num >> 24) & 0xFF); // 0x12 
    ss << u8((num >> 16) & 0xFF); // 0x34
    ss << u8((num >> 8) & 0xFF);  // 0x56
    ss << u8(num & 0xFF);         // 0x78
    return ss.str();
}

void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
  std::string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;
  while(std::string::npos != pos2)
  {
    v.push_back(s.substr(pos1, pos2-pos1));
 
    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if(pos1 != s.length())
    v.push_back(s.substr(pos1));
}

// Pre-processing (Padding): 预处理
std::string PreProcessing(std::string message) {
    unsigned long long msgLen = message.size();
    std::string padded = message + '\x80';
    int len = padded.size() + 8;
    int tmp = len % 64;
    tmp = tmp == 0? 64:64-tmp;
    padded.append(tmp, '\x00');
    SS.str("");
    SS << padded;
    SS << Uint64ToString(msgLen);
    return SS.str();
}

vector<string> DivideString(string s, int part_size) {
     
    int str_size = s.size();
    int i;
    vector<string> res;
 
    // Check if string can be divided in
    // n equal parts
    if (str_size % part_size != 0)
    {
        spdlog::error("Invalid Input: String size");
        spdlog::error(" is not divisible by n");
        throw "Unexception message length " + std::to_string(str_size);
    }
    int n = str_size / part_size;
 
    // Calculate the size of parts to
    // find the division points
    for (i=0; i<n; i++) {
        res.push_back(s.substr(i*part_size, part_size));
    }
    // for(i=0; i<res.size(); i++){
    //     spdlog::info("vec[{}]={}", i, res[i]);
    // }
    return res;
}

// (The initial values in w[0..63] don't matter, so many implementations zero them here)
void copyByte64ToU32Arr(string chunk, u32* pW) {
    const unsigned char* data = (unsigned char *)chunk.data();
    for(int i=0; i<16; i++){
        *(pW+i) = u32(
            (u8)*(data+i*4) << 24 |
            (u8)*(data+i*4+1) << 16 |
            (u8)*(data+i*4+2) << 8 |
            (u8)*(data+i*4+3)
        );
    }
    // spdlog::info("copy: chunk={}", StringToHex(chunk));
    // for(int i=0; i<16; i++){
    //     spdlog::info("copy: w[{}]={}", i, *(pW+i));
    // }
}

/*循环左移*/
unsigned rol(unsigned val, int size)
{
  unsigned res = val << size;
  res |= val >> (32 - size);
  return res;
}

/*循环右移*/
unsigned ror(unsigned val, int size)
{
  unsigned res = val >> size;
  res |= val << (32 - size);
  return res;
}

string SHA256(string message) {
    //初始哈希值
	u32 h0 = 0x6a09e667,
	    h1 = 0xbb67ae85,
	    h2 = 0x3c6ef372,
	    h3 = 0xa54ff53a,
	    h4 = 0x510e527f,
	    h5 = 0x9b05688c,
	    h6 = 0x1f83d9ab,
	    h7 = 0x5be0cd19;

    u32 k[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    string padded = PreProcessing(message);
    vector<string> chunks = DivideString(padded, 64);
    vector<string>::iterator it = chunks.begin();
    while(it != chunks.end()) {
        uint32 w[64];
        string chunk = *it;
        copyByte64ToU32Arr(chunk, w);
        // Extend the first 16 words into the remaining 48 words w[16..63] of the message schedule array:
        for (int i=16; i<64; i++) {
            uint32 s0 = ror(w[i-15],  7) ^ ror(w[i-15], 18) ^ (w[i-15] >> 3);
            uint32 s1 = ror(w[i- 2], 17) ^ ror(w[i- 2], 19) ^ (w[i-2] >> 10);
            w[i] = w[i-16] + s0 + w[i-7] + s1;
        }

        // Initialize working variables to current hash value:
        u32 a = h0,
        b = h1,
        c = h2,
        d = h3,
        e = h4,
        f = h5,
        g = h6,
        h = h7;

        // Compression function main loop:
        for(int i=0; i<64; i++){
            // spdlog::info("SHA256: a={:x}", a);
            u32 S1 = ror(e, 6) ^ ror(e, 11) ^ ror(e, 25);
            u32 ch = (e & f) ^ ((~ e) & g);
            u32 temp1 = h + S1 + ch + k[i] + w[i];
            u32 S0 = ror(a, 2) ^ ror(a, 13) ^ ror(a, 22);
            u32 maj = (a & b) ^ (a & c) ^ (b & c);
            u32 temp2 = S0 + maj;
    
            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
            // spdlog::info("SHA256: temp1={:x}, temp2={:x}", temp1, temp2);
        }

        // Add the compressed chunk to the current hash value:
        h0 = h0 + a;
        h1 = h1 + b;distinct
    // Produce the final hash value (big-endian):
    string hash = "";
    hash.append(U32ToString(h0))
        .append(U32ToString(h1))
        .append(U32ToString(h2))
        .append(U32ToString(h3))
        .append(U32ToString(h4))
        .append(U32ToString(h5))
        .append(U32ToString(h6))
        .append(U32ToString(h7));
    return hash;
}