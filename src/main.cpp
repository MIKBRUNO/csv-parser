#include <iostream>
#include <tuple>
#include "TupleUtils.hpp"
#include "CSVParser.hpp"

using namespace std;
using namespace csv;
using namespace tuple_utils;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        tuple<int, string, float> t = make_tuple(0, "rec0", 123.321f);
        cout << t << endl;
        return 0;
    }

    ifstream f { argv[1] };
    try {
        CSVParser<string, string, string> csvparser {f, 0};
        for (auto t : csvparser)
            operator<<<0, '\n'>(cout, t) << endl << endl;
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
