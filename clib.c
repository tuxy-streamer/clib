#include "./clib.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
