#include <dna/genecodec.hpp>

namespace dna
{
    GeneCodec::GeneCodec(uint32_t informationDensity)
    {
    }

    Gene GeneCodec::encode(uint32_t value, uint32_t maxValue)
    {
        return "";
    }

    void GeneCodec::encode(uint32_t value, uint32_t maxValue, Gene& targetGene)
    {
    }

    uint32_t GeneCodec::decode(const Gene& gene, uint32_t maxValue)
    {
        return 0;
    }

    uint32_t GeneCodec::decode(const Gene& gene, uint32_t maxValue, uint32_t offset, uint32_t& decodeEnd)
    {
        return 0;
    }
}
