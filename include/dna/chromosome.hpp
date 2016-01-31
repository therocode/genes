#pragma once
#include <array>
#include <dna/gene.hpp>

namespace dna
{
    template <size_t tGeneAmount>
    using Chromosome = std::array<Gene, tGeneAmount>;
}
