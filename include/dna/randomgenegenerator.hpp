#pragma once
#include <dna/gene.hpp>
#include <unordered_set>
#include <random>
#include <iostream>
#include <algorithm>

namespace dna
{
    template <typename RandomNumberEngine = std::mt19937>
        class RandomGeneGenerator
        {
            public:
                RandomGeneGenerator(std::unordered_set<Nucleotide> avaliableNucleotides, std::unordered_set<Gene> illegalSequences = {}, int32_t seed = std::random_device()());
                Gene generate(int32_t length) const;
            private:
                bool containsIllegal(const Gene& gene, size_t startSearch, std::vector<std::pair<size_t, size_t>>& illegalStartAndSize) const;
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

            std::vector<std::pair<size_t, size_t>> illegalStartAndSize;
            int64_t safePos = 0;

            while(containsIllegal(result, safePos, illegalStartAndSize))
            {
                safePos = length;
                for(const auto& startSizeIter : illegalStartAndSize)
                {
                    int32_t rewriteStart = startSizeIter.first;
                    for(int32_t i = rewriteStart; i < rewriteStart + startSizeIter.second; ++i)
                    {
                        result[i] = mAvailableNucleotides[range(mRandomNumberEngine)];
                        static int kalle = 0;
                        ++kalle;
                        std::cout << "edited nuc: " << kalle << "\n";
                    }

                    safePos = std::max(int64_t(0), std::min(safePos, static_cast<int64_t>(rewriteStart) - mBiggestIllegalSize));
                }

                safePos = std::max(int64_t(0), safePos);
            }

            return result;
        }


    template <typename RandomNumberEngine>
        bool RandomGeneGenerator<RandomNumberEngine>::containsIllegal(const Gene& gene, size_t startSearch, std::vector<std::pair<size_t, size_t>>& illegalStartAndSize) const
        {
            illegalStartAndSize.clear();

            for(const auto& illegalSequence : mIllegalSequences)
            {
                auto positionIter = std::search(gene.begin() + startSearch, gene.end(), illegalSequence.begin(), illegalSequence.end());
                        static int kalle = 0;
                        ++kalle;
                std::cout << "searched string: " << kalle << "\n";
                if(positionIter != gene.end())
                    illegalStartAndSize.push_back({positionIter - gene.begin(), illegalSequence.size()});
            }

            return illegalStartAndSize.size() != 0;
        }
}

