#pragma once
#include <cstdint>
#include <dna/gene.hpp>
#include <unordered_map>
#include <bitset>
#include <vector>
#include <dna/information.hpp>
#include <dna/randomgenegenerator.hpp>
#include <dna/geneconfiguration.hpp>
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
            GeneCodec(GeneConfiguration configuration, int32_t seed = std::random_device()());
            Gene encode(const std::vector<Information>& informationContainer) const;
            void encode(const std::vector<Information>& informationContainer, Gene& output) const;
            std::vector<Information> decode(const Gene& toDecode) const;
            Gene toGene(uint32_t data, uint32_t maxValue) const;
            uint32_t fromGene(const Gene& gene, uint32_t maxValue) const;
        private:
            int32_t digitsAmount(uint32_t n) const;
            int32_t digitsAmount(uint32_t n, uint32_t base) const;
            bool dataExistsAfter(const Gene& gene, int32_t startPos, int32_t& nextPosition) const;
            uint32_t intPow(uint32_t base, uint32_t exp) const;
            GeneConfiguration mConfiguration;
            std::unordered_map<char, int32_t> mNucleotideIndices;
            mutable RandomNumberEngine mRandomNumberEngine;
            char mZeroNucleotide;
    };

    template <typename RandomNumberEngine>
    GeneCodec<RandomNumberEngine>::GeneCodec(GeneConfiguration configuration, int32_t seed):
        mConfiguration(std::move(configuration)),
        mRandomNumberEngine(seed),
        mZeroNucleotide(mConfiguration.nucleotides.front())
    {
        int32_t i = 0;

        for(char nucleotide : mConfiguration.nucleotides)
        {
            mNucleotideIndices.emplace(nucleotide, i);
            ++i;
        }
    }

    template <typename RandomNumberEngine>
    Gene GeneCodec<RandomNumberEngine>::encode(const std::vector<Information>& informationContainer) const
    {
        Gene result;
        encode(informationContainer, result);
        return std::move(result);
    }

    template <typename RandomNumberEngine>
    void GeneCodec<RandomNumberEngine>::encode(const std::vector<Information>& informationContainer, Gene& output) const
    {
        if(informationContainer.empty())
            return;

        std::uniform_int_distribution<> spreadGen(mConfiguration.minSpread, mConfiguration.maxSpread);

        RandomGeneGenerator<RandomNumberEngine> randomGeneGenerator(mConfiguration, {mConfiguration.segmentStartCode, mConfiguration.segmentEndCode}, mRandomNumberEngine());
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

                for(int32_t i = 0; i < 4 && informationDataIter != information.data.end(); ++i, ++informationDataIter)
                {
                    uint8_t byte = *informationDataIter;

                    batchBits <<= 8;
                    batchBits |= byte;

                    batchSize += 8;
                }

                uint32_t maxValue = std::pow(2, batchSize) - 1;
                dataParts.emplace_back(toGene(batchBits.to_ulong(), maxValue));
            }

            size_t identifierPartOldSize = identifierPart.size();

            //apply start and end markers
            identifierPart = mConfiguration.segmentStartCode + identifierPart;

            //sum sizes
            int32_t totalDataSize = 0;
            for(const auto& dataPart : dataParts)
                totalDataSize += dataPart.size();

            int32_t paddingSize = spreadGen(mRandomNumberEngine);
            dataParts.back() = dataParts.back() + mConfiguration.segmentEndCode + randomGeneGenerator.generate(paddingSize);

            Gene dataLengthPart = toGene(information.data.size() * 8, std::numeric_limits<int32_t>().max());

            output.resize(output.size() + identifierPart.size() + dataLengthPart.size() + totalDataSize + mConfiguration.segmentEndCode.size() + paddingSize);

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
    std::vector<Information> GeneCodec<RandomNumberEngine>::decode(const Gene& toDecode) const
    {
        std::vector<Information> result;

        int32_t currentSearchPos = 0;
        int32_t nextDataPosition = 0;

        while(dataExistsAfter(toDecode, currentSearchPos, nextDataPosition))
        {
            int16_t identifierMaxValue = std::numeric_limits<int16_t>().max();
            uint32_t identifierPartDigitAmount = digitsAmount(identifierMaxValue);
            Gene identifierPart(toDecode, nextDataPosition, identifierPartDigitAmount);

            uint32_t identifier = fromGene(identifierPart, identifierMaxValue);

            nextDataPosition += identifierPartDigitAmount;

            int32_t dataLengthMaxValue = std::numeric_limits<int32_t>().max();
            uint32_t dataLengthDigitAmount = digitsAmount(dataLengthMaxValue);
            Gene dataLengthPart(toDecode, nextDataPosition, dataLengthDigitAmount);

            uint32_t dataLength = fromGene(dataLengthPart, dataLengthMaxValue);

            nextDataPosition += dataLengthDigitAmount;

            std::vector<uint8_t> resultData;
            uint32_t remainingDataBits = dataLength;
            while(remainingDataBits != 0)
            {
                int32_t batchSize = std::min(32u, remainingDataBits);
                uint32_t maxValue = intPow(2, batchSize) - 1;
                uint32_t thisDigitLength = digitsAmount(maxValue);

                Gene dataPart(toDecode, nextDataPosition, thisDigitLength);

                uint32_t data = fromGene(dataPart, maxValue);
                std::bitset<32> dataBits(data);

                resultData.resize(resultData.size() + batchSize / 8);
                for(int32_t i = 0; i < batchSize / 8; ++i)
                {
                    uint8_t entry = dataBits.to_ulong() & 255;
                    dataBits >>= 8;
                    resultData[resultData.size() - 1 - i] = entry;
                }

                remainingDataBits -= batchSize;
                nextDataPosition += thisDigitLength;
            }

            result.emplace_back(Information{static_cast<int16_t>(identifier), std::move(resultData)});

            currentSearchPos = nextDataPosition;
        }
        
        return result;
    }

    template <typename RandomNumberEngine>
    Gene GeneCodec<RandomNumberEngine>::toGene(uint32_t data, uint32_t maxValue) const
    {
        DNA_ASSERT(data <= maxValue, "cannot pass bigger integer than maxValue");
        DNA_ASSERT(maxValue > 0, "maxValue cannot be zero");

        uint32_t base = mConfiguration.nucleotides.size();
        uint32_t digitAmount = digitsAmount(maxValue);

        Gene result(digitAmount, mConfiguration.nucleotides.front());

        uint32_t writeIndex = 0;

        uint32_t left = data;
        while(left)
        {
            uint32_t thisLeft = left;

            left = thisLeft / base;
            uint32_t thisValue = thisLeft % base; //smart compilers just get the remainder STD DIV!!!!!!!!!!!!!!!!!!!!

            result[writeIndex] = mConfiguration.nucleotides[thisValue];

            ++writeIndex;
        }

        return result;
    }

    template <typename RandomNumberEngine>
    uint32_t GeneCodec<RandomNumberEngine>::fromGene(const Gene& gene, uint32_t maxValue) const
    {
        DNA_ASSERT(maxValue > 0, "maxValue cannot be zero");

        uint32_t base = mConfiguration.nucleotides.size();
        uint32_t digitAmount = digitsAmount(maxValue);

        uint32_t multiplier = 1;
        uint32_t result = 0;

        for(int32_t i = 0; i < digitAmount && i < gene.size(); ++i)
        {
            char nucleotide = gene[i];
            result += mNucleotideIndices.at(nucleotide) * multiplier;
            multiplier *= base;
        }

        return result;
    }

    template <typename RandomNumberEngine>
    int32_t GeneCodec<RandomNumberEngine>::digitsAmount(uint32_t n) const
    {
        uint32_t base = mConfiguration.nucleotides.size();
        return digitsAmount(n, base);
    }

    template <typename RandomNumberEngine>
    int32_t GeneCodec<RandomNumberEngine>::digitsAmount(uint32_t n, uint32_t base) const
    {
        int s = int(std::log10(1.0*n)/std::log10(1.0*base)) + 1;
        return std::pow(base, s) > n ? s : s+1;
    }

    template <typename RandomNumberEngine>
    bool GeneCodec<RandomNumberEngine>::dataExistsAfter(const Gene& gene, int32_t startPos, int32_t& nextPosition) const
    {
        int32_t nextStart = gene.find(mConfiguration.segmentStartCode, startPos);
        int32_t nextEnd = gene.find(mConfiguration.segmentEndCode, startPos);

        if(nextStart != Gene::npos && nextEnd != Gene::npos)
        {
            nextPosition = nextStart + mConfiguration.segmentStartCode.size();
            return true;
        }
        else
        {
            return false;
        }
    }

    template <typename RandomNumberEngine>
    uint32_t GeneCodec<RandomNumberEngine>::intPow(uint32_t base, uint32_t exp) const
    {
        if(exp == 0)
            return 1;

        uint32_t accumulator = base;

        while(exp > 1)
        {
            accumulator *= base;
            --exp;
        }

        return accumulator;
    }
}
