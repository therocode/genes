#pragma once
#include <vector>

namespace dna
{
    enum class CompeteType { Exclusive, Mix };

    template <typename StorageType, CompeteType tCompeteType>
    class Allele
    {
        public:
            using Type = StorageType;
            static constexpr CompeteType competeType = tCompeteType;
            Allele(StorageType&& value, int32_t strength);
            const StorageType& value() const;
            int32_t strength() const;
        private:
            StorageType mValue;
            int32_t mStrength;
    };

    template<typename StorageType, CompeteType tCompeteType>
    using AlleleList = std::vector<Allele<StorageType, tCompeteType>>;

    template <typename StorageType, CompeteType tCompeteType>
    constexpr CompeteType Allele<StorageType, tCompeteType>::competeType;

    template <typename StorageType, CompeteType tCompeteType>
    Allele<StorageType, tCompeteType>::Allele(StorageType&& value, int32_t strength):
        mValue(std::forward<StorageType>(value)),
        mStrength(strength)
    {
    }

    template <typename StorageType, CompeteType tCompeteType>
    const StorageType& Allele<StorageType, tCompeteType>::value() const
    {
        return mValue;
    }

    template <typename StorageType, CompeteType tCompeteType>
    int32_t Allele<StorageType, tCompeteType>::strength() const
    {
        return mStrength;
    }
}
