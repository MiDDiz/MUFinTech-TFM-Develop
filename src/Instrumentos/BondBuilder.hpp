#ifndef BONDBUILDER_H
#define BONDBUILDER_H

#include "Factory.hpp"

class BondBuilder {
public:
    static std::unique_ptr<IPricer> build(const InstrumentDescription& instrument);
    static InstrumentDescription::Type getId();
};
#endif
