// #include <iostream>
#include "spdlog/spdlog.h"
using namespace std;
int main()
{
    // cout << "Hello, world!" << endl;
    spdlog::info("Hello, {}!", "World");
    return 0;
}
