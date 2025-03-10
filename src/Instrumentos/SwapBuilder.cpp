#include "SwapBuilder.hpp"

#include <CalculadoraFechas/Calculator.hpp>
#include <Swap.hpp>

// Registration magic.
namespace register_swap {
FactoryRegistrator<SwapBuilder> registration;
}

InstrumentDescription::Type SwapBuilder::getId()
{
    return InstrumentDescription::Type::swap;
}

std::unique_ptr<IPricer> SwapBuilder::build(const InstrumentDescription& instrument)
{
    std::unique_ptr<IPricer> swap;
    std::vector<double> paymentPeriods;
    std::shared_ptr<LegDescription> receiver;
    std::shared_ptr<LegDescription> payer;
    double fixedRate;

    // Convertir el vector de periodos de string a doubles
    if (instrument.instrumentDateConvention == InstrumentDescription::DateConvention::typeActual360) {
        _generateInstrumentPaymentPeriods(Actual_360(),
            paymentPeriods,
            instrument.instrumentPaymentPeriods,
            instrument.instrumentStartDate);
    } else if (instrument.instrumentDateConvention == InstrumentDescription::DateConvention::typeThirty360) {
        _generateInstrumentPaymentPeriods(Thirty_360(),
            paymentPeriods,
            instrument.instrumentPaymentPeriods,
            instrument.instrumentStartDate);
    }

    //  Computar curva cupón cero
    auto zerocouponCurve = std::make_shared<ZerocouponCurve>(
        paymentPeriods, instrument.instrumentZeroRates);

    // Computar la pata recibidora
    if (instrument.swapType == InstrumentDescription::SwapType::FixedFloating) {
        // La pata recibidora es fija
        auto periodicPayment = instrument.instrumentReceivingRate * instrument.instrumentNominal;
        std::map<double, double> cashFlows;
        for (auto period : paymentPeriods) {
            cashFlows[period] = periodicPayment * zerocouponCurve->dateCountFractions[period];
        }
        fixedRate = instrument.instrumentReceivingRate;
        receiver = std::make_shared<FixedLegDescription>(
            paymentPeriods, cashFlows, zerocouponCurve);

    } else {
        // La pata recibidora es flotante
        auto index = std::make_shared<Index>(instrument.instrumentReceivingIndex);
        receiver = std::make_shared<FloatingLegDescription>(
            index, paymentPeriods, zerocouponCurve,
            instrument.instrumentNominal);
    }
    // Computar pata pagadora
    if (instrument.swapType == InstrumentDescription::SwapType::FloatingFixed) {
        // Pata pagadora es fija
        auto periodicPayment
            = instrument.instrumentPayingRate * instrument.instrumentNominal;
        std::map<double, double> cashFlows;
        for (auto period : paymentPeriods) {
            cashFlows[period] = periodicPayment * zerocouponCurve->dateCountFractions[period];
        }
        fixedRate = instrument.instrumentPayingRate;
        payer = std::make_shared<FixedLegDescription>(paymentPeriods, cashFlows, zerocouponCurve);
    } else {
        // Pata pagadora es flotante
        auto index = std::make_shared<Index>(std::move(instrument.instrumentPayingIndex));
        payer = std::make_shared<FloatingLegDescription>(
            index, paymentPeriods, zerocouponCurve,
            instrument.instrumentNominal);
    }
    swap = std::move(std::make_unique<Swap>(receiver, payer, zerocouponCurve, paymentPeriods, fixedRate));
    return swap;
}

/*
        Computar los periodos como doubles manteniendo el valor entre cada día
    de un vector y una referencia contractStartDate. Usar el método Actual360 o
   Thirty360 dado por el conversor.
*/
template <class Convention>
void SwapBuilder::_generateInstrumentPaymentPeriods(
    Convention conversor, std::vector<double>& _instrumentPaymentPeriods,
    const std::vector<std::string>& contractPaymentPeriods_,
    const std::string& contractStartDate)
{
    _instrumentPaymentPeriods.clear();
    for (auto period : contractPaymentPeriods_) {
        double periodYearFraction = conversor(contractStartDate, period);
        _instrumentPaymentPeriods.push_back(periodYearFraction);
    }
}
