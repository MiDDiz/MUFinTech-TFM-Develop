#ifndef BOND_H
#define BOND_H

#include "FixedLegDescription.hpp"
#include "IPricer.hpp"
#include "ZerocouponCurve.hpp"
#include <boost/math/tools/roots.hpp>
#include <cmath>

class Bond : public IPricer {
private:
    std::shared_ptr<FixedLegDescription> receiverLeg; // Bond Cash Flows
    std::shared_ptr<ZerocouponCurve> curve; // bond Periods + zero rates + dcfs..
    double marketPrice;

    double newton_raphson_compute_f(double realBondPrice)
    {
        double calculatedPrice = 0;

        for (auto it = curve->zeroCoupouns.begin();
            it != curve->zeroCoupouns.end(); ++it) {
            //  Sumatorio: C_i * e ^ (- t_i * y) --> La función de valoracion de un
            //  bono.
            calculatedPrice = calculatedPrice
                + receiverLeg->cashFlows[it->first]
                    * std::exp(-it->first * marketPrice);
        }
        // f(y) = C_i * e ^ (- t_i * y) - B
        return calculatedPrice - realBondPrice;
    }

    double newton_raphson_compute_derivate(double iteratedRate)
    {
        double calculatedDerivate = 0;
        for (auto it = curve->zeroCoupouns.begin();
            it != curve->zeroCoupouns.end(); ++it) {
            // Sumatorio: C_i * y * e ^( - t_i * y) --> La primera derivada de la
            // función de valoracion de un bono.
            calculatedDerivate = calculatedDerivate
                + receiverLeg->cashFlows[it->first] * it->first
                    * std::exp(-it->first * iteratedRate);
        }
        // f'(y) = C_i * y * e ^( - t_i * y)
        return -calculatedDerivate;
    }

    double newton_raphson(double initialRate, double tolerance, int max_iter)
    {
        double iteratedRate = initialRate;
        for (int i = 0; i < max_iter; i++) {
            double calculatedBondPrice = newton_raphson_compute_f(iteratedRate);
            double calculatedBondPriceDerivative
                = newton_raphson_compute_derivate(iteratedRate);

            if (fabs(calculatedBondPriceDerivative) < 1e-10) { // Evitar divisiones por cero
                return -1;
            }

            double r_next = iteratedRate
                - calculatedBondPrice / calculatedBondPriceDerivative;

            if (fabs(r_next - iteratedRate) < tolerance) {
                return r_next; // Convergió
            }
            iteratedRate = r_next;
        }
        return -1; // Error, no convergió en el número de iteraciones
    }
    double computeTIR(double tolerance, int max_iter)
    {
        auto nuestro = newton_raphson(0.01, tolerance, max_iter);
        return (nuestro);
    }

public:
    Bond(std::shared_ptr<FixedLegDescription> receiver,
        std::shared_ptr<ZerocouponCurve> curve)
        : receiverLeg { receiver }
        , curve { curve }
    {
    }

    double getValuation() override
    {
        marketPrice = receiverLeg->computePrice();
        return (marketPrice);
    }

    double getTIR() override
    {
        return computeTIR(0.0001, 50);
    }

    double computeDiscountFactor(std::map<double, double>& boostrapingCurve) override
    {
        // Calcular el factor de descuento para el bono
        return 0;
    }
};

#endif
