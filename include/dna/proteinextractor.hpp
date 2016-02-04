#pragma once
#include <dna/protein.hpp>
#include <dna/chromosome.hpp>
#include <dna/makeproteins.hpp>

namespace dna
{
    template <typename CodecBackend>
    class ProteinExtractor
    {
        public:
            ProteinExtractor(GeneConfiguration configuration);
            template <typename AlleleType, size_t tChromosomeAmount>
            ProteinList<typename AlleleType::Type> extract(int32_t identifier, const Chromosome<tChromosomeAmount>& chromosome);
        private:
            GeneCodec<> mGeneCodec;
            BinaryCodec<CodecBackend> mBinaryCodec;

    };

    template <typename CodecBackend>
    ProteinExtractor<CodecBackend>::ProteinExtractor(GeneConfiguration configuration):
        mGeneCodec(configuration)
    {

    }

    template <typename CodecBackend>
    template <typename AlleleType, size_t tChromosomeAmount>
     ProteinList<typename AlleleType::Type> ProteinExtractor<CodecBackend>::extract(int32_t identifier, const Chromosome<tChromosomeAmount>& chromosome)
     {
         std::vector<AlleleType> alleles;

         for(const Gene& gene : chromosome)
         {
             auto informationList = mGeneCodec.decode(gene);

             for(const Information& information : informationList)
             {
                if(information.identifier == identifier)
                {
                    alleles.emplace_back(mBinaryCodec.template decode<AlleleType>(information.data));
                }
             }
         }

         return makeProteins(alleles);
     }
};
