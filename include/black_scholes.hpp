#pragma once
#include "common.hpp"

namespace qpe{
    struct Greeks{
        double delta;
        double gamma;
        double theta;
        double vega;
        double rho;
    };

    class BlackScholes{
        public:
            static double price(const OptionParams& p, OptionType type);
            static Greeks computeGreeks(const OptionParams& p, OptionType type);

        private:
            static double d1(const OptionParams& p);
            static double d2(const OptionParams& p);
    };
}