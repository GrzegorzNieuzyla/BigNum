#include <gtest/gtest.h>
#include <cstdint>
#include "../src/BigInteger.h"

using namespace BigNum;

TEST(BigInteger, DefaultConstructor) {
    BigInteger bi;
    ASSERT_EQ(0, bi.value<int>());
}

TEST(BigInteger, IntConstructor) {
    int value = 2134;
    BigInteger bi(value);
    ASSERT_EQ(value, bi.value<decltype(value)>());

    value = -112;
    BigInteger bi2(value);
    ASSERT_EQ(value, bi2.value<decltype(value)>());

    int8_t value2 = -128;
    BigInteger bi3(value2);
    ASSERT_EQ(value2, bi3.value<decltype(value)>());
}

TEST(BigInteger, FromConstructor) {
    uint64_t value = std::numeric_limits<uint64_t>::max();
    BigInteger bi = BigInteger::from<decltype(value)>(value);
    ASSERT_EQ(value, bi.value<decltype(value)>());
}


TEST(BigInteger, StringConstructor) {
    int value = 1345434;
    BigInteger bi(std::to_string(value));
    ASSERT_EQ(bi.value<int>(), value);

    int value2 = -770;
    BigInteger bi2(std::to_string(value2));
    ASSERT_EQ(bi2.value<int>(), value2);
}


TEST(BigInteger, ToString) {
    int value = 981234;
    BigInteger bi(value);
    ASSERT_EQ(std::to_string(value), bi.toString());

    int value2 = -21345;
    BigInteger bi2(value2);
    ASSERT_EQ(std::to_string(value2), bi2.toString());

}


TEST(BigInteger, OverflowError) {

    BigInteger bi(128);
    ASSERT_THROW({
                     bi.value<int8_t>();
                 }, BigInteger::OverflowException);
    BigInteger bi2(-128);
    ASSERT_NO_THROW({
                        bi2.value<int8_t>();
                    });
    BigInteger bi3(22222);
    ASSERT_THROW({
                     bi.value<int8_t>();
                 }, BigInteger::OverflowException);

}

TEST(BigInteger, SignError) {
    BigInteger bi(-2);
    ASSERT_THROW({
                     bi.value<uint8_t>();
                 }, BigInteger::SignException);
}


TEST(BigInteger, CompareGreaterLessOrEqual) {
    ASSERT_TRUE(BigInteger(12) >= BigInteger(12));
    ASSERT_FALSE(BigInteger(-122) >= BigInteger(12));
    ASSERT_FALSE(BigInteger(122) >= BigInteger(222));

    ASSERT_TRUE(BigInteger(14) <= BigInteger(14));
    ASSERT_FALSE(BigInteger(144) <= BigInteger(-444));
    ASSERT_FALSE(BigInteger(144) <= BigInteger(14));

    ASSERT_TRUE(BigInteger(12) >= 12);
    ASSERT_FALSE(BigInteger(-122) >= 12);
    ASSERT_FALSE(BigInteger(122) >= 222);

    ASSERT_TRUE(BigInteger(14) <= 14);
    ASSERT_FALSE(BigInteger(144) <= -444);
    ASSERT_FALSE(BigInteger(144) <= 14);

    ASSERT_TRUE(BigInteger("9223372036854775809") >= BigInteger(std::numeric_limits<int64_t>::max()));
    ASSERT_TRUE(BigInteger("-9223372036854775810") <= BigInteger(std::numeric_limits<int64_t>::min()));

}

TEST(BigInteger, CompareEquality) {
    ASSERT_TRUE(BigInteger(12) == BigInteger(12));
    ASSERT_TRUE(BigInteger(122) != BigInteger(12));
    ASSERT_FALSE(BigInteger(-122) == BigInteger(122));
    ASSERT_TRUE(BigInteger(14) != BigInteger(17));

    ASSERT_TRUE(BigInteger(12) == 12);
    ASSERT_TRUE(BigInteger(122) != 12);
    ASSERT_FALSE(BigInteger(-122) == 122);
    ASSERT_TRUE(BigInteger(14) != 17);
}


TEST(BigInteger, CompareGreaterLess) {
    ASSERT_FALSE(BigInteger(12) > BigInteger(12));
    ASSERT_FALSE(BigInteger(-122) > BigInteger(12));
    ASSERT_TRUE(BigInteger(122) < BigInteger(222));
    ASSERT_TRUE(BigInteger(-122) < BigInteger(22));

    ASSERT_FALSE(BigInteger(12) > 12);
    ASSERT_FALSE(BigInteger(-122) > 12);
    ASSERT_TRUE(BigInteger(122) < 222);
    ASSERT_TRUE(BigInteger(-122) < 22);


    ASSERT_FALSE(BigInteger(-122) < -122);
    ASSERT_FALSE(BigInteger(-122) > -122);

    ASSERT_TRUE(BigInteger("9223372036854775809") > BigInteger(std::numeric_limits<int64_t>::max()));
    ASSERT_TRUE(BigInteger("-9223372036854775810") < BigInteger(std::numeric_limits<int64_t>::min()));

}


