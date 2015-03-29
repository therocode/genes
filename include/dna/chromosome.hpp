#pragma once
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <dna/cistron.hpp>

namespace dna
{
    using CistronMap = std::unordered_map<uint32_t, Cistron>;
    class Chromosome
    {
        public:
            Chromosome(uint32_t maxCistronIds, uint32_t informationDensity, const Gene& startSequence, const Gene& stopSequence, const std::vector<Cistron>& cistrons, uint32_t minJunkGenes, uint32_t maxJunkGenes);
            uint32_t cistronAmount() const;
            CistronMap cistrons() const;
            const Gene& gene() const;
        private:
            uint32_t mInformationDensity;
            uint32_t mMaxCistronIds;
            Gene mStartSequence;
            Gene mStopSequence;
            Gene mGene;
    };
}
