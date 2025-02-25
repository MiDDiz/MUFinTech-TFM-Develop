#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <boost/date_time.hpp>
#include <string>

class DayCountCalculator
{
public:
	static boost::gregorian::date make_date(const std::string &date);
};

class Actual_360 : public DayCountCalculator
{
public:
	static short compute_daycount(const std::string &from, const std::string &to);
	static short compute_daycount(const boost::gregorian::date &from,
								  const boost::gregorian::date &to);
	template <class DATE>
	double operator()(const DATE &start, const DATE &end) const
	{
		return compute_daycount(start, end) / 360.0;
	}
};

class Thirty_360 : public DayCountCalculator
{
public:
	static short compute_daycount(const std::string &from, const std::string &to);
	static short compute_daycount(const boost::gregorian::date &from,
								  const boost::gregorian::date &to);
	static short compute_daycount(const short years, const short months,
								  const short days_from, const short days_to);
	template <class DATE>
	double operator()(const DATE &start, const DATE &end) const
	{
		return compute_daycount(start, end) / 360.0;
	}
};
#endif