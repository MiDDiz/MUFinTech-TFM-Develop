boost_test_project(NAME test_foo DEPS TestingLib)

enable_testing()

add_executable(
  hello_test
  hello_test.cc
)
target_link_libraries(
  hello_test
  GTest::gtest_main
)
target_link_libraries(
  hello_test
  TestingLib
)

include(GoogleTest)
gtest_discover_tests(hello_test)