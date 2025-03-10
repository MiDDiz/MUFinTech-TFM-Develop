#ifndef BOOTSTRAPPING_H
#define BOOTSTRAPPING_H

#include "IPricer.hpp"
#include <map>
#include <memory>

class Bootstrapping {
public:
    Bootstrapping() = default;

    typedef std::map<double, double> DiscountCurveType;
    typedef std::map<double, std::shared_ptr<IPricer>> InstrumentsMapType;

    DiscountCurveType operator()(const InstrumentsMapType& instruments) const;

    virtual ~Bootstrapping() = default;
};

#endif // BOOTSTRAPPING_H
