#define BOOST_TEST_MODULE test_fechas
#include "Calculator.hpp"
#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <string>

BOOST_AUTO_TEST_SUITE(practica1)

BOOST_AUTO_TEST_CASE(dias_anyo_actual)
{
    BOOST_TEST_MESSAGE("Calcular días de un año con actual/360");
    std::string from = "2024-01-01";
    std::string to = "2025-01-01";
    short days = Actual_360::compute_daycount(from, to);
    // Actual/360 cuenta los dias del año, 2024 es bisiesto
    BOOST_TEST(days == 366);
}

BOOST_AUTO_TEST_CASE(dias_anyo_thirty)
{
    BOOST_TEST_MESSAGE("Calcular días de un año con 30/360");
    std::string from = "2024-01-01";
    std::string to = "2025-01-01";
    short days = Thirty_360::compute_daycount(from, to);
    // Thirty/360 asume que un mes siempre tiene 30 dias, un año → 360
    BOOST_TEST(days == 360);
}

BOOST_AUTO_TEST_CASE(interval_actual)
{
    BOOST_TEST_MESSAGE("Calcular dias entre 2 fechas con 30/360");
    std::string from = "2025-02-01";
    std::string to = "2025-03-01";
    short days = Actual_360::compute_daycount(from, to);
    BOOST_TEST(days == 28);
}

BOOST_AUTO_TEST_CASE(interval_thrity)
{
    BOOST_TEST_MESSAGE("Calcular dias entre 2 fechas con 30/360");
    std::string from = "2025-02-01";
    std::string to = "2025-03-01";
    short days = Thirty_360::compute_daycount(from, to);
    BOOST_TEST(days == 30);
}

BOOST_AUTO_TEST_SUITE_END()
