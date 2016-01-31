#pragma once
#include <cstdint>
#include <vector>

namespace dna
{
    struct Information
    {
        int16_t identifier;
        std::vector<uint8_t> data;
    };

    bool operator==(const Information a, const Information b);
}
