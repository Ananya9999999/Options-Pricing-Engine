#include "binomial_tree.hpp"
#include <algorithm>
#include <vector>

    namespace qpe {
    double BinomialTree::price(const OptionParams& p, OptionType type, int steps,bool american) {
        double dt = p.T / steps;
        double u = std::exp(p.sigma * std::sqrt(dt));  
        double d = 1.0 / u;                              
        double disc = std::exp(-p.r * dt);
        double growth = std::exp(p.r * dt);
        double q = (growth - d) / (u - d);                

        std::vector<double> values(steps + 1);
        for (int i = 0; i <= steps; ++i) {
            double ST = p.S * std::pow(u, steps - i) * std::pow(d, i);
            values[i] = (type == OptionType::Call) ? std::max(ST - p.K, 0.0): std::max(p.K - ST, 0.0);
        }
        for (int step = steps - 1; step >= 0; --step) {
            for (int i = 0; i <= step; ++i) {
                double continuation = disc * (q * values[i] + (1.0 - q) * values[i + 1]);

                if (american) {
                    double ST = p.S * std::pow(u, step - i) * std::pow(d, i);
                    double intrinsic = (type == OptionType::Call) ? std::max(ST - p.K, 0.0): std::max(p.K - ST, 0.0);
                    values[i] = std::max(continuation, intrinsic);
                } else {
                    values[i] = continuation;
                }
            }
        }

        return values[0];
    }
} 
