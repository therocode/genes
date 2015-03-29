#include <dna/chromosome.hpp>
#include <dna/randomgene.hpp>
#include <iostream>

namespace dna
{
    Chromosome::Chromosome(uint32_t maxCistronIds, uint32_t informationDensity, const Gene& startSequence, const Gene& stopSequence, const std::vector<Cistron>& cistrons, uint32_t minJunkGenes, uint32_t maxJunkGenes):
        mInformationDensity(informationDensity),
        mMaxCistronIds(maxCistronIds),
        mStartSequence(startSequence),
        mStopSequence(stopSequence)
    {
           mGene.append(RandomGene::ofRange(informationDensity, minJunkGenes, maxJunkGenes));
           for(const auto& cistron : cistrons)
           {
                mGene.append(startSequence);
                mGene.append(cistron.gene());
                mGene.append(stopSequence);
                mGene.append(RandomGene::ofRange(informationDensity, minJunkGenes, maxJunkGenes));
           }
    }

    uint32_t Chromosome::cistronAmount() const
    {
        size_t start = 0;
        uint32_t chromosomeAmount = 0;

        while((start = mGene.find(mStartSequence, start)) != std::string::npos)
        {
            size_t stop = mGene.find(mStopSequence, start);

            if(stop != std::string::npos)
            {
                chromosomeAmount++;
            }

            start = stop;
        }

        return chromosomeAmount;
    }

    CistronMap Chromosome::cistrons() const
    {
        CistronMap result;

        size_t start = 0;

        while((start = mGene.find(mStartSequence, start)) != std::string::npos)
        {
            size_t stop = mGene.find(mStopSequence, start);

            if(stop != std::string::npos)
            {
                size_t cistronStart = start + mStartSequence.size();
                Cistron cistron(mGene.substr(cistronStart, stop - cistronStart), mMaxCistronIds, mInformationDensity);

                result.emplace(cistron.id(), std::move(cistron));
            }

            start = stop;
        }

        return result;
    }
    
    const Gene& Chromosome::gene() const
    {
        return mGene;
    }
}
