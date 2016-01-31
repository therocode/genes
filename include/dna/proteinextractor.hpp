#pragma once
#include <dna/protein.hpp>
#include <dna/chromosome.hpp>

namespace dna
{
    class ProteinExtractor
    {
        public:
            template <typename ProteinType, size_t tChromosomeAmount>
            ProteinList<ProteinType> extract(int32_t identifier, const Chromosome<tChromosomeAmount>& chromosome);
    };


    template <typename ProteinType, size_t tChromosomeAmount>
     ProteinList<ProteinType> ProteinExtractor::extract(int32_t identifier, const Chromosome<tChromosomeAmount>& chromosome)
     {
         return {};
     }
};
