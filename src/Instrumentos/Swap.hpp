#ifndef SWAP_H
#define SWAP_H

#include "LegDescription.hpp"
#include "ZerocouponCurve.hpp"
#include "IPricer.hpp"

class Swap : public IPricer
{
private:
	LegDescription receiver;
	LegDescription payer;
	ZerocouponCurve curve;

public:
	Swap(LegDescription receiver, LegDescription payer, ZerocouponCurve curve)
		: receiver{receiver}, payer{payer}, curve{curve} {}
	~Swap() = default;
};
#endif