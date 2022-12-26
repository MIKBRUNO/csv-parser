#include <iostream>
#include <tuple>
#include "TupleUtils.hpp"
#include "CSVParser.hpp"

using namespace std;
using namespace csv;
using namespace tuple_utils;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        auto t = make_tuple("123"s, 2ul, 0.2f);
        cout << t << endl;
        return 0;
    }

    ifstream f { argv[1] };
    CSVParser<unsigned int, int, long, string, string> csvparser {f, 0};
    for (auto t : csvparser)
        cout << t << endl;

    return 0;
}
