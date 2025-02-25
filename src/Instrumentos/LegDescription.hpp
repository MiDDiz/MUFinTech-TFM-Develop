#ifndef LEGDESCRIPTION_H
#define LEGDESCRIPTION_H
class LegDescription
{
public:
	LegDescription(double price) : price_{price} {};
	LegDescription() : price_{0} {};
	// ~LegDescription() = default;
private:
	double price_ = 0;
};
#endif