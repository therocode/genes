#include "catch.hpp"
#include <dna/allele.hpp>
#include <dna/chromosomegenerator.hpp>
#include <dna/genecodec.hpp>
#include <cereal/types/string.hpp>
#include <dna/binarycodec.hpp>
#include <dna/cerealbackend.hpp>
#include <dna/genemanipulator.hpp>

SCENARIO("Given a gene string with data, the gene manipulator class is capable of replacing data of specific sections", "[dna]")
{
    GIVEN("A gene string with data and a gene manipulator")
    {
        dna::ChromosomeGenerator<2> generator;

        auto chromosome = generator.generate(
            std::make_pair(0, std::string("Kalle")),
            std::make_pair(0, std::string("Roger")),
            std::make_pair(1, int16_t(17))
        );

        dna::Gene gene = chromosome[0];

        dna::GeneManipulator manipulator(chromosome[0]);

        WHEN("The manipulator is used to manipulate data in the string")
        {
            manipulator.replace(0, 0, std::string("Nisse"));
            manipulator.replace(0, 1, std::string("Nasse"));
            manipulator.replace(1, 0, int16_t(42));

            THEN("the string contains the updated data")
            {
                dna::GeneCodec<> geneCodec({'A', 'T', 'C', 'G'}, "GATTA", "CATTA", 50, 200);
                auto data = geneCodec.decode(gene);

                REQUIRE(data.size() == 3);

                CHECK(data[0].identifier == 0);
                CHECK(data[1].identifier == 0);
                CHECK(data[2].identifier == 1);

                dna::BinaryCodec<dna::CerealBackend> binaryCodec;

                std::string decodedString1 = binaryCodec.decode<std::string>(data[0].data);
                std::string decodedString2 = binaryCodec.decode<std::string>(data[1].data);
                int16_t decodedInt = binaryCodec.decode<int16_t>(data[2].data);

                CHECK(decodedString1 == "Nisse");
                CHECK(decodedString2 == "Nasse");
                CHECK(decodedInt == 42);
            }
        }
    }
}
