#include <dna/breeder.hpp>
#include <random>

namespace dna
{
    std::pair<Chromosome, Breeder::BreedStats> Breeder::breed(const Chromosome& mum, const Chromosome& dad, uint32_t crossOverPerMillion)
    {
        BreedStats stats;
        stats.crossOvers = 0;

        std::random_device rd;
        std::uniform_int_distribution<> randomMillion(0, 999999);
        std::uniform_int_distribution<> mumOrDad(0, 1);
        bool startWithMum = mumOrDad(rd) == 0;

        const Gene* currentGene = startWithMum ? &mum.gene() : &dad.gene();
        const Gene* otherGene = startWithMum ? &dad.gene() : &mum.gene();

        Gene resultingGene;
        resultingGene.resize(currentGene->size());

        for(uint32_t i = 0; i < resultingGene.size(); i++)
        {
            resultingGene[i] = (*currentGene)[i];

            if(crossOverPerMillion > randomMillion(rd))
            {
                std::swap(currentGene, otherGene);
                ++stats.crossOvers;
            }
        }

        return {Chromosome(mum.maxCistronIds(), mum.informationDensity(), mum.startSequence(), mum.stopSequence(), resultingGene), stats};
    }
}
