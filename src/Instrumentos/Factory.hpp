#ifndef FACTORY_H
#define FACTORY_H

#include "IPricer.hpp"
#include "LegDescription.hpp"
#include "ZerocouponCurve.hpp"
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>

struct InstrumentDescription
{
	/* Aqui se tiene que introducir qué tipo de instrumento financiero queremos
	 * obtener. Y las patas indican los flujos de caja. */
	enum Type
	{
		bond,
		swap
	};
	// Necesitamos que del instrumento, se defina cómo se trabajarán las fechas interamente.
	enum DateType
	{
		typeActual360,
		type30360
	};
	Type type;
	DateType dateType;
	InstrumentDescription(Type type_) : type(type_) {}
	LegDescription payer;
	// Let's assume receiver as the coupon bond definition -> Entonces payer tiene
	// que estar vacio cuando Bond.
	LegDescription receiver;
	//...
	ZerocouponCurve zerocouponCurve;
};

class Factory
{
public:
	std::unique_ptr<IPricer>
	operator()(const InstrumentDescription &description) const;
	virtual ~Factory();
	typedef std::function<std::unique_ptr<IPricer>(const InstrumentDescription &)>
		Builder;

	template <typename BuilderClass>
	void register_constructor(const InstrumentDescription::Type &id,
							  const BuilderClass &builder);
	// singleton implementation ....
	static Factory &instance()
	{
		static Factory factory;
		return factory;
	}

private:
	Factory();
	InstrumentDescription::Type
	getBuilderId(const InstrumentDescription &description) const;
	std::map<InstrumentDescription::Type, Builder> buildersMap_;
};

template <typename BuilderClass>
class FactoryRegistrator
{
public:
	FactoryRegistrator()
	{
		Factory::instance().register_constructor(BuilderClass::getId(),
												 &BuilderClass::build);
	}
};

template <typename BuilderClass>
void Factory::register_constructor(const InstrumentDescription::Type &id,
								   const BuilderClass &builder)
{
	buildersMap_.insert(std::make_pair(id, builder));
}

#endif
