#ifndef FLOATINGLEGDESCRIPTION_H
#define FLOATINGLEGDESCRIPTION_H

#include "Index.hpp"
#include "LegDescription.hpp"

class FloatingLegDescription : public LegDescription {
public:
    FloatingLegDescription(
        std::shared_ptr<Index>& indexValuator_, // índice para generar los forwards
        std::vector<double>& paymentPeriods_, // periodos para cada flujo
        std::shared_ptr<ZerocouponCurve>& zeroCurve_, // curva para descontar precios
        double nominal_)
        : indexValuator { indexValuator_ }
        , paymentPeriods { paymentPeriods_ }
        , zeroCurve { zeroCurve_ }
        , nominal { nominal_ }
    {
        indexValuator_->generateRates(zeroCurve); // generar forward rates
        for (auto period : paymentPeriods) // calcular precios forward
        {
            cashFlows[period] = indexValuator_->get_rate(period) * nominal
                * zeroCurve_->dateCountFractions[period];
        }
    }
    double computePrice() override
    {
        double calculatedPrice = 0;
        for (auto period : paymentPeriods) // descontar los flujos
        {
            calculatedPrice += cashFlows[period] * zeroCurve->get_dcf(period);
        }
        return calculatedPrice;
    }
    std::map<double, double> cashFlows;

private:
    std::shared_ptr<Index> indexValuator;
    std::shared_ptr<ZerocouponCurve> zeroCurve;
    std::vector<double> paymentPeriods;
    double nominal;
};
#endif
