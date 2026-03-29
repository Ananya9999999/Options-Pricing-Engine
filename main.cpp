#include <iostream>
#include <cmath>

double N(double x) {
    return 0.5 * erfc(-x / sqrt(2));
}

double n(double x) {
    return exp(-0.5*x*x) / sqrt(2*M_PI);
}

int main() {
    double S=100, K=100, r=0.05, T=1.0, sigma=0.2;

    double d1 = (log(S/K) + (r + 0.5*sigma*sigma)*T) / (sigma*sqrt(T));
    double d2 = d1 - sigma*sqrt(T);
    
    double delta = N(d1);                           
    double gamma = n(d1) / (S * sigma * sqrt(T));  
    double theta = (-(S * n(d1) * sigma) / (2 * sqrt(T)) - r * K * exp(-r*T) * N(d2)) / 365; 
    double vega  = S * n(d1) * sqrt(T) / 100;     
    double callPrice = S*N(d1) - K*exp(-r*T)*N(d2); 

    std::cout << "Call Price: " << callPrice;
    std::cout << "\n--- Greeks ---\n";
    std::cout << "Delta: " << delta << "\n";
    std::cout << "Gamma: " << gamma << "\n";
    std::cout << "Theta: " << theta << "\n";
    std::cout << "Vega:  " << vega  << "\n";
}