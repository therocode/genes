#include "catch.hpp"
#include <dna/genecodec.hpp>

SCENARIO("A GeneCodec can be used to encode and decode integers <-> genes of different information densities", "[genes]")
{
    GIVEN("A GeneCodec with an information density of 2")
    {
        dna::GeneCodec geneCodec(2);

        WHEN("integers are encoded")
        {
            dna::Gene encoded3 = geneCodec.encode(3, 32);
            dna::Gene encoded8 = geneCodec.encode(8, 32);
            dna::Gene encoded21 = geneCodec.encode(21, 32);

            THEN("the right gene string is produced")
            {
                CHECK(encoded3  == "BBAAAA");
                CHECK(encoded8  == "AAABAA");
                CHECK(encoded21 == "BABABA");
            }
        }

        WHEN("genes are decoded")
        {
            uint32_t decoded3 = geneCodec.decode("BB", 32);
            uint32_t decoded8 = geneCodec.decode("AAAB", 32);
            uint32_t decoded21 = geneCodec.decode("BABAB", 32);

            THEN("the right numbers are extracted")
            {
                CHECK(decoded3 == 3);
                CHECK(decoded8 == 8);
                CHECK(decoded21 == 21);
            }
        }
    }

    GIVEN("A GeneCodec with an information density of 3")
    {
        dna::GeneCodec geneCodec(3);

        WHEN("integers are encoded")
        {
            dna::Gene encoded3 = geneCodec.encode(3, 32);
            dna::Gene encoded8 = geneCodec.encode(8, 32);
            dna::Gene encoded16 = geneCodec.encode(16, 32);

            THEN("the right gene string is produced")
            {
                CHECK(encoded3  == "ABAA");
                CHECK(encoded8  == "CCAA");
                CHECK(encoded16 == "BCBA");
            }
        }

        WHEN("genes are decoded")
        {
            uint32_t decoded3 = geneCodec.decode("AB", 32);
            uint32_t decoded8 = geneCodec.decode("CC", 32);
            uint32_t decoded16 = geneCodec.decode("BCB", 32);

            THEN("the right numbers are extracted")
            {
                CHECK(decoded3 == 3);
                CHECK(decoded8 == 8);
                CHECK(decoded16 == 16);
            }
        }
    }
}

SCENARIO("The length of the resulting gene strings, and the amount of genes parsed in a string depends on the maximum value of the values given", "[genes]")
{
    GIVEN("A GeneCodec with an information density of 3")
    {
        dna::GeneCodec geneCodec(3);

        WHEN("values with different max values are encoded")
        {
            dna::Gene encodedMax5 = geneCodec.encode(2, 5);
            dna::Gene encodedMax8 = geneCodec.encode(2, 8);
            dna::Gene encodedMax9 = geneCodec.encode(2, 9);

            THEN("the resulting gene strings are padded with 'zeroes' to fit the entire range")
            {
                CHECK(encodedMax5 == "CA");
                CHECK(encodedMax8 == "CA");
                CHECK(encodedMax9 == "CAA");
            }
        }

        WHEN("values with different max values are decoded, with different amounts of extra bits")
        {
            uint32_t decodedMax5 = geneCodec.decode("CAAAA", 5);
            uint32_t decodedMax5SignificantChanged = geneCodec.decode("CBAAA", 5);
            uint32_t decodedMax5InsignificantChanged = geneCodec.decode("CABCC", 5);

            uint32_t decodedMax9 = geneCodec.decode("CAAAA", 9);
            uint32_t decodedMax9SignificantChanged = geneCodec.decode("CABAA", 9);
            uint32_t decodedMax9InsignificantChanged = geneCodec.decode("CAABC", 9);

            THEN("the resulting numbers are only different if a significant bit was changed")
            {
                CHECK(decodedMax5 != decodedMax5SignificantChanged);
                CHECK(decodedMax5 == decodedMax5InsignificantChanged);
                CHECK(decodedMax9 != decodedMax9SignificantChanged);
                CHECK(decodedMax9 == decodedMax9InsignificantChanged);
            }
        }
    }
}

SCENARIO("Genes can be encoded into/decoded from longer gene sequences", "[genes]")
{
    GIVEN("A GeneCodec")
    {
        dna::GeneCodec geneCodec(2);

        WHEN("3 values are encoded into the same gene")
        {
            dna::Gene gene;
            geneCodec.encode(10, 15, gene);
            geneCodec.encode(1, 1, gene);
            geneCodec.encode(15, 15, gene);

            THEN("the gene sequence contains all three values encoded next to each other")
            {
                CHECK(gene == "ABABBBBBB");
            }
        }

        WHEN("3 values are decoded from the same gene using offset access")
        {
            dna::Gene gene = "ABABBBBBB";
            uint32_t offset = 0;
            uint32_t decoded10 = geneCodec.decode(gene, 15, offset, offset);
            uint32_t decoded1 = geneCodec.decode(gene, 1, offset, offset);
            uint32_t decoded15 = geneCodec.decode(gene, 15, offset, offset);

            THEN("the values are correct")
            {
                CHECK(decoded10 == 10);
                CHECK(decoded1 == 1);
                CHECK(decoded15 == 15);
            }
        }
    }
}
