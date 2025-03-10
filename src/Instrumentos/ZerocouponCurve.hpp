#ifndef ZEROCOUPONCURVE_H
#define ZEROCOUPONCURVE_H

#include "boost/format/format_fwd.hpp"
#include <boost/format.hpp>
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <iterator>
#include <map>
#include <vector>

class ZerocouponCurve {
private:
    void _generateDiscountFactors(const std::vector<double>& periods,
        const std::vector<double>& rates)
    {
        for (size_t i = 0; i < periods.size(); ++i) {
            discountFactors[periods[i]] = std::exp(-rates[i] * periods[i]);
        }
    }
    void _generateZeroRates(const std::vector<double>& periods,
        const std::vector<double>& rates)
    {
        for (size_t i = 0; i < periods.size(); ++i) {
            zeroRates[periods[i]] = rates[i];
        }
    }
    void _generateZeroCoupons(const std::vector<double>& periods,
        const std::vector<double>& rates)
    {
        for (size_t i = 0; i < periods.size(); ++i) {
            zeroCoupouns[periods[i]] = std::exp(rates[i] * periods[i]);
        }
    }
    void _generateDateCountFractions(std::map<double, double>& zeroCoup)
    {
        for (auto it = zeroCoup.begin(); it != zeroCoup.end(); ++it) {
            if (it == zeroCoup.begin()) {
                dateCountFractions[it->first] = it->first;
            } else {
                dateCountFractions[it->first] = it->first - std::prev(it)->first;
            }
        }
    }

public:
    std::map<double, double> zeroCoupouns;
    std::map<double, double> zeroRates;
    std::map<double, double> discountFactors;
    std::map<double, double> dateCountFractions;

    ZerocouponCurve(const std::vector<double>& periods,
        const std::vector<double>& rates)
    {
        _generateZeroRates(periods, rates);
        _generateZeroCoupons(periods, rates);
        _generateDiscountFactors(periods, rates);
        _generateDateCountFractions(zeroCoupouns);
    }
    ZerocouponCurve() = delete;
    ~ZerocouponCurve() = default;

    // Get zero coupon
    double get_zc(double period);
    // Get discount factor
    double get_dcf(double period);
};

#endif
