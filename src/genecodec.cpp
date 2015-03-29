#include <cmath>
#include <dna/genecodec.hpp>
#include <dna/assert.hpp>

const std::string charTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

namespace dna
{
    GeneCodec::GeneCodec(uint32_t informationDensity):
        mInformationDensity(informationDensity)
    {
    }

    Gene GeneCodec::encode(uint32_t value, uint32_t maxValue)
    {
        return convertToBase(value, maxValue, mInformationDensity);
    }

    void GeneCodec::encode(uint32_t value, uint32_t maxValue, Gene& targetGene)
    {
    }

    uint32_t GeneCodec::decode(const Gene& gene, uint32_t maxValue)
    {
        return 0;
    }

    uint32_t GeneCodec::decode(const Gene& gene, uint32_t maxValue, uint32_t offset, uint32_t& decodeEnd)
    {
        return 0;
    }

    std::string GeneCodec::convertToBase(uint32_t integer, uint32_t maxValue, uint32_t base)
    {
        DNA_ASSERT(integer <= maxValue, "cannot pass bigger integer than maxValue");
        DNA_ASSERT(base < charTable.size(), "base cannot be bigger than" + std::to_string(charTable.size()));
        DNA_ASSERT(base > 0, "base cannot be zero");

        uint32_t digitAmount = digitsSize(maxValue, base);
        std::string result;
        result.resize(digitAmount, charTable[0]);
        int32_t writeIndex = 0;

        int32_t left = integer;
        while(left)
        {
            int32_t thisLeft = left;

            left = thisLeft / base;
            int32_t thisValue = thisLeft % base; //smart compilers just get the remainder

            result[writeIndex] = charTable[thisValue];

            ++writeIndex;
        }

        return result;
    }

    uint32_t GeneCodec::digitsSize(uint32_t n, uint32_t base)
    {
        int s = int(std::log10(1.0*n)/std::log10(1.0*base)) + 1;
        return std::pow(base, s) > n ? s : s+1;
    }
}
