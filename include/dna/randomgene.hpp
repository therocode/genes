#pragma once
#include <dna/gene.hpp>

namespace dna
{
    class RandomGene
    {
        public:
            static Gene ofSize(uint32_t informationDensity, uint32_t size);
            static Gene ofRange(uint32_t informationDensity, uint32_t min, uint32_t max);
    };
}
