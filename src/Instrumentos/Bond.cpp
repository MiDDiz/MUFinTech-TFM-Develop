#include "Bond.hpp"

double Bond::getValuation()
{
	double calculatedPrice = 0;
	for (auto period : receiverLeg->dateVector)
	{
		auto discountRate = curve->get_dcf(period);
		calculatedPrice = calculatedPrice + coupon_ * discountRate;
		// std::cout << "Discounting " << coupon_ << " for period " << period << " = " << coupon_ * discountRate << std::endl;
	}
	calculatedPrice = calculatedPrice + notional_ * curve->get_dcf(maturity_);
	// std::cout << "Calculated bond price is : " <<  calculatedPrice << std::endl;
	return (calculatedPrice);
}