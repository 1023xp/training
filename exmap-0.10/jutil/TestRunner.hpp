#ifndef _TESTRUNNER_H
#define _TESTRUNNER_H

#include <iostream>
#include <string>
#include <list>

class TestRunner
{
public:
    void add_file(const std::string &filename);
    /// Run the tests, no reporting
    void run(void);
    /// Run the tests, progressively reporting to ostream
    void run_and_report(std::ostream &os);
    /// Report on the tests to ostream
    void report(std::ostream &os);
private:
    /// Implements both run() and run_and_report(), doesn't report if
    /// os is NULL
    void run_and_report(std::ostream *os = NULL);
    
    class Test
    {
    public:
	Test(const std::string &filename);
	void run(void);
	void report(std::ostream &os);
	std::string file();
	int passed();
	int total();
	int planned();
	int failed();
    private:
	void process_line(const std::string &line,
			  bool check_for_plan);
	std::string _file;
	int _planned;
	int _total;
	int _passed;
    };
    
    void report_summary(std::ostream &os);

    std::list<std::string> _files;
    std::list<Test> _completed_tests;
    std::list<Test> _tests_with_failures;
    std::list<Test> _tests_with_bad_plan;
};

#endif
