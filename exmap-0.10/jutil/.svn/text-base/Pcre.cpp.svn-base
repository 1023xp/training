#include "Pcre.hpp"
#include "jutil.hpp"

using namespace std;
using namespace Pcre;
using namespace jutil;


Regexp::Regexp()
    : _regexp(NULL)
{ }

void Regexp::pcre_check_free()
{
    if (_regexp != NULL) {
	(*pcre_free)(_regexp);
	_regexp = NULL;
    }
}

Regexp::~Regexp()
{
    pcre_check_free();
}

	
bool Regexp::compile(const string &pattern)
{
    const char *errptr = NULL;
    int erroffset;
    pcre_check_free();
    _regexp = pcre_compile(pattern.c_str(),
			   0,
			   &errptr,
			   &erroffset,
			   NULL);
    if (errptr != NULL) {
	_lasterr = errptr;
	_lasterroffset = erroffset;
    }
    else {
	_lasterr.clear();
	_lasterroffset = 0;
    }

    if (pcre_fullinfo(_regexp,
		      NULL,
		      PCRE_INFO_CAPTURECOUNT,
		      &_num_captures) < 0) {
	return false;
    }
    
    return _regexp != NULL;
}

bool Regexp::matches(const string &s)
{
    list<string> captures;
    return match_capture(s, captures);
}

bool Regexp::match_capture(const string &s,
			   list<string> &captures)
{
    captures.clear();
    if (_regexp == NULL) {
	return false;
    }
    int *ovector = NULL;
    // 3 == start, end, workspace (per capture).
    // +1 because first pair returned isn't a capture, it relates to
    // the string
    int ovec_size = (_num_captures + 1) * 3;
    ovector = (int *) alloca(ovec_size * sizeof(int));
    int num_pairs = pcre_exec(_regexp,
			      NULL,
			      s.data(),
			      s.length(),
			      0,
			      0,
			      ovector,
			      ovec_size);
    if (num_pairs < 0) {
	return false;
    }
    // Skip first pair, it refers to the portion of the subject string
    for (int pair = 2; pair < 2*num_pairs; pair += 2) {
	if (ovector[pair] < 0) {
	    // Failed optional capture
	    captures.push_back("");
	}
	else {
	    int capturelen = ovector[pair+1] - ovector[pair];
	    string capture(s, ovector[pair], capturelen);
	    captures.push_back(capture);
	}
    }
    return true;
}

void Regexp::grep(list<string> &lines)
{
    list<string>::iterator it;

    it = lines.begin();
    while(it != lines.end()) {
	if (!matches(*it)) {
	    it = lines.erase(it);
	}
	else {
	    ++it;
	}
    }
}
