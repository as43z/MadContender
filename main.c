#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gmp.h"

// Error Codes
#define NO_ERROR 0
#define E_INTEGER_TOO_LARGE 1000

void raise_error(int err_code) {
	switch ( err_code ) {
	case E_INTEGER_TOO_LARGE:
		fprintf(stderr, "E_INTEGER_TOO_LARGE raised");
		break;
	default:
		assert(0 && "unreachable");
		break;
	}
}
// Macros
#define LEN(c) (sizeof(c)/c[0])

// Data Conversion Primitives 

/*
Integer to Octet-String Primitive
*/
int i2osp(mpz_t x, char *c, size_t l) {
	mpz_t maxl;
	mpz_init(maxl);
	mpz_ui_pow_ui(maxl, 256, l);

	if (mpz_cmp(x, maxl) >= 0)
		return E_INTEGER_TOO_LARGE;

	mpz_t bp;
	mpz_init(bp);
	for (size_t i = 0; i < l; i++) {
		// c[i] = ( x >> i*8 ) & 0xFF; // We need octets
		mpz_mod_ui(bp, x, 256);	
		char b = (char) mpz_get_ui(bp);
		mpz_divexact_ui(x, x, 256);
		c[i] = b;
	}

	// Free
	mpz_clear(maxl);
	mpz_clear(bp);
	return NO_ERROR;
} 

/*
Octet-String to Integer Primitive
*/
void os2ip(char *c, size_t l, mpz_t d) {
	mpz_t bp;
	mpz_init(bp);
	// x = x_(i-1) * (256^(i-1)) + x_(i-2) * (256^(i-2)) + ... + x_0 * (256^0)
	for (size_t i = l; i > 0; i--) {
		// d += c[i - 1]*256^(i-1) 
		mpz_ui_pow_ui(bp, 256, i - 1);
		mpz_mul_ui(bp, bp, c[i-1]);
		mpz_add(d, bp, d);
	}

	// Free
	mpz_clear(bp);
}

int main(void) {
	char *input = "Hi.";
	size_t l = strlen(input);
	char *output = malloc(sizeof(char)*l);

	mpz_t d;
	mpz_init(d);

	// Encode
	os2ip(input, l, d);
	char *repr = mpz_get_str(NULL, 10, d);
	printf("os2ip(%s) = %s\n", input, repr);

	// Try to decode it.
	if (i2osp(d, output, l))
		raise_error(E_INTEGER_TOO_LARGE);
	else
		printf("i2osp(%s) = %s\n", repr, output);

	mpz_clear(d);
	free(output);
	return 0;
}
