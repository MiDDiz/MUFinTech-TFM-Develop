#ifndef ZEROCOUPONCURVE_H
#define ZEROCOUPONCURVE_H

#include <vector>
#include <cmath>

class ZerocouponCurve
{
private:
	/* data */
	std::vector<double> periods;
	std::vector<double> rates;
	// std::vector<double> discountFactors;
	// std::vector<double> zeroCoupons;

public:
	ZerocouponCurve(std::vector<double> periods, std::vector<double> rates) : periods{periods}, rates{rates} {}
	ZerocouponCurve() : periods{}, rates{} {};
	~ZerocouponCurve() = default;

	// Get zero coupon
	double get_zc(double period);
	// Get discount Factor
	double get_dcf(double period);
};

#endif