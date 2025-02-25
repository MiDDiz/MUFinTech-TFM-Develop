#ifndef ZEROCOUPONCURVE_H
#define ZEROCOUPONCURVE_H

#include <vector>
#include <cmath>
#include "CalculadoraFechas/Calculator.hpp"

class ZerocouponCurve
{
private:
	/* data */
	std::vector<DayCountCalculator> periods;
	std::vector<double> rates;
	// std::vector<double> discountFactors;
	// std::vector<double> zeroCoupons;

public:
	ZerocouponCurve(std::vector<DayCountCalculator> periods, std::vector<double> rates) : periods{periods}, rates{rates} {}
	ZerocouponCurve() : periods{}, rates{} {};
	~ZerocouponCurve() = default;

	// Get zero coupon
	double get_zc(DayCountCalculator period);
	// Get discount Factor
	double get_dcf(DayCountCalculator period);
};

#endif