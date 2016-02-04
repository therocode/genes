#pragma once
#include <dna/chromosome.hpp>
#include <dna/information.hpp>
#include <dna/genecodec.hpp>
#include <dna/binarycodec.hpp>

namespace dna
{
    template <size_t tChromosomeAmount, typename CodecBackend>
    class ChromosomeGenerator
    {
        public:
            ChromosomeGenerator(GeneConfiguration configuration);
            template <typename... Alleles>
            Chromosome<tChromosomeAmount> generate(const std::pair<int32_t, Alleles>&... alleles);
        private:
            template <typename Allele, typename... Alleles>
            void generateInformation(std::vector<Information>& out, const std::pair<int32_t, Allele>& first, const std::pair<int32_t, Alleles>&... tail);
            void generateInformation(std::vector<Information>& out);
            GeneCodec<> mGeneCodec;
            BinaryCodec<CodecBackend> mBinaryCodec;
    };

    template <size_t tChromosomeAmount, typename CodecBackend>
    ChromosomeGenerator<tChromosomeAmount, CodecBackend>::ChromosomeGenerator(GeneConfiguration configuration):
        mGeneCodec(configuration)
    {
    }

    template <size_t tChromosomeAmount, typename CodecBackend>
    template <typename... Alleles>
    Chromosome<tChromosomeAmount> ChromosomeGenerator<tChromosomeAmount, CodecBackend>::generate(const std::pair<int32_t, Alleles>&... alleles)
    {
        std::vector<Information> information;

        generateInformation(information, alleles...);

        Gene singleGene = mGeneCodec.encode(information);
        Chromosome<tChromosomeAmount> result;
        result.fill(singleGene);

        return result;
    }

    template <size_t tChromosomeAmount, typename CodecBackend>
    template <typename Allele, typename... Alleles>
    void ChromosomeGenerator<tChromosomeAmount, CodecBackend>::generateInformation(std::vector<Information>& out, const std::pair<int32_t, Allele>& first, const std::pair<int32_t, Alleles>&... tail)
    {
        out.emplace_back(Information{static_cast<int16_t>(first.first), mBinaryCodec.encode(first.second)});
        generateInformation(out, tail...);
    }

    template <size_t tChromosomeAmount, typename CodecBackend>
    void ChromosomeGenerator<tChromosomeAmount, CodecBackend>::generateInformation(std::vector<Information>& out)
    {
    }
}
