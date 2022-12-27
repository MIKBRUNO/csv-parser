#include <iostream>
#include <tuple>
#include "TupleUtils.hpp"
#include "CSVParser.hpp"

using namespace std;
using namespace csv;
using namespace tuple_utils;

int main(int argc, char* argv[]) {
    ifstream f { "../../res/addresses.csv" };
    try {
        CSVParser<string, string, string, string, string, string> csvparser {f, 0};
        for (auto t : csvparser)
            operator<<<0, '\n'>(cout, t) << endl << endl;
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
    }

    return 0;
}
