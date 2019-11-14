#include "randlib.h"
#include <immintrin.h>

/* Return a random value */
extern unsigned long long // Changed static to extern
rand64(void) {
    unsigned long long int x;
    while (!_rdrand64_step (&x))
        continue;
    return x;
}