#ifndef MEMORY_TABLE_H
#define MEMORY_TABLE_H

#include <cstdint>

extern uint8_t registerSize(uint8_t address);

union MemoryTableDescription {
    uint8_t memory[256];

    struct Reg {
        struct Readings {
            struct GpioInput {
                union value {
                    uint8_t all;
                    struct bit {
                        uint8_t Ch0       :  1;  // LSB
                        uint8_t Ch1       :  1;
                        uint8_t Ch2       :  1;
                        uint8_t Ch3       :  1;
                        uint8_t Ch4       :  1;
                        uint8_t Ch5       :  1;
                        uint8_t Ch6       :  1;
                        uint8_t Ch7       :  1;  // MSB
                    } bit;
                } value;
                constexpr static uint8_t address = 0;
            } GpioInput;

            uint8_t reserved[3];

            struct Adc {
                struct Ch0 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 4;
                } Ch0;

                struct Ch1 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 8;
                } Ch1;

                struct Ch2 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 12;
                } Ch2;

                struct Ch3 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 16;
                } Ch3;

                struct Ch4 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 20;
                } Ch4;

                struct Ch5 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 24;
                } Ch5;

                struct Ch6 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 28;
                } Ch6;

                struct Ch7 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 32;
                } Ch7;
            } Adc;

            struct Encoder0 {
                union value {
                    int32_t asInteger;
                    uint32_t asBit32;
                    uint8_t asBit8[4];
                } value;
                constexpr static uint8_t address = 36;
            } Encoder0;

            struct Encoder1 {
                union value {
                    int32_t asInteger;
                    uint32_t asBit32;
                    uint8_t asBit8[4];
                } value;
                constexpr static uint8_t address = 40;
            } Encoder1;

            struct Encoder2 {
                union value {
                    int32_t asInteger;
                    uint32_t asBit32;
                    uint8_t asBit8[4];
                } value;
                constexpr static uint8_t address = 44;
            } Encoder2;

            struct Encoder3 {
                union value {
                    int32_t asInteger;
                    uint32_t asBit32;
                    uint8_t asBit8[4];
                } value;
                constexpr static uint8_t address = 48;
            } Encoder3;

            struct Encoder4 {
                union value {
                    int32_t asInteger;
                    uint32_t asBit32;
                    uint8_t asBit8[4];
                } value;
                constexpr static uint8_t address = 52;
            } Encoder4;

            struct Encoder5 {
                union value {
                    int32_t asInteger;
                    uint32_t asBit32;
                    uint8_t asBit8[4];
                } value;
                constexpr static uint8_t address = 56;
            } Encoder5;

            struct Encoder6 {
                union value {
                    int32_t asInteger;
                    uint32_t asBit32;
                    uint8_t asBit8[4];
                } value;
                constexpr static uint8_t address = 60;
            } Encoder6;

            struct Encoder7 {
                union value {
                    int32_t asInteger;
                    uint32_t asBit32;
                    uint8_t asBit8[4];
                } value;
                constexpr static uint8_t address = 64;
            } Encoder7;
        } Readings;

        struct Action {
            struct PwmDuty {
                struct Ch0 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 68;
                    constexpr static double defaultValue = 0.0f;
                } Ch0;

                struct Ch1 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 72;
                    constexpr static double defaultValue = 0.0f;
                } Ch1;

                struct Ch2 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 76;
                    constexpr static double defaultValue = 0.0f;
                } Ch2;

                struct Ch3 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 80;
                    constexpr static double defaultValue = 0.0f;
                } Ch3;

                struct Ch4 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 84;
                    constexpr static double defaultValue = 0.0f;
                } Ch4;

                struct Ch5 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 88;
                    constexpr static double defaultValue = 0.0f;
                } Ch5;

                struct Ch6 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 92;
                    constexpr static double defaultValue = 0.0f;
                } Ch6;

                struct Ch7 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 96;
                    constexpr static double defaultValue = 0.0f;
                } Ch7;
            } PwmDuty;

            struct Dac {
                struct Ch0 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 100;
                    constexpr static double defaultValue = 0.0f;
                } Ch0;

                struct Ch1 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 104;
                    constexpr static double defaultValue = 0.0f;
                } Ch1;

                struct Ch2 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 108;
                    constexpr static double defaultValue = 0.0f;
                } Ch2;

                struct Ch3 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 112;
                    constexpr static double defaultValue = 0.0f;
                } Ch3;

                struct Ch4 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 116;
                    constexpr static double defaultValue = 0.0f;
                } Ch4;

                struct Ch5 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 120;
                    constexpr static double defaultValue = 0.0f;
                } Ch5;

                struct Ch6 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 124;
                    constexpr static double defaultValue = 0.0f;
                } Ch6;

                struct Ch7 {
                    union value {
                        float asFloat;
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 128;
                    constexpr static double defaultValue = 0.0f;
                } Ch7;
            } Dac;

            struct GpioOutput {
                union value {
                    uint16_t all;
                    struct bit {
                        uint8_t Ch0       :  1;  // LSB
                        uint8_t Ch1       :  1;
                        uint8_t Ch2       :  1;
                        uint8_t Ch3       :  1;
                        uint8_t Ch4       :  1;
                        uint8_t Ch5       :  1;
                        uint8_t Ch6       :  1;
                        uint8_t Ch7       :  1;
                        uint8_t reserved  :  8;  // MSB
                    } bit;
                } value;
                constexpr static uint8_t address = 132;
            } GpioOutput;

            uint8_t reserved[2];
        } Action;

        struct Setup {
            struct Adc {
                struct Range {
                    struct Max {
                        struct Ch0 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 136;
                        } Ch0;

                        struct Ch1 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 137;
                        } Ch1;

                        struct Ch2 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 138;
                        } Ch2;

                        struct Ch3 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 139;
                        } Ch3;

                        struct Ch4 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 140;
                        } Ch4;

                        struct Ch5 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 141;
                        } Ch5;

                        struct Ch6 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 142;
                        } Ch6;

                        struct Ch7 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 143;
                        } Ch7;
                    } Max;

                    struct Min {
                        struct Ch0 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 144;
                        } Ch0;

                        struct Ch1 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 145;
                        } Ch1;

                        struct Ch2 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 146;
                        } Ch2;

                        struct Ch3 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 147;
                        } Ch3;

                        struct Ch4 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 148;
                        } Ch4;

                        struct Ch5 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 149;
                        } Ch5;

                        struct Ch6 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 150;
                        } Ch6;

                        struct Ch7 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 151;
                        } Ch7;
                    } Min;
                } Range;

                struct Mode {
                    union value {
                        uint16_t all;
                        struct bit {
                            uint8_t Ch0 : 2;
                            uint8_t Ch1 : 2;
                            uint8_t Ch2 : 2;
                            uint8_t Ch3 : 2;
                            uint8_t Ch4 : 2;
                            uint8_t Ch5 : 2;
                            uint8_t Ch6 : 2;
                            uint8_t Ch7 : 2;
                        } bit;
                    } value;
                    constexpr static uint8_t address = 152;
                } Mode;
            } Adc;

            struct Encoder {
                struct Quadrature {
                    union value {
                        uint16_t all;
                        struct bit {
                            uint8_t Encoder0 : 2;
                            uint8_t Encoder1 : 2;
                            uint8_t Encoder2 : 2;
                            uint8_t Encoder3 : 2;
                            uint8_t Encoder4 : 2;
                            uint8_t Encoder5 : 2;
                            uint8_t Encoder6 : 2;
                            uint8_t Encoder7 : 2;
                        } bit;
                    } value;
                    constexpr static uint8_t address = 154;
                } Quadrature;
            } Encoder;

            struct Pwm {
                struct Frequency {
                    struct Ch0 {
                        union value {
                            float asFloat;
                            uint32_t asBit32;
                            uint8_t asBit8[4];
                        } value;
                        constexpr static uint8_t address = 156;
                    } Ch0;

                    struct Ch1 {
                        union value {
                            float asFloat;
                            uint32_t asBit32;
                            uint8_t asBit8[4];
                        } value;
                        constexpr static uint8_t address = 160;
                    } Ch1;

                    struct Ch2 {
                        union value {
                            float asFloat;
                            uint32_t asBit32;
                            uint8_t asBit8[4];
                        } value;
                        constexpr static uint8_t address = 164;
                    } Ch2;

                    struct Ch3 {
                        union value {
                            float asFloat;
                            uint32_t asBit32;
                            uint8_t asBit8[4];
                        } value;
                        constexpr static uint8_t address = 168;
                    } Ch3;

                    struct Ch4 {
                        union value {
                            float asFloat;
                            uint32_t asBit32;
                            uint8_t asBit8[4];
                        } value;
                        constexpr static uint8_t address = 172;
                    } Ch4;

                    struct Ch5 {
                        union value {
                            float asFloat;
                            uint32_t asBit32;
                            uint8_t asBit8[4];
                        } value;
                        constexpr static uint8_t address = 176;
                    } Ch5;

                    struct Ch6 {
                        union value {
                            float asFloat;
                            uint32_t asBit32;
                            uint8_t asBit8[4];
                        } value;
                        constexpr static uint8_t address = 180;
                    } Ch6;

                    struct Ch7 {
                        union value {
                            float asFloat;
                            uint32_t asBit32;
                            uint8_t asBit8[4];
                        } value;
                        constexpr static uint8_t address = 184;
                    } Ch7;
                } Frequency;

                struct Config {
                    union Ch0 {
                        uint8_t all;
                        struct bit {
                            uint8_t alignment : 2;
                            uint8_t config    : 2;
                            uint8_t mode      : 3;
                            uint8_t polarity  : 1;
                        } bit;
                        constexpr static uint8_t address = 188;
                    } Ch0;

                    union Ch1 {
                        uint8_t all;
                        struct bit {
                            uint8_t alignment : 2;
                            uint8_t config    : 2;
                            uint8_t mode      : 3;
                            uint8_t polarity  : 1;
                        } bit;
                        constexpr static uint8_t address = 189;
                    } Ch1;

                    union Ch2 {
                        uint8_t all;
                        struct bit {
                            uint8_t alignment : 2;
                            uint8_t config    : 2;
                            uint8_t mode      : 3;
                            uint8_t polarity  : 1;
                        } bit;
                        constexpr static uint8_t address = 190;
                    } Ch2;

                    union Ch3 {
                        uint8_t all;
                        struct bit {
                            uint8_t alignment : 2;
                            uint8_t config    : 2;
                            uint8_t mode      : 3;
                            uint8_t polarity  : 1;
                        } bit;
                        constexpr static uint8_t address = 191;
                    } Ch3;

                    union Ch4 {
                        uint8_t all;
                        struct bit {
                            uint8_t alignment : 2;
                            uint8_t config    : 2;
                            uint8_t mode      : 3;
                            uint8_t polarity  : 1;
                        } bit;
                        constexpr static uint8_t address = 192;
                    } Ch4;

                    union Ch5 {
                        uint8_t all;
                        struct bit {
                            uint8_t alignment : 2;
                            uint8_t config    : 2;
                            uint8_t mode      : 3;
                            uint8_t polarity  : 1;
                        } bit;
                        constexpr static uint8_t address = 193;
                    } Ch5;

                    union Ch6 {
                        uint8_t all;
                        struct bit {
                            uint8_t alignment : 2;
                            uint8_t config    : 2;
                            uint8_t mode      : 3;
                            uint8_t polarity  : 1;
                        } bit;
                        constexpr static uint8_t address = 194;
                    } Ch6;

                    union Ch7 {
                        uint8_t all;
                        struct bit {
                            uint8_t alignment : 2;
                            uint8_t config    : 2;
                            uint8_t mode      : 3;
                            uint8_t polarity  : 1;
                        } bit;
                        constexpr static uint8_t address = 195;
                    } Ch7;
                } Config;

                struct DeadTime {
                    struct FallingEdge {
                        uint16_t Ch0;
                        uint16_t Ch1;
                        uint16_t Ch2;
                        uint16_t Ch3;
                        uint16_t Ch4;
                        uint16_t Ch5;
                        uint16_t Ch6;
                        uint16_t Ch7;
                        constexpr static uint8_t address = 196;
                    } FallingEdge;

                    struct RisingEdge {
                        uint16_t Ch0;
                        uint16_t Ch1;
                        uint16_t Ch2;
                        uint16_t Ch3;
                        uint16_t Ch4;
                        uint16_t Ch5;
                        uint16_t Ch6;
                        uint16_t Ch7;
                        constexpr static uint8_t address = 212;
                    } RisingEdge;
                } DeadTime;
            } Pwm;

            struct Dac {
                struct Range {
                    struct Max {
                        struct Ch0 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 228;
                        } Ch0;

                        struct Ch1 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 229;
                        } Ch1;

                        struct Ch2 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 230;
                        } Ch2;

                        struct Ch3 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 231;
                        } Ch3;

                        struct Ch4 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 232;
                        } Ch4;

                        struct Ch5 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 233;
                        } Ch5;

                        struct Ch6 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 234;
                        } Ch6;

                        struct Ch7 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 235;
                        } Ch7;
                    } Max;

                    struct Min {
                        struct Ch0 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 236;
                        } Ch0;

                        struct Ch1 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 237;
                        } Ch1;

                        struct Ch2 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 238;
                        } Ch2;

                        struct Ch3 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 239;
                        } Ch3;

                        struct Ch4 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 240;
                        } Ch4;

                        struct Ch5 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 241;
                        } Ch5;

                        struct Ch6 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 242;
                        } Ch6;

                        struct Ch7 {
                            union value {
                                int8_t asInteger;
                                uint8_t asBit8;
                            } value;
                            constexpr static uint8_t address = 243;
                        } Ch7;
                    } Min;
                } Range;
            } Dac;

            struct AutoRead {
                struct Period {
                    union value {
                        uint32_t asBit32;
                        uint8_t asBit8[4];
                    } value;
                    constexpr static uint8_t address = 244;
                    constexpr static uint8_t defaultValue = 0;
                } Period;

                struct FirstReg {
                    uint8_t value;
                    constexpr static uint8_t address = 248;
                    constexpr static uint8_t defaultValue = 0;
                } FirstReg;

                struct Length {
                    uint8_t value;
                    constexpr static uint8_t address = 249;
                    constexpr static uint8_t defaultValue = 0;
                } Length;

                uint8_t reserved[2]; // 250, 251
            } AutoRead;

            /*
            struct Log {
                struct Period {
                    union value {
                        uint8_t asBit8[2];
                        uint16_t asBit16;
                        int16_t asInteger;
                    } value;
                    constexpr static uint8_t address = 220;
                    constexpr static uint8_t defaultValue = 0;
                } Period;
            } Log;
            */

            uint8_t reserved2[2]; // 252, 253

            struct BoardModel {
                uint8_t value;
                constexpr static uint8_t address = 254;
            } BoardModel;

            struct WhoAmI {
                uint8_t value;
                constexpr static uint8_t address = 255;
                constexpr static uint8_t defaultValue = 33;
            } WhoAmI;
        } Setup;
    } Reg;
} PACKED;

typedef union MemoryTableDescription MemoryTable;


#endif // MEMORY_TABLE_H
