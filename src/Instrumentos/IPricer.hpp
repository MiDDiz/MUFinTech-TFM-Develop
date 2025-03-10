#ifndef IPRICER_H
#define IPRICER_H

#include <map>
class IPricer {
public:
    virtual double getValuation() = 0; // Funciones virtuales
    virtual double computeDiscountFactor(std::map<double, double>& curve) = 0;
    virtual ~IPricer() = default; // Virtual destructor
    virtual double getTIR()
    {
        return (0);
    };
};

#endif
