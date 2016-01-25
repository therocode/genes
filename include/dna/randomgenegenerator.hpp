#pragma once
#include <dna/gene.hpp>
#include <unordered_set>
#include <random>
#include <iostream>

namespace dna
{
    template <typename RandomNumberEngine = std::mt19937>
    class RandomGeneGenerator
    {
        public:
            RandomGeneGenerator(std::unordered_set<Nucleotide> avaliableNucleotides, std::unordered_set<Gene> illegalSequences = {}, int32_t seed = std::random_device()());
            Gene generate(int32_t length) const;
        private:
            bool containsIllegal(const Gene& gene, size_t startSearch, std::pair<size_t, size_t>& illegalStartAndSize) const;
            mutable RandomNumberEngine mRandomNumberEngine;
            std::vector<Nucleotide> mAvailableNucleotides;
            std::unordered_set<Gene> mIllegalSequences;
            int64_t mBiggestIllegalSize;
    };

    template <typename RandomNumberEngine>
    RandomGeneGenerator<RandomNumberEngine>::RandomGeneGenerator(std::unordered_set<Nucleotide> avaliableNucleotides, std::unordered_set<Gene> illegalSequences, int32_t seed):
        mAvailableNucleotides(avaliableNucleotides.begin(), avaliableNucleotides.end()),
        mIllegalSequences(std::move(illegalSequences)),
        mRandomNumberEngine(seed),
        mBiggestIllegalSize(0)
    {
        for(const auto& illegal : mIllegalSequences)
            mBiggestIllegalSize = std::max(static_cast<int64_t>(illegal.size()), mBiggestIllegalSize);
    }

    template <typename RandomNumberEngine>
    Gene RandomGeneGenerator<RandomNumberEngine>::generate(int32_t length) const
    {
        std::uniform_int_distribution<size_t> range(0, mAvailableNucleotides.size() - 1);

        Gene result(length, ' ');

        for(int32_t i = 0; i < length; ++i)
            result[i] = mAvailableNucleotides[range(mRandomNumberEngine)];

        std::pair<size_t, size_t> illegalStartAndSize;
        size_t safePos = 0;

        while(containsIllegal(result, safePos, illegalStartAndSize))
        {
            int32_t rewriteStart = illegalStartAndSize.first;
            for(int32_t i = rewriteStart; i < rewriteStart + illegalStartAndSize.second; ++i)
            {
                result[i] = mAvailableNucleotides[range(mRandomNumberEngine)];
                std::cout << i << " ";
            }

            std::cout << "\n";
            safePos = std::max(int64_t(0), static_cast<int64_t>(rewriteStart) - mBiggestIllegalSize);

            std::cout << "rewrote from " << rewriteStart << " and rewrote " <<  illegalStartAndSize.second << "\n";
            std::cout << containsIllegal(result, safePos, illegalStartAndSize) << " " << illegalStartAndSize.first << " is safepos\n";
            std::cout << containsIllegal(result, 0, illegalStartAndSize) << " " << illegalStartAndSize.first << " is 0\n\n";
        }

        return result;
    }

    template <typename RandomNumberEngine>
    bool RandomGeneGenerator<RandomNumberEngine>::containsIllegal(const Gene& gene, size_t startSearch, std::pair<size_t, size_t>& illegalStartAndSize) const
    {
        for(const auto& illegalSequence : mIllegalSequences)
        {
            size_t position = gene.find(illegalSequence, startSearch);
            if(position != Gene::npos)
            {
                illegalStartAndSize.first = position;
                illegalStartAndSize.second = illegalSequence.size();
                return true;
            }
        }

        return false;
    }
}
