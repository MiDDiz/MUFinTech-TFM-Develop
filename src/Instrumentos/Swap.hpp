#ifndef SWAP_H
#define SWAP_H

#include "IPricer.hpp"
#include "LegDescription.hpp"
#include "ZerocouponCurve.hpp"

class Swap : public IPricer {
private:
    std::shared_ptr<LegDescription> receiverLeg;
    std::shared_ptr<LegDescription> payerLeg;
    std::shared_ptr<ZerocouponCurve> curve;
    std::vector<double> periods;
    double fixedRate;

public:
    Swap(std::shared_ptr<LegDescription> receiver,
        std::shared_ptr<LegDescription> payer,
        std::shared_ptr<ZerocouponCurve> curve,
        std::vector<double> periods,
        double fixedRate)
        : receiverLeg { receiver }
        , payerLeg { payer }
        , curve { curve }
        , periods { periods }
        , fixedRate { fixedRate }
    {
    }

    double getValuation() override
    {
        // Implementa la lógica de valoración del swap
        double receiverValue = receiverLeg->computePrice();
        double payerValue = payerLeg->computePrice();
        return receiverValue - payerValue; // Valor neto del swap
    }

    double
    computeDiscountFactor(std::map<double, double>& boostrapingCurve) override
    {
        // annuity es el tipo swap de S(t0, ti)
        auto s = fixedRate;
        auto b = periods.back() - periods.end()[-2];

        double suma = 0;
        for (auto it : boostrapingCurve) {
            suma += b * it.second;
        }
        auto result = (1 - s * suma) / (1 + s * b);
        return result;
    }
};

#endif
