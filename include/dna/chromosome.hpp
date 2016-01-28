#pragma once
#include <array>

template <int32_t tGeneAmount>
using Chromosome = std::Array<Gene, tGeneAmount>;
