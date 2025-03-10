#ifndef LEGDESCRIPTION_H
#define LEGDESCRIPTION_H

#include <map>
#include <vector>
class LegDescription {
public:
    LegDescription(std::vector<double> priceVector,
        std::vector<double> inputDateVector)
        : priceVector { priceVector }
        , dateVector(inputDateVector.size())
    {
    }

    LegDescription() = default;
    virtual ~LegDescription() = default; // Virtual destructor

    std::vector<double> priceVector;
    std::vector<double> dateVector;
    std::map<double, double> cashFlows;
    virtual double
    computePrice()
    {
        return 0.0;
    } // Default implementation
};

#endif
