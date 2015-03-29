#include <dna/cistron.hpp>
#include <dna/genecodec.hpp>

namespace dna
{
    Cistron::Cistron(uint32_t id, uint32_t maxCistronIds, uint32_t informationDensity, const std::vector<CistronValue>& values):
        mId(id),
        mInformationDensity(informationDensity)
    {
        GeneCodec geneCodec(informationDensity);

        geneCodec.encode(id, maxCistronIds, mGene);

        for(auto value : values)
        {
            geneCodec.encode(value.value, value.max, mGene);
        }

        geneCodec.decode(mGene, maxCistronIds, 0, mDataOffset);
    }

    Cistron::Cistron(const Gene& gene, uint32_t maxCistronIds, uint32_t informationDensity):
        mGene(gene),
        mInformationDensity(informationDensity)
    {
        GeneCodec geneCodec(informationDensity);
        mId = geneCodec.decode(mGene, maxCistronIds, 0, mDataOffset);
    }

    const Gene& Cistron::gene() const
    {
        return mGene;
    }

    void Cistron::decode(const std::vector<CistronOutValue>& outValues) const
    {
        GeneCodec geneCodec(mInformationDensity);
        uint32_t offset = mDataOffset;

        for(auto& outValue : outValues)
        {
            outValue.outValue = geneCodec.decode(mGene, outValue.max, offset, offset);
        }
    }

    uint32_t Cistron::id() const
    {
        return mId;
    }
}
