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
    ASSERT_EQ(BigInteger().toString(), "0");
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


TEST(BigInteger, Add) {
    auto bi = BigInteger(1) + BigInteger("100000000000000000000");
    ASSERT_EQ(bi.toString(), "100000000000000000001");

    auto bi2 = bi + BigInteger("-99999999999999999999");
    ASSERT_EQ(bi2.value<int>(), 2);

    bi += BigInteger("-100000000000000000002");
    ASSERT_EQ(bi.value<int>(), -1);

    bi += 59;
    ASSERT_EQ(bi.value<int>(), 58);

    ASSERT_EQ(bi, BigInteger(-10) + 68);

}

TEST(BigInteger, Subtract) {
    auto bi = BigInteger(std::numeric_limits<int64_t>::max()) - BigInteger(std::numeric_limits<int64_t>::max());
    ASSERT_EQ(bi.value<int>(), 0);

    bi -= -1000;
    ASSERT_EQ(bi.value<int>(), 1000);

    bi -= BigInteger("9999999999999998999");

    ASSERT_EQ(bi.toString(), "-9999999999999997999");

    ASSERT_EQ(BigInteger(-50), BigInteger(100) - 150);
}

TEST(BigInteger, Multiply) {
    auto bi = BigInteger(-1) * std::numeric_limits<int64_t>::min();
    ASSERT_GT(bi, 0);
    ASSERT_EQ(12344 * BigInteger("200000000000000"), BigInteger("2468800000000000000"));

    auto bi2 = BigInteger(55);
    bi2 *= 4;
    ASSERT_EQ(bi2, 55 * 4);
}

TEST(BigInteger, Divide) {
    auto bi = BigInteger(7999) / BigInteger(1000);
    ASSERT_EQ(bi, 7);
    auto bi2 = BigInteger("3345464456576867887943425467865434247657999354657687987987654235345")
               / BigInteger("123346576786546566787654356576876865");
    ASSERT_EQ(bi2, BigInteger("27122475092002377281597876586575"));


    bi /= -2;
    ASSERT_EQ(bi, -3);

    ASSERT_THROW({ BigInteger(234345) / 0; }, std::invalid_argument);
    ASSERT_THROW({ BigInteger(234345) / BigInteger(0); }, std::invalid_argument);

}

TEST(BigInteger, Modulus) {
    auto bi = BigInteger(7999) % BigInteger(1000);
    ASSERT_EQ(bi, 999);

    bi = BigInteger(12345) % -17;
    ASSERT_EQ(bi, 12345 % -17);

    bi = -7777 % BigInteger(12);
    ASSERT_EQ(bi, -7777 % 12);

    bi = BigInteger(-4564);
    bi %= BigInteger(-77);
    ASSERT_EQ(bi, -4564 % -77);
}

TEST(BigInteger, RealDivide) {
    ASSERT_NEAR(BigInteger(1999).realDivide(1000), 1999.0 / 1000, 1e-20);
}

TEST(BigInteger, abs) {
    auto bi = BigInteger(std::numeric_limits<int64_t>::min()).abs();
    ASSERT_THROW({ bi.value<int64_t>(); }, BigInteger::OverflowException);

    ASSERT_EQ(BigInteger(-234).abs(), 234);
    BigInteger b2(1234);
    ASSERT_EQ(b2, b2.abs());
}

TEST(BigInteger, negate) {
    auto bi = -BigInteger(1236);
    ASSERT_EQ(bi, -1236);

    bi = -BigInteger(-4906);
    ASSERT_EQ(bi, 4906);
}