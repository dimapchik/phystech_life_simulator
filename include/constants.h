#pragma once
const int DAY_DURATION = 60 * 24;
const int HOUR_DURATION = 60;
const int WEEK_DURATION = 7;
const int MAX_ENERGY = 100;
const int MAX_LONELINESS = 100;
const int MAX_STUDYING_CAPACITY = 100;
const int START_MONEY = 500;

namespace Color {
    constexpr const char* reset  = "\033[0m";
    constexpr const char* red    = "\033[31m";
    constexpr const char* green  = "\033[32m";
    constexpr const char* yellow = "\033[33m";
    constexpr const char* blue   = "\033[34m";
    constexpr const char* magenta = "\033[35m";
    constexpr const char* cyan    = "\033[36m";
    constexpr const char* bold    = "\033[1m";
}
