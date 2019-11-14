#include "randlib.h"
#include <stdio.h>
#include <stdlib.h>

/* Input stream containing random bytes.  */
static FILE *urandstream;

/* Initialize the software rand64 implementation.  */
__attribute__ ((constructor)) static void // added __attribute__ ((constructor))
                                          // like the instructions
software_rand64_init (void)
{
    urandstream = fopen ("/dev/urandom", "r");
    if (! urandstream)
        abort ();
}

/* Return a random value, using software operations.  */
extern unsigned long long // changed static to extern
rand64 (void) // changed software_rand64 to my own function
{
    unsigned long long int x;
    if (fread (&x, sizeof x, 1, urandstream) != 1)
        abort ();
    return x;
}

/* Finalize the software rand64 implementation.  */
__attribute__ ((destructor)) static void // added __attribute__ ((destructor))
                                         // like the instructions
software_rand64_fini (void)
{
    fclose (urandstream);
}