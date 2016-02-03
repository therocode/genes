#pragma once
#include <vector>
#include <cstdint>

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
        private:
            CodecBackend mBackend;
    };

    template <typename CodecBackend>
    template <typename InType>
    std::vector<uint8_t> BinaryCodec<CodecBackend>::encode(InType&& data)
    {
        return mBackend. template encode(std::forward<InType>(data));
    }

    template <typename CodecBackend>
    template <typename OutType>
    OutType BinaryCodec<CodecBackend>::decode(const std::vector<uint8_t>& data)
    {
        return mBackend. template decode<OutType>(data);
    }
}
