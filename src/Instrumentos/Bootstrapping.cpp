#include "Bootstrapping.hpp"

Bootstrapping::DiscountCurveType Bootstrapping::operator()(
    const InstrumentsMapType& instruments) const
{
    DiscountCurveType curve;
    double annuity = 0;
	 
    for (auto instrument : instruments) {
        curve.insert(std::make_pair(
            instrument.first,
            instrument.second->computeDiscountFactor(curve)));
    }
    return curve;
}
