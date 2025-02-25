#ifndef BOND_H
#define BOND_H

#include "LegDescription.hpp"
#include "ZerocouponCurve.hpp"
#include "IPricer.hpp"

class Bond : public IPricer
{
private:
	std::shared_ptr<LegDescription> receiverLeg;
	std::shared_ptr<ZerocouponCurve> curve;

	double notional_ = 0;
	DayCountCalculator maturity_;
	double coupon_ = 0;
	double frequency_ = 0;
	double price_ = 0;

	// double annualizedCoupon_ = 0;
	// std::vector<double> couponDates_;
	// std::shared_ptr<YieldCurve> yieldCurve_;

public:
	// Maturity is the last date of the contract so we make it the last date that the receiver leg is paying us.
	Bond(std::shared_ptr<LegDescription> receiver, std::shared_ptr<ZerocouponCurve> curve)
		: receiverLeg{receiver}, curve{curve}, price_{getValuation()}, maturity_{receiver->dateVector.back()} {}
	~Bond() = default;

	double getValuation() override;

	// Getters for member attributes
	DayCountCalculator getMaturity() const { return maturity_; }
	double getCoupon() const { return coupon_; }
	double getFrequency() const { return frequency_; }
	double getNotional() const { return notional_; }
	double getPrice() const { return price_; }
};
#endif