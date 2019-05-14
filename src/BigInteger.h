#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <sstream>

namespace BigNum {
    class BigInteger {
    public:
        explicit BigInteger(std::string str);

        class OverflowException : public std::invalid_argument {
        public:
            explicit OverflowException(char const *message) noexcept;

            char const *what() const noexcept override;
        };

        class SignException : public std::invalid_argument {
        public:
            explicit SignException(char const *message) noexcept;

            char const *what() const noexcept override;
        };

        BigInteger() = default;

        BigInteger(const BigInteger &oth) = default;

        BigInteger(BigInteger &&oth) = default;

        BigInteger &operator=(const BigInteger &oth) = default;

        BigInteger &operator=(BigInteger &&oth) = default;

        explicit BigInteger(int64_t val);

        template<typename T>
        T value();

        std::string toString() const;

        bool operator>(const BigInteger &oth) const;

        bool operator<(const BigInteger &oth) const;

        bool operator>=(const BigInteger &oth) const;

        bool operator<=(const BigInteger &oth) const;

        bool operator!=(const BigInteger &oth) const;

        bool operator==(const BigInteger &oth) const;

        bool operator>(int64_t val) const;

        bool operator<(int64_t val) const;

        bool operator>=(int64_t val) const;

        bool operator<=(int64_t val) const;

        bool operator!=(int64_t val) const;

        bool operator==(int64_t val) const;

        BigInteger operator+(const BigInteger &oth) const;

        BigInteger operator-(const BigInteger &oth) const;

        BigInteger operator*(const BigInteger &oth) const;

        BigInteger operator/(const BigInteger &oth) const;

        BigInteger operator%(const BigInteger &oth) const;

        BigInteger operator+(int64_t val) const;

        BigInteger operator-(int64_t val) const;

        BigInteger operator*(int64_t val) const;

        BigInteger operator/(int64_t val) const;

        BigInteger operator%(int64_t val) const;

        BigInteger abs() const;

        BigInteger operator-() const;

        BigInteger &operator+=(const BigInteger &oth);

        BigInteger &operator-=(const BigInteger &oth);

        BigInteger &operator*=(const BigInteger &oth);

        BigInteger &operator/=(const BigInteger &oth);

        BigInteger &operator%=(const BigInteger &oth);

        BigInteger &operator+=(int64_t val);

        BigInteger &operator-=(int64_t val);

        BigInteger &operator*=(int64_t val);

        BigInteger &operator/=(int64_t val);

        BigInteger &operator%=(int64_t val);

        double realDivide(uint64_t) const;

        double realDivide(const BigInteger &oth) const;

        friend BigInteger operator+(int64_t val, const BigInteger &bi);

        friend BigInteger operator-(int64_t val, const BigInteger &bi);

        friend BigInteger operator*(int64_t val, const BigInteger &bi);

        friend BigInteger operator/(int64_t val, const BigInteger &bi);

        friend BigInteger operator%(int64_t val, const BigInteger &bi);

        template<typename T>
        static BigInteger from(T val);

    private:
        int sign = 1;
        std::vector<int8_t> digits;

        void trimZeros();

        void multiplyByFactorOf10(int factor);

        void divideBy2();

        double toDouble() const;
    };

    BigInteger operator+(int64_t val, const BigInteger &bi);

    BigInteger operator-(int64_t val, const BigInteger &bi);

    BigInteger operator*(int64_t val, const BigInteger &bi);

    BigInteger operator/(int64_t val, const BigInteger &bi);

    BigInteger operator%(int64_t val, const BigInteger &bi);

}
