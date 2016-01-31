#include <dna/information.hpp>

namespace dna
{
    bool operator==(const Information a, const Information b)
    {
        return a.identifier == b.identifier && a.data == b.data;
    }
}
