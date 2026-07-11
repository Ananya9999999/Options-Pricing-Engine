#include "black_scholes.hpp"

namespace qpe{
    double BlackScholes::d1(const OptionParams& p){
        return (std::log(p.S/p.K)+(p.r+0.5*p.sigma*p.sigma)*p.T)/ (p.sigma*std::sqrt(p.T));
    }

    double BlackScholes::d2(const OptionParams& p){
        return d1(p)-p.sigma*std::sqrt(p.T);
    }

    double BlackScholes::price(const OptionParams& p, OptionType type){
        double D1= d1(p);
        double D2= d2(p);
        double disc= std::exp(-p.r*p.T);

        if (type==OptionType::Call){
            return p.S*normCDF(D1)-p.K*disc*normCDF(D2);
        }
        return p.K*disc*normCDF(-D2)-p.S*normCDF(-D1);
    }

    Greeks BlackScholes::computeGreeks(const OptionParams& p, OptionType type){
        double D1= d1(p);
        double D2= d2(p);
        double disc= std::exp(-p.r*p.T);
        double sqrtT= std::sqrt(p.T);

        Greeks g{};

        g.delta= (type==OptionType::Call) ? normCDF(D1):normCDF(D1)-1.0;
        g.gamma= normPDF(D1)/(p.S*p.sigma*sqrtT);
        g.vega= p.S*normPDF(D1)*sqrtT/100.0;

        if (type==OptionType::Call){
            g.theta= (-(p.S*normPDF(D1)*p.sigma)/(2.0*sqrtT)-p.r*p.K*disc*normCDF(D2))/365.0;
        }
        else{
            g.theta= (-(p.S*normPDF(D1)*p.sigma)/(2.0*sqrtT)+p.r*p.K*disc*normCDF(-D2))/365.0;
        }

        if (type==OptionType::Call){
            g.rho= p.K*p.T*disc*normCDF(D2)/100.0;
        }
        else{
            g.rho= -p.K*p.T*disc*normCDF(-D2)/100.0;
        }
        return g;
    }
}