#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "./clib.h"
#include <assert.h>

extern int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    /* Create a NUL-terminated buffer from the fuzzer input */
    char buf[size + 1];
    if (size > 0) memcpy(buf, data, size);
    buf[size] = '\0';

    /* Call the function under test */
    String s = string_new(buf);

    /* Invariants to check */
    // pointer should point into our buffer
    assert(s.first_char == buf);
    // length must match strlen
    assert(s.length == strlen(buf));

    /* Extra sanity: length shouldn't exceed size (it can't because we NUL-terminated) */
    assert(s.length <= size);

    return 0;
}
