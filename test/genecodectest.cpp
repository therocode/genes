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

            THEN("A gene string with the right symbols and 2 pairs of starts/ends, sufficiently spread out")
            {
                for(auto nucleotide : encoded)
                    CHECK(availableNucleotidesSet.count(nucleotide) != 0);

                size_t firstStartPos = encoded.find(start);
                size_t secondStartPos = encoded.find(start, firstStartPos + 1);
                size_t firstEndPos = encoded.find(end);
                size_t secondEndPos = encoded.find(end, firstEndPos + 1);

                CHECK(firstStartPos != dna::Gene::npos);
                CHECK(secondStartPos != dna::Gene::npos);
                CHECK(firstEndPos != dna::Gene::npos);
                CHECK(secondEndPos != dna::Gene::npos);

                size_t distanceBetweenInformation = secondStartPos - firstEndPos;

                CHECK(distanceBetweenInformation < 100);
                CHECK(distanceBetweenInformation >= 50);
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
