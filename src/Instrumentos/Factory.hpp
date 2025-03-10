#ifndef FACTORY_H
#define FACTORY_H

#include "IPricer.hpp"

#include "FixedLegDescription.hpp"
#include "FloatingLegDescription.hpp"
#include "ZerocouponCurve.hpp"
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>

class InstrumentDescription {
public:
    /* Aqui se tiene que introducir qué tipo de instrumento financiero queremos
     * obtener. Y las patas indican los flujos de caja. */
    enum Type {
        bond,
        swap,
        deposit,
    };
    // Necesitamos que del instrumento, se defina cómo se trabajarán las fechas
    // interamente, esto nos lo da el contrato financiero y hay que añadirlo al
    // instrument description.
    enum DateConvention {
        typeActual360,
        typeThirty360
    };
    enum SwapType {
        FixedFloating,
        FloatingFixed,
        FloatingFloating
    };
    InstrumentDescription() = delete;
    InstrumentDescription(Type type_);

    InstrumentDescription(
        Type type_, DateConvention contractDateConvention_, // Especificar tipo de contrato para Bond,
                                                            // para calcular el tiempo se usa el conversor de fechas.
        double contractNominal_, // nominal
        double contractPaymentFrequency_, // frecuencia
        double contractAnnualInterestRate_, // interes anual del bono
        double contractMaturity_, // maturity
        std::vector<double> contractZeroRates_ // precios zero rates
        )
        : instrumentDateConvention { contractDateConvention_ }
        , instrumentNominal { contractNominal_ }
        , instrumentPaymentFrequency { contractPaymentFrequency_ }
        , instrumentAnnualInterestRate { contractAnnualInterestRate_ }
        , instrumentMaturity { contractMaturity_ }
        , instrumentZeroRates { contractZeroRates_ }

    {
        _assertInstrumentType(type_, Type::bond);
    }

    // Constructores de swaps Hay 3:
    // [Pata Fija, Pata Variable]
    // [Pata Variable, Pata Fija]
    // [Pata Variable, Pata Variable]

    // [Pata Fija, Pata Variable]
    InstrumentDescription( // En el acuerdo se especifica:
        Type type_, // Swap definition
        DateConvention contractDateConvention_, // Date convention
        std::vector<std::string> contractPaymentPeriods_, // Las fechas en las que se van a producir
                                                          // los pagos.
        double contractNominal_, // Nominal de referencia para el calculo de flujos
        // Inherentemente, la forma sobre la que se calcularán los pagos: a
        // partir de la sobrecarga de estos parámetros IRS, CCS, CDS...
        double contractReceivingRate_, Index contractPayingValuator_,

        double contractMaturity_,
        std::string contractStartDate_, // Fecha de inicio del contrato a dónde queremos
                                        // que se lleven todos los flujos de caja.
                                        // Aparece en la grafica del ejemplo.

        std::vector<double> contractZeroRates // Necesitamos conocer los tipos cero cupón a las
                                              // fechas de los flujos.
        )
        : instrumentDateConvention { contractDateConvention_ }
        , instrumentNominal { contractNominal_ }
        , instrumentPaymentPeriods { contractPaymentPeriods_ }
        , instrumentReceivingRate { contractReceivingRate_ }
        , instrumentPayingIndex { contractPayingValuator_ }
        , instrumentMaturity { contractMaturity_ }
        , instrumentStartDate { contractStartDate_ }
        , instrumentZeroRates { contractZeroRates }
    {
        _assertInstrumentType(type_, Type::swap);
        _assertSameSizeVectors(contractPaymentPeriods_, contractZeroRates);
        swapType = SwapType::FixedFloating;
    }
    // [Pata Variable, Pata Fija]
    InstrumentDescription( // En el acuerto se especifica:
        Type type_, // Swap definition
        DateConvention contractDateConvention_, // Date convention
        std::vector<std::string> contractPaymentPeriods_, // Las fechas en las que se van a producir
                                                          // los pagos.
        double contractNominal_, // Nominal de referencia para el calculo de flujos
        // Inherentemente, la forma sobre la que se calcularán los pagos: a
        // partir de la sobrecarga de estos parámetros IRS, CCS, CDS...
        Index& contractReceivingValuator_, double contractPayingRate_,
        double contractMaturity_,
        std::string contractStartDate_, // Fecha de inicio del contrato a dónde queremos
                                        // que se lleven todos los flujos de caja.
                                        // Aparece en la grafica del ejemplo.

        std::vector<double> contractZeroRates // Necesitamos conocer los tipos cero cupón a las
                                              // fechas de los flujos.
        )
        : instrumentDateConvention { contractDateConvention_ }
        , instrumentNominal { contractNominal_ }
        , instrumentPaymentPeriods { contractPaymentPeriods_ }
        , instrumentReceivingIndex { contractReceivingValuator_ }
        , instrumentPayingRate { contractPayingRate_ }
        , instrumentMaturity { contractMaturity_ }
        , instrumentStartDate { contractStartDate_ }
        , instrumentZeroRates { contractZeroRates }
    {
        _assertInstrumentType(type_, Type::swap);
        _assertSameSizeVectors(contractPaymentPeriods_, contractZeroRates);
        swapType = SwapType::FloatingFixed;
    }
    // [Pata Variable, Pata Variable]
    InstrumentDescription( // En el acuerto se especifica:
        Type type_, // Swap definition
        DateConvention contractDateConvention_, // Date convention
        std::vector<std::string>
            contractPaymentPeriods_, // Las fechas en las que se van a producir
                                     // los pagos.
        double
            contractNominal_, // Nominal de referencia para el calculo de flujos
        // Inherentemente, la forma sobre la que se calcularán los pagos: a
        // partir de la sobrecarga de estos parámetros IRS, CCS, CDS...
        Index contractReceivingValuator_, Index contractPayingValuator_,
        //
        double contractMaturity_,
        std::string contractStartDate_, // Fecha de inicio del contrato a dónde queremos
                                        // que se lleven todos los flujos de caja.
                                        // Aparece en la grafica del ejemplo.

        std::vector<double> contractZeroRates // Necesitamos conocer los tipos cero cupón a las
                                              // fechas de los flujos.
        )
        : instrumentDateConvention { contractDateConvention_ }
        , instrumentNominal { contractNominal_ }
        , instrumentPaymentPeriods { contractPaymentPeriods_ }
        , instrumentReceivingIndex { contractReceivingValuator_ }
        , instrumentPayingIndex { contractPayingValuator_ }
        , instrumentMaturity { contractMaturity_ }
        , instrumentStartDate { contractStartDate_ }
        , instrumentZeroRates { contractZeroRates }
    {
        _assertInstrumentType(type_, Type::swap);
        _assertSameSizeVectors(contractPaymentPeriods_, contractZeroRates);
        swapType = SwapType::FloatingFloating;
    }

