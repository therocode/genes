#pragma once
#include <dna/chromosome.hpp>

namespace dna
{
    template <size_t tChromosomeAmount>
    class ChromosomeGenerator
    {
        public:
            template <typename Allele, typename... Alleles>
            Chromosome<tChromosomeAmount> generate(const std::pair<int32_t, Allele>& first, const std::pair<int32_t, Alleles>&... tail);
    };

    template <size_t tChromosomeAmount>
    template <typename Allele, typename... Alleles>
    Chromosome<tChromosomeAmount> ChromosomeGenerator<tChromosomeAmount>::generate(const std::pair<int32_t, Allele>& first, const std::pair<int32_t, Alleles>&... tail)
    {
        return {};
    }
}
