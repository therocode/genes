#include "catch.hpp"
#include <dna/allele.hpp>
#include <dna/chromosomegenerator.hpp>
#include <dna/binarycodec.hpp>
#include <cereal/types/string.hpp>
#include <dna/cerealbackend.hpp>
#include <dna/cerealalleleserializer.hpp>
#include <dna/genecodec.hpp>

const dna::GeneConfiguration config{{'A', 'T', 'C', 'G'}, "GATTA", "CATTA", 50, 200};

SCENARIO("A ChromosomeGenerator can generate a chromosome with alleles placed in it", "[dna]")
{
    GIVEN("A chromosome generator")
    {
        dna::ChromosomeGenerator<2, dna::CerealBackend> generator(config);

        WHEN("A chromosome is generated from some alleles")
        {
            auto chromosome = generator.generate(
                std::make_pair(0, dna::Allele<std::string, dna::CompeteType::Exclusive>("Kalle", 100)),
                std::make_pair(1, dna::Allele<int16_t, dna::CompeteType::Mix>(17, 150))
            );

            THEN("the chromosome contains the right amount of dna strings with the right data")
            {
                REQUIRE(chromosome.size() == 2);

                dna::GeneCodec<> codec(config);

                auto decodedChromosomeList = {codec.decode(chromosome[0]), codec.decode(chromosome[1])};
                for(const auto& dataList : decodedChromosomeList)
                {
                    REQUIRE(dataList.size() == 2);
                    CHECK(dataList[0].identifier == 0);
                    CHECK(dataList[1].identifier == 1);

                    dna::BinaryCodec<dna::CerealBackend> binaryCodec;

                    auto decodedString = binaryCodec.decode<dna::Allele<std::string, dna::CompeteType::Exclusive>>(dataList[0].data);
                    auto decodedInt = binaryCodec.decode<dna::Allele<int16_t, dna::CompeteType::Mix>>(dataList[1].data);

                    CHECK(decodedString.value() == "Kalle");
                    CHECK(decodedString.strength() == 100);
                    CHECK(decodedInt.value() == 17);
                    CHECK(decodedInt.strength() == 150);
                }
            }
        }
    }
}
