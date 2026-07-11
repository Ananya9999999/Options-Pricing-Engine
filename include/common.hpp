#pragma once
#include <cmath>
namespace qpe{
    enum class OptionType{Call, Put};

    struct OptionParams{
        double S;     
        double K;      
        double r;      
        double T;      
        double sigma;  
    };

    inline double normCDF(double x){
        return 0.5 * std::erfc(-x / std::sqrt(2.0));
    }

    inline double normPDF(double x){
        static const double INV_SQRT_2PI= 1.0/std::sqrt(2.0*M_PI);
        return INV_SQRT_2PI*std::exp(-0.5*x*x);
    }
}