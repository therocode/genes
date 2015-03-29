#include <catch.hpp>
#include <dna/cistron.hpp>
#include <dna/chromosome.hpp>

const uint32_t maxCistronIds = 5;
const uint32_t informationDensity = 4;
const std::string startSequence = "AAAAAAA";
const std::string stopSequence = "BBBBBBB";

SCENARIO("chromosomes can be made from a group of cistrons, and the cistrons can be fetched out even with junk data inserted", "[data]")
{
    GIVEN("some cistrons")
    {
        dna::Cistron cist1(0, maxCistronIds, informationDensity, {{3, 7}, {8, 16}});
        dna::Cistron cist2(1, maxCistronIds, informationDensity, {{63, 63}});
        dna::Cistron cist3(2, maxCistronIds, informationDensity, {{1, 8}, {80, 160}});

        WHEN("a chromosome is created from the cistrons, and cistrons are then created from the chromosome")
        {
            dna::Chromosome chromosome(maxCistronIds, informationDensity, startSequence, stopSequence, {cist1, cist2, cist3}, 10, 20);

            uint32_t cistronAmount = chromosome.cistronAmount();

            THEN("the cistron amount in the chromosome is correct")
            {
                CHECK(cistronAmount == 3);
            }

            THEN("a map of the cistron genes with correct values can be retreived")
            {
                dna::CistronMap cistrons = chromosome.cistrons();
                
                REQUIRE(cistrons.size() == 3);

                for(auto cistronIter : cistrons)
                {
                    dna::Cistron cistron = cistronIter.second;

                    if(cistronIter.first == 0)
                    {
                        uint32_t a;
                        uint32_t b;
                        cistron.decode({{a, 7},{b, 16}});

                        CHECK(a == 3);
                        CHECK(b == 8);
                    }
                    else if(cistronIter.first == 1)
                    {
                        uint32_t a;
                        cistron.decode({{a, 63}});
                        CHECK(a == 63);
                    }
                    else if(cistronIter.first == 2)
                    {
                        uint32_t a;
                        uint32_t b;
                        cistron.decode({{a, 8},{b, 160}});

                        CHECK(a == 1);
                        CHECK(b == 80);
                    }
                }
            }
        }
    }
}
