#include "utsname_windows.h"

//from: http://www9.biostr.washington.edu/webalizer/src/stonesteps.ca/webalizer_src_3_4_4_1/platform/sys/utsname.cpp

/*#include <windows.h>
#include <errno.h>
#include <stdio.h>
#include "utsname.h"

int uname(utsname *)
{
	OSVERSIONINFO verinfo = { 0 };
	DWORD buffsize;

	if (name == NULL)
		return EFAULT;

	buffsize = SYS_NMLN;
	GetComputerName(name->machine, &buffsize);
	strcpy(name->nodename, name->machine);

	verinfo.dwOSVersionInfoSize = sizeof(verinfo);
	GetVersionEx(&verinfo);

	sprintf(name->version, "%d.%d.%d", verinfo.dwMajorVersion, verinfo.dwMinorVersion, verinfo.dwBuildNumber);
	strcpy(name->release, name->version);

	sprintf(name->sysname, "Windows");

	return 0;
}*/
