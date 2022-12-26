#include <iostream>
#include <tuple>
#include "TupleUtils.hpp"
#include "CSVParser.hpp"

using namespace std;
using namespace csv;
using namespace tuple_utils;

int main(int argc, char* argv[]) {
    auto t = make_tuple("123"s, 2ul, 0.2f);
    cout << t << endl;

    return 0;
}
