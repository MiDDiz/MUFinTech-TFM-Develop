#ifndef IPRICER_H
#define IPRICER_H

class IPricer
{
public:
	virtual double getValuation();
	IPricer &operator=(IPricer &&other) = default;
	IPricer &operator=(IPricer &other) = default;

private:
};
#endif