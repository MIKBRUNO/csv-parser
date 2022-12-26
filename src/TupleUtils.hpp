#pragma once
#include <iostream>
#include <fstream>
#include <tuple>
#include <type_traits>
#include <vector>
#include <string>
#include <sstream>

namespace tuple_utils {

    template <size_t I = 0, typename Ch, typename Tr, typename... Args>
    typename std::enable_if<I >= std::tuple_size<std::tuple<Args...>>::value,
    std::basic_ostream<Ch, Tr>&>::type
    operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t) {
        return os;
    }

    template <size_t I = 0, typename Ch, typename Tr, typename... Args>
    typename std::enable_if<I < std::tuple_size<std::tuple<Args...>>::value,
    std::basic_ostream<Ch, Tr>&>::type
    operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t) {
        if (0 != I)
            os << ',';
        os << std::get<I>(t);
        return operator<<<I+1>(os, t);
    }

    template <size_t I = 0, typename... Args>
    typename std::enable_if<
        I >= std::tuple_size<std::tuple<Args...>>::value,
        void
    >::type
    parse(std::vector<std::string>& ss, std::tuple<Args...>& t) { }

    template <size_t I = 0, typename... Args>
    typename std::enable_if<
        I < std::tuple_size<std::tuple<Args...>>::value,
        void
    >::type
    parse(std::vector<std::string>& ss, std::tuple<Args...>& t) {
        if (ss.size() < I)
            throw std::invalid_argument("Not enough elements to fill tuple.");
        std::stringstream sss;
        sss << ss[I];
        sss >> std::get<I>(t);
        if (sss.fail())
            throw std::invalid_argument("Bad " + std::to_string(I+1) + "th tuple element");
        parse<I+1>(ss, t);
    }

}