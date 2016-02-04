#pragma once
#include <dna/assert.hpp>
#include <dna/protein.hpp>
#include <dna/allele.hpp>

namespace dna
{
    template <typename AlleleType>
    typename std::enable_if<AlleleType::competeType == CompeteType::Exclusive, ProteinList<typename AlleleType::Type>>::type makeProteins(const std::vector<AlleleType>& competingAlleles)
    {
        if(competingAlleles.empty())
            return {};

        ProteinList<typename AlleleType::Type> result;

        int32_t biggestStrength = std::numeric_limits<int32_t>().min();
        std::vector<const AlleleType*> strongestAlleles;

        for(const auto& allele : competingAlleles)
        {
            if(strongestAlleles.empty())
            {
                strongestAlleles.push_back(&allele);
                biggestStrength = allele.strength();
            }
            else
            {
                if(allele.strength() == biggestStrength)
                    strongestAlleles.push_back(&allele);
                else if(allele.strength() > biggestStrength)
                {
                    strongestAlleles.clear();
                    strongestAlleles.push_back(&allele);
                    biggestStrength = allele.strength();
                }
            }
        }

        DNA_ASSERT(!strongestAlleles.empty(), "should not be empty");
        for(auto winnerAllele : strongestAlleles)
        {
            result.emplace_back(Protein<typename AlleleType::Type>(winnerAllele->value()));
        }

        return result;
    }

    template <typename AlleleType>
    typename std::enable_if<AlleleType::competeType == CompeteType::Mix, ProteinList<typename AlleleType::Type>>::type makeProteins(const std::vector<AlleleType>& competingAlleles)
    {
        if(competingAlleles.empty())
            return {};

        ProteinList<typename AlleleType::Type> result;

        double totalStrength = 0.0f;

        for(const auto& allele : competingAlleles)
        {
            double strength = allele.strength();
                totalStrength += strength;
        }

        const AlleleType& firstAllele = competingAlleles[0];
        typename AlleleType::Type mixedValue = firstAllele.value() * (static_cast<double>(firstAllele.strength()) / static_cast<double>(totalStrength));

        for(int32_t i = 1; i < competingAlleles.size(); ++i)
        {
            const AlleleType& allele = competingAlleles[i];

            mixedValue += allele.value() * (static_cast<double>(allele.strength() / static_cast<double>(totalStrength)));
        }

        result.emplace_back(Protein<typename AlleleType::Type>(std::move(mixedValue)));

        return result;
    }
}
