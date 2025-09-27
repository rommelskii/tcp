#ifndef BUFFER_H
#define BUFFER_H

int extract_next_keyword(char* buf, size_t buf_size, char* end_of_buf, char** it_ptr);
int extract_next_header_key(char* buf, size_t buf_size, char* end_of_buf, char** it_ptr);
int extract_next_header_value(char* buf, size_t buf_size, char* end_of_buf, char** it_ptr);
int extract_body(char* buf, size_t buf_size, char* end_of_buf, char** it_ptr);



#endif //BUFFER_H
