#pragma once
#include <fstream>
#include <tuple>
#include <type_traits>
#include <string>
#include <iterator>
#include <sstream>

namespace csv {

    template <typename... Args>
    class CSVParser {
    public:
        CSVParser(
            std::ifstream& f, size_t skip,
            char lineDelim = '\n', char colDelim = ',', char fieldDelim = '\"'
        );

        class iterator : public std::iterator<
            std::input_iterator_tag,
            std::tuple<Args...>
        > {
        public:
            using base = std::iterator<std::input_iterator_tag, std::tuple<Args...>>;
            using df_t = typename base::difference_type;
            using typename base::value_type;
            explicit iterator(CSVParser<Args...>& p, df_t idx);
            iterator& operator++();
            inline const iterator operator++(int) {
                iterator o { *this };
                ++(*this);
                return 0;
            }
            inline bool operator==(iterator o) const {
                return (
                    &parent == &(o.parent) &&
                    (
                        ((index == o.index) && !(isend || o.isend)) ||
                        (isend && isend)
                    )
                );
            }
            inline bool operator!=(iterator other) const {
                return !(*this == other);
            }
            value_type operator*() const;
        private:
        friend class CSVParser<Args...>;
            CSVParser<Args...>& parent;
            df_t index;
            bool isend;
        };

        inline iterator begin() {
            return iterator {*this, 0};
        }
        inline iterator end() {
            iterator it {*this, 0};
            it.isend = true;
            return it;
        }
    private:
        friend class iterator;
        std::ifstream& file;
        size_t skip;
        
        size_t size;
        size_t getSize();
        
        const char lineDelim;
        const char colDelim;
        const char fieldDelim;
        
        void parseLine(std::vector<std::string>& out, const std::string&);
    };

    template <typename... Args>
    CSVParser<Args...>::CSVParser(
        std::ifstream& f, size_t skipCount, char ldelim, char cdelim, char fdelim
    )
    : file(f), skip(skipCount), lineDelim(ldelim), colDelim(cdelim), fieldDelim(fdelim) {
        if (!file.is_open())
            throw std::invalid_argument("Cannot open file");
        file.exceptions(std::ios::goodbit);
        file.clear();
        size = getSize();
        if (skip > size)
            throw std::invalid_argument("Bad offset");
    }

    template <typename... Args>
    size_t CSVParser<Args...>::getSize() {
        file.clear();
        file.seekg(0, std::ios::beg);
        std::string buf;
        size_t size = 0;
        while (std::getline(file, buf, lineDelim))
            ++size;
        file.clear();
        file.seekg(0, std::ios::beg);
        return size;
    }

    template <typename... Args>
    void CSVParser<Args...>::parseLine(std::vector<std::string>& out, const std::string& s) {
        out.clear();
        enum { START, INSIDE, FIELD, CHECKDELIM } STATE = START;
        std::stringstream ss;
        ss << s;
        int c = ss.get();
        std::string buf;
        while (!ss.eof()) {
            switch (STATE) {
            case START:
                buf.clear();
                if (c == fieldDelim)
                    STATE = INSIDE;
                else {
                    buf.push_back(c);
                    STATE = FIELD;
                }
                break;
            case INSIDE:
                if (c == fieldDelim && buf.back() != '\\') {
                    out.push_back(buf);
                    buf.clear();
                    STATE = CHECKDELIM;
                }
                else if (c == fieldDelim) {
                    buf.back() = c;
                }
                else {
                    buf.push_back(c);
                }
                break;
            case FIELD:
                if (c == colDelim) {
                    out.push_back(buf);
                    buf.clear();
                    STATE = START;
                }
                else {
                    buf.push_back(c);
                }
                break;
            case CHECKDELIM:
                if (c != colDelim) {
                    throw std::invalid_argument("Bad field at " + std::to_string(out.size()));
                }
                STATE = START;
                break;
            
            default:
                break;
            }
            c = ss.get();
        }
        switch (STATE)
        {
        case START:
            out.push_back("");
            break;
        case FIELD:
            out.push_back(buf);
            break;
        case INSIDE:
            throw std::invalid_argument("Bad field at " + std::to_string(out.size()));
            break;
        case CHECKDELIM:
            break;
        
        default:
            break;
        }
    }

    template <typename... Args>
    CSVParser<Args...>::iterator::iterator(CSVParser<Args...>& p, df_t idx)
    : parent(p), index(idx), isend(false) {
        if (index >= parent.size) {
            index = parent.size;
            isend = true;
        }
    }

    template <typename... Args>
    typename CSVParser<Args...>::iterator& CSVParser<Args...>::iterator::operator++() {
        if (++index >= parent.size) {
            index = parent.size;
            isend = true;
        }
        return (*this);
    }

    template <typename... Args>
    typename CSVParser<Args...>::iterator::value_type
    CSVParser<Args...>::iterator::operator*() const {
        if (isend)
            throw std::logic_error("Cannot get data from after-end iterator");
        std::ifstream& f = parent.file;
        f.clear();
        f.seekg(0, std::ios::beg);
        std::string buf;
        size_t i = 0;
        while (i < index) {
            std::getline(f, buf, parent.lineDelim);
            ++i;
        }
        std::getline(f, buf, parent.lineDelim);
        std::vector<std::string> v;
        parent.parseLine(v, buf);
        std::tuple<Args...> t;
        tuple_utils::parse(v, t);
        f.clear();
        f.seekg(0, std::ios::beg);
        return t;
    }
    
}
