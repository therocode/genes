#pragma once
#include <dna/allele.hpp>

namespace dna
{
    template <typename Archive, typename StorageType, CompeteType tCompeteType>
    void save(Archive& ar, const Allele<StorageType, tCompeteType>& allele)
    {
        const StorageType& value = allele.value();
        int32_t strength = allele.strength();
        ar(strength, value);
    }

    template <typename Archive, typename StorageType, CompeteType tCompeteType>
    void load(Archive& ar, Allele<StorageType, tCompeteType>& allele)
    {
        StorageType value;
        int32_t strength;

        ar(strength, value);

        allele = Allele<StorageType, tCompeteType>(value, strength);
    }
}
