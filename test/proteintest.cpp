#include "catch.hpp"
#include <dna/protein.hpp>

SCENARIO("Proteins can store a value", "[dna]")
{
    GIVEN("Proteins with different data")
    {
        dna::Protein<int> protein1(34);
        dna::Protein<std::string> protein2("hej");

        WHEN("")
        {
            THEN("the proteins' data can be accessed")
            {
                CHECK(protein1.value() == 34);

                CHECK(protein2.value() == "hej");
            }
        }
    }
}
