#define BOOST_TEST_MODULE test_instrumentos
#include "Bootstrapping.hpp"
#include "Factory.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(practica1)

BOOST_AUTO_TEST_CASE(calculo_fracciones)
{
    BOOST_TEST_MESSAGE("Calculo fracciones");
    std::vector<double> periods = {
        0.5,
        1,
        1.5,
        2,
        2.5,
        3,
    };
    std::vector<double> zeroRates = {
        0.01,
        0.025,
        0.04,
        0.055,
        0.06,
        0.065,
    };
    ZerocouponCurve curve = ZerocouponCurve(periods, zeroRates);

    std::map<double, double> frac = curve.dateCountFractions;

    for (auto fraci : frac) {
        BOOST_TEST_MESSAGE("\tFracción: " << fraci.second);
    }
}

BOOST_AUTO_TEST_CASE(valoracion_bono)
{
    BOOST_TEST_MESSAGE("Valoración de un bono");

    std::vector<double> zeroRates = { 0.05, 0.058, 0.064, 0.068 };

    auto mi_descriptor_bono = InstrumentDescription(
        InstrumentDescription::Type::bond,
        InstrumentDescription::DateConvention::typeActual360,
        100,
        0.5,
        0.06,
        2,
        zeroRates);

    auto mibond = Factory::instance()(mi_descriptor_bono);

    double valuation = mibond->getValuation();

    BOOST_TEST_MESSAGE("\tBond Valuation: " << valuation);

    BOOST_CHECK_CLOSE(valuation, 98.3851, 0.001);
}

BOOST_AUTO_TEST_CASE(valoracion_swap)
{
    BOOST_TEST_MESSAGE("Valoracion de un swap");

    BOOST_TEST_MESSAGE("Cálculo de un swap");
    std::vector<double> zeroRates = {
        0.0474,
        0.05,
        0.051,
        0.052,
    };
    std::vector<std::string> fechas = { "2016-10-03", "2017-04-03", "2017-10-02", "2018-04-02" };
    Index index = Index(0.0474, 2);
    auto mi_descriptor_swap = InstrumentDescription(
        InstrumentDescription::Type::swap,
        InstrumentDescription::DateConvention::typeActual360,
        fechas,
        100,
        index,
        0.05,
        1,
        "2016-04-01",
        zeroRates);

    auto miswap = Factory::instance()(mi_descriptor_swap);

    double valuation = miswap->getValuation();

    BOOST_TEST_MESSAGE("\tSwap Valuation: " << valuation);

    BOOST_CHECK_CLOSE(valuation, 0.4957, 0.01);
}

BOOST_AUTO_TEST_CASE(valoracion_TIR)
{
    BOOST_TEST_MESSAGE("Calculo de la TIR");
    std::vector<double> zeroRates = {
        0.01,
        0.025,
        0.04,
        0.055,
        0.06,
        0.065,
    };

    auto mi_descriptor_bono = InstrumentDescription(
        InstrumentDescription::Type::bond,
        InstrumentDescription::DateConvention::typeActual360,
        1000,
        0.5,
        0.04,
        3,
        zeroRates);

    auto mibond = Factory::instance()(mi_descriptor_bono);

    mibond->getValuation();

    double TIR = mibond->getTIR();

    BOOST_TEST_MESSAGE("\tTIR Calculation: " << TIR);
}

BOOST_AUTO_TEST_CASE(calibracion_curva)
{
    BOOST_TEST_MESSAGE("Calibración de la curva con Bootstrapping");

    // Calibration start date
    std::string startDate = "2016-04-01";
    Index index(0.0474, 2);
    // Define the swap instruments
    std::vector<InstrumentDescription> swaps = {
        InstrumentDescription(
            InstrumentDescription::Type::deposit,
            InstrumentDescription::DateConvention::typeActual360,
            1000000.0,
            0.05,
            0.5,
            "2023-01-01",
            "2023-07-01",
            { 0.0474 }),
        // Swap 12m: Rate = 5.5%, Maturity = 12 months
        InstrumentDescription(
            InstrumentDescription::Type::swap,
            InstrumentDescription::DateConvention::typeActual360,
            { "2016-10-01", "2017-04-01" },
            1000000.0,
            0.055,
            index,
            1,
            "2016-04-01",
            { 0.0474, 0.05 }),
        // Swap 18m: Rate = 6%, Maturity = 18 months
        InstrumentDescription(
            InstrumentDescription::Type::swap,
            InstrumentDescription::DateConvention::typeActual360,
            { "2016-10-01", "2017-04-01", "2017-10-01" },
            1000000.0,
            0.06,
            index,
            1.5,
            "2016-04-01",
            { 0.0474, 0.05, 0.051 }),
        // Swap 2y: Rate = 6.4%, Maturity = 2 years
        InstrumentDescription(
            InstrumentDescription::Type::swap,
            InstrumentDescription::DateConvention::typeActual360,
            { "2016-10-01", "2017-04-01", "2017-10-01", "2018-04-01" },
            1000000.0,
            0.064,
            index,
            2,
            "2016-04-01",
            { 0.0474, 0.05, 0.051, 0.052 })
    };

    Bootstrapping::InstrumentsMapType instruments;
    for (const auto& swapDesc : swaps) {
        auto swap = Factory::instance()(swapDesc);
        double maturity = swapDesc.instrumentMaturity;
        instruments[maturity] = std::move(swap);
    }

    Bootstrapping bootstrapper;
    auto discountCurve = bootstrapper(instruments);

    std::map<double, double> expectedDiscountFactors = {
        { 0.5, 0.975610 }, { 1, 0.9466 }, { 1.5, 0.91351 }, { 2, 0.8798 }
    };

    for (const auto& entry : discountCurve) {
        double maturity = entry.first;
        double computedDiscountFactor = entry.second;
        double expectedDiscountFactor = expectedDiscountFactors[maturity];
    }

    for (const auto& entry : discountCurve) {
        BOOST_TEST_MESSAGE("\tMaturity: " << entry.first
                                          << " años, \tDiscount Factor: "
                                          << entry.second);
        BOOST_CHECK_CLOSE(entry.second, expectedDiscountFactors[entry.first], 0.1);
    }
}

BOOST_AUTO_TEST_SUITE_END()
