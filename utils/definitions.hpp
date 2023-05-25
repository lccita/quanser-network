#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#ifdef _MSC_VER
// see:
// https://stackoverflow.com/questions/1537964/visual-c-equivalent-of-gccs-attribute-packed
#define PACKED
#else
#define PACKED __attribute__((__packed__))
#endif

typedef enum channel {
    CH0 = 0,
    CH1 = 1,
    CH2 = 2,
    CH3 = 3,
    CH4 = 4,
    CH5 = 5,
    CH6 = 6,
    CH7 = 7,
    Ch0 = CH0,
    Ch1 = CH1,
    Ch2 = CH2,
    Ch3 = CH3,
    Ch4 = CH4,
    Ch5 = CH5,
    Ch6 = CH6,
    Ch7 = CH7,
} channel_t;

typedef enum board {
    boardNotFound = 0,
    boardQ2USB,
    boardQ8USB,
    boardDummy,
    boardNumber
} board_t;

extern std::vector<std::string> supportedBoards;

typedef enum boardFeature {
    featurePwmMinFreqHz = 0,
    featurePwmMaxFreqHz,
    featurePwmResolutionBits,
    featurePwmChannels,
    featureAdcResolutionBits,
    featureAdcChannels,
    featureDacResultionBits,
    featureDacChannels,
    featureEncoderChannels,
    featureNumber,
} boardFeature_t;

extern const int boardInfo[boardNumber][featureNumber];

typedef enum adcMode {
    adcModeReferencedSingleEnded = 0,  // The input is referenced single-ended.
    adcModeNonReferencedSingleEnded,   // The input is non-referenced single-ended.
    adcModeDifferential,               // The input is differential.
    adcModePseudoDifferential,         // The input is pseudo-differential.
} adcMode_t;

typedef enum EncoderQuadrature {
    quadratureNone = 0,  // No quadrature. Inputs are count and direction.
    quadrature1X,        // Inputs are A and B channels.
    quadrature2X,        // Inputs are A and B channels.
    quadrature4X,        // Inputs are A and B channels.
} encoderQuadrature_t;

typedef enum pwmMode {
    pwmModeDutyCycle = 0,
    pwmModeFrequency,
    pwmModePeriod,
    pwmModeOneShot,
    pwmModeTime,
    pwmModeEncoderEmulation,
    pwmModeRaw,
} pwmMode_t;

typedef enum pwmConfig {
    pwmConfigUnipolar = 0,
    pwmConfigBipolar,
    pwmConfigPaired,
    pwmConfigComplementary,
} pwmConfig_t;

typedef enum pwmAlign {
    pwmAlignRisingEdge = 0,
    pwmAlignFallingEdge,
    pwmAlignCenter,
} pwmAlign_t;

typedef enum pwmPolarity {
    pwmPolarityLow = 0,
    pwmPolarityHigh
} pwmPolarity_t;

#endif //DEFINITIONS_H
