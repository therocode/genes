#include "catch.hpp"
#include <unordered_set>
#include <dna/genecodec.hpp>

SCENARIO("A GeneCodec can be used to encode/decode a list of ID's and binary data into a gene string based on specific settings", "[dna]")
{
    GIVEN("A GeneCodec with a specific spread, available symbols, and a given start/end sequence, and some arbitrary data")
    {
        std::vector<dna::Nucleotide> availableNucleotides{'A', 'C', 'G', 'T',};
        std::unordered_set<dna::Nucleotide> availableNucleotidesSet(availableNucleotides.begin(), availableNucleotides.end());
        dna::Gene start = "GATTA";
        dna::Gene end = "CATTA";
        dna::GeneCodec<> geneCodec({availableNucleotides, start, end, 50, 100});

        std::vector<dna::Information> data
        {
            {
                666,
                {
                    23, 43, 1, 43, 0, 5, 45, 2,
                },
            },
            {
                10,
                {
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 1,
                    23, 1, 2, 0, 1, 21, 101, 120, 0,
                },
            },
        };


        WHEN("some data is encoded")
        {
            dna::Gene encoded = geneCodec.encode(data);

            THEN("A gene string with the right symbols is generated")
            {
                INFO(encoded);
                for(auto nucleotide : encoded)
                    CHECK(availableNucleotidesSet.count(nucleotide) != 0);
            }
        }

        WHEN("some data is encoded and decoded")
        {
            auto decoded = geneCodec.decode(geneCodec.encode(data));

            THEN("The decoded data matches the original data")
            {
                CHECK(decoded == data);
            }
        }
    }
}

SCENARIO("A GeneCodec can be used to encode/decode single values into a gene string", "[dna]")
{
    GIVEN("A GeneCodec with a specific spread, available symbols, and a given start/end sequence")
    {
        std::vector<dna::Nucleotide> availableNucleotides{'A', 'C', 'G', 'T',};
        std::unordered_set<dna::Nucleotide> availableNucleotidesSet(availableNucleotides.begin(), availableNucleotides.end());
        dna::Gene start = "GATTA";
        dna::Gene end = "CATTA";
        dna::GeneCodec<> geneCodec({availableNucleotides, start, end, 50, 100});

        WHEN("some data is encoded")
        {
            dna::Gene encoded = geneCodec.toGene(54, 150);

            THEN("A gene string with the right symbols is generated")
            {
                INFO(encoded);
                for(auto nucleotide : encoded)
                    CHECK(availableNucleotidesSet.count(nucleotide) != 0);
            }
        }

        WHEN("some data is encoded and decoded")
        {
            dna::Gene encoded = geneCodec.toGene(54, 150);

            uint32_t decoded = geneCodec.fromGene(encoded, 150);

            THEN("The decoded data matches the original data")
            {
                CHECK(decoded == 54);
            }
        }
    }
}
