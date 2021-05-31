// #include <iostream>
#include <cstdio>

void load_levels_example();

#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h" // support for loading levels from the environment variable

int main()
{
    // std::cout << "Hello World!\n";
    load_levels_example();

    spdlog::info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
    return 0;
}

void load_levels_example()
{
    // Set the log level to "info" and mylogger to "trace":
    // SPDLOG_LEVEL=info,mylogger=trace && ./example
    spdlog::cfg::load_env_levels();
    // or from command line:
    // ./example SPDLOG_LEVEL=info,mylogger=trace
    // #include "spdlog/cfg/argv.h" // for loading levels from argv
    // spdlog::cfg::load_argv_levels(args, argv);
}