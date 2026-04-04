#include "./clib.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void log_success(const char *msg) { printf("\x1b[32m%s\x1b[0m\n", msg); }

static void test_string_new(void) {
  char *input[] = {"Hello", " Hello"};
  size_t count = sizeof(input) / sizeof(input[0]);
  for (size_t i = 0; i < count; ++i) {
    String s = string_new(input[i]);
    assert(strlen(input[i]) == s.len);
  }
  log_success("Passed: test_string_new");
}

static void test_string_find(void) {
  String input = string_new("Hello, World!");
  String comma = string_new(",");
  size_t index = string_find(&input, &comma);
  assert(index == 5);
  String wor = string_new("Wor");
  index = string_find(&input, &wor);
  assert(index == 7);
  log_success("Passed: test_string_find");
}

static void test_array_new(void) {
  int input[3] = {1, 2, 3};
  size_t len = sizeof input / sizeof input[0];
  Array *arr = array_new(len, sizeof input[0]);
  assert(arr);
  int *p = (int *)arr->data;
  for (size_t i = 0; i < len; ++i) {
    p[i] = input[i];
  }
  for (size_t i = 0; i < arr->len; ++i) {
    assert(p[i] == input[i]);
  }
  array_free(arr);
  log_success("Passed: test_array_new");
}

static void test_array_concat(void) {
  int input1[3] = {1, 2, 3};
  Array *arr1 = array_conversion(input1, sizeof(input1) / sizeof input1[0],
                                 sizeof input1[0]);
  assert(arr1);
  int input2[3] = {4, 5, 6};
  Array *arr2 = array_conversion(input2, sizeof(input2) / sizeof input2[0],
                                 sizeof input2[0]);
  assert(arr2);
  Array *ac = array_concat(arr1, arr2);
  assert(ac);
  assert(ac->len == arr1->len + arr2->len);
  int *p_src1 = (int *)arr1->data;
  int *p_dst = (int *)ac->data;
  for (size_t i = 0; i < arr1->len; ++i) {
    assert(p_src1[i] == p_dst[i]);
  }
  int *p_src2 = (int *)arr2->data;
  for (size_t i = 0; i < arr2->len; ++i) {
    assert(p_src2[i] == p_dst[arr1->len + i]);
  }
  array_free(arr1);
  array_free(arr2);
  array_free(ac);
  log_success("Passed: test_array_concat");
}

static void test_array_split(void) {
    int input[6] = {1, 2, 3, 4, 5, 6};
    size_t len = sizeof input / sizeof input[0];
    Array *arr = array_conversion(input, len, sizeof input[0]);
    assert(arr);
    Array **arr_comb = array_split(arr, len - 1);
    assert(arr_comb);
    Array *arr1 = arr_comb[0];
    Array *arr2 = arr_comb[1];
    int *orig = (int *)arr->data;
    int *p1 = (int *)arr1->data;
    for (size_t i = 0; i < arr1->len; ++i) {
        assert(p1[i] == orig[i]);
    }
    int *p2 = (int *)arr2->data;
    for (size_t i = 0; i < arr2->len; ++i) {
        assert(p2[i] == orig[arr1->len + i]);
    }
    array_free(arr1);
    array_free(arr2);
    free(arr_comb);
    array_free(arr);
    log_success("Passed: test_array_split");
}

static void test_array_push(void) {
  int input[3] = {1, 2, 3};
  size_t len = sizeof input / sizeof input[0];
  Array *arr = array_conversion(input, len, sizeof input[0]);
  assert(arr);
  int input1 = 4;
  int input2 = 5;
  array_push(arr, &input1, 3);
  array_push(arr, &input2, 4);
  int *data = (int *)arr->data;
  assert(arr->len == 5);
  assert(data[3] == input1);
  assert(data[4] == input2);
  int mid = 99;
  array_push(arr, &mid, 2);
  data = (int *)arr->data;
  assert(data[2] == 99);
  assert(arr->len == 6);
  array_free(arr);
  log_success("Passed: test_array_push");
}

int main(void) {
  test_string_new();
  test_string_find();
  test_array_new();
  test_array_concat();
  test_array_split();
  test_array_push();
  log_success("Passed: All Tests.");
  return 0;
}
