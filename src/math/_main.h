#include <iostream>
#include <string>

#include "spdlog/spdlog.h"

int factorial(int number) { return number <= 1 ? number : factorial(number - 1) * number; }

unsigned int h0 = 0x6a09e667,
    h1 = 0xbb67ae85,
    h2 = 0x3c6ef372,
    h3 = 0xa54ff53a,
    h4 = 0x510e527f,
    h5 = 0x9b05688c,
    h6 = 0x1f83d9ab,
    h7 = 0x5be0cd19;

unsigned int k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
}

std::string SHA256(std::string src)
{
    spdlog::info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
    // TODO("1. complement")
    unsigned long len = str.size();
    unsigned long extLen = 56 - len%64;
    // unsigned char*  = new unsigned char[extLen]; 
    // memcpy(extSrc, (unsigned char*)strdup(src), sizeof(char) * len);
    src.append((char)0x80);
    src.append(extLen-1, (char)0x00);
    src.append(std::to_string(len));
    // TODO("2. block")
    int N = src.length / 8;  // ([N] = 补码后消息块个数)
    // int* M = int[N][16];
    for(int i=0; i<N; i++){
        str tmp = src.substr(i*64, (i+1)*64);
        char bytes[] = tmp.c_str();
        // strcpy(bytes, tmp.c_str());
        int W[16];
        for(int j=0; j<16; j++){
            std::memcpy(&W[j], bytes[j*4], 4);
        }

    }

    return "sha256";
}