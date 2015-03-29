#include <catch.hpp>
#include <dna/cistron.hpp>
#include <dna/genecodec.hpp>

SCENARIO("cistrons can be used to encode/decode a whole set of data at once", "[genes]")
{
    GIVEN("some r g b data and a cistron id")
    {
        uint32_t r = 10;
        uint32_t g = 100;
        uint32_t b = 255;
        uint32_t maxValue = 255;
        uint32_t id = 1;
        uint32_t maxCistronIds = 10;
        uint32_t informationDensity = 4;

        WHEN("a cistron is created from data")
        {
            dna::Cistron colorCistron(id, maxCistronIds, informationDensity, {{r, maxValue}, {g, maxValue}, {b, maxValue}});

            THEN("a correct gene string is accessible")
            {
                dna::GeneCodec codec(informationDensity);
                dna::Gene correctGene;

                codec.encode(id, maxCistronIds, correctGene);
                codec.encode(r, maxValue, correctGene);
                codec.encode(g, maxValue, correctGene);
                codec.encode(b, maxValue, correctGene);

                CHECK(correctGene == colorCistron.gene());
            }
        }

        WHEN("a cistron is created from a gene")
        {
            dna::Cistron creator(id, maxCistronIds, informationDensity, {{r, maxValue}, {g, maxValue}, {b, maxValue}});

            dna::Cistron colorCistron(creator.gene(), maxCistronIds, informationDensity);

            THEN("original data can be accessed")
            {
                uint32_t unpackedR;
                uint32_t unpackedG;
                uint32_t unpackedB;

                colorCistron.decode({{unpackedR, maxValue}, {unpackedG, maxValue}, {unpackedB, maxValue}});

                CHECK(colorCistron.id() == id);
                CHECK(unpackedR == r);
                CHECK(unpackedG == g);
                CHECK(unpackedB == b);
            }
        }
    }
}
