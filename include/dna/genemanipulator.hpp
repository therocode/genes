#pragma once
#include <dna/gene.hpp>
#include <dna/genecodec.hpp>
#include <dna/binarycodec.hpp>
#include <dna/geneconfiguration.hpp>

namespace dna
{
    template <typename CodecBackend>
    class GeneManipulator
    {
        public:
            GeneManipulator(GeneConfiguration configuration, Gene& gene);
            template <typename Data>
            void replace(int16_t dataId, int32_t dataIndex, Data data);
        private:
            GeneConfiguration mConfiguration;
            Gene& mGene;
            GeneCodec<> mGeneCodec;
            BinaryCodec<CodecBackend> mBinaryCodec;
    };

    template <typename CodecBackend>
    GeneManipulator<CodecBackend>::GeneManipulator(GeneConfiguration configuration, Gene& gene):
        mConfiguration(configuration),
        mGene(gene),
        mGeneCodec(std::move(configuration))
    {
        mConfiguration.minSpread = 0;
        mConfiguration.maxSpread = 0;
    }

    template <typename CodecBackend>
    template <typename Data>
    void GeneManipulator<CodecBackend>::replace(int16_t dataId, int32_t dataIndex, Data data)
    {
        Gene identifierPart = mGeneCodec.toGene(dataId, std::numeric_limits<decltype(dataId)>().max());

        Gene toSearchFor = mConfiguration.segmentStartCode + identifierPart;

        size_t startPosition = mGene.find(toSearchFor, startPosition + 1);
        for(int32_t i = 0; i < dataIndex && startPosition != Gene::npos; ++i)
        {
            startPosition = mGene.find(toSearchFor, startPosition + 1);
        }

        size_t endPosition = mGene.find(mConfiguration.segmentEndCode, startPosition);

        if(startPosition != Gene::npos && endPosition != Gene::npos)
        {
            Information information;
            information.identifier = dataId;
            information.data = mBinaryCodec.encode(data);

            Gene newGeneSegment = mGeneCodec.encode({std::move(information)});

            mGene.replace(startPosition, endPosition + mConfiguration.segmentEndCode.size()- startPosition, newGeneSegment);
        }
    }
}
