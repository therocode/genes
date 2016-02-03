#pragma once
#include <vector>
#include <dna/gene.hpp>

namespace dna
{
    struct GeneConfiguration
    {
        std::vector<char> nucleotides;
        Gene segmentStartCode;
        Gene segmentEndCode;
        int32_t minSpread;
        int32_t maxSpread;
    };
}
