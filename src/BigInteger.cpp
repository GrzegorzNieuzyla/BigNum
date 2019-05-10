#include "BigInteger.h"
#include "helpers.h"

namespace BigNum {
    template<typename T>
    T BigInteger::value() {
        if (std::is_unsigned<T>::value && sign < 0)
            throw SignException("Casting negative value to unsigned");

        if (*this > BigInteger::from<T>(std::numeric_limits<T>::max()) ||
            *this < BigInteger::from<T>(std::numeric_limits<T>::min())) {
            auto msg = "BigInteger '" + toString() + "' cannot be represented as type: " + typeid(T).name();
            throw OverflowException(msg.c_str());
        }


        T res = 0;
        size_t pow = 1;
        for (auto digit : digits) {
            res += digit * pow;
            pow *= 10;
        }
        res *= sign;
        return res;
    }

    std::string BigInteger::toString() {
        std::stringstream res;
        if (sign < 0)
            res << '-';

        for (auto it = digits.rbegin(); it != digits.rend(); it++) {
            res << std::to_string(*it);
        }
        return res.str();
    }


    BigInteger::BigInteger(int64_t val) : BigInteger(std::to_string(val)) {}

    BigInteger::BigInteger(std::string str) {
        trim(str);
        if (str.empty())
            throw std::invalid_argument("Cannot parse empty string");

        if (str[0] == '-') {
            sign = -1;
            str = std::string(str.begin() + 1, str.end());
        }
        for (auto it = str.rbegin(); it != str.rend(); it++) {
            if (!std::isdigit(*it))
                throw std::invalid_argument("Cannot parse string as number");

            digits.push_back(*it - '0'); // ascii value to actual number
        }

    }

    bool BigInteger::operator>(const BigInteger &oth) {
        if (sign > 0 && oth.sign < 0)return true;
        if (sign < 0 && oth.sign > 0)return false;


        if (oth.digits.size() != digits.size())
            return (digits.size() > oth.digits.size()) != (sign < 0);
        for (int64_t i = digits.size() - 1; i >= 0; i--) {
            if (digits.at(i) != oth.digits.at(i)) {
                return (digits.at(i) > oth.digits.at(i)) != (sign < 0);
            }
        }

        return false; // equal
    }

    bool BigInteger::operator<(const BigInteger &oth) {
        return !(*this >= oth);
    }

    bool BigInteger::operator>=(const BigInteger &oth) {
        return *this > oth || *this == oth;
    }

    bool BigInteger::operator==(const BigInteger &oth) {
        return sign == oth.sign && digits == oth.digits;
    }

    bool BigInteger::operator!=(const BigInteger &oth) {
        return !(*this == oth);
    }

    bool BigInteger::operator<=(const BigInteger &oth) {
        return *this < oth || *this == oth;
    }

    template<typename T>
    BigInteger BigInteger::from(T val) {
        return BigInteger(std::to_string(val));
    }

    bool BigInteger::operator>(int64_t val) {
        return *this > BigInteger(val);
    }

    bool BigInteger::operator<(int64_t val) {
        return *this < BigInteger(val);
    }

    bool BigInteger::operator>=(int64_t val) {
        return *this >= BigInteger(val);
    }

    bool BigInteger::operator<=(int64_t val) {
        return *this <= BigInteger(val);
    }

    bool BigInteger::operator!=(int64_t val) {
        return *this != BigInteger(val);
    }

    bool BigInteger::operator==(int64_t val) {
        return *this == BigInteger(val);
    }


    template BigInteger BigInteger::from(uint64_t val);

    template BigInteger BigInteger::from(int64_t val);

    template BigInteger BigInteger::from(uint32_t val);

    template BigInteger BigInteger::from(int32_t val);

    template BigInteger BigInteger::from(uint16_t val);

    template BigInteger BigInteger::from(int16_t val);

    template BigInteger BigInteger::from(uint8_t val);

    template BigInteger BigInteger::from(int8_t val);


    template uint64_t BigInteger::value<uint64_t>();

    template int64_t BigInteger::value<int64_t>();

    template int32_t BigInteger::value<int32_t>();

    template uint32_t BigInteger::value<uint32_t>();

    template uint16_t BigInteger::value<uint16_t>();

    template int16_t BigInteger::value<int16_t>();

    template int8_t BigInteger::value<int8_t>();

    template uint8_t BigInteger::value<uint8_t>();


    BigInteger::OverflowException::OverflowException(char const *const message) noexcept : invalid_argument(message) {}

    char const *BigInteger::OverflowException::what() const noexcept {
        return logic_error::what();
    }

    BigInteger::SignException::SignException(char const *const message) noexcept : invalid_argument(message) {}

    char const *BigInteger::SignException::what() const noexcept {
        return logic_error::what();
    }
}
