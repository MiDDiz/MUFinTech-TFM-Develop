#include "ZerocouponCurve.hpp"

double ZerocouponCurve::get_zc(double period)
{
	auto low = std::lower_bound(periods.begin(), periods.end(), period);
	double zeroRates = rates[(low - periods.begin())];
	return exp(zeroRates * period);
}

double ZerocouponCurve::get_dcf(double period)
{
	auto low = std::lower_bound(periods.begin(), periods.end(), period);
	double zeroRates = rates[(low - periods.begin())];
	return exp(-zeroRates * period);
}