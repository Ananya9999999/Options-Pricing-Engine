#pragma once
#include "common.hpp"

namespace qpe {
    class BinomialTree {
    public:
        static double price(const OptionParams& p, OptionType type, int steps, bool american = false);
    };
}  
