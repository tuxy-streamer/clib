#ifndef CLIB_H
#define CLIB_H

#include <stddef.h>

typedef struct {
  char *first_char;
  size_t length;
} String;

String string_new(char *first_char);

void string_remove_left(String *s, size_t nchar);
void string_remove_right(String *s, size_t nchar);

int count_space_left(String s);
int count_space_right(String s);

void string_trim_left(String *s);
void string_trim_right(String *s);
void string_trim(String *s);


#endif //CLIB_H
