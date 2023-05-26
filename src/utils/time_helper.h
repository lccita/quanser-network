#ifndef UTILS_TIME_H
#define UTILS_TIME_H

/**
 **************************************************************************************************
 * @file    time_helper.h
 * @author  José Roberto Colombo Junior
 * @brief   Header file of the time helper library.
 **************************************************************************************************
 *
 * This file describe basic functions used to manipulate time easily. Although this code was
 * written in C++, special care was taken to make it compatible within C code. However, this API
 * must be compiled with a C++ compiler.
 *
 * @attention This is free software licensed under GPL.
 *
 **************************************************************************************************
 */

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * This code is based on this topic:
 * https://stackoverflow.com/questions/31255486/c-how-do-i-convert-a-stdchronotime-point-to-long-and-back
 */

/**
 * Get current time (in milliseconds) since system initialization.
 *
 * @return current time in milliseconds.
 */
uint64_t getTimeMs();

/**
 * Get current time (in microseconds) since system initialization.
 *
 * @return current time in microseconds.
 */
uint64_t getTimeUs();

/**
 * Blocks the CPU during timeToWait milliseconds.
 *
 * @param timeToWait
 */
void delayMs(uint64_t timeToWait);

/**
 * Blocks the CPU during timeToWait microseconds.
 *
 * @param timeToWait
 */
void delayUs(uint64_t timeToWait);

#ifdef __cplusplus
}
#endif

#endif //UTILS_TIME_H
