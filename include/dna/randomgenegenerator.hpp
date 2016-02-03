#pragma once
#include <dna/gene.hpp>
#include <dna/geneconfiguration.hpp>
#include <unordered_set>
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>

namespace dna
{
    template <typename RandomNumberEngine = std::mt19937>
        class RandomGeneGenerator
        {
            public:
                RandomGeneGenerator(GeneConfiguration configuration, std::unordered_set<Gene> illegalSequences = {}, int32_t seed = std::random_device()());
                Gene generate(int32_t length) const;
            private:
                size_t findFirstIllegal(const Gene& gene, size_t startSearch) const;
                bool containsIllegal(const Gene& gene, size_t startSearch, std::pair<size_t, size_t>& illegalStartAndSize) const;
                mutable RandomNumberEngine mRandomNumberEngine;
                GeneConfiguration mConfiguration;
                std::vector<Gene> mIllegalSequences;
                int64_t mBiggestIllegalSize;
        };

    template <typename RandomNumberEngine>
        RandomGeneGenerator<RandomNumberEngine>::RandomGeneGenerator(GeneConfiguration configuration, std::unordered_set<Gene> illegalSequences, int32_t seed):
            mConfiguration(std::move(configuration)),
            mIllegalSequences(illegalSequences.begin(), illegalSequences.end()),
            mRandomNumberEngine(seed),
            mBiggestIllegalSize(0)
    {
        for(const auto& illegal : mIllegalSequences)
            mBiggestIllegalSize = std::max(static_cast<int64_t>(illegal.size()), mBiggestIllegalSize);
    }

    template <typename RandomNumberEngine>
    Gene RandomGeneGenerator<RandomNumberEngine>::generate(int32_t length) const
    {
        std::uniform_int_distribution<size_t> range(0, mConfiguration.nucleotides.size() - 1);

        Gene result(length, ' ');

        for(int32_t i = 0; i < length; ++i)
            result[i] = mConfiguration.nucleotides[range(mRandomNumberEngine)];

        std::pair<size_t, size_t> illegalStartAndSize;
        int64_t safePos = 0;

        while(containsIllegal(result, safePos, illegalStartAndSize))
        {
            int32_t rewriteStart = illegalStartAndSize.first;
            for(int32_t i = rewriteStart; i < rewriteStart + illegalStartAndSize.second; ++i)
            {
                result[i] = mConfiguration.nucleotides[range(mRandomNumberEngine)];
            }

            safePos = static_cast<int64_t>(rewriteStart) - mBiggestIllegalSize;
            safePos = std::max(int64_t(0), safePos);
        }

        return result;
    }

    template <typename RandomNumberEngine>
    size_t RandomGeneGenerator<RandomNumberEngine>::findFirstIllegal(const Gene& gene, size_t startSearch) const
    {
        auto currentGeneIter = gene.begin() + startSearch;

        for(;currentGeneIter != gene.end(); ++currentGeneIter)
        {
            char currentChar = *currentGeneIter;

            for(auto& illegal : mIllegalSequences)
            {
                auto illegalIter = illegal.begin();
                auto geneIter = currentGeneIter;

                while(geneIter != gene.end() && *illegalIter == *geneIter)
                {
                    ++geneIter;
                    ++illegalIter;

                    if(illegalIter == illegal.end())
                        return currentGeneIter - gene.begin();
                }
            }
        }

        return Gene::npos;
    }

    template <typename RandomNumberEngine>
        bool RandomGeneGenerator<RandomNumberEngine>::containsIllegal(const Gene& gene, size_t startSearch, std::pair<size_t, size_t>& illegalStartAndSize) const
        {
            auto positionIter = findFirstIllegal(gene, startSearch);

            if(positionIter != Gene::npos)
            {
                illegalStartAndSize = {positionIter, mBiggestIllegalSize};
                return true;
            }

            return false;
        }
}

