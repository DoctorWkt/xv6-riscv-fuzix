/* strerror - map error number to descriptive string
 *
 * This comes from Minix 1.5.
 */

#include <string.h>

int sys_nerr= 60;

char *sys_errlist[] = {
        "Error 0",
        "Not owner",
        "No such file or directory",
        "No such process",
        "Interrupted system call",
        "I/O error",
        "No such device or address",
        "Arg list too long",
        "Exec format error",
        "Bad file number",
        "No children",
        "No more processes",
        "Not enough core",
        "Permission denied",
        "Bad address",
        "Block device required",
        "Mount device busy",
        "File exists",
        "Cross-device link",
        "No such device",
        "Not a directory",
        "Is a directory",
        "Invalid argument",
        "File table overflow",
        "Too many open files",
        "Not a typewriter",
        "Text file busy",
        "File too large",
        "No space left on device",
        "Illegal seek",
        "Read-only file system",
        "Too many links",
        "Broken pipe",
        "Argument too large",
        "Result too large",
	"Lock table full",
	"Directory is not empty",
	"File name too long",
	"Address family not supported",
	"Operation already in progress",
	"Address already in use",
	"Address not available",
	"No such system call",
	"Protocol not supported",
	"Operation not supported on transport endpoint",
	"Connection reset by peer",
	"Network is down",
	"Message too long",
	"Connection timed out",
	"Connection refused",
	"No route to host",
	"Host is down",
	"Network is unreachable",
	"Transport endpoint is not connected",
	"Operation now in progress",
	"Cannot send after transport endpoint shutdown",
	"Socket is already connected",
	"No destination address specified",
	"No buffer space available",
	"Protocol not supported"
};

char *strerror(errnum)
int errnum;
{
  extern int sys_nerr;
  extern char *sys_errlist[];

  if (errnum > 0 && errnum < sys_nerr)
        return(sys_errlist[errnum]);
  else
        return("unknown error");
}
