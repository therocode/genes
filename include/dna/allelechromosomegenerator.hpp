#pragma once
#include <dna/chromosome.hpp>

namespace dna
{
    template <int32_t chromosomeAmount>
    class AlleleChromosomeGenerator
    {
        public:
            template <typename Allele, typename... Alleles>
            Chromosome<chromosomeAmount> generate(const std::pair<int32_t, Allele>& first, const std::pair<int32_t, Alleles>&... tail);
    };
}
