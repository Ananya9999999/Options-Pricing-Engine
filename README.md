# Options Pricing Engine

A C++ implementation of three option pricing methods, cross-validated against
each other: **Black-Scholes** (closed-form), **Monte Carlo** (GBM simulation),
and **Binomial Tree** (Cox-Ross-Rubinstein lattice).

Repo: [github.com/Ananya9999999/Options-Pricing-Engine](https://github.com/Ananya9999999/Options-Pricing-Engine)

## Why three methods

Black-Scholes gives an exact answer but only for European options under
strong assumptions (constant volatility, no dividends, continuous trading).
Monte Carlo and the binomial tree are more general — they extend to American
exercise, path-dependent payoffs, and non-lognormal dynamics — but they're
numerical approximations. Running all three on the same option is a
correctness check: the numerical methods should converge to the
Black-Scholes price for a plain European option, and diverge from it in
exactly the way theory predicts (e.g. American puts pricing higher due to
early exercise value).

## Project structure

```
Options-Pricing-Engine/
├── CMakeLists.txt
├── include/
│   ├── common.hpp          # OptionParams, OptionType, normCDF/normPDF
│   ├── black_scholes.hpp
│   ├── monte_carlo.hpp
│   └── binomial_tree.hpp
├── src/
│   ├── black_scholes.cpp   # closed-form price + Greeks
│   ├── monte_carlo.cpp     # GBM simulation with antithetic variates
│   ├── binomial_tree.cpp   # CRR lattice, European + American
│   └── main.cpp            # runs all three, prints comparison tables
└── README.md
```

## The math

### Black-Scholes
Closed-form solution assuming `S_T` is lognormally distributed under the
risk-neutral measure:

```
d1 = [ln(S/K) + (r + 0.5*sigma^2)*T] / (sigma*sqrt(T))
d2 = d1 - sigma*sqrt(T)

Call = S*N(d1) - K*e^(-rT)*N(d2)
Put  = K*e^(-rT)*N(-d2) - S*N(-d1)
```

Greeks (delta, gamma, theta, vega, rho) are the analytic partial derivatives
of this formula. Theta is reported per calendar day, vega per 1% vol move,
and rho per 1% rate move — matching how they're quoted on a trading desk.

### Monte Carlo
Simulate terminal spot prices directly under the risk-neutral GBM dynamics:

```
S_T = S * exp[(r - 0.5*sigma^2)*T + sigma*sqrt(T)*Z],   Z ~ N(0,1)
```

Price = `e^(-rT) * average(payoff(S_T))` across all simulated paths.

**Antithetic variates**: for every random draw `Z`, the engine also computes
the path using `-Z`. Since `Z` and `-Z` are perfectly negatively correlated,
this pairs each high outcome with a low one, reducing the variance of the
average without needing extra independent random draws. A standard error and
95% confidence interval are reported alongside the price, since a Monte
Carlo estimate is statistical, not exact.

### Binomial Tree (CRR)
Discretize time into `N` steps. At each step, price moves up by factor `u`
or down by factor `d`:

```
u = e^(sigma*sqrt(dt)),   d = 1/u
q = (e^(r*dt) - d) / (u - d)     # risk-neutral probability of an up move
```

Build the tree forward to get terminal payoffs, then work backward,
discounting the risk-neutral expectation at each node:

```
V(node) = e^(-r*dt) * [q*V(up) + (1-q)*V(down)]
```

For **American** options, each node takes `max(continuation value, intrinsic
value)` — this is what allows the model to capture early exercise. The
implementation uses O(N) space (one array, overwritten each step) rather
than storing the full triangle.

## Build & run

Clone and enter the repo first:
```bash
git clone https://github.com/Ananya9999999/Options-Pricing-Engine.git
cd Options-Pricing-Engine
```

**With CMake:**
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
./pricing_engine
```

**Or directly with g++ (no CMake required):**
```bash
g++ -std=c++17 -O2 -Iinclude src/main.cpp src/black_scholes.cpp src/monte_carlo.cpp src/binomial_tree.cpp -o pricing_engine
./pricing_engine
```

## Sample output (S=100, K=100, r=5%, T=1yr, sigma=20%, Call)

```
=== Black-Scholes (closed form) ===
Price: 10.450584
  Delta: 0.636831
  Gamma: 0.018762
  Theta: -0.017573  (per calendar day)
  Vega:  0.375240  (per 1% vol move)
  Rho:   0.532325  (per 1% rate move)

=== Binomial Tree Convergence (European Call) ===
     Steps          Price   Abs Error vs BS
        10      10.253409          0.197175
        50      10.410692          0.039892
       100      10.430612          0.019972
       500      10.446585          0.003998
      1000      10.448584          0.001999
      5000      10.450184          0.000400

=== Monte Carlo Convergence (European Call, antithetic variates) ===
       Paths          Price      Std Error                        95% CI
        1000      10.635034       0.478005                [9.698, 11.57]
       10000      10.516702       0.147758                [10.23, 10.81]
      100000      10.452037       0.046501                [10.36, 10.54]
     1000000      10.449871       0.014738                [10.42, 10.48]

=== Early Exercise Premium (Binomial, 1000 steps, Put) ===
European Put: 5.571527
American Put: 6.089595
Early exercise premium: 0.518069
```

## Possible next steps
- Implied volatility solver (Newton-Raphson or Brent's method on the BS price)
- Dividend yield support (affects both drift and early-exercise logic for calls)
- Vega/gamma for the binomial and Monte Carlo methods via finite differences,
  to cross-check the analytic BS Greeks
- Unit tests (e.g. Catch2) instead of eyeballing convergence output