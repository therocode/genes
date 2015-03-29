#include <cmath>
#include <unordered_map>
#include <dna/genecodec.hpp>
#include <dna/assert.hpp>

const std::string charTable = "ATCGEFDHIJKLMNOPQRSBUVWXYZ";
const std::unordered_map<char, uint32_t> numberTable = 
{
    {'A', 0},
    {'T', 1},
    {'C', 2},
    {'G', 3},
    {'E', 4},
    {'F', 5},
    {'D', 6},
    {'H', 7},
    {'I', 8},
    {'J', 9},
    {'K', 10},
    {'L', 11},
    {'M', 12},
    {'N', 13},
    {'O', 14},
    {'P', 15},
    {'Q', 16},
    {'R', 17},
    {'S', 18},
    {'B', 19},
    {'U', 20},
    {'V', 21},
    {'W', 22},
    {'X', 23},
    {'Y', 24},
    {'Z', 25}
};

namespace dna
{
    GeneCodec::GeneCodec(uint32_t informationDensity):
        mInformationDensity(informationDensity)
    {
        DNA_ASSERT(informationDensity > 1, "information density must be bigger than 1");
    }

    Gene GeneCodec::encode(uint32_t value, uint32_t maxValue)
    {
        std::string result;
        convertToBase(value, maxValue, mInformationDensity, result);
        return result;
    }

    void GeneCodec::encode(uint32_t value, uint32_t maxValue, Gene& targetGene)
    {
        convertToBase(value, maxValue, mInformationDensity, targetGene);
    }

    uint32_t GeneCodec::decode(const Gene& gene, uint32_t maxValue)
    {
        uint32_t dummy;
        return convertToNumber(gene, maxValue, 0, mInformationDensity, dummy);
    }

    uint32_t GeneCodec::decode(const Gene& gene, uint32_t maxValue, uint32_t offset, uint32_t& decodeEnd)
    {
        return convertToNumber(gene, maxValue, offset, mInformationDensity, decodeEnd);
    }

    void GeneCodec::convertToBase(uint32_t integer, uint32_t maxValue, uint32_t base, std::string& outString)
    {
        DNA_ASSERT(integer <= maxValue, "cannot pass bigger integer than maxValue");
        DNA_ASSERT(base <= charTable.size(), "base cannot be bigger than " + std::to_string(charTable.size()));
        DNA_ASSERT(base > 0, "base cannot be zero");
        DNA_ASSERT(maxValue > 0, "maxValue cannot be zero");

        uint32_t digitAmount = digitsSize(maxValue, base);
        std::string result;
        result.resize(digitAmount, charTable[0]);
        int32_t writeIndex = 0;

        int32_t left = integer;
        while(left)
        {
            int32_t thisLeft = left;

            left = thisLeft / base;
            int32_t thisValue = thisLeft % base; //smart compilers just get the remainder STD DIV!!!!!!!!!!!!!!!!!!!!

            result[writeIndex] = charTable[thisValue];

            ++writeIndex;
        }

        outString.append(result);
    }

    uint32_t GeneCodec::convertToNumber(const std::string& sequence, uint32_t maxValue, uint32_t offset, uint32_t base, uint32_t& newOffset)
    {
        DNA_ASSERT(base <= charTable.size(), "base cannot be bigger than " + std::to_string(charTable.size()));
        DNA_ASSERT(base > 0, "base cannot be zero");
        DNA_ASSERT(maxValue > 0, "maxValue cannot be zero");
        
        uint32_t maxDigitsToProcess = digitsSize(maxValue, base);
        uint32_t currentDigit = 0;
        uint32_t value = 0;
        uint32_t multiplier = 1;

        std::string subSequence = sequence.substr(offset, maxDigitsToProcess);
        newOffset = offset + std::min((size_t)maxDigitsToProcess, subSequence.size());

        while(currentDigit < maxDigitsToProcess && currentDigit < subSequence.size())
        {
            uint32_t currentDigitValue = numberTable.at(subSequence[currentDigit]);
            DNA_ASSERT(currentDigitValue < base, "given gene " + sequence + " does not contain only base " + std::to_string(base) + " numbers");
            value += currentDigitValue * multiplier;
            multiplier *= base;
            ++currentDigit;
        }

        return value;
    }

    uint32_t GeneCodec::digitsSize(uint32_t n, uint32_t base)
    {
        int s = int(std::log10(1.0*n)/std::log10(1.0*base)) + 1;
        return std::pow(base, s) > n ? s : s+1;
    }
}
