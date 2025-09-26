#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include "buffer.h"

int extract_next_keyword(char* buf, size_t buf_size, char* end_of_buf, char** it_ptr)
{
  char* it = *it_ptr;
  while ( it < end_of_buf && isspace(*it) )
  {
    ++it;
  }
  if ( it >= end_of_buf )
  {
    return 0;
  }
  char* start = it;
  while ( it < end_of_buf && !isspace(*it) )
  {
    ++it;
  }
  ptrdiff_t length = it - start;
  snprintf(buf, buf_size, "%.*s", (int)length, start);

  *it_ptr = it;
  return 1;
}

