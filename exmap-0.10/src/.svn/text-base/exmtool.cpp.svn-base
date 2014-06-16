/*
 * (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license.
 */
#include "Exmap.hpp"

#include <sstream>
#include <iostream>
#include <vector>
#include <string.h>

using namespace std;
using namespace Exmap;

static int usage();
static int do_showmaps(SnapshotPtr &snap, char *args[]);
static int do_procs(SnapshotPtr &snap, char *args[]);
static int do_files(SnapshotPtr &snap, char *args[]);
typedef int (*Handler)(SnapshotPtr &snap, char *args[]);

struct command
{
    char *command;
    Handler handler;
    char *usage;
} cmd_handles[] = {
    { "procs",
      do_procs,
    "list the known processes"},
    { "files",
      do_files,
    "list the known files"},
    { "showmaps",
      do_showmaps,
    "list the maps of a particular process"},
    { NULL, NULL, NULL },
};

int main(int argc, char *argv[])
{
    if (argc < 2) {
	return usage();
    }

    Handler handler = NULL;
    struct command *chandler = cmd_handles;
    while (chandler->command != NULL) {
	if (strcmp(chandler->command, argv[1]) == 0) {
	    handler = chandler->handler;
	    break;
	}
	++chandler;
    }

    if (handler == NULL) {
	cerr << "Unrecognised command: " << argv[1] << "\n";
	return usage();
    }

    SysInfoPtr sysinfo(new LinuxSysInfo);
    SnapshotPtr snapshot(new Snapshot(sysinfo));
    if (!snapshot->load()) {
	cerr << "Failed to load snapshot - aborting" << endl;
	return -1;
    }

    return chandler->handler(snapshot, argv + 2);
}

static int usage()
{
    struct command *chandler = cmd_handles;
    ostream &os = cerr;
    os << "\n";
    while (chandler->command != NULL) {
	os << chandler->command << ": " << chandler->usage << "\n";
	++chandler;
    }
    return -1;
}

static int do_procs(SnapshotPtr &snap, char *args[])
{
    list<ProcessPtr> procs;
    list<ProcessPtr>::iterator it;

    procs = snap->procs();
    Sizes::scale_kbytes();
    cout << "PID";
    for (int i = 0; i < Sizes::NUM_SIZES; ++i) {
	cout << "\t" << Sizes::size_name(i);
    }
    cout << "\t" << "CMD";
    cout << "\n";
    
    for (it = procs.begin(); it != procs.end(); ++it) {
	ProcessPtr proc = *it;
	SizesPtr sizes = proc->sizes();
	cout << proc->pid();
	for (int i = 0; i < Sizes::NUM_SIZES; ++i) {
	    cout << "\t" << sizes->sval(i);
	}
	cout << "\t" << proc->cmdline();
	cout << "\n";
    }
    return 0;
}

static int do_files(SnapshotPtr &snap, char *args[])
{
    list<FilePtr> files;
    list<FilePtr>::iterator it;

    files = snap->files();
    Sizes::scale_kbytes();
    for (int i = 0; i < Sizes::NUM_SIZES; ++i) {
	cout << Sizes::size_name(i) << "\t";
    }
    cout << "NAME\n";
    
    for (it = files.begin(); it != files.end(); ++it) {
	FilePtr file = *it;
	SizesPtr sizes = file->sizes();
	for (int i = 0; i < Sizes::NUM_SIZES; ++i) {
	    cout << sizes->sval(i) << "\t";
	}
	cout << file->name();
	cout << "\n";
    }
    return 0;
}

static int do_showmaps(SnapshotPtr &snap, char *args[])
{
    char *pidarg = args[0];
    if (pidarg == NULL) {
	cerr << "no pid specified\n";
	return usage();
    }
    pid_t pid = atoi(args[0]);
    if (pid <= 0) {
	cerr << "invalid pid: " << args[0] << "\n";
	return usage();
    }

    ProcessPtr proc = snap->proc(pid);
    if (!proc) {
	cerr << "No information on pid: " << pid << "\n";
	return -1;
    }

    cout << *proc << "\n";
    
    return 0;
}
