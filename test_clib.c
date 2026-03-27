#include "./clib.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
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

static void test_string_find(void) {
  String input = string_new("Hello, World!");
  size_t index = string_find(input, string_new(","));
  assert(index == 5);
  index = string_find(input, string_new("Wor"));
  assert(index == 7);
  log_success("Passed: test_string_find");
}

static void test_array_new(void) {
  int input[3] = {1, 2, 3};
  const void *src_arr = input;
  Array arr = array_new(src_arr, 2, INT_T);
  int *original = (int *)src_arr;
  int *copy = (int *)arr.data;
  for (size_t i = 0; i < arr.length; ++i) {
    assert(copy[i] == original[i]);
  }
  array_free(&arr);
  log_success("Passed: test_array_new");
}

// static void test_array_push(void) {
//   int input[3] = {1, 2, 3};
//   const void *src_arr = input;
//   Array arr = array_new(src_arr, 3, INT_T);
//   int input1 = 4;
//   int input2 = 5;
//   array_push(&arr, &input1, 3);
//   array_push(&arr, &input2, 4);
//   int *data = (int *)arr.data;
//   assert(data[3] == input1);
//   log_success("Passed First");
//   assert(data[4] == input2);
//   log_success("Passed Second");
//   array_free(&arr);
//   log_success("Passed: test_array_push");
// }
//
int main(void) {
  test_string_new();
  test_string_find();
  test_array_new();
  // test_array_push();
  log_success("Passed: All Tests.");
  return 0;
}