    InstrumentDescription(Type type_, DateConvention contractDateConvention_,
        double contractNominal_,
        double contractAnnualInterestRate_,
        double contractMaturity_,
        std::string contractStartDate_,
        std::string contractEndDate_,
        std::vector<double> contractZeroRates_)
        : instrumentType { type_ }
        , instrumentDateConvention { contractDateConvention_ }
        , instrumentNominal { contractNominal_ }
        , instrumentAnnualInterestRate { contractAnnualInterestRate_ }
        , instrumentMaturity { contractMaturity_ }
        , instrumentStartDate { contractStartDate_ }
        , instrumentPaymentPeriods { contractEndDate_ }
        , instrumentZeroRates { contractZeroRates_ }

    {
        _assertInstrumentType(type_, Type::deposit); // Ensure the type is deposit
    }

    void setInstrumentType(Type _type)
    {
        instrumentType = _type;
    }

    Type getInstrumentType() const
    {
        return instrumentType;
    }
    Type instrumentType;
    DateConvention instrumentDateConvention;
    double instrumentNominal;
    double instrumentPaymentFrequency;
    double instrumentAnnualInterestRate;
    double instrumentMaturity;
    std::vector<double> instrumentZeroRates;
    double instrumentReceivingRate;
    double instrumentPayingRate;
    Index instrumentReceivingIndex;
    Index instrumentPayingIndex;
    // Necesario para comunicar a swap builder que queremos hacer.
    SwapType swapType;
    std::string instrumentStartDate;
    std::shared_ptr<LegDescription> payer;
    std::shared_ptr<LegDescription> receiver;
    std::shared_ptr<ZerocouponCurve> zerocouponCurve;
    std::vector<std::string> instrumentPaymentPeriods;
    std::vector<double> coupon;

private:
    void _assertSameSizeVectors(std::vector<std::string>& contractPaymentPeriods_,
        std::vector<double>& contractZeroRates)
    {
        if (contractPaymentPeriods_.size() != contractZeroRates.size()) {
            throw std::invalid_argument(
                "Error specifying same number of contract variable assets.");
        }
    }
    void _assertInstrumentType(Type contractType_, Type expectedType_)
    {
        // Parsear tipo de Instrument
        if (contractType_ != expectedType_) {
            throw std::invalid_argument("Error using instument blueprint without "
                                        "explicty specifying expected type.");
        }
        setInstrumentType(contractType_);
    }
};

class Factory {
public:
    std::unique_ptr<IPricer>
    operator()(const InstrumentDescription& description) const;
    ~Factory() = default;
    typedef std::function<std::unique_ptr<IPricer>(const InstrumentDescription&)>
        Builder;
    void register_constructor(const InstrumentDescription::Type& id,
        const Builder& builder);
    // implementación de singleton
    static Factory& instance()
    {
        static Factory factory;
        return factory;
    }

private:
    Factory() = default;
    InstrumentDescription::Type getBuilderId(const InstrumentDescription& description) const;
    std::map<InstrumentDescription::Type, Builder> buildersMap_;
};

template <typename BuilderClass>
class FactoryRegistrator {
public:
    FactoryRegistrator()
    {
        Factory::instance().register_constructor(BuilderClass::getId(),
            &BuilderClass::build);
    }
};

#endif
