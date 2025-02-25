#ifndef LEGDESCRIPTION_H
#define LEGDESCRIPTION_H

#include <vector>
#include "CalculadoraFechas/Calculator.hpp"
class LegDescription
{
public:												// No entran DayCountCalculator entra Thirty_360 o Actual_360
	LegDescription(std::vector<double> priceVector, 
		std::vector<DayCountCalculator> inputDateVector)
		: priceVector{priceVector} {

			std::vector<double> periods(inputDateVector.size());

			for (auto iter : inputDateVector){
				auto annualFraction = (inputDateVector.front()) // esto nos da 30 31 o lo que sea
				periods[indice_que_toca] = annualFraction;
			}

			dateVector = periods;
		}
	LegDescription() = default;
	std::vector<double> priceVector;
	std::vector<double> dateVector;

	// ~LegDescription() = default;
private:
};
#endif