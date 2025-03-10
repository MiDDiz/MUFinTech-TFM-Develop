#include "ZerocouponCurve.hpp"

double ZerocouponCurve::get_zc(double period)
{
    return zeroCoupouns[period];
}

double ZerocouponCurve::get_dcf(double period)
{
    return discountFactors[period];
}
