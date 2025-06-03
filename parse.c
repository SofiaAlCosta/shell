#include "shell.h"

int parse(char *buf, char **args)
{
  int cnt = 0;
  while (*buf != '\0')
  {
    while (*buf == ' ' || *buf == '\t')
      *buf++ = '\0';
    if (*buf != '\0')
    {
      *args++ = buf;
      cnt++;
    }
    while (*buf != '\0' && *buf != ' ' && *buf != '\t')
      buf++;
  }
  *args = NULL;
  return cnt;
}