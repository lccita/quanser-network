#include "definitions.hpp"

std::vector<std::string> supportedBoards = {"q2_usb", "q8_usb", "dummy"};

const int boardInfo[boardNumber][featureNumber] = {
    { 0,        0,  0, 0,  0, 0,  0, 0, 0}, // boardNotFound
    { 3, 40000000, 16, 2, 12, 2, 12, 2, 2}, // boardQ2USB
    {24, 49000000, 16, 8, 16, 8, 16, 8, 8}, // boardQ8USB
    {24, 49000000, 16, 8, 16, 8, 16, 8, 8}, // boardDummy
};


