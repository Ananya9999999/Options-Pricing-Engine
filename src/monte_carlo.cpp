#include "monte_carlo.hpp"
#include <algorithm>
#include <random>
#include <vector>

namespace qpe {

    MCResult MonteCarlo::price(const OptionParams& p, OptionType type, long numPaths, bool antithetic, unsigned seed) {
        std::mt19937_64 rng(seed);
        std::normal_distribution<double> stdNormal(0.0, 1.0);
        double drift = (p.r - 0.5 * p.sigma * p.sigma) * p.T;
        double diffusionCoef = p.sigma * std::sqrt(p.T);
        double disc = std::exp(-p.r * p.T);

        auto payoffOf = [&](double ST) {
            return (type == OptionType::Call) ? std::max(ST - p.K, 0.0): std::max(p.K - ST, 0.0);
        };

        long baseDraws = antithetic ? numPaths / 2 : numPaths;
        std::vector<double> payoffs;
        payoffs.reserve(antithetic ? baseDraws * 2 : baseDraws);

        for (long i = 0; i < baseDraws; ++i) {
            double z = stdNormal(rng);
            double ST1 = p.S * std::exp(drift + diffusionCoef * z);
            payoffs.push_back(payoffOf(ST1));

            if (antithetic) {
                double ST2 = p.S * std::exp(drift - diffusionCoef * z);
                payoffs.push_back(payoffOf(ST2));
            }
        }

        long n = static_cast<long>(payoffs.size());
        double mean = 0.0;
        for (double v : payoffs) mean += v;
        mean /= n;

        double variance = 0.0;
        for (double v : payoffs) variance += (v - mean) * (v - mean);
        variance /= (n - 1);

        double stderrOfMean = std::sqrt(variance / n);

        MCResult result;
        result.price = disc * mean;
        result.stdError = disc * stderrOfMean;
        result.ciLow = result.price - 1.96 * result.stdError;
        result.ciHigh = result.price + 1.96 * result.stdError;
        return result;
    }

}  
