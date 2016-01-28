#include "catch.hpp"
#include <dna/protein.hpp>

SCENARIO("Proteins can store an ID and a value", "[dna]")
{
    GIVEN("Proteins with different data")
    {
        dna::Protein<int> protein1(3, 34);
        dna::Protein<std::string> protein2(4, "hej");

        WHEN("")
        {
            THEN("the proteins' data can be accessed")
            {
                CHECK(protein1.id() == 3);
                CHECK(protein1.value() == 34);

                CHECK(protein2.id() == 4);
                CHECK(protein2.value() == "hej");
            }
        }
    }
}
