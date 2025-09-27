#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include "buffer.h"

#define MAX_BUFFER_SIZE 1024

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

int extract_next_header_key(char* buf, size_t buf_size, char* end_of_buf, char** it_ptr)
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
  while ( it < end_of_buf && *it != ':' )
  {
    ++it;
  }
  ptrdiff_t length = it - start;
  snprintf(buf, buf_size, "%.*s", (int)length, start);
  ++it;

  *it_ptr = it;
  return 1;
}

int extract_next_header_value(char* buf, size_t buf_size, char* end_of_buf, char** it_ptr)
{
  char* it = *it_ptr;

  while (it < end_of_buf && isspace(*it))
  {
    ++it;
  }
  char* start=it;
  while (it < end_of_buf && *it != '\r') {
    ++it; 
  }
  if (it >= end_of_buf) {
    return 0;
  }
  ptrdiff_t length = it - start;
  snprintf(buf,buf_size,"%.*s",(int)length,start);
  *it_ptr = it;

  return 1;
}

int extract_body(char* buf, size_t buf_size, char* end_of_buf, char** it_ptr) {
  char* it = *it_ptr;
  char* start = it;
  while (it < end_of_buf)
  {
    ++it;
  }
  if (it != end_of_buf) 
  {
    return 0;
  }
  ptrdiff_t length = it - start;
  snprintf(buf,buf_size,"%.*s",(int)length,start);
  *it_ptr = it;
  return 1;
}
