#pragma once

namespace dna
{
    template <typename CodecBackend>
    class BinaryCodec
    {
        public:
            template <typename InType>
            std::vector<uint8_t> encode(InType&& data);
            template <typename OutType>
            OutType decode(const std::vector<uint8_t>& data);
    };

    template <typename CodecBackend>
    template <typename InType>
    std::vector<uint8_t> BinaryCodec<CodecBackend>::encode(InType&& data)
    {
        return {};
    }

    template <typename CodecBackend>
    template <typename OutType>
    OutType BinaryCodec<CodecBackend>::decode(const std::vector<uint8_t>& data)
    {
        return {};
    }
}
