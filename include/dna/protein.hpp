#pragma once
#include <vector>

namespace dna
{
    template <typename Type>
    class Protein
    {
        public:
            Protein(Type value);
            const Type& value() const;
        private:
            Type mValue;
    };

    template <typename Type>
    using ProteinList = std::vector<Protein<Type>>;

    template <typename Type>
    Protein<Type>::Protein(Type value):
        mValue(std::move(value))
    {
    }

    template <typename Type>
    const Type& Protein<Type>::value() const
    {
        return mValue;
    }
}
