#include <milt/nurbs/nurbs.hpp>

#include <gtest/gtest.h>

TEST(NurbsPlaceholder, DoublesPositive) { EXPECT_EQ(milt::nurbs::placeholder(21), 42); }

TEST(NurbsPlaceholder, ZeroStaysZero) { EXPECT_EQ(milt::nurbs::placeholder(0), 0); }

TEST(NurbsPlaceholder, DoublesNegative) { EXPECT_EQ(milt::nurbs::placeholder(-5), -10); }
