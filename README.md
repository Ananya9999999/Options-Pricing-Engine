# Options Pricing Engine

A Black-Scholes options pricing engine built in C++, capable of pricing 
European call options and calculating key risk sensitivities (Greeks).

## Background

The Black-Scholes model is the foundation of modern options pricing. It 
calculates the theoretical fair value of a European option given a set of 
market parameters, assuming constant volatility and no arbitrage.

## What it Calculates

**Call Price** — Fair value of a European call option

**Greeks (Risk Sensitivities)**
- Delta — Price sensitivity to a ₹1 move in the underlying
- Gamma — Rate of change of Delta
- Theta — Daily time decay (value lost per day)
- Vega  — Sensitivity to a 1% change in volatility

## Sample Output
```
Call Price: 10.4506

--- Greeks ---
Delta: 0.636831
Gamma: 0.018762
Theta: -0.0175727
Vega:  0.37524
```

**Interpretation:**
- A ₹1 rise in stock price increases the option value by ₹0.64 (Delta)
- The option loses ₹0.0176 in value every day (Theta)
- A 1% rise in volatility increases option value by ₹0.375 (Vega)

## Parameters Used

| Parameter | Value | Description |
|---|---|---|
| S | 100 | Current stock price |
| K | 100 | Strike price |
| r | 0.05 | Risk-free rate (5%) |
| T | 1.0 | Time to expiry (1 year) |
| σ | 0.20 | Volatility (20%) |

## Usage
```bash
g++ main.cpp -o main
./main
```

## Next Steps
- [ ] Put option pricing
- [ ] User input for parameters
- [ ] Monte Carlo pricing simulation
- [ ] Implied volatility calculator
