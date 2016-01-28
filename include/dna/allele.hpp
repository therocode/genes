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
            Allele(int32_t id, StorageType&& value, int32_t strength);
            int32_t id() const;
            const StorageType& value() const;
            int32_t strength() const;
            CompeteType competeType() const;
        private:
            int32_t mId;
            StorageType mValue;
            int32_t mStrength;
    };

    template<typename StorageType, CompeteType tCompeteType>
    using AlleleList = std::vector<Allele<StorageType, tCompeteType>>;

    template <typename StorageType, CompeteType tCompeteType>
    Allele<StorageType, tCompeteType>::Allele(int32_t id, StorageType&& value, int32_t strength):
        mId(id),
        mValue(std::forward<StorageType>(value)),
        mStrength(strength)
    {
    }

    template <typename StorageType, CompeteType tCompeteType>
    int32_t Allele<StorageType, tCompeteType>::id() const
    {
        return mId;
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

    template <typename StorageType, CompeteType tCompeteType>
    CompeteType Allele<StorageType, tCompeteType>::competeType() const
    {
        return tCompeteType;
    }
}
