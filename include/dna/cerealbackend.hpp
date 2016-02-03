#pragma once
#include <vector>
#include <cstdint>
#include <sstream>
#include <cereal/archives/binary.hpp>

namespace dna
{
    class CerealBackend
    {
        public:
            template <typename InType>
            std::vector<uint8_t> encode(InType&& data);
            template <typename OutType>
            OutType decode(const std::vector<uint8_t>& data);
    };

    template <typename InType>
    std::vector<uint8_t> CerealBackend::encode(InType&& data)
    {
        std::stringstream stream;
        cereal::BinaryOutputArchive archive(stream);

        archive(data);   

        std::string temp = stream.str();
        return {temp.begin(), temp.end()};
    }

    template <typename OutType>
    OutType CerealBackend::decode(const std::vector<uint8_t>& data)
    {
        std::stringstream stream(std::string(data.begin(), data.end()));
        cereal::BinaryInputArchive archive(stream);

        OutType result;

        archive(result);

        return result;
    }
}
