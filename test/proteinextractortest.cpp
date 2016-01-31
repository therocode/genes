#include "catch.hpp"
#include <dna/allele.hpp>
#include <dna/allelechromosomegenerator.hpp>
#include <dna/genemanipulator.hpp>

SCENARIO("Given a chromosome with data, a protein extractor can extract proteins of the data, merging alleles as needed", "[dna]")
{
    GIVEN("A chromosome with some alleles in it")
    {
        dna::AlleleChromosomeGenerator<2> generator; //

        auto chromosome = generator.generate(
            std::make_pair(0, dna::Allele<std::string, dna::CompeteType::Exclusive>("Kalle", 100)),
            std::make_pair(1, dna::Allele<int16_t, dna::CompeteType::Mix>(17, 100))
        );

        dna::GeneManipulator manipulator(chromosome[0]); //

        manipulator.replace(0, 0, dna::Allele<std::string, dna::CompeteType::Exclusive>("Nisse", 80)); //
        manipulator.replace(1, 0, dna::Allele<int16_t, dna::CompeteType::Mix>(42, 130)); //

        WHEN("")
        {
            THEN("the alleles' data can be accessed")
            {
            }
        }
    }
}
