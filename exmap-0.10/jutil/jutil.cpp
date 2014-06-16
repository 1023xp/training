#include "jutil.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <string.h>

using namespace std;
using namespace jutil;

ustream jutil::warn("WARN");
ustream jutil::log("LOG");
ustream jutil::dbg("DEBUG", false);

/// \todo: add 'current_errno_string' method and add error messages to
/// failure cases using this string...

bool jutil::read_directory(const string &dirname, list<string> &fnames)
{
    DIR *dir = NULL;
    struct dirent *dent;
    char buf[NAME_MAX+1];
    bool worked = false;

    fnames.clear();
    
    dir = opendir(dirname.c_str());
    if (dir == NULL) goto out;

    while ((dent = readdir(dir)) != NULL) {
	strncpy(buf, dent->d_name, NAME_MAX);
	fnames.push_back(string(buf));
    }

    worked = true;
    
 out:
    if (dir != NULL) {
	closedir(dir);
	dir = NULL;
    }

    return worked;
}



static void readline_helper(FILE *fp,
			    list<string> &lines)
{
    char buf[LINE_MAXLEN+1];

    lines.clear();

    while (fgets(buf, LINE_MAXLEN, fp) != NULL) {
	lines.push_back(string(buf));
    }
}

bool jutil::read_textfile(const string &name,
			  list<string> &lines)
{
    FILE *fp = NULL;
    
    fp = fopen(name.c_str(), "r"); // Text mode open
    if (fp != NULL) {
	readline_helper(fp, lines);
	fclose(fp);
	return true;
    }
    return false;
}

bool jutil::read_proc_output(const string &cmdline,
			     list<string> &lines)
{
    FILE *fp = NULL;
    
    fp = popen(cmdline.c_str(), "r");
    if (fp != NULL) {
	readline_helper(fp, lines);
	pclose(fp);
	return true;
    }
    return false;
}

bool jutil::overwrite_textfile(const string &name,
			       const string &line)
{
    std::list<std::string> lines;
    lines.push_back(line);
    return overwrite_textfile(name, lines);
}

bool jutil::overwrite_textfile(const string &name,
			       const list<string> &lines)
{
    FILE *fp = NULL;
    bool worked = false;
    list<string>::const_iterator it;

    fp = fopen(name.c_str(), "w+"); // Destructive text mode open
    if (fp == NULL) goto out;

    for (it = lines.begin(); it != lines.end(); ++it) {
	if (fputs(it->c_str(), fp) == EOF) goto out;
    }

    worked = true;
    
 out:
    if (fp != NULL) {
	fclose(fp);
	fp = NULL;
    }

    return worked;
}

bool jutil::file_exists(const string &fname)
{
    struct stat sb;
    return stat(fname.c_str(), &sb) == 0;
}

bool jutil::is_regular_file(const string &fname)
{
    struct stat sb;
    return (stat(fname.c_str(), &sb) == 0
	    && S_ISREG(sb.st_mode));
}

bool jutil::file_size(const string &fname, off_t &fsize)
{
    struct stat sb;
    fsize = 0;
    if (stat(fname.c_str(), &sb) == 0) {
	fsize = sb.st_size;
	return true;
    }
    return false;
}

bool jutil::file_owner(const string &fname, uid_t &uid)
{
    struct stat sb;
    uid = 0;
    if (stat(fname.c_str(), &sb) == 0) {
	uid = sb.st_uid;
	return true;
    }
    return false;
}

bool jutil::file_readable(const string &fname)
{
    struct stat sb;

    if (stat(fname.c_str(), &sb) != 0) {
      return false;
    }

    return 
      (S_IROTH & sb.st_mode)
      || ((S_IRGRP & sb.st_mode) && sb.st_gid == getegid())
      || ((S_IRUSR & sb.st_mode) && sb.st_uid == geteuid());
}



void jutil::chomp(string &line)
{
    string::size_type len = line.length();
    if (len < 1) return;
    if (line[len-1] == '\n') {
	line.erase(len-1);
    }
    return;
}


void jutil::trim(std::string &line, const std::string trimchars)
{
    string::size_type start, finish;
    start = line.find_first_not_of(trimchars);
    if (start == string::npos) {
	line.clear();
    }
    else {
	finish = line.find_last_not_of(trimchars);
	if (finish <= start) {
	    line.clear();
	}
	else {
	    line = line.substr(start, finish-start+1);
	}
    }
}

int jutil::count_occurences(const std::string &s, char c)
{
    int count = 0;
    string::size_type pos = 0;
    string::size_type len = s.length();
    while (pos < len) {
	if (s[pos] == c) {
	    ++count;
	}
	++pos;
    }
    return count;
}
