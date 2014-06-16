#include <iostream>
#include "Exmap.hpp"

using namespace std;
using namespace Exmap;
using namespace jutil;

int main(int argc, char **argv)
{
    if (argc != 2) {
	cerr << "Usage: " << argv[0] << " pid\n";
	return -1;
    }

    pid_t pid = atoi(argv[1]);

    SysInfoPtr sysinfo(new LinuxSysInfo);
    SnapshotPtr snapshot(new Snapshot(sysinfo));
    if(!snapshot->load()) {
	cerr << "Failed to load snapshot\n";
	return -1;
    }

    ProcessPtr proc = snapshot->proc(pid);
    if (!proc) {
	warn << "Can't find process for pid " << pid << "\n";
	return -1;
    }

    proc->print(cout);

    return 0;
}
