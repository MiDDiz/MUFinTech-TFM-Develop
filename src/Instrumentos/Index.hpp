#ifndef INDEX_H
#define INDEX_H

#include "ZerocouponCurve.hpp"
#include <boost/format.hpp>
#include <memory>

class Index {
    // Generador de interés variable.
    // Responsable de Calcular tipos forward
    // Responsable de Calcular tipos continuamente compuestos a tipos
    // intermedios.

private:
    int annualFrequency;
    std::vector<double> dateCountFractions;

public:
    // La curva cero cupon no puede entrar a nivel de constructor.
    Index() = default;
    Index(Index&& idx) = default;
    Index(Index const& idx) = default;
    Index& operator=(Index&& idx) = default;
    Index& operator=(Index& idx) = default;
    Index(double firstRate_, int annualFrequency_)
        : firstRate { firstRate_ }
        , annualFrequency { annualFrequency_ } { };

    double get_rate(double period)
    {
        return (rates[period]);
    }

    void
    generateRates(const std::shared_ptr<ZerocouponCurve>& zeroCouponCurve)
    {
        // Este método implementa la lógica para:
        // 1. Hacer los cómputos del forward para la pata flotante
        // 2. Convertir los tipos continuamente compuestos en intereses que
        // podemos usar
        rates.clear();
        for (auto it = zeroCouponCurve->zeroRates.begin();
            it != zeroCouponCurve->zeroRates.end(); ++it) {
            if (it == zeroCouponCurve->zeroRates.begin()) {
                rates[it->first]
                    = annualFrequency * (std::exp(firstRate / 2) - 1);
            } else {
                auto computedForward = computeForward(it->second, it->first,
                    std::prev(it)->second,
                    std::prev(it)->first);
                rates[it->first]
                    = annualFrequency
                    * (std::exp(computedForward / annualFrequency) - 1);
            }
        }
    }

    double computeForward(double zeroCuponI, double periodI, double zeroCuponBefore,
        double periodBefore)
    {
        return (zeroCuponI * periodI - zeroCuponBefore * periodBefore)
            / (periodI - periodBefore);
    }
    std::map<double, double> rates;
    double firstRate;
};

#endif
