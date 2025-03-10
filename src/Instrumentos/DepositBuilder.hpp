#ifndef DEPOSITBUILDER_H
#define DEPOSITBUILDER_H

#include "Deposit.hpp"
#include "Factory.hpp"
#include "ZerocouponCurve.hpp"

class DepositBuilder {
public:
    static std::unique_ptr<IPricer> build(const InstrumentDescription& instrument);
    static InstrumentDescription::Type getId();

    template <class Convention>
    static void _generateInstrumentPaymentPeriods(Convention conversor,
        std::vector<double>& _instrumentPaymentPeriods,
        const std::vector<std::string>& contractPaymentPeriods_,
        const std::string& contractStartDate);
};

#endif
