#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "main.h"

TEST_CASE("testing the factorial function") {
    CHECK(factorial(1) == 1);
    CHECK(factorial(2) == 2);
    CHECK(factorial(3) == 6);
    CHECK(factorial(10) == 3628800);
}

// TEST_CASE("testing the sha256 function") {
//     CHECK(SHA256("") == "8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92");
//     CHECK(SHA256(" ") == "8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92");
//     CHECK(SHA256("123456") == "8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92");
//     CHECK(SHA256("BlockChain") == "3a6fed5fc11392b3ee9f81caf017b48640d7458766a8eb0382899a605b41f2b9")
//     CHECK(SHA256("从而排解心情，丰富自身精神面貌") == "ad33913bd36726df41f5a14cf7a1d1ef4c6961071b5b30aed1d4c436c2b77c08");
// }

TEST_CASE("testing string to hex") {
    CHECK(StringToHex(string{'\0'}) == "00");
    CHECK(StringToHex(" ") == "20");
    CHECK(StringToHex("123456") == "313233343536");
    CHECK(StringToHex("BlockChain") == "426c6f636b436861696e");
    CHECK(StringToHex("从而排解心情，丰富自身精神面貌") == "e4bb8ee8808ce68e92e8a7a3e5bf83e68385efbc8ce4b8b0e5af8ce887aae8baabe7b2bee7a59ee99da2e8b28c");
}

TEST_CASE("testing pre processing") {
    CHECK(StringToHex(preProcessing(""))           == "8000000000000000000000000000000000000000000000000000000000000000");
    CHECK(StringToHex(preProcessing(" "))          == "2010000000000000000000000000000000000000000000000000000000000001");
    CHECK(StringToHex(preProcessing("123456"))     == "3132333435361000000000000000000000000000000000000000000000000006");
    CHECK(StringToHex(preProcessing("BlockChain")) == "426c6f636b436861696e0000000000000000000000000000000000000000000a");
}