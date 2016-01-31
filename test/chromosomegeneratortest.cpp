#include "catch.hpp"
#include <dna/allele.hpp>
#include <dna/chromosomegenerator.hpp>
#include <dna/binarycodec.hpp>
#include <dna/cerealbackend.hpp>
#include <dna/genecodec.hpp>

SCENARIO("A ChromosomeGenerator can generate a chromosome with alleles placed in it", "[dna]")
{
    GIVEN("A chromosome generator")
    {
        dna::ChromosomeGenerator<2> generator;

        WHEN("A chromosome is generated from some alleles")
        {
            auto chromosome = generator.generate(
                std::make_pair(0, dna::Allele<std::string, dna::CompeteType::Exclusive>("Kalle", 100)),
                std::make_pair(1, dna::Allele<int16_t, dna::CompeteType::Mix>(17, 100))
            );
            THEN("the chromosome contains the right amount of dna strings with the right data")
            {
                REQUIRE(chromosome.size() == 2);

                dna::GeneCodec<> codec({'A', 'T', 'C', 'G'}, "GATTA", "CATTA", 50, 200);

                for(const auto& dataList : {codec.decode(chromosome[0]), codec.decode(chromosome[1])})
                {
                    REQUIRE(dataList.size() == 2);
                    CHECK(dataList[0].identifier == 0);
                    CHECK(dataList[1].identifier == 1);

                    dna::BinaryCodec<dna::CerealBackend> binaryCodec;

                    std::string decodedString = binaryCodec.decode<std::string>(dataList[0].data);
                    int16_t decodedInt = binaryCodec.decode<int16_t>(dataList[1].data);

                    CHECK(decodedString == "Kalle");
                    CHECK(decodedInt == 17);
                }
            }
        }
    }
}
