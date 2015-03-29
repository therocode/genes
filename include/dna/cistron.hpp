#pragma once
#include <vector>
#include <cstdint>
#include <dna/gene.hpp>

namespace dna
{
    class Cistron
    {
        public:
            struct CistronValue
            {
                uint32_t value;
                uint32_t max;
            };
            struct CistronOutValue
            {
                uint32_t& outValue;
                uint32_t max;
            };

            Cistron(uint32_t id, uint32_t maxCistronIds, uint32_t informationDensity, const std::vector<CistronValue>& values);
            Cistron(const Gene& gene, uint32_t maxCistronIds, uint32_t informationDensity);
            const Gene& gene() const;
            void decode(const std::vector<CistronOutValue>& outValues) const;
            uint32_t id() const;
        private:
            Gene mGene;
            uint32_t mId;
            uint32_t mInformationDensity;
            uint32_t mDataOffset;
    };
}
