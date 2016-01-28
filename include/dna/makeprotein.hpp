#pragma once
#include <dna/protein.hpp>
#include <dna/allele.hpp>

namespace dna
{
    template <typename AlleleType>
    ProteinList<typename AlleleType::Type> makeProteins(const std::vector<AlleleType>& competingAlleles)
    {
        return ProteinList<typename AlleleType::Type>();
    }
}
