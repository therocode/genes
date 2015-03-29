#include <dna/assert.hpp>
#include <dna/randomgene.hpp>
#include <dna/genetable.hpp>
#include <random>

namespace dna
{
    Gene RandomGene::ofSize(uint32_t informationDensity, uint32_t size)
    {
        DNA_ASSERT(informationDensity > 1, "not enough information density");
        std::random_device rd;
        std::uniform_int_distribution<uint32_t> random(0, informationDensity - 1);

        Gene gene;
        gene.resize(size);

        for(char& c : gene)
        {
            c = charTable[random(rd)];
        }

        return gene;
    }

    Gene RandomGene::ofRange(uint32_t informationDensity, uint32_t min, uint32_t max)
    {
        DNA_ASSERT(informationDensity > 1, "not enough information density");
        DNA_ASSERT(min <= max, "min must be smaller than or equal to max");

        std::random_device rd;
        std::uniform_int_distribution<uint32_t> random(min, max);

        return ofSize(informationDensity, random(rd));
    }
}
