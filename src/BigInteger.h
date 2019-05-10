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

        explicit BigInteger(int64_t val);

        template<typename T>
        T value();

        std::string toString();

        bool operator>(const BigInteger &oth);

        bool operator<(const BigInteger &oth);

        bool operator>=(const BigInteger &oth);

        bool operator<=(const BigInteger &oth);

        bool operator!=(const BigInteger &oth);

        bool operator==(const BigInteger &oth);

        bool operator>(int64_t val);

        bool operator<(int64_t val);

        bool operator>=(int64_t val);

        bool operator<=(int64_t val);

        bool operator!=(int64_t val);

        bool operator==(int64_t val);


        template<typename T>
        static BigInteger from(T val);

    private:
        int sign = 1;
        std::vector<uint8_t> digits;
    };

}
