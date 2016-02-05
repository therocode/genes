#pragma once
#include <dna/assert.hpp>
#include <random>

namespace dna
{
    template <typename RandomNumberEngine = std::mt19937>
    class Recombinator
    {
        public:
            template <size_t tChromosomeAmount>
            Gene crossover(const Chromosome<tChromosomeAmount>& sourceChromosome, int32_t rate, int32_t seed = std::random_device()()) const;
        private:
            mutable RandomNumberEngine mRandomNumberEngine;
    };

    template <typename RandomNumberEngine>
    template <size_t tChromosomeAmount>
    Gene Recombinator<RandomNumberEngine>::crossover(const Chromosome<tChromosomeAmount>& sourceChromosome, int32_t rate, int32_t seed) const
    {
        DNA_ASSERT(rate > 0, "Rate cannot be zero or less");
        DNA_ASSERT(sourceChromosome.size() > 0, "Chromosome cannot be empty");

        std::uniform_int_distribution<int32_t> crossoverChance(0, rate - 1);
        std::uniform_int_distribution<int32_t> geneSelector(0, sourceChromosome.size() - 1);

        size_t currentChromosomeGene = geneSelector(mRandomNumberEngine);

        Gene result(sourceChromosome[0].size(), ' ');

        for(size_t i = 0; i < sourceChromosome[0].size(); ++i)
        {
            result[i] = sourceChromosome[currentChromosomeGene][i];

            if(crossoverChance(mRandomNumberEngine) == 0)
                currentChromosomeGene = geneSelector(mRandomNumberEngine);
        }

        return result;
    }
}
