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
  Array arr = array_conversion(&input, sizeof(input) / sizeof(input[0]),
                               sizeof(input[0]));
  for (size_t i = 0; i < arr.length; ++i) {
    arr.data[i] = (void *)&input[i];
  }
  for (size_t i = 0; i < arr.length; ++i) {
    int *p = (int *)arr.data[i];
    assert(p != NULL);
    assert(*p == input[i]);
  }
  array_free(&arr);
  log_success("Passed: test_array_new");
}

static void test_array_concat(void) {
  int input1[3] = {1, 2, 3};
  Array arr1 = array_conversion(&input1, sizeof(input1) / sizeof(input1[0]),
                                sizeof(input1[0]));
  int input2[3] = {4, 5, 6};
  Array arr2 = array_conversion(&input2, sizeof(input2) / sizeof(input2[0]),
                                sizeof(input2[0]));
  for (size_t i = 0; i < arr2.length; ++i) {
    arr2.data[i] = &input2[i];
  }
  Array ac = array_concat(arr1, arr2);
  assert(ac.length == arr1.length + arr2.length);
  /* verify first part matches arr1 */
  for (size_t i = 0; i < arr1.length; ++i) {
    int *p_src = (int *)arr1.data[i];
    int *p_dst = (int *)ac.data[i];
    assert(p_src != NULL && p_dst != NULL);
    assert(*p_src == *p_dst);
  }
  /* verify second part matches arr2 */
  for (size_t i = 0; i < arr2.length; ++i) {
    int *p_src = (int *)arr2.data[i];
    int *p_dst = (int *)ac.data[arr1.length + i];
    assert(p_src != NULL && p_dst != NULL);
    assert(*p_src == *p_dst);
  }
  array_free(&arr1);
  array_free(&arr2);
  /* array_concat performed a shallow copy of pointers; free its data only */
  array_free(&ac);
  log_success("Passed: test_array_concat");
}

static void test_array_split(void) {
  int input[6] = {1, 2, 3, 4, 5, 6};
  size_t len = sizeof input / sizeof input[0];
  Array arr = array_conversion(&input, len, sizeof input[0]);
  Array *arr_comb = array_split(arr, len - 1);
  Array arr1 = arr_comb[0];
  Array arr2 = arr_comb[1];
  for (size_t i = 0; i < arr1.length; ++i) {
    int *p_src = (int *)arr1.data[i];
    int *p_dst = (int *)arr.data[i];
    assert(p_src && p_dst);
    assert(*p_src == *p_dst);
  }
  for (size_t i = 0; i < arr2.length; ++i) {
    int *p_src = (int *)arr2.data[i];
    int *p_dst = (int *)arr.data[arr1.length + i];
    assert(p_src && p_dst);
    assert(*p_src == *p_dst);
  }
  array_free(&arr1);
  array_free(&arr2);
  array_free(&arr);
  free(arr_comb);
  log_success("Passed: test_array_split");
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
  test_array_concat();
  test_array_split();
  // test_array_push();
  log_success("Passed: All Tests.");
  return 0;
}
