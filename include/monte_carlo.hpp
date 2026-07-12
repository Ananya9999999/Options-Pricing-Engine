#pragma once
#include "common.hpp"

namespace qpe {

    struct MCResult {
        double price;
        double stdError;  
        double ciLow;    
        double ciHigh;     
    };

    class MonteCarlo {
    public:
        static MCResult price(const OptionParams& p, OptionType type, long numPaths, bool antithetic = true, unsigned seed = 42);
    };

}  
