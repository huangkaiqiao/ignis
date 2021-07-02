#include <iostream>
#include <string>

#include "spdlog/spdlog.h"

using std::string; using std::hex;
std::stringstream ss;

int factorial(int number) { return number <= 1 ? number : factorial(number - 1) * number; }

// std::string SHA256(std::string message) {
//     //初始哈希值
// 	unsigned int h0 = 0x6a09e667,
// 	    h1 = 0xbb67ae85,
// 	    h2 = 0x3c6ef372,
// 	    h3 = 0xa54ff53a,
// 	    h4 = 0x510e527f,
// 	    h5 = 0x9b05688c,
// 	    h6 = 0x1f83d9ab,
// 	    h7 = 0x5be0cd19;
//     unsigned int k[64] = {
//         0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
// 		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
// 		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
// 		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
// 		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
// 		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
// 		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
// 		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
//     };
//     string padded = message.append('\x80');
//     if(padded.size() % 64 < 56) {
//         int suffixLen = 64 + 56 - (padded.size() % 64);
//         padded.append('\x00', suffixLen);
//     } else {
//         int suffixLen = 56 - (padded.size() % 64);
//         padded.append('\x00', suffixLen);
//     }
//     unsigned long msgLen = message.size();
//     src.append(std::to_string(msgLen));
    
//     char broken[][] = 
// }

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

// Pre-processing (Padding): 预处理
std::string preProcessing(std::string message) {
    unsigned long msgLen = message.size();
    // spdlog::info("preProcessing: msgLen={}", msgLen);
    std::string padded = message + '\x80';
    int len = padded.size();
    int tmp = len % 64;
    tmp = tmp == 0? 64:64-tmp;
    spdlog::info("preProcessing: message={} tmp={}", message, tmp);
    padded.append(tmp, '\x00');
    unsigned char arr[2];
    arr[0] = (msgLen >> 8) & 0xFF; // 0x56
    arr[1] = msgLen & 0xFF; // 0x78
    ss.str("");
    ss << padded;
    // ss << (unsigned char*)&msgLen;
    ss << arr;
    return ss.str();
}
