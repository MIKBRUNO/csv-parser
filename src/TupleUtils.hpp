#pragma once
#include <iostream>
#include <fstream>
#include <tuple>
#include <type_traits>
#include <vector>
#include <string>
#include <sstream>
#include "CSVExceptions.hpp"

namespace csv::tuple_utils {

    template <size_t I = 0, char Delim = ',', typename Ch, typename Tr, typename... Args>
    typename std::enable_if<I >= std::tuple_size<std::tuple<Args...>>::value,
    std::basic_ostream<Ch, Tr>&>::type
    operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t) {
        return os;
    }

    template <size_t I = 0, char Delim = ',', typename Ch, typename Tr, typename... Args>
    typename std::enable_if<I < std::tuple_size<std::tuple<Args...>>::value,
    std::basic_ostream<Ch, Tr>&>::type
    operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t) {
        if (0 != I)
            os << Delim;
        os << std::get<I>(t);
        return operator<<<I+1, Delim>(os, t);
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
            throw syntax_error(0, 0, "Bad number of elements in a row");
        if constexpr (std::is_same<
                typename std::tuple_element<I, std::tuple<Args...>>::type,
                std::string
            >::value)
        {
            std::get<I>(t) = ss[I];
            parse<I+1>(ss, t);
            return;
        }
        std::stringstream sss;
        sss << ss[I];
        sss >> std::get<I>(t);
        if ( sss.fail() || !sss.eof() )
            throw interpret_error(
                0, I+1, std::string("Bad element"));
        parse<I+1>(ss, t);
    }

}