#include "Trun.hpp"

class EgTest : public Test
{
public:
    bool run(void);
};

bool EgTest::run(void)
{
    plan(4);
    ok(true, "pass a test");
    ok(false, "fail a test");
    is(2 + 2, 4, "pass some arithmetic");
    is(2 - 1, 3, "fail some arithmetic");
    return true;
}

RUN_TEST_CLASS(EgTest);
