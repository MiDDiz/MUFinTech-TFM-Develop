#include "BondBuilder.hpp"
#include "Bond.hpp"
#include "FixedLegDescription.hpp"

namespace register_bond {
FactoryRegistrator<BondBuilder> registration;
}

InstrumentDescription::Type BondBuilder::getId()
{
    return InstrumentDescription::Type::bond;
}

std::unique_ptr<IPricer> BondBuilder::build(const InstrumentDescription& instrument)
{

    // Calcular el cupon periodico a partir del rate anualizado
    double periodicCoupon = instrument.instrumentAnnualInterestRate
        * instrument.instrumentNominal
        * instrument.instrumentPaymentFrequency;
    // Computar los periodos de pago.
    std::vector<double> paymentPeriods;
    for (double i = instrument.instrumentPaymentFrequency;
        i <= instrument.instrumentMaturity;
        i += instrument.instrumentPaymentFrequency) {
        paymentPeriods.push_back(i);
    }

    // Calcular los flujos de caja sin llevarnoslos al presente.
    std::map<double, double> cashFlows;
    for (auto period : paymentPeriods) {
        cashFlows[period] = periodicCoupon;
    }
    cashFlows[paymentPeriods.back()] += instrument.instrumentNominal;
    // En la pata nos los llevamos al presente.
    auto zerocouponCurve = std::make_shared<ZerocouponCurve>(
        paymentPeriods, instrument.instrumentZeroRates);
    auto receiver = std::make_shared<FixedLegDescription>(
        paymentPeriods, cashFlows, zerocouponCurve);

    return (std::move(std::make_unique<Bond>(receiver, zerocouponCurve)));
}
