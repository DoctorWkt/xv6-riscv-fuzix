#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

void cprintf(char *fmt, ...);

char **environ=NULL;

#define BUFLEN 200
static char _execfullpath[BUFLEN];	// Full path name of executable
static char _interppath[BUFLEN];	// Full path name of #! interpreter

extern int exec(const char *path, char *const argv[]);

int execl(const char *path, const char *arg, ...)
{
  return exec(path, (char * const *) &arg);
}
                       
int execle(const char *path, const char *arg, ...)
{
  return exec(path, (char * const *) &arg);
}

int execv(const char *path, char *const argv[])
{
  return(exec(path, argv));
}

int execve(const char *path, char *const argv[], char *const envp[])
{
  return(execv(path, argv));
}

// Given a filename, return the name with "/bin" prepended if
// the filename doesn't exist. Return NULL if neither exist.
static char *expand_filename(char *file) {
  int fd;

  // Error if no filename
  if (file==NULL) return(NULL);

  // Try to open the file
  if ((fd= open(file, O_RDONLY)) == -1) {
    // Not there, try in /bin
    strcpy(_execfullpath, "/bin/");
    strncpy(&(_execfullpath[5]), file, BUFLEN-6);
    file= _execfullpath;
    if ((fd= open(file, O_RDONLY)) == -1) {
      // Not there either
      return(NULL);
    }
  }
  close(fd); return(file);
}

// For execlp() we prepend /bin if needed but
// we don't deal with #!
int execlp(const char *path, const char *arg, ...)
{
  const char *f= expand_filename((char *)path);
  if (f==NULL) { errno= EACCES; return(-1); }
  return exec(f, (char * const *) &arg);
}

// For execvp(), we prepend /bin if needed and
// also deal with #!
int execvp(const char *path, char *const argv[])
{
  int fd, cnt, argc=0;
  char *cptr;
  char **newargv= (char **)argv;
  char *f= expand_filename((char *)path);
  if (f==NULL) { errno= EACCES; return(-1); }

  // The file f exists. See if it's a script
  fd= open(f, O_RDONLY);

  // Read in the first line
  if ((cnt= read(fd, _interppath, BUFLEN-1)) < 1) {
    // Nothing in the file, or a read error.
    close(fd); errno= EACCES; return(-1);
  } 
  close(fd);

  // If the buffer starts with #!, find the newline and NUL it.
  // Mark it as the filename to exec()
  if ((_interppath[0] == '#') && (_interppath[1] == '!')) {
    cptr= strchr(_interppath, '\n');
    if (cptr!=NULL) *cptr=0;
    f= &_interppath[2];

    // We now need to prepend the interpreter's name to argv.
    // Count the number of arguments
    while (argv[argc] != NULL) argc++;

    // Get new memory for the arguments, copy the
    // old ones and put the interpreter at the start.
    // Make sure to copy the NULL at the end of the argv!
    newargv= (char **) malloc((argc+2) * sizeof(char *));
    if (newargv == NULL) { errno= E2BIG; return(-1); }
    memcpy(&newargv[1], argv, (argc+1) * sizeof(char *));
    newargv[0]= f;
  }

  // Finally execute the file with its arguments
  return(exec(f, newargv));
  return(-1);
}

int execvpe(const char *path, char *const argv[], char *const envp[])
{
  return(execvp(path, argv));
}
