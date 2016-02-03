#include "catch.hpp"
#include <dna/binarycodec.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <dna/cerealbackend.hpp>
#include <unordered_map>

SCENARIO("A BinaryCodec can be used to serialize/deserialize thing to and from std::vector<uint8_t>", "[dna]")
{
    GIVEN("A binary codec and some data")
    {
        dna::BinaryCodec<dna::CerealBackend> binaryCodec;

        std::vector<std::unordered_map<std::string, int>> data
        {
            {
                {"hej", 23},
                {"hoj", 100},
            },
            {
                {"blapp", 1203},
                {"blopp", 293423},
            }
        };

        WHEN("When the data is serialized and deserialized")
        {
            std::vector<uint8_t> serialized = binaryCodec.encode(data);
            auto deserialized = binaryCodec.decode<std::vector<std::unordered_map<std::string, int>>>(serialized);

            THEN("the deserialized data matches the original data")
            {
                CHECK(data == deserialized);
            }
        }
    }
}
