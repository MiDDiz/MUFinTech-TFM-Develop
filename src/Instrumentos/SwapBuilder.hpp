#ifndef SWAPBUILDER_H
#define SWAPBUILDER_H

#include "Factory.hpp"

class SwapBuilder {
public:
    static std::unique_ptr<IPricer>
    build(const InstrumentDescription& instrument);
    static InstrumentDescription::Type getId();

    template <class Convention>
    static void _generateInstrumentPaymentPeriods(
        Convention conversor, std::vector<double>& _instrumentPaymentPeriods,
        const std::vector<std::string>& contractPaymentPeriods_,
        const std::string& contractStartDate);
};
#endif
