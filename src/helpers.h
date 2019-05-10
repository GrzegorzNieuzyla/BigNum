#include <algorithm>
#include <cctype>
#include <locale>

namespace BigNum {
    // from https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

    void ltrim(std::string &s);

    void rtrim(std::string &s);

    void trim(std::string &s);

}
