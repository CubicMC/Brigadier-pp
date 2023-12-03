#include <brigadier/TypeHolder.hpp>
#include <gtest/gtest.h>

using brigadier::TypeHolder;

TEST(TypeHolder, holdingTest)
{
    int value = 5;
    TypeHolder holder = value;
    TypeHolder holder2 = TypeHolder(value);

    EXPECT_TRUE(holder.is<int>());
    EXPECT_TRUE(holder2.is<int>());
    EXPECT_EQ(holder.getAs<int>(), 5);
    EXPECT_EQ(holder2.getAs<int>(), 5);
}

TEST(TypeHolder, holdingPointerTest)
{
    int value = 5;
    int *ptr = &value;
    TypeHolder holder = ptr;
    TypeHolder holder2 = TypeHolder(ptr);

    EXPECT_EQ(*static_cast<void **>(holder.get()), &value);
    EXPECT_EQ(*static_cast<void **>(holder2.get()), &value);
    EXPECT_TRUE(holder.is<int *>());
    EXPECT_TRUE(holder2.is<int *>());
    EXPECT_EQ(holder.getAs<int *>(), &value);
    EXPECT_EQ(holder2.getAs<int *>(), &value);
    EXPECT_EQ(*holder.getAs<int *>(), value);
    EXPECT_EQ(*holder2.getAs<int *>(), value);
}

TEST(TypeHolder, copyTest)
{
    int value = 5;
    TypeHolder holder = value;
    TypeHolder holderCopy = holder;
    EXPECT_EQ(holderCopy.getAs<int>(), 5);
}

TEST(TypeHolder, moveTest)
{
    int value = 5;
    TypeHolder holder = value;
    TypeHolder holder2 = std::move(holder);
    EXPECT_EQ(holder2.getAs<int>(), 5);
}
