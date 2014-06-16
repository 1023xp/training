#ifndef _TEST_H
#define _TEST_H

#include <string>
#include <list>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <jutil.hpp> // For std::list operator<<

// Macro wrappers for methods to get at FILE and LINE info
#define ok(b, name)\
    ok_f(b, name, __FILE__, __LINE__)
#define notok(b, name)\
    ok_f(!(b), name, __FILE__, __LINE__)
#define is(got, expected, name)\
    is_f(true, got, expected, name, __FILE__, __LINE__)
#define isnt(got, expected, name)\
    is_f(false, got, expected, name, __FILE__, __LINE__)
#define pass(name)\
    ok_f(true, name, __FILE__, __LINE__)
#define fail(name)\
    ok_f(false, name, __FILE__, __LINE__)
#define is_approx(got, expected, epsilon, name)\
    is_approx_f(got, expected, epsilon, name, __FILE__, __LINE__)
#define is_approx_rel(got, expected, fepsilon, name)			\
    is_approx_rel_f(got, expected, fepsilon, name, __FILE__, __LINE__)

#define RUN_TEST_CLASS(testname) \
int main(void) \
{ \
    bool ran_ok; \
    TestSuite ts; \
    testname *t = new testname; \
    ts.add(t); \
    ran_ok = ts.run(); \
    ts.summarise(); \
    return ran_ok ? 0 : -1; \
}

///
/// Base class for unit testing. The methods within should be accessed
/// via via the preprocessor macros above, to provide file and line
/// information.
///
/// Test success/failure information is to stdout in the format used
/// by the perl Test::More/Test::Unit modules (the 'Test Anything
/// Protocol'), see:
/// http://search.cpan.org/dist/Test-Harness/lib/Test/Harness/TAP.pod
///
class Test
{
public:
    Test() : _nexttestnum(1), _planned(0), _failed(0), _os(std::cout) { }
    virtual bool setup(void) { return true; }
    virtual bool run(void) = 0;
    virtual bool teardown(void) { return true; }
    virtual void summarise() {
	    _os << "# Planned " << num_planned()
	    	<< " Ran " << num_ran()
		<< " Failed " << num_failed()
		<< "\n";
    };
    virtual ~Test() { }

    int num_planned() { return _planned; }
    int num_failed() { return _failed; }
    int num_ran() { return _nexttestnum - 1; }

protected:

    /// Generate the test plan line.
    void plan(int ntests = 0) {
	_planned = ntests;
	if (_planned > 0) {
	    _os << "1.." << _planned << std::endl;
	}
    }

    /// Simple boolean test
    void ok_f(bool result, const std::string &name,
	      const std::string &file, int line) {
	if (result) {
	    passed(name);
	}
	else {
	    failed(name, file, line);
	}
    }

    /// Comparison via this function allows specialisation for containers
    template <typename T> bool are_equal(const T &a, const T &b)
    {
	return a == b;
    }

    template <typename T> bool are_equal(
	    const std::list<boost::shared_ptr<T> > &a,
	    const std::list<boost::shared_ptr<T> > &b)
    {
	if (a.size() != b.size()) { return false; }
	typename std::list<boost::shared_ptr<T> >::const_iterator a_it, b_it;
	for (a_it = a.begin(), b_it = b.begin();
		a_it != a.end();
		++a_it, ++b_it) {
	    boost::shared_ptr<T> a = *a_it;
	    boost::shared_ptr<T> b = *b_it;
	    if (*a != *b) { return false; }
	}

	return true;
    };
    template <typename T> bool are_equal(const std::list<T> &a,
	    const std::list<T> &b)
    {
	if (a.size() != b.size()) { return false; }
	typename std::list<T>::const_iterator a_it, b_it;
	for (a_it = a.begin(), b_it = b.begin();
		a_it != a.end(); ++a_it, ++b_it) {
	    if (*a_it != *b_it) { return false; }
	}

	return true;
    };
    
    /// Exact comparison test
    template <typename T> void is_f(bool should_be_same,
				    const T &got,
				    const T &expected,
				    const std::string &name,
				    const std::string &file,
				    int line) {
	bool same = are_equal(expected, got);
	if (same == should_be_same) {
	    passed(name);
	}
	else {
	    failed(name, file, line);
	    _os << "# got: '" << got << "'" << std::endl
		<< "# expected: '" << expected << "'" << std::endl;
	}
    }

    /// Approx comparison, absolute difference
    template <typename T, typename I>void is_approx_f(const T &got,
						      const T &expected,
						      const I &epsilon,
						      const std::string &name,
						      const std::string &file,
					  int line) {
	I delta = (I) (got - expected);
	delta = abs(delta);
	if (delta <= epsilon) {
	    passed(name);
	}
	else {
	    failed(name, file, line);
	    _os << "# got: '" << got << "'" << std::endl
		<< "# expected: '" << expected << "'" << std::endl
		<< "# delta: '" << delta << "'" << std::endl;
	}
    }

    /// Approx comparison, relative difference
    template <typename T>void is_approx_rel_f(const T &got,
					      const T &expected,
					      const float &factor,
					      const std::string &name,
					      const std::string &file,
					      int line) {
	double delta = got - expected;
	const double epsilon = 1e-20;
	if (delta < 0.0) delta = -delta;
	if (delta < epsilon
	    || (expected > epsilon
		&& ((float) delta / (float) expected) < factor)) {
	    passed(name);
	}
	else {
	    failed(name, file, line);
	    _os << "# got: '" << got << "'" << std::endl
		<< "# expected: '" << expected << "'" << std::endl
		<< "# factor: '" << factor << "'" << std::endl;
	}
    }
 
    /// Number of current test
    int _nexttestnum;
    /// Number of expected tests - 0 if not known
    int _planned;
    /// Number of failures
    int _failed;

private:
    /// Called when a test passes
    void passed(const std::string &name) {
	report(true, name);
    }
    /// Called when a test fails
    void failed(const std::string &name, const std::string &file, int line) {
	report(false, name);
	++_failed;
	_os << "# Failed test (" << file
	    << " at line " << line << ")" << std::endl;
    }
    void report(bool passed, const std::string &name) {
	if (!passed) {
	    _os << "not ";
	}
	_os << "ok " << _nexttestnum++ << " " << name << std::endl;
    }

    /// The stream to which we write results
    std::ostream &_os;
};

class TestSuite : public Test
{
public:
    void add(Test *t) {
	_tests.push_back(t);
    };
    bool run(void) {
	bool worked = true;
	std::list<Test *>::iterator it;
	for (it = _tests.begin(); it != _tests.end(); ++it) {
	    if ((*it)->setup()) {
		if (!(*it)->run()) {
		    worked = false;
		}
		_planned += (*it)->num_planned();
		_failed += (*it)->num_failed();
		_nexttestnum += (*it)->num_ran();
	    }
	    (*it)->teardown();
	}
	return worked;
    }
    bool teardown(void) {
	std::list<Test *>::iterator it;
	for (it = _tests.begin(); it != _tests.end(); ++it) {
	    delete *it;
	}
	_tests.clear();
	return true;
    }
private:
    std::list<Test *> _tests;
};

#endif
