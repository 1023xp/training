#ifndef _PCRE_HPP
#define _PCRE_HPP

#include <string>
#include <list>

#include <pcre.h>
namespace Pcre
{
    class Regexp
    {
    public:
	Regexp();
	~Regexp();

	bool compile(const std::string &patten);
	bool matches(const std::string &s);
	bool match_capture(const std::string &s,
			   std::list<std::string> &captures);

	/// Modifies the list!
	void grep(std::list<std::string> &lines);
    private:
	void pcre_check_free();
	Regexp(const Regexp &other);
	const Regexp &operator=(const Regexp &other);
	pcre *_regexp;
	int _num_captures;
	std::string _lasterr;
	int _lasterroffset;
    };
};

#endif

