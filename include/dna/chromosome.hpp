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
            Chromosome(uint32_t maxCistronIds, uint32_t informationDensity, const Gene& startSequence, const Gene& stopSequence, const Gene& gene);
            uint32_t cistronAmount() const;
            CistronMap cistrons() const;
            const Gene& gene() const;
            uint32_t maxCistronIds() const;
            uint32_t informationDensity() const;
            const Gene& startSequence() const;
            const Gene& stopSequence() const;
            void replaceCistronValues(const std::vector<Cistron>& cistrons);
        private:
            uint32_t mInformationDensity;
            uint32_t mMaxCistronIds;
            Gene mStartSequence;
            Gene mStopSequence;
            Gene mGene;
    };
}
