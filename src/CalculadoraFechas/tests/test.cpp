#define BOOST_TEST_MODULE test_fechas
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(practica1)

BOOST_AUTO_TEST_CASE(dias_anyo_actual) {
  BOOST_TEST_MESSAGE("Calcular días de un año con actual/360");
}

BOOST_AUTO_TEST_CASE(dias_anyo_thirty) {
  BOOST_TEST_MESSAGE("Calcular días de un año con 30/360");
}

BOOST_AUTO_TEST_CASE(interval_actual) {
  BOOST_TEST_MESSAGE("Calcular dias entre 2 fechas con 30/360");
}

BOOST_AUTO_TEST_CASE(interval_thrity) {
  BOOST_TEST_MESSAGE("Calcular dias entre 2 fechas con 30/360");
}

BOOST_AUTO_TEST_SUITE_END()
