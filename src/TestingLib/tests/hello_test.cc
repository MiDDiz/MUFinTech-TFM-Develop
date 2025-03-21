#include <gtest/gtest.h>
#include <TestingLib/foo.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions)
{
	// Expect two strings not to be equal.
	EXPECT_STRNE("hello", "world");
	// Expect equality.
	EXPECT_EQ(7 * 6, 42);
}

TEST(TestAddNE, BasicAssertions)
{
	// Expect two strings not to be equal.
	double f = 0.1;
	ASSERT_NEAR(add(f, 10), f * 10, 0.000001);
}
