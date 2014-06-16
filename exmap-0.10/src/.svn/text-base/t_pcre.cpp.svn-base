/*
 * (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license.
 */
#include <Trun.hpp>
#include "Pcre.hpp"

/// Not testing the underlying pcre engine, just our wrapping
class PcreTest : public Test
{
public:
    bool run();
};

using namespace std;
using namespace Pcre;

bool PcreTest::run()
{
    Regexp re;

    plan(22);

    notok(re.matches("foo"), "empty regexp doesn't match");
    ok(re.compile("^foo$"), "can compile simple re");
    ok(re.matches("foo"), "simple match");
    notok(re.matches("bar"), "simple fail");
    notok(re.matches("b"), "simple fail");

    ok(re.compile("a"), "can recompile");
    ok(re.matches("a"), "can match 1");
    ok(re.matches("abba"), "can match 2");
    ok(re.matches("hootenanny"), "can match 2");
    notok(re.matches("x"), "can fail 1");

    list<string> eglines, lines;
    eglines.push_back("foo");
    eglines.push_back("bar");
    eglines.push_back("baz");
    eglines.push_back("quux");

    lines = eglines;
    re.grep(lines);
    is(lines.size(), (string::size_type) 2,
       "grep out two lines with an a in them");
    ok(lines.front() == "bar", "right first line");
    ok(lines.back() == "baz", "right last line");

    ok(re.compile("\\d"), "can compile numeric match");
    ok(re.matches("abc123"), "numeric match 1");
    ok(re.matches("123"), "numeric match 2");
    notok(re.matches("abc"), "numeric match 3");

    ok(re.compile("foo(\\d+)bar"), "compile regexp with capture");
    list<string> captures;
    ok(re.matches("foo123bar"), "matches");
    ok(re.match_capture("foo123bar", captures), "can match_capture");
    is((int) captures.size(), 1, "found one capture");
    is(captures.front(), string("123"), "right capture found");
    
    return true;
}

RUN_TEST_CLASS(PcreTest);
