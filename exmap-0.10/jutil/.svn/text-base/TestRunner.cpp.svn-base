#include "TestRunner.hpp"
#include "Pcre.hpp"

using namespace std;

/// TODO: use a decent lib instead of these hacks
static bool is_in_current_dir(const string &fname)
{
    return (fname.find_first_of("/\\") == string::npos);
}

void TestRunner::add_file(const std::string &filename)
{
    if (is_in_current_dir(filename)) {
	_files.push_back("./" + filename);
    }
    else {
	_files.push_back(filename);
    }
}

void TestRunner::run(void)
{
    run_and_report(NULL);
}

void TestRunner::run_and_report(ostream &os)
{
    run_and_report(&os);
    report_summary(os);
}

void TestRunner::run_and_report(ostream *os)
{
    list<string>::iterator it;
    for (it = _files.begin(); it != _files.end(); ++it) {
	Test active(*it);
	active.run();
	_completed_tests.push_back(active);
	if (active.passed() != active.total()) {
	    _tests_with_failures.push_back(active);
	}
	if (active.total() != active.planned()) {
	    _tests_with_bad_plan.push_back(active);
	}
	if (os != NULL) {
	    active.report(*os);
	}
    }
}

void TestRunner::report(ostream &os)
{
    list<Test>::iterator it;

    for (it = _completed_tests.begin(); it != _completed_tests.end(); ++it) {
	(*it).report(os);
    }

    report_summary(os);
}

void TestRunner::report_summary(ostream &os)
{
    list<Test>::iterator it;

    os << "--------------------------------\n";
    if (_tests_with_failures.empty() && _tests_with_bad_plan.empty()) {
	os << "All tests passed successfully\n";
    }

    if (!_tests_with_failures.empty()) {
	for (it = _tests_with_failures.begin();
	     it != _tests_with_failures.end();
	     ++it) {
	    os << "FAIL: " << it->file() << " failed " << it->failed() << "\n";
	}
    }
    if (!_tests_with_bad_plan.empty()) {
	for (it = _tests_with_bad_plan.begin();
	     it != _tests_with_bad_plan.end();
	     ++it) {
	    os << "FAIL: " << it->file() << " ran " << it->total()
	       << " but " << it->planned() << " were planned\n";
	}
    }
}


TestRunner::Test::Test(const string &filename)
    : _file(filename), _planned(0), _total(0), _passed(0)
{ }

string TestRunner::Test::file() { return _file; }
int TestRunner::Test::passed() { return _passed; }
int TestRunner::Test::total() { return _total; }
int TestRunner::Test::planned() { return _planned; }
int TestRunner::Test::failed() { return total() - passed(); }

void TestRunner::Test::run(void)
{
    FILE *fp = NULL;
    char buf[BUFSIZ];
    bool check_for_plan = true;
     
    fp = popen(_file.c_str(), "r");
    if (fp == NULL) {
	goto Exit;
    }
    while (fgets(buf, BUFSIZ, fp) != NULL) {
	process_line(string(buf), check_for_plan);
	check_for_plan = false;
    }

 Exit:
    if (fp != NULL) {
	fclose(fp);
	fp = NULL;
    }

    return;
}

void TestRunner::Test::report(ostream &os)
{
    string padded_file = file();
    const unsigned int file_width = 20;


    // I hate formatting with ostreams. Should probably use sprintf().
    // I suck.
    while (padded_file.length() < file_width) {
	padded_file += " ";
    }
    os << padded_file << " "
	 << passed() << "/" << total() << endl;
    if (failed() > 0) {
	os << "WARNING: " << file() << " failed " << failed() << "\n";
    }
    if (planned() < total()) {
	os << "NOTICE: Ran more tests than planned: "
	   << planned() << " < " << total() << "\n";
    }
    else if (planned() > total()) {
	os << "WARNING: Ran fewer tests than planned: "
	   << total () << " < " << planned() << "\n";
    }
}

void TestRunner::Test::process_line(const string &line,
				    bool check_for_plan)
{
    string::size_type len;
    list<string> captures;
    Pcre::Regexp re;

    len = line.length();
    if (len == 0) return;
    if (line[0] == '#') return;
    if (line[len-1] != '\n') return;

    // todo - some error reporting...
    
    if (check_for_plan) {
	re.compile("^(\\d+)\\.\\.(\\d+)");
	if (re.match_capture(line, captures)
	    && captures.size() == 2) {
	    _planned = atoi(captures.back().c_str());
	}
	return;
    }

    re.compile("^(not )?ok\\s+(\\d+)\\s+(.*)");
    if (!re.match_capture(line, captures)
	|| captures.size() != 3) {
	return;
    }
    
    bool passed = !(captures.front() == "not ");
    captures.pop_front();

    // todo...make use of the test number and string in the captures
    
    if (passed) {
	_passed++;
    }
    _total++;
}
    
