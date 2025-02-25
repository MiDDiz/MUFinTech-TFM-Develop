#include "SwapBuilder.hpp"

// Registration magic.
namespace register_swap {
	FactoryRegistrator<SwapBuilder> registration;
}

InstrumentDescription::Type SwapBuilder::getId() {
  return InstrumentDescription::Type::swap;
}

std::unique_ptr<IPricer>
SwapBuilder::build(const InstrumentDescription &instrument) {
  //...
  std::unique_ptr<IPricer> swap;
  swap = std::move(std::make_unique<Swap>(instrument.receiver, instrument.payer, instrument.zerocouponCurve));
  return swap;
}
