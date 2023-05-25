#include "time_helper.h"
#include <chrono>

using namespace std::chrono;

uint64_t getTimeMs()
{
    const uint64_t now = duration_cast<milliseconds>(time_point_cast<milliseconds>(high_resolution_clock::now()).time_since_epoch()).count();
    return now;
}

uint64_t getTimeUs()
{
    const uint64_t now = duration_cast<microseconds>(time_point_cast<microseconds>(high_resolution_clock::now()).time_since_epoch()).count();
    return now;
}

void delayMs(uint64_t timeToWait)
{
    const uint64_t deadline = 1000UL * timeToWait + getTimeUs();
    while (deadline > getTimeUs()) {}
}

void delayUs(uint64_t timeToWait)
{
    const uint64_t deadline = timeToWait + getTimeUs();
    while (deadline > getTimeUs()) {}
}
