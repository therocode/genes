#include "catch.hpp"
#include <dna/genecodec.hpp>

//SCENARIO("A GeneCodec can be used to encode/decode a list of ID's and binary data into a gene string based on specific settings", "[dna]")
//{
//    GIVEN("A GeneCodec with a specific spread, available symbols, and a given start/end sequence")
//    {
//        dna::GeneCodec geneCodec({50, 100}, {'', '', '', ''}, "GATTA", "CATTA");
//
//        WHEN("some data is encoded")
//        {
//            std::pair<int32_t, std::vector<bool>> data
//            {
//                666,
//                {
//                    true, false, false, true, true, true, false,
//                },
//            };
//
//            dna::Gene encoded3 = geneCodec.encode(3, 32);
//            dna::Gene encoded8 = geneCodec.encode(8, 32);
//            dna::Gene encoded21 = geneCodec.encode(21, 32);
//
//            THEN("the right gene string is produced")
//            {
//                CHECK(encoded3  == "TTAAAA");
//                CHECK(encoded8  == "AAATAA");
//                CHECK(encoded21 == "TATATA");
//            }
//        }
//    }
//}
