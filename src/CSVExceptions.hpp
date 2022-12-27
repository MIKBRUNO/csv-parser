#pragma once
#include <exception>
#include <string>

namespace csv {

    struct parse_error : public std::exception {
        inline parse_error(unsigned int r, unsigned int c, const char* m)
        : row(r), col(c), msg(new std::string(m)), whatStr(new std::string) { }
        inline parse_error(unsigned int r, unsigned int c, const std::string& m)
        : row(r), col(c), msg(new std::string(m)), whatStr(new std::string) { }
        virtual ~parse_error() {
            delete whatStr;
            delete msg;
        }
        virtual const char* what() const noexcept {
            *whatStr = *msg + ". at row " + std::to_string(row) + ", col "
                        + std::to_string(col) + ".";
            return whatStr->c_str();
        }
        unsigned int row, col;
        const std::string* msg;

    private:
        std::string* whatStr;
    };

    struct syntax_error : public parse_error
    {
        using parse_error::parse_error;
    };

    struct interpret_error : public parse_error
    {
        using parse_error::parse_error;
    };
    

}
