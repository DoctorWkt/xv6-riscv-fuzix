/*
 * Copyright (c) 1993 by David I. Bell
 * Permission is granted to use, distribute, or modify this source,
 * provided that this copyright notice remains intact.
 *
 * Stdio usage removed Alan Cox 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Because xv6 doesn't set the environ, we simulate it here
#define ENVLEN 100
#define ENV_FILE "/etc/env"
#define ENV_LINE_LEN 200

static char * xv6env[ENVLEN];
extern char **environ;

void loadxv6env()
{
  FILE *in;
  char env_line[ENV_LINE_LEN];
  int posn=0;

    // Try to open the env file
  if ((in=fopen(ENV_FILE, "r"))==NULL) return;

  while (fgets(env_line, ENV_LINE_LEN-1, in)!=NULL) {
    // Ignore comments
    if (env_line[0] == '#') continue;

    // Stop if we run out of room
    if (posn==ENVLEN) break;

    // Remove the newline
    env_line [ strlen(env_line) - 1] = '\0';

    // Copy the variable into the array
    xv6env[posn++]=strdup(env_line);
  }
  fclose(in);
  environ= xv6env;
  return;
}


void writesnl(const char *p)
{
    write(1, p, strlen(p));
    write(1, "\n", 1);
}

int main(int argc, char *argv[])
{
    char **env;
    extern char **environ;
    int len;

    loadxv6env();

    env = environ;

    if (argc == 1) {
	while (*env)
	    writesnl(*env++);
	return 0;
    }

    len = strlen(argv[1]);
    while (*env) {
	if ((strlen(*env) > len) && (env[0][len] == '=') &&
	    (memcmp(argv[1], *env, len) == 0)) {
	    writesnl(&env[0][len + 1]);
	    return 0;
	}
	env++;
    }
    
    return 1;
}
