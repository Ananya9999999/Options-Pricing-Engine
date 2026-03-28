# Options Pricing Engine

A Black-Scholes options pricing engine built in C++.

## What it does
Calculates the fair value of European call/put options using the 
Black-Scholes model.

## Inputs
- S = Stock price
- K = Strike price  
- r = Risk-free rate
- T = Time to expiry (years)
- σ = Volatility

## Usage
g++ main.cpp -o main
./main

## Sample Output
Call Price: 10.4506

## Next Steps
- Adding Greeks (Delta, Gamma, Theta)
- Put option pricing
