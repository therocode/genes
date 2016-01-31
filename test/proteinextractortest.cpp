#include "catch.hpp"
#include <dna/allele.hpp>
#include <dna/chromosomegenerator.hpp>
#include <dna/genemanipulator.hpp>
#include <dna/proteinextractor.hpp>

SCENARIO("Given a chromosome with data, a protein extractor can extract proteins of the data, merging alleles as needed", "[dna]")
{
    GIVEN("A chromosome with some alleles in it")
    {
        dna::ChromosomeGenerator<2> generator;

        auto chromosome = generator.generate(
            std::make_pair(0, dna::Allele<std::string, dna::CompeteType::Exclusive>("Kalle", 100)),
            std::make_pair(1, dna::Allele<int16_t, dna::CompeteType::Mix>(17, 70))
        );

        dna::GeneManipulator manipulator(chromosome[0]);

        manipulator.replace(0, 0, dna::Allele<std::string, dna::CompeteType::Exclusive>("Nisse", 80));
        manipulator.replace(1, 0, dna::Allele<int16_t, dna::CompeteType::Mix>(42, 30));

        WHEN("A protein extractor is used to extract proteins")
        {
            dna::ProteinExtractor extractor;

            dna::ProteinList<std::string> nameProteins = extractor.extract<std::string>(0, chromosome);
            dna::ProteinList<int16_t> numberProteins = extractor.extract<int16_t>(1, chromosome);

            THEN("the data is properly extracted")
            {
                REQUIRE(nameProteins.size() == 1);
                REQUIRE(numberProteins.size() == 1);

                CHECK(nameProteins[0].value() == "Kalle");
                CHECK(numberProteins[1].value() == 23);
            }
        }
    }
}
