#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int gethostname(char *name, size_t len)
{
  char *h;
  h= getenv("HOSTNAME");
  if (h==NULL) h="xv6";
  strncpy(name, h, len-1);
  return(0);
}

int sethostname(const char *name, size_t len)
{
  return(0);
}
