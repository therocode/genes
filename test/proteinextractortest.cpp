#include "catch.hpp"
#include <dna/allele.hpp>
#include <dna/chromosomegenerator.hpp>
#include <dna/cerealbackend.hpp>
#include <dna/genemanipulator.hpp>
#include <dna/proteinextractor.hpp>
#include <dna/cerealalleleserializer.hpp>
#include <cereal/types/string.hpp>

const dna::GeneConfiguration config{{'A', 'T', 'C', 'G'}, "GATTA", "CATTA", 50, 200};

SCENARIO("Given a chromosome with data, a protein extractor can extract proteins of the data, merging alleles as needed", "[dna]")
{
    GIVEN("A chromosome with some alleles in it")
    {
        dna::ChromosomeGenerator<2, dna::CerealBackend> generator(config);

        auto chromosome = generator.generate(
            std::make_pair(0, dna::Allele<std::string, dna::CompeteType::Exclusive>("Kalle", 100)),
            std::make_pair(1, dna::Allele<int16_t, dna::CompeteType::Mix>(17, 70)),
            std::make_pair(2, dna::Allele<std::string, dna::CompeteType::Exclusive>("Blopp", 150))
        );

        dna::GeneManipulator<dna::CerealBackend> manipulator(config, chromosome[0]);

        manipulator.replace(0, 0, dna::Allele<std::string, dna::CompeteType::Exclusive>("Nisse", 80));
        manipulator.replace(1, 0, dna::Allele<int16_t, dna::CompeteType::Mix>(42, 30));
        manipulator.replace(2, 0, dna::Allele<std::string, dna::CompeteType::Exclusive>("Blapp", 150));

        WHEN("A protein extractor is used to extract proteins")
        {
            dna::ProteinExtractor<dna::CerealBackend> extractor(config);

            dna::ProteinList<std::string> nameProteins = extractor.extract<dna::Allele<std::string, dna::CompeteType::Exclusive>>(0, chromosome);
            dna::ProteinList<int16_t> numberProteins = extractor.extract<dna::Allele<int16_t, dna::CompeteType::Mix>>(1, chromosome);
            dna::ProteinList<std::string> textProteins = extractor.extract<dna::Allele<std::string, dna::CompeteType::Exclusive>>(2, chromosome);

            THEN("the data is properly extracted")
            {
                REQUIRE(nameProteins.size() == 1);
                REQUIRE(numberProteins.size() == 1);
                REQUIRE(textProteins.size() == 2);

                CHECK(nameProteins[0].value() == "Kalle");
                CHECK(numberProteins[0].value() == 23);

                bool value1Good = textProteins[0].value() == "Blopp" || textProteins[0].value() == "Blapp";
                bool value2Good = textProteins[1].value() == "Blapp" || textProteins[1].value() == "Blopp";
                CHECK(value1Good);
                CHECK(value2Good);
                CHECK(textProteins[0].value() != textProteins[1].value());
            }
        }
    }
}
