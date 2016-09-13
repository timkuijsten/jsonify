#include "jsonify.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 16 * 1024

int main()
{
  char *line = NULL;
  size_t linesize = 0;
  ssize_t linelen;
  char output[MAXLEN];

  while ((linelen = getline(&line, &linesize, stdin)) != -1) {
    relaxed_to_strict(output, MAXLEN, line, linelen - 1);
    printf("%s\n", output);
  }
  free(line);

  return 0;
}
