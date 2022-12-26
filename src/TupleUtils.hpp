#pragma once
#include <iostream>
#include <fstream>
#include <tuple>
#include <type_traits>
#include <vector>
#include <string>
#include <sstream>

namespace tuple_utils {

    template <size_t I = 0, char Delim = ' ', typename Ch, typename Tr, typename... Args>
    typename std::enable_if<I >= std::tuple_size<std::tuple<Args...>>::value,
    std::basic_ostream<Ch, Tr>&>::type
    operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t) {
        return os;
    }

    template <size_t I = 0, char Delim = ' ', typename Ch, typename Tr, typename... Args>
    typename std::enable_if<I < std::tuple_size<std::tuple<Args...>>::value,
    std::basic_ostream<Ch, Tr>&>::type
    operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t) {
        if (0 != I)
            os << Delim;
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
        if (ss.size() != std::tuple_size<std::tuple<Args...>>::value)
            throw std::invalid_argument("Bad number of fields");
        std::stringstream sss;
        sss << ss[I];
        sss >> std::get<I>(t);
        if (
            (
                sss.fail() &&
                !std::is_same<
                    std::string&,
                    typename std::tuple_element<I, std::tuple<Args...>>::type
                >::value
            ) ||
            !sss.eof()
        )
            throw std::invalid_argument("Bad " + std::to_string(I+1) + "th tuple element");
        parse<I+1>(ss, t);
    }

}