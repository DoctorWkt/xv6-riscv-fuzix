#include <limits.h>
#include <unistd.h>

int getpagesize(void)
{ return(4096); }

int getdtablesize(void)
{ return(_POSIX_OPEN_MAX); }
