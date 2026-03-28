#include <iostream>
#include <cmath>

double N(double x) {
    return 0.5 * erfc(-x / sqrt(2));
}

double blackScholes(double S, double K, double r, double T, double sigma) {
    double d1 = (log(S/K) + (r + 0.5*sigma*sigma)*T) / (sigma*sqrt(T));
    double d2 = d1 - sigma*sqrt(T);
    return S*N(d1) - K*exp(-r*T)*N(d2); 
}

int main() {
    double S=100, K=100, r=0.05, T=1.0, sigma=0.2;
    std::cout << "Call Price: " << blackScholes(S,K,r,T,sigma);
}