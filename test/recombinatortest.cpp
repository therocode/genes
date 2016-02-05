#include "catch.hpp"
#include <dna/chromosome.hpp>
#include <dna/allele.hpp>
#include <dna/recombinator.hpp>
#include <dna/cerealbackend.hpp>
#include <cereal/types/string.hpp>
#include <dna/chromosomegenerator.hpp>
#include <dna/cerealalleleserializer.hpp>

const dna::GeneConfiguration config{{'A', 'T', 'C', 'G'}, "GATTA", "CATTA", 50, 200};

SCENARIO("Recombinator can be used to recombine genes of a chromosome into new genetic material", "[dna]")
{
    GIVEN("A chromosome with some alleles in it")
    {
        dna::ChromosomeGenerator<2, dna::CerealBackend> generator(config);

        auto chromosome = generator.generate(
            std::make_pair(0, dna::Allele<std::string, dna::CompeteType::Exclusive>("Kalle", 100)),
            std::make_pair(1, dna::Allele<int16_t, dna::CompeteType::Mix>(17, 70)),
            std::make_pair(2, dna::Allele<std::string, dna::CompeteType::Exclusive>("Blopp", 150))
        );

        WHEN("A recombinator is used to recombine the chromosome through crossover")
        {
            dna::Recombinator<> recombinator;

            dna::Gene recombined = recombinator.crossover(chromosome, 25);

            THEN("the new string is of the right size")
            {
                CHECK(recombined.size() == chromosome[0].size());
            }
        }
    }
}
