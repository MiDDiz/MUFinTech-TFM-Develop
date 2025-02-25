#ifndef BOND_H
#define BOND_H

#include "LegDescription.hpp"
#include "ZerocouponCurve.hpp"
#include "IPricer.hpp"

class Bond : public IPricer
{
private:
	LegDescription receiver;
	LegDescription payer;
	ZerocouponCurve curve;

public:
	Bond(LegDescription receiver, LegDescription payer, ZerocouponCurve curve)
		: receiver{receiver}, payer{payer}, curve{curve} {}
	~Bond() = default;
};
#endif