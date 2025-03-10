#ifndef DEPOSIT_H
#define DEPOSIT_H

#include "IPricer.hpp"
#include "LegDescription.hpp"
#include "ZerocouponCurve.hpp"

class Deposit : public IPricer {
private:
    std::shared_ptr<LegDescription> receiverLeg;
    std::shared_ptr<ZerocouponCurve> curve;
    double depositRate;
    double maturity;
    double period;

public:
    Deposit(std::shared_ptr<LegDescription> receiver,
        std::shared_ptr<ZerocouponCurve> curve,
        double rate,
        double maturity,
        double period)
        : receiverLeg { receiver }
        , curve { curve }
        , depositRate { rate }
        , maturity { maturity }
        , period { period }
    {
    }

    double getValuation() override
    {
        return receiverLeg->computePrice();
    }

    double computeDiscountFactor(std::map<double, double>& boostrapingCurve) override
    {
        return curve->get_dcf(period);
    }
};

#endif
