#include "./clib.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void log_success(char *msg) { printf("\x1b[32m%s\x1b[0m\n", msg); }

static void test_string_new(void) {
  char *input[] = {"Hello", " Hello"};
  size_t count = sizeof(input) / sizeof(input[0]);
  for (size_t i = 0; i < count; ++i) {
    String s = string_new(input[i]);
    assert(strlen(input[i]) == s.length);
  }
  log_success("Passed: test_string_new");
}

int main(void) {
  test_string_new();
  log_success("Passed: All Tests.");
  return 0;
}
