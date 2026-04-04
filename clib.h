#ifndef CLIB_H
#define CLIB_H

#include <stddef.h>

typedef struct {
  char *first_char;
  size_t len;
} String;

typedef struct {
  size_t len;
  size_t item_size;
  void *data;
} Array;

// String

String string_new(char *first_char);

void string_remove_left(String *s, size_t nchar);
void string_remove_right(String *s, size_t nchar);

int count_space_left(String *s);
int count_space_right(String *s);

void string_trim_left(String *s);
void string_trim_right(String *s);
void string_trim(String *s);

size_t string_find(String *s, String *delim);
void string_split_delim(String s, String delim, int nsplit);

// Array

Array *array_new(size_t len, size_t item_size);
Array *array_conversion(const void *input, size_t len, size_t elem_size);
void array_free(Array *arr);
Array *array_concat(const Array *arr1, const Array *arr2);
Array **array_split(const Array *arr, size_t index);
void array_push(Array *arr, const void *item, size_t index);
void array_pop(Array *arr, const void *item, size_t index);

#endif // CLIB_H
