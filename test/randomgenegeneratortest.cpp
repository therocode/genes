#include "catch.hpp"
#include <dna/randomgenegenerator.hpp>

SCENARIO("RandomGeneGenerator can produce genes with randomized content", "[dna]")
{
    GIVEN("A RandomGeneGenerator with a certain sets of chars and with no illegal sequences given")
    {
        std::vector<dna::Nucleotide> availableNucleotides{'A', 'C', 'G', 'T'};
        std::unordered_set<dna::Nucleotide> availableNucleotidesSet(availableNucleotides.begin(), availableNucleotides.end());

        dna::RandomGeneGenerator<> generator({availableNucleotides}, {}, 0);

        WHEN("A random gene of a certain length is requested")
        {
            size_t geneLength = 1000;

            dna::Gene randomGene = generator.generate(geneLength);

            THEN("The gene is of the right length and does not contain any other char than the ones provided")
            {
                CHECK(randomGene.size() == geneLength);
                for(dna::Nucleotide nucleotide : randomGene)
                    CHECK(availableNucleotidesSet.find(nucleotide) != availableNucleotidesSet.end());
            }
        }
    }

    GIVEN("A RandomGeneGenerator with a certain sets of chars and with some illegal sequences given")
    {
        std::vector<dna::Nucleotide> availableNucleotides{'A', 'C', 'G', 'T'};
        std::unordered_set<dna::Nucleotide> availableNucleotidesSet(availableNucleotides.begin(), availableNucleotides.end());
        std::unordered_set<dna::Gene> illegalSequences{"GATCA", "CATA", "TATA", "GATTACA"};

        dna::RandomGeneGenerator<> generator({availableNucleotides}, illegalSequences, 0);

        WHEN("A random gene of a certain length is requested")
        {
            size_t geneLength = 100000;

            dna::Gene randomGene = generator.generate(geneLength);

            THEN("The gene is of the right length and does not contain any other char than the ones provided, and does not contain any of the illegal sequences")
            {
                CHECK(randomGene.size() == geneLength);

                for(dna::Nucleotide nucleotide : randomGene)
                    CHECK(availableNucleotidesSet.count(nucleotide) != 0);

                for(dna::Gene illegalSequence : illegalSequences)
                {
                    INFO("Illegal sequence: " << illegalSequence);
                    CHECK(randomGene.find(illegalSequence) == dna::Gene::npos);
                }
            }
        }
    }
}
