#pragma once
#include <cstdint>
#include <dna/gene.hpp>
#include <unordered_set>
#include <bitset>
#include <vector>
#include <dna/information.hpp>
#include <dna/randomgenegenerator.hpp>
#include <dna/assert.hpp>
#include <iostream>
#include <limits>
#include <random>

namespace dna
{
    template <typename RandomNumberEngine = std::mt19937>
    class GeneCodec
    {
        public:
            GeneCodec(const std::vector<char>& availableNucleotides, const Gene& segmentStartCode, const Gene& segmentEndCode, int32_t minSpread, int32_t maxSpread, int32_t seed = std::random_device()());
            template<typename InformationContainer>
            Gene encode(InformationContainer&& informationContainer) const;
            template<typename InformationContainer>
            void encode(InformationContainer&& informationContainer, Gene& output) const;
        private:
            Gene toGene(uint32_t data, uint32_t maxValue) const;
            int32_t digitsAmount(uint32_t n, int32_t base) const;
            std::vector<char> mAvailableNucleotides;
            mutable RandomNumberEngine mRandomNumberEngine;
            Gene mSegmentStartCode;
            Gene mSegmentEndCode;
            int32_t mMinSpread;
            int32_t mMaxSpread;
            char mZeroNucleotide;
    };

    template <typename RandomNumberEngine>
    GeneCodec<RandomNumberEngine>::GeneCodec(const std::vector<char>& availableNucleotides, const Gene& segmentStartCode, const Gene& segmentEndCode, int32_t minSpread, int32_t maxSpread, int32_t seed):
        mAvailableNucleotides(availableNucleotides),
        mRandomNumberEngine(seed),
        mSegmentStartCode(segmentStartCode),
        mSegmentEndCode(segmentEndCode),
        mMinSpread(minSpread),
        mMaxSpread(maxSpread),
        mZeroNucleotide(mAvailableNucleotides.front())
    {
    }

    template <typename RandomNumberEngine>
    template<typename InformationContainer>
    Gene GeneCodec<RandomNumberEngine>::encode(InformationContainer&& informationContainer) const
    {
        Gene result;
        encode(std::forward<InformationContainer>(informationContainer), result);
        return std::move(result);
    }

    template <typename RandomNumberEngine>
    template<typename InformationContainer>
    void GeneCodec<RandomNumberEngine>::encode(InformationContainer&& informationContainer, Gene& output) const
    {
        if(informationContainer.empty())
            return;

        std::uniform_int_distribution<> spreadGen(mMinSpread, mMaxSpread);

        RandomGeneGenerator<RandomNumberEngine> randomGeneGenerator(mAvailableNucleotides, {mSegmentStartCode, mSegmentEndCode}, mRandomNumberEngine());
        output = randomGeneGenerator.generate(spreadGen(mRandomNumberEngine));
        auto nextOutputIndex = output.size();

        for(auto informationIter : informationContainer)
        {
            Information information = informationIter;

            constexpr int32_t identifierBitLength = sizeof(information.identifier) * 8;
            uint32_t identifierData = information.identifier;

            Gene identifierPart = toGene(identifierData, std::numeric_limits<decltype(information.identifier)>().max());

            std::vector<Gene> dataParts;
            auto informationDataIter = information.data.begin();

            while(informationDataIter != information.data.end())
            {
                std::bitset<32> batchBits;
                int32_t batchSize = 0;

                for(int32_t i = 0; i < 32 && informationDataIter != information.data.end(); ++i, ++informationDataIter)
                {
                    batchBits[i] = *informationDataIter;
                    ++batchSize;
                }

                uint32_t maxValue = std::pow(2, batchSize) - 1;
                dataParts.emplace_back(toGene(batchBits.to_ulong(), maxValue));
            }

            size_t identifierPartOldSize = identifierPart.size();

            //apply start and end markers
            identifierPart = mSegmentStartCode + identifierPart;

            //sum sizes
            int32_t totalDataSize = 0;
            for(const auto& dataPart : dataParts)
                totalDataSize += dataPart.size();

            int32_t paddingSize = spreadGen(mRandomNumberEngine);
            dataParts.back() = dataParts.back() + mSegmentEndCode + randomGeneGenerator.generate(paddingSize);

            Gene dataLengthPart = toGene(totalDataSize, std::numeric_limits<decltype(totalDataSize)>().max());

            output.resize(output.size() + identifierPart.size() + dataLengthPart.size() + totalDataSize + mSegmentEndCode.size() + paddingSize);

            std::copy(identifierPart.begin(), identifierPart.end(), output.begin() + nextOutputIndex);
            nextOutputIndex += identifierPart.end() - identifierPart.begin();

            std::copy(dataLengthPart.begin(), dataLengthPart.end(), output.begin() + nextOutputIndex);
            nextOutputIndex += dataLengthPart.end() - dataLengthPart.begin();

            for(int32_t i = 0; i < dataParts.size(); ++i)
            {
                const auto& dataPart = dataParts[i];
                std::copy(dataPart.begin(), dataPart.end(), output.begin() + nextOutputIndex);
                nextOutputIndex += dataPart.end() - dataPart.begin();
            }
        }
    }

    template <typename RandomNumberEngine>
    Gene GeneCodec<RandomNumberEngine>::toGene(uint32_t data, uint32_t maxValue) const
    {
        DNA_ASSERT(data <= maxValue, "cannot pass bigger integer than maxValue");
        DNA_ASSERT(maxValue > 0, "maxValue cannot be zero");

        uint32_t base = mAvailableNucleotides.size();
        uint32_t digitAmount = digitsAmount(maxValue, base);

        Gene result(digitAmount, mAvailableNucleotides.front());

        uint32_t writeIndex = 0;

        uint32_t left = data;
        while(left)
        {
            uint32_t thisLeft = left;

            left = thisLeft / base;
            uint32_t thisValue = thisLeft % base; //smart compilers just get the remainder STD DIV!!!!!!!!!!!!!!!!!!!!

            result[writeIndex] = mAvailableNucleotides[thisValue];

            ++writeIndex;
        }

        return result;
    }

    template <typename RandomNumberEngine>
    int32_t GeneCodec<RandomNumberEngine>::digitsAmount(uint32_t n, int32_t base) const
    {
        int s = int(std::log10(1.0*n)/std::log10(1.0*base)) + 1;
        return std::pow(base, s) > n ? s : s+1;
    }
}
