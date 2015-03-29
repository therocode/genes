#pragma once
#include <cstdint>
#include <dna/gene.hpp>

namespace dna
{
    class GeneCodec
    {
        public:
            GeneCodec(uint32_t informationDensity);
            Gene encode(uint32_t value, uint32_t maxValue);
            void encode(uint32_t value, uint32_t maxValue, Gene& targetGene);
            uint32_t decode(const Gene& gene, uint32_t maxValue);
            uint32_t decode(const Gene& gene, uint32_t maxValue, uint32_t offset, uint32_t& decodeEnd);
        private:
            void convertToBase(uint32_t integer, uint32_t maxValue, uint32_t base, std::string& outString);
            uint32_t convertToNumber(const std::string& sequence, uint32_t maxValue, uint32_t offset, uint32_t base, uint32_t& newOffset);
            uint32_t digitsSize(uint32_t n, uint32_t base);
            uint32_t mInformationDensity;
    };
}
