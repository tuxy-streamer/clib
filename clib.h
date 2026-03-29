#ifndef CLIB_H
#define CLIB_H

#include <stddef.h>

// String

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

size_t string_find(String s, String delim);
void string_split_delim(String s, String delim, int nsplit);

// Array

typedef struct {
  size_t length;
  size_t size;
  void **data;
} Array;

Array array_new(size_t length);
Array array_conversion(void *input, size_t length, size_t elem_size);
void array_free(Array *arr);
Array array_concat(Array arr1, Array arr2);
void array_push(Array *arr, const void *item, size_t index);
void array_pop(Array *arr, const void *item, size_t index);

#endif // CLIB_H
