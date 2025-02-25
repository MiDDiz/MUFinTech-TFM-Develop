#ifndef BONDBUILDER_H
#define BONDBUILDER_H

#include "Bond.hpp"
#include "Factory.hpp"
#include "ZerocouponCurve.hpp"

class BondBuilder {
public:
  static std::unique_ptr<IPricer>
  build(const InstrumentDescription &instrument);
  static InstrumentDescription::Type getId();
};
#endif