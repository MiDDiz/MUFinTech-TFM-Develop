#ifndef FIXEDLEGDESCRIPTION_H
#define FIXEDLEGDESCRIPTION_H

#include "LegDescription.hpp"
#include "ZerocouponCurve.hpp"
#include <memory>

class FixedLegDescription : public LegDescription {
public:
    FixedLegDescription(std::vector<double>& paymentPeriods_,
        std::map<double, double>& cashFlows_,
        std::shared_ptr<ZerocouponCurve> zeroCurve_)
        : cashFlows { cashFlows_ }
        , paymentPeriods { paymentPeriods_ }
        , zeroCurve { zeroCurve_ }
    {
    }

    double computePrice() override
    {
        double calculatedPrice = 0;
        // Descontar todos los flujos con el cupon cero
        for (auto period : paymentPeriods) {
            calculatedPrice += cashFlows[period] * zeroCurve->get_dcf(period);
        }
        return calculatedPrice;
    }

    std::map<double, double> cashFlows;

private:
    std::vector<double> paymentPeriods;
    std::shared_ptr<ZerocouponCurve> zeroCurve;
};

#endif
