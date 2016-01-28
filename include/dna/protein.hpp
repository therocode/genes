#pragma once
#include <vector>

namespace dna
{
    template <typename Type>
    class Protein
    {
        public:
            Protein(int32_t id, Type value);
            int32_t id() const;
            const Type& value() const;
        private:
            int32_t mId;
            Type mValue;
    };

    template <typename Type>
    using ProteinList = std::vector<Protein<Type>>;

    template <typename Type>
    Protein<Type>::Protein(int32_t id, Type value):
        mId(id),
        mValue(std::move(value))
    {
    }

    template <typename Type>
    int32_t Protein<Type>::id() const
    {
        return mId;
    }

    template <typename Type>
    const Type& Protein<Type>::value() const
    {
        return mValue;
    }
}
