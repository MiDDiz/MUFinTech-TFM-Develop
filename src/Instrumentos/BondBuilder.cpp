#include "BondBuilder.hpp"

namespace register_bond
{
	FactoryRegistrator<BondBuilder> registration;
}
InstrumentDescription::Type BondBuilder::getId()
{
	return InstrumentDescription::Type::bond;
}

std::unique_ptr<IPricer>
BondBuilder::build(const InstrumentDescription &instrument)
{
	//...
	return (std::move(std::make_unique<Bond>(instrument.receiver, instrument.payer,
											 instrument.zerocouponCurve)));
}
