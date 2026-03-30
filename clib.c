#include "./clib.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// String

/**
 * @brief Convert string literal to String
 *
 * @param c String Literal
 * @return String
 */
String string_new(char *c) {
  String string = {.first_char = c, .length = strlen(c)};
  return string;
}

/**
 * @brief Remove `nchar` from the right of 's'
 *
 * @param s Text
 * @param nchar no of characters
 */
void string_remove_right(String *s, size_t nchar) {
  if (nchar > s->length) {
    nchar = s->length;
  }

  s->length -= nchar;
}

/**
 * @brief Remove `nchar` from the left of 's'
 *
 * @param s Text
 * @param nchar no of characters
 */
void string_remove_left(String *s, size_t nchar) {
  if (nchar > s->length) {
    nchar = s->length;
  }
  s->length -= nchar;
  s->first_char += nchar;
}

/**
 * @brief Count spaces from left of 's'
 *
 * @param s Text
 */
int count_space_left(String s) {
  assert(s.length > 0);
  int counter = 0;
  for (size_t i = 0; i <= s.length; ++i) {
    if (!isspace(s.first_char[i])) {
      break;
    };
    counter += 1;
  }
  return counter;
}

/**
 * @brief Count spaces from right of 's'
 *
 * @param s Text
 */
int count_space_right(String s) {
  assert(s.length > 0);
  int counter = 0;
  for (size_t i = s.length; i > 0; --i) {
    if (!isspace(s.first_char[i])) {
      break;
    };
    counter += 1;
  }
  return counter;
}

/**
 * @brief Trim spaces from right of 's'
 *
 * @param s Text
 */
void string_trim_right(String *s) {
  int space_count = count_space_right(*s);
  string_remove_right(s, space_count);
}

/**
 * @brief Trim spaces from left of 's'
 *
 * @param s Text
 */
void string_trim_left(String *s) {
  int space_count = count_space_left(*s);
  string_remove_left(s, space_count);
}

/**
 * @brief Trim spaces from left and right of 's'
 *
 * @param s Text
 */
void string_trim(String *s) {
  string_trim_left(s);
  string_trim_right(s);
}

size_t string_find(String s, String delim) {
  for (size_t i = 0; i < s.length; ++i) {
    if (s.first_char[i] == delim.first_char[0]) {
      for (size_t j = i; j < delim.length; ++j) {
        if (s.first_char[j] != delim.first_char[j]) {
          break;
        }
      }
      return i;
    }
  }
  return 0;
}

// Array

Array array_new(size_t length) {
  Array arr = {
      .length = length,
      .size = sizeof(void *) * length,
      .data = malloc(sizeof(void *) * length),
  };
  if (!arr.data)
    abort();
  return arr;
}

Array array_conversion(void *input, size_t length, size_t elem_size) {
  Array arr = array_new(length);
  char *base = (char *)input;
  for (size_t i = 0; i < length; ++i)
    arr.data[i] = (void *)(base + i * elem_size);
  return arr;
}

void array_free(Array *arr) {
  if (!arr)
    return;
  free(arr->data);
  arr->length = 0;
  arr->size = 0;
  arr->data = NULL;
}

Array array_concat(Array arr1, Array arr2) {
  size_t len1 = arr1.length;
  size_t len2 = arr2.length;
  Array arr_concat = {
      .length = len1 + len2,
      .size = (len1 + len2) * sizeof(void *),
      .data = calloc(len1 + len2, sizeof(void *)),
  };
  if (!arr_concat.data)
    abort();
  memcpy(arr_concat.data, arr1.data, arr1.size);
  memcpy(arr_concat.data + len1, arr2.data, arr2.size);
  return arr_concat;
}

Array *array_split(Array arr, size_t index) {
  if (arr.length == 0) {
    Array *ret = malloc(2 * sizeof *ret);
    if (!ret)
      abort();
    ret[0] = array_new(0);
    ret[1] = array_new(0);
    return ret;
  }
  if (index >= arr.length)
    index = arr.length - 1;
  size_t left_count = index + 1;
  size_t right_count = arr.length - left_count;
  Array *ret_arr = malloc(2 * sizeof *ret_arr);
  if (!ret_arr)
    abort();
  ret_arr[0] = array_new(left_count);
  ret_arr[1] = array_new(right_count);
  if (left_count > 0) {
    memcpy(ret_arr[0].data, arr.data, left_count * sizeof(void *));
  }
  if (right_count > 0) {
    memcpy((char *)ret_arr[1].data,
           (char *)arr.data + left_count * sizeof(void *),
           right_count * sizeof(void *));
  }
  return ret_arr;
}

// WARN: Array push doesn't work
// void array_push(Array *arr, const void *item, size_t index) {
//   size_t item_size = arr->size / arr->length;
//   if (index > arr->length)
//     index = arr->length;
//   size_t new_length = arr->length + 1;
//   size_t new_bytes = new_length * item_size;
//   void *new_data = realloc(arr->data, new_bytes);
//   arr->data = new_data;
//   size_t tail_count = arr->length - index;
//   if (tail_count > 0) {
//     void *base = (char *)arr->data;
//     void *src = base + index * item_size;
//     void *dst = base + (index + 1) * item_size;
//     memmove(dst, src, tail_count * item_size);
//   }
//   void *target = (char *)arr->data + index * item_size;
//   memcpy(target, item, item_size);
//   arr->length = new_length;
// }
