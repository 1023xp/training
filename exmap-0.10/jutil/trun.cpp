#include "TestRunner.hpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    TestRunner tr;

    for (int i = 1; i < argc; ++i) {
	tr.add_file(argv[i]);
    }
    tr.run();
    tr.report(cout);
    return 0;
}
