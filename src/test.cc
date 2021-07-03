#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "main.h"

TEST_CASE("testing the factorial function") {
    CHECK(factorial(1) == 1);
    CHECK(factorial(2) == 2);
    CHECK(factorial(3) == 6);
    CHECK(factorial(10) == 3628800);
}

TEST_CASE("testing string to hex") {
    CHECK(StringToHex(string{'\0'}) == "00");
    CHECK(StringToHex(" ") == "20");
    CHECK(StringToHex("123456") == "313233343536");
    CHECK(StringToHex("BlockChain") == "426c6f636b436861696e");
    CHECK(StringToHex("从而排解心情，丰富自身精神面貌") == "e4bb8ee8808ce68e92e8a7a3e5bf83e68385efbc8ce4b8b0e5af8ce887aae8baabe7b2bee7a59ee99da2e8b28c");
}

TEST_CASE("testing uint to string") {
    CHECK(StringToHex(Uint64ToString(0ULL))                    == "0000000000000000");
    CHECK(StringToHex(Uint64ToString(1024ULL))                 == "0000000000000400");
    CHECK(StringToHex(Uint64ToString(2147483647ULL))           == "000000007fffffff");
    CHECK(StringToHex(Uint64ToString(4294967295ULL))           == "00000000ffffffff");
    CHECK(StringToHex(Uint64ToString(18446744073709551615ULL)) == "ffffffffffffffff");
}

TEST_CASE("testing devide string") {
    vector<string> vec1 = DivideString("StreamingAudioService", 7);
    vector<string> vec2 = {"Streami", "ngAudio", "Service"};
    CHECK(std::equal(vec1.begin(), vec1.end(), vec2.begin()));
}

TEST_CASE("testing pre processing") {
    CHECK(StringToHex(PreProcessing(""))           == "80000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    CHECK(StringToHex(PreProcessing(" "))          == "20800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001");
    CHECK(StringToHex(PreProcessing("123456"))     == "31323334353680000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000006");
    CHECK(StringToHex(PreProcessing("BlockChain")) == "426c6f636b436861696e80000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000a");
}

TEST_CASE("testing copy char* to int") {
    string chunk = "80000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    uint32 w[16];
    copyByte64ToU32Arr(chunk, w);
    CHECK(w[0] == 942682160);
    CHECK(w[1] == 808464432);
    CHECK(w[15] == 808464432);
}

TEST_CASE("testing the sha256 function") {
    CHECK(StringToHex(SHA256("")) == "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    // CHECK(StringToHex(SHA256(" ")) == "36a9e7f1c95b82ffb99743e0c5c4ce95d83c9a430aac59f84ef3cbfab6145068");
    // CHECK(StringToHex(SHA256("123456")) == "8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92");
    // CHECK(StringToHex(SHA256("BlockChain")) == "3a6fed5fc11392b3ee9f81caf017b48640d7458766a8eb0382899a605b41f2b9");
    // CHECK(StringToHex(SHA256("从而排解心情，丰富自身精神面貌")) == "ad33913bd36726df41f5a14cf7a1d1ef4c6961071b5b30aed1d4c436c2b77c08");
}