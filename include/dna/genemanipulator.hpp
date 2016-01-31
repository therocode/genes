#pragma once
#include <dna/gene.hpp>

namespace dna
{
    class GeneManipulator
    {
        public:
            GeneManipulator(const Gene& gene);
            template <typename Data>
            void replace(int32_t dataId, int32_t dataIndex, Data data);
    };

    template <typename Data>
    void GeneManipulator::replace(int32_t dataId, int32_t dataIndex, Data data)
    {
    }
}
