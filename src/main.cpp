#include <iomanip>
#include <iostream>
#include <sstream>

#include "binomial_tree.hpp"
#include "black_scholes.hpp"
#include "monte_carlo.hpp"

using namespace qpe;

void printGreeks(const Greeks& g) {
    std::cout << "  Delta: " << g.delta << "\n";
    std::cout << "  Gamma: " << g.gamma << "\n";
    std::cout << "  Theta: " << g.theta << "  (per calendar day)\n";
    std::cout << "  Vega:  " << g.vega << "  (per 1% vol move)\n";
    std::cout << "  Rho:   " << g.rho << "  (per 1% rate move)\n";
}

int main() {
    OptionParams p{100.0, 100.0, 0.05, 1.0, 0.20};
    OptionType type = OptionType::Call;

    std::cout << std::fixed << std::setprecision(6);

    double bsPrice = BlackScholes::price(p, type);
    Greeks g = BlackScholes::computeGreeks(p, type);

    std::cout << "=== Black-Scholes (closed form) ===\n";
    std::cout << "Price: " << bsPrice << "\n";
    printGreeks(g);

    std::cout << "\n=== Binomial Tree Convergence (European Call) ===\n";
    std::cout << std::setw(10) << "Steps" << std::setw(15) << "Price"
               << std::setw(18) << "Abs Error vs BS" << "\n";
    for (int steps : {10, 50, 100, 500, 1000, 5000}) {
        double treePrice = BinomialTree::price(p, type, steps, false);
        std::cout << std::setw(10) << steps << std::setw(15) << treePrice
                   << std::setw(18) << std::abs(treePrice - bsPrice) << "\n";
    }

    std::cout << "\n=== Monte Carlo Convergence (European Call, antithetic variates) ===\n";
    std::cout << std::setw(12) << "Paths" << std::setw(15) << "Price"
               << std::setw(15) << "Std Error" << std::setw(30) << "95% CI" << "\n";
    for (long paths : {1000L, 10000L, 100000L, 1000000L}) {
        MCResult mc = MonteCarlo::price(p, type, paths, true);
        std::ostringstream ci;
        ci << "[" << std::setprecision(4) << mc.ciLow << ", " << mc.ciHigh << "]";
        std::cout << std::setw(12) << paths << std::setprecision(6) << std::setw(15)
                   << mc.price << std::setw(15) << mc.stdError << std::setw(30)
                   << ci.str() << "\n";
    }
    std::cout << "\n=== Early Exercise Premium (Binomial, 1000 steps, Put) ===\n";
    OptionType putType = OptionType::Put;
    double euroPut = BinomialTree::price(p, putType, 1000, false);
    double amerPut = BinomialTree::price(p, putType, 1000, true);
    std::cout << "European Put: " << euroPut << "\n";
    std::cout << "American Put: " << amerPut << "\n";
    std::cout << "Early exercise premium: " << (amerPut - euroPut) << "\n";

    return 0;
}
