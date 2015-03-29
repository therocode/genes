#pragma once
#include <string>
#include <unordered_map>

const std::string charTable = "ATCGEFDHIJKLMNOPQRSBUVWXYZ";
const std::unordered_map<char, uint32_t> numberTable = 
{
    {'A', 0},
    {'T', 1},
    {'C', 2},
    {'G', 3},
    {'E', 4},
    {'F', 5},
    {'D', 6},
    {'H', 7},
    {'I', 8},
    {'J', 9},
    {'K', 10},
    {'L', 11},
    {'M', 12},
    {'N', 13},
    {'O', 14},
    {'P', 15},
    {'Q', 16},
    {'R', 17},
    {'S', 18},
    {'B', 19},
    {'U', 20},
    {'V', 21},
    {'W', 22},
    {'X', 23},
    {'Y', 24},
    {'Z', 25}
};
