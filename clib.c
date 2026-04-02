#include "./clib.h"
#include <asm-generic/errno-base.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Error checks

static int err_check(bool condition, const char fn_name[], const char *msg,
                     int err) {
  if (!condition)
    return 0;
  fprintf(stderr, "%s: errno = %d - %s\n", fn_name, err, msg);
  errno = err;
  return 1;
}

static int mem_err_check(void *mem_ptr, const char fn_name[], const char *msg) {
  return err_check(mem_ptr == NULL, fn_name, msg, ENOMEM);
}

static int input_err_check(bool condition, const char fn_name[],
                           const char *msg) {
  return err_check(condition, fn_name, msg, EINVAL);
}

static int is_mem_ovf_check(const char fn[], size_t len, size_t item_size) {
  if (input_err_check(item_size == 0, fn, "zero item_size"))
    return 1;
  if (err_check(len > SIZE_MAX / item_size, fn,
                "len and item_size can overflow", ENOMEM))
    return 1;
  return 0;
}

// String

/**
 * @brief Convert string literal to String
 *
 * @param c String Literal
 * @return String
 */
String string_new(char *c) {
  String string = {.first_char = c, .len = strlen(c)};
  return string;
}

/**
 * @brief Remove `nchar` from the right of 's'
 *
 * @param s Text
 * @param nchar no of characters
 */
void string_remove_right(String *s, size_t nchar) {
  if (nchar > s->len) {
    nchar = s->len;
  }

  s->len -= nchar;
}

/**
 * @brief Remove `nchar` from the left of 's'
 *
 * @param s Text
 * @param nchar no of characters
 */
void string_remove_left(String *s, size_t nchar) {
  if (nchar > s->len) {
    nchar = s->len;
  }
  s->len -= nchar;
  s->first_char += nchar;
}

/**
 * @brief Count spaces from left of 's'
 *
 * @param s Text
 */
int count_space_left(String s) {
  assert(s.len > 0);
  int counter = 0;
  for (size_t i = 0; i <= s.len; ++i) {
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
  assert(s.len > 0);
  int counter = 0;
  for (size_t i = s.len; i > 0; --i) {
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
  for (size_t i = 0; i < s.len; ++i) {
    if (s.first_char[i] == delim.first_char[0]) {
      for (size_t j = i; j < delim.len; ++j) {
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

Array *array_new(size_t len, size_t item_size) {
  const char *fn = __func__;
  // INFO: Input validation
  if (item_size != 0 && len > SIZE_MAX / item_size)
    return NULL;
  Array *out = malloc(sizeof(*out));
  if (mem_err_check(out, fn, "array allocation failed"))
    return NULL;
  out->len = len;
  out->item_size = item_size;
  out->data = calloc(len, item_size);
  if (mem_err_check(out->data, fn, "data allocation failed")) {
    array_free(out);
    return NULL;
  }
  return out;
}

Array *array_conversion(const void *a, size_t len, size_t item_size) {
  const char *fn = __func__;
  if (input_err_check(a == NULL, fn, "NULL array passed"))
    return NULL;
  Array *out = array_new(len, item_size);
  if (mem_err_check(out, fn, "array allocation failed"))
    return NULL;
  memcpy(out->data, a, len * item_size);
  return out;
}

void array_free(Array *arr) {
  if (input_err_check(arr == NULL, __func__, "NULL array passed"))
    return;
  free(arr->data);
  free(arr);
}

Array *array_concat(const Array *a, const Array *b) {
  const char *fn = __func__;
  // INFO: Input validation
  if (err_check(!a || !b, fn, "invalid source arrays", EINVAL))
    return NULL;
  if (err_check(a->item_size != b->item_size, fn, "item_size mismatch", EINVAL))
    return NULL;
  if (is_mem_ovf_check(fn, a->len, a->item_size))
    return NULL;
  if (is_mem_ovf_check(fn, b->len, b->item_size))
    return NULL;
  if (is_mem_ovf_check(fn, a->len + b->len, a->item_size))
    return NULL;
  size_t a_size = a->len * a->item_size;
  size_t b_size = b->len * b->item_size;
  Array *out = array_new(a->len + b->len, a->item_size);
  if (mem_err_check(out, fn, "array allocation failed"))
    return NULL;
  if (a_size > 0)
    memcpy(out->data, a->data, a_size);
  if (b_size > 0)
    memcpy((char *)out->data + a_size, b->data, b_size);
  return out;
}

Array **array_split(const Array *a, size_t index) {
    if (!a || a->item_size == 0) return NULL;
    if (index > a->len) index = a->len;
    size_t left_count = index;
    size_t right_count = a->len - left_count;
    Array **out = malloc(2 * sizeof(*out));
    if (!out) return NULL;
    Array *left = array_new(left_count, a->item_size);
    if (!left) { free(out); return NULL; }
    Array *right = array_new(right_count, a->item_size);
    if (!right) { array_free(left); free(out); return NULL; }
    if (left_count > 0)
        memcpy(left->data, a->data, left_count * a->item_size);
    if (right_count > 0)
        memcpy(right->data, (char *)a->data + left_count * a->item_size,
               right_count * a->item_size);
    out[0] = left;
    out[1] = right;
    return out;
}

void array_push(Array *a, const void *item, size_t index) {
  const char *fn = __func__;
  if (!a || !item)
    return;
  if (a->item_size == 0)
    return;
  if (index > a->len)
    index = a->len;
  Array **parts = array_split(a, index);
  if (!parts)
    return;
  Array *left = parts[0];
  Array *right = parts[1];
  size_t new_left_len = left->len + 1;
  size_t new_left_bytes = new_left_len * left->item_size;
  void *tmp = realloc(left->data, new_left_bytes);
  if (mem_err_check(tmp, fn, "realloc failed")) {
    free(left->data);
    free(right->data);
    free(parts);
    return;
  }
  left->data = tmp;
  char *dest = (char *)left->data + left->len * left->item_size;
  memcpy(dest, item, left->item_size);
  left->len = new_left_len;
  Array *new_arr = array_concat(left, right);
  if (!new_arr) {
    free(left->data);
    free(right->data);
    free(parts);
    return;
  }
free(a->data);
a->data = new_arr->data;
a->len = new_arr->len;
a->item_size = new_arr->item_size;
array_free(left);
array_free(right);
free(new_arr);
free(parts);
}
