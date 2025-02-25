#include "foo.h"

double add(double f, int total)
{
	double sum = 0.0;
	for (int i = 0; i < total; ++i)
	{
		sum += f;
	}
	return sum;
}

double multiply(double f, int total)
{
	double sum = 0.0;
	for (int i = 0; i < total; ++i)
	{
		sum += f;
	}
	return sum;
}

double substract(double f, int total)
{
	double sum = 0.0;
	for (int i = 0; i < total; ++i)
	{
		sum += f;
	}
	return sum;
}

double leak()
{
	// OK
	int *p = new int;
	delete p;

	// Memory leak
	int *q = new int;
	// no delete

	return (-1);
}