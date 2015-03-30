#pragma once
#include <dna/chromosome.hpp>

namespace dna
{
    class Breeder
    {
        public:
            struct BreedStats
            {
                uint32_t crossOvers;  
            };
            std::pair<Chromosome, BreedStats> breed(const Chromosome& mum, const Chromosome& dad, uint32_t crossOverPerMillion);
    };
}
