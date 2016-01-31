#include "catch.hpp"
#include <dna/allele.hpp>
#include <dna/makeprotein.hpp>

SCENARIO("Alleles can store a.value(), and a strength and a competetive type", "[dna]")
{
    GIVEN("Alleles with different data")
    {
        dna::Allele<int, dna::CompeteType::Exclusive> allele1(34, 100);
        dna::Allele<std::string, dna::CompeteType::Mix> allele2("hej", 230);

        WHEN("")
        {
            THEN("the alleles' data can be accessed")
            {
                CHECK(allele1.value() == 34);
                CHECK(allele1.strength() == 100);
                CHECK(allele1.competeType == dna::CompeteType::Exclusive);

                CHECK(allele2.value() == "hej");
                CHECK(allele2.strength() == 230);
                CHECK(allele2.competeType == dna::CompeteType::Mix);
            }
        }
    }
}

SCENARIO("Proteins can be generated from sets of several competing alleles", "[dna]")
{
    GIVEN("Same-type alleles that compete through exclusiveness")
    {
        using NameAllele = dna::Allele<std::string, dna::CompeteType::Exclusive>;
        NameAllele allele1("hej", 10);
        NameAllele allele2("hoj", 70);
        NameAllele allele3("kalle", 100);
        NameAllele allele4("roger", 100);

        WHEN("proteins are generated from a set with differing strengths")
        {
            auto proteins = dna::makeProteins<NameAllele>({allele1, allele2, allele3});

            THEN("the protein list contains one protein with data equivalent to the strongest allele")
            {
                REQUIRE(proteins.size() == 1);

                const dna::Protein<std::string>& protein = proteins[0];
                CHECK(protein.value() == allele3.value());
            }
        }

        WHEN("proteins are generated from a set with some alleles of matched strengths")
        {
            auto proteins = dna::makeProteins<NameAllele>({allele1, allele2, allele3, allele4});

            THEN("the protein list contains proteins with data equivalent to the strongest alleles")
            {
                REQUIRE(proteins.size() == 2);

                bool correctValues = true;

                for(const dna::Protein<std::string>& protein : proteins)
                {
                    if(protein.value() != "kalle" && protein.value() != "roger")
                        correctValues = false;
                }

                CHECK(correctValues);
                CHECK(proteins[0].value() != proteins[1].value());
            }
        }
    }

    GIVEN("Same-type alleles that compete through mixing")
    {
        using FloatMixAllele = dna::Allele<float, dna::CompeteType::Mix>;
        FloatMixAllele allele1(1.0f, 20);
        FloatMixAllele allele2(2.0f, 80);
        FloatMixAllele allele3(4.0f, 40);

        WHEN("proteins are generated from a set")
        {
            auto proteins = dna::makeProteins<FloatMixAllele>({allele1, allele2, allele3});

            THEN("the protein list contains one protein with all data mixed with the relative strengths as weights")
            {
                REQUIRE(proteins.size() == 1);

                const auto& protein = proteins[0];

                CHECK(protein.value() == Approx(2.42857f));
            }
        }
    }
}
