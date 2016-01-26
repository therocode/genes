#pragma once
#include <cstdint>
#include <deque>

namespace dna
{
    struct Information
    {
        int16_t identifier;
        std::deque<bool> data;
    };

    bool operator==(const Information a, const Information b)
    {
        return a.identifier == b.identifier && a.data == b.data;
    }
}
