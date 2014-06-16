#ifndef _JUTIL_H
#define _JUTIL_H

#include <iostream>
#include <string>
#include <list>
#include <map>

#include <stdio.h>

// For getuid/setuid
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

namespace jutil
{
    // ------------------------------------------------------------
    // iostream helpers
    // ------------------------------------------------------------
    
    class ustream : public std::ostream
    {
    public:
	ustream(const std::string &name,
		bool on = true)
	    : _name(name), _on(on) {
		check_enable();
	    }
	template <typename T> ustream &operator<<(const T &t) {
	    if (_on) {
		std::cerr << t;
	    }
	    return *this;
	}
    private:
	void check_enable() {
	    std::string env_var = "JUTIL_STREAM_" + _name;
	    if (getenv(env_var.c_str()) != NULL) {
		    _on = true;
	    }
	}
	std::string _name;
	bool _on;
    };
    extern ustream warn;
    extern ustream log;
    extern ustream dbg;

    /// Output STL lists of anything to an ostream
    template <typename T> std::ostream &operator<<(std::ostream &os,
						   const std::list<T> &l) {
	typename std::list<T>::const_iterator it;
	for (it = l.begin(); it != l.end(); ++it) {
	    if (it != l.begin()) {
		os << " ";
	    }
	    os << *it;
	}
	return os;
    }
    
    // ------------------------------------------------------------
    // STL container helpers
    // ------------------------------------------------------------
    
    /// Return a list of the map keys
    template <typename K, typename V>
    std::list<K> map_keys(const std::map<K, V> &m)
    {
	std::list<K> keylist;
	typename std::map<K, V>::const_iterator it;

	for (it = m.begin(); it != m.end(); ++it) {
	    keylist.push_back(it->first);
	}
    
	return keylist;
    }
    
    /// Return a list of the map values
    template <typename K, typename V>
    std::list<V> map_values(const std::map<K, V> &m) {
	std::list<V> vallist;
	typename std::map<K, V>::const_iterator it;

	for (it = m.begin(); it != m.end(); ++it) {
	    vallist.push_back(it->second);
	}
    
	return vallist;
    }

    
    // ------------------------------------------------------------
    // Filesystem ops
    // ------------------------------------------------------------

    /// Get the directory contents as a list of strings.
    bool read_directory(const std::string &dirname,
			std::list<std::string> &fnames);
    
    /// Open (for reading), read and close the file in text mode,
    /// returning the contents broken into lines, with the trailing
    /// newline. Lines longer than LINE_MAXLEN will be split into
    /// multiple lines.
    const int LINE_MAXLEN = 1023;
    bool read_textfile(const std::string &name,
		       std::list<std::string> &lines);

    /// As for read_textfile, but uses the stdout of the cmdline as
    /// the input.
    bool read_proc_output(const std::string &cmdline,
			  std::list<std::string> &lines);
    
    /// Open (for writing, at beginning), write the lines and close the
    /// file.
    bool overwrite_textfile(const std::string &name,
			    const std::list<std::string> &lines);
    bool overwrite_textfile(const std::string &name,
			    const std::string &line);

    /// True if the file exists
    bool file_exists(const std::string &fname);

    /// True if the filename exists as a regular file
    bool is_regular_file(const std::string &fname);

    /// Sets size if file exists. Returns false if it doesn't.
    bool file_size(const std::string &fname, off_t &fsize);

    /// Sets uid to owner if file exists. Returns false if it doesn't.
    bool file_owner(const std::string &fname, uid_t &uid);

    /// true if the file is readable
    bool file_readable(const std::string &fname);

    /// Read any type from a stream, bitwise.
    template <typename T> bool binary_read(std::istream &is, T &val)
    {
	return is.read((char *)&val, sizeof(val));
    }

    
    // ------------------------------------------------------------
    // String helpers
    // ------------------------------------------------------------

    
    /// Remove a trailing \n if there is one.
    void chomp(std::string &line);

    /// Remove leading and/or trailing chars (by default, whitespace)
    void trim(std::string &line, const std::string trimchars = " \t\n");
    
    /// Count number of occurences of char in string
    int count_occurences(const std::string &s, char c);

};

#endif
