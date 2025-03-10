#include "DepositBuilder.hpp"

#include <CalculadoraFechas/Calculator.hpp>

// Registration magic.
namespace register_deposit {
FactoryRegistrator<DepositBuilder> registration;
}

InstrumentDescription::Type DepositBuilder::getId()
{
    return InstrumentDescription::Type::deposit; // Tipo depósito
}

std::unique_ptr<IPricer>
DepositBuilder::build(const InstrumentDescription& instrument)
{
    // Extraer los parámetros específicos para el depósito de la descripción
    double depositRate = instrument.instrumentAnnualInterestRate;
    double maturity = instrument.instrumentMaturity;
    std::vector<double> paymentPeriods;

    // Generar los periodos de pago (por simplicidad, asumimos un solo pago
    // cuando llega a maturity)
    if (instrument.instrumentDateConvention == InstrumentDescription::DateConvention::typeActual360) {
        _generateInstrumentPaymentPeriods(
            Actual_360(),
            paymentPeriods,
            instrument.instrumentPaymentPeriods,
            instrument.instrumentStartDate);

    } else if (instrument.instrumentDateConvention == InstrumentDescription::DateConvention::typeThirty360) {
        _generateInstrumentPaymentPeriods(
            Thirty_360(),
            paymentPeriods,
            instrument.instrumentPaymentPeriods,
            instrument.instrumentStartDate);
    }

    auto zerocouponCurve = std::make_shared<ZerocouponCurve>(
        paymentPeriods, instrument.instrumentZeroRates);

    // Generar los flujos de caja
    std::map<double, double> cashFlows;
    cashFlows[paymentPeriods[0]] = instrument.instrumentNominal * (1.0 + depositRate);

    // Crear la pata recibidora
    auto receiver = std::make_shared<FixedLegDescription>(
        paymentPeriods, cashFlows, zerocouponCurve);

    // Crear y devolver el objeto Deposit
    return std::make_unique<Deposit>(receiver, zerocouponCurve, depositRate,
        maturity, paymentPeriods[0]);
}

template <class Convention>
void DepositBuilder::_generateInstrumentPaymentPeriods(
    Convention conversor,
    std::vector<double>& _instrumentPaymentPeriods,
    const std::vector<std::string>& contractPaymentPeriods_,
    const std::string& contractStartDate)
{
    _instrumentPaymentPeriods.clear();
    for (auto period : contractPaymentPeriods_) {
        double periodYearFraction = conversor(contractStartDate, period);
        _instrumentPaymentPeriods.push_back(periodYearFraction);
    }
}
