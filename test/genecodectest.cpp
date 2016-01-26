#include "catch.hpp"
#include <unordered_set>
#include <dna/genecodec.hpp>

SCENARIO("A GeneCodec can be used to encode/decode a list of ID's and binary data into a gene string based on specific settings", "[dna]")
{
    GIVEN("A GeneCodec with a specific spread, available symbols, and a given start/end sequence, and some arbitrary data")
    {
        std::vector<dna::Nucleotide> availableNucleotides{'K', 'A', 'L', 'E',};
        //std::vector<dna::Nucleotide> availableNucleotides{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
        std::unordered_set<dna::Nucleotide> availableNucleotidesSet(availableNucleotides.begin(), availableNucleotides.end());
        dna::Gene start = "KKAALLEE";
        dna::Gene end = "EELLAAKK";
        dna::GeneCodec<> geneCodec(availableNucleotides, start, end, 50, 100);

        std::vector<dna::Information> data
        {
            {
                666,
                {
                    true, false, true, false, false, true, true,
                },
            },
            {
                10,
                {
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, false, true,
                    true, true, false, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, false, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, false, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, false, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                    true, true, true, true, true, true, true,
                     false, true, false, true, false, false,
                },
            },
        };


        WHEN("some data is encoded")
        {
            dna::Gene encoded = geneCodec.encode(data);

            THEN("A gene string with the right symbols")
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
