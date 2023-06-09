/*
 * Copyright (c) 2008-2010 Stefan Krah. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


#ifndef UMODARITH_H
#define UMODARITH_H


#include "constants.h"
#include "mpdecimal.h"
#include "typearith.h"


/**************************************************************************/
/*                      ANSI C99 modular arithmetic                       */
/**************************************************************************/


/*
 * Restrictions: a < m and b < m
 * ACL2 proof: umodarith.lisp: addmod-correct
 */
static INLINE mpd_uint_t
addmod(mpd_uint_t a, mpd_uint_t b, mpd_uint_t m)
{
	mpd_uint_t s;

	s = a + b;
	s = (s < a) ? s - m : s;
	s = (s >= m) ? s - m : s;

	return s;
}

/*
 * Restrictions: a < m and b < m
 * ACL2 proof: umodarith.lisp: submod-2-correct
 */
static INLINE mpd_uint_t
submod(mpd_uint_t a, mpd_uint_t b, mpd_uint_t m)
{
	mpd_uint_t d;

	d = a - b;
	d = (a < b) ? d + m : d;

	return d;
}

/*
 * Restrictions: a < 2m and b < 2m
 * ACL2 proof: umodarith.lisp: ext-submod-2-correct
 */
static INLINE mpd_uint_t
ext_submod(mpd_uint_t a, mpd_uint_t b, mpd_uint_t m)
{
	mpd_uint_t d;

	a = (a >= m) ? a - m : a;
	b = (b >= m) ? b - m : b;

	d = a - b;
	d = (a < b) ? d + m : d;

	return d;
}

/* divide 2 words by v, return remainder */
static INLINE mpd_uint_t
bigmod2(mpd_uint_t hi, mpd_uint_t lo, mpd_uint_t v)
{
	mpd_uint_t r1, r2, w;

	_mpd_div_word(&w, &r1, hi, v);
	_mpd_div_words(&w, &r2, r1, lo, v);

	return r2;
}

/* subtract double word hi,lo from a */
static INLINE mpd_uint_t
dw_submod(mpd_uint_t a, mpd_uint_t hi, mpd_uint_t lo, mpd_uint_t m)
{
	mpd_uint_t d, r;

	r = bigmod2(hi, lo, m);
	d = a - r;
	d = (a < r) ? d + m : d;

	return d;
}

#ifdef CONFIG_64

/**************************************************************************/
/*                        64-bit modular arithmetic                       */
/**************************************************************************/

/*
 * Description of the algorithm in apfloat.pdf, Chapter 7.1.1,
 * by Mikko Tommila:  http://www.apfloat.org/apfloat/2.41/
 *
 * ACL2 proof: umodarith.lisp: section "Fast modular reduction"
 */

static INLINE mpd_uint_t
x64_mulmod(mpd_uint_t a, mpd_uint_t b, mpd_uint_t m)
{
	mpd_uint_t hi, lo, x, y;


	_mpd_mul_words(&hi, &lo, a, b);

	if (m & (1ULL<<32)) { /* P1 */

		/* first reduction */
		x = y = hi;
		hi >>= 32;

		x = lo - x;
		if (x > lo) hi--;

		y <<= 32;
		lo = y + x;
		if (lo < y) hi++;

		/* second reduction */
		x = y = hi;
		hi >>= 32;

		x = lo - x;
		if (x > lo) hi--;

		y <<= 32;
		lo = y + x;
		if (lo < y) hi++;

		return (hi || lo >= m ? lo - m : lo);
	}
	else if (m & (1ULL<<34)) { /* P2 */

		/* first reduction */
		x = y = hi;
		hi >>= 30;

		x = lo - x;
		if (x > lo) hi--;

		y <<= 34;
		lo = y + x;
		if (lo < y) hi++;

		/* second reduction */
		x = y = hi;
		hi >>= 30;

		x = lo - x;
		if (x > lo) hi--;

		y <<= 34;
		lo = y + x;
		if (lo < y) hi++;

		/* third reduction */
		x = y = hi;
		hi >>= 30;

		x = lo - x;
		if (x > lo) hi--;

		y <<= 34;
		lo = y + x;
		if (lo < y) hi++;

		return (hi || lo >= m ? lo - m : lo);
	}
	else { /* P3 */

		/* first reduction */
		x = y = hi;
		hi >>= 24;

		x = lo - x;
		if (x > lo) hi--;

		y <<= 40;
		lo = y + x;
		if (lo < y) hi++;

		/* second reduction */
		x = y = hi;
		hi >>= 24;

		x = lo - x;
		if (x > lo) hi--;

		y <<= 40;
		lo = y + x;
		if (lo < y) hi++;

		/* third reduction */
		x = y = hi;
		hi >>= 24;

		x = lo - x;
		if (x > lo) hi--;

		y <<= 40;
		lo = y + x;
		if (lo < y) hi++;

		return (hi || lo >= m ? lo - m : lo);
	}
}

static INLINE void
x64_mulmod2c(mpd_uint_t *a, mpd_uint_t *b, mpd_uint_t w, mpd_uint_t m)
{
	*a = x64_mulmod(*a, w, m);
	*b = x64_mulmod(*b, w, m);
}

static INLINE void
x64_mulmod2(mpd_uint_t *a0, mpd_uint_t b0, mpd_uint_t *a1, mpd_uint_t b1,
            mpd_uint_t m)
{
	*a0 = x64_mulmod(*a0, b0, m);
	*a1 = x64_mulmod(*a1, b1, m);
}

static INLINE mpd_uint_t
x64_powmod(mpd_uint_t base, mpd_uint_t exp, mpd_uint_t umod)
{
	mpd_uint_t r = 1;

	while (exp > 0) {
		if (exp & 1)
			r = x64_mulmod(r, base, umod);
		base = x64_mulmod(base, base, umod);
		exp >>= 1;
	}

	return r;
}

/* END CONFIG_64 */
#else /* CONFIG_32 */


/**************************************************************************/
/*                        32-bit modular arithmetic                       */
/**************************************************************************/

#if defined(ANSI)
#if !defined(LEGACY_COMPILER)
/* HAVE_UINT64_T */
static INLINE mpd_uint_t
std_mulmod(mpd_uint_t a, mpd_uint_t b, mpd_uint_t m)
{
	return ((mpd_uuint_t) a * b) % m;
}

static INLINE void
std_mulmod2c(mpd_uint_t *a, mpd_uint_t *b, mpd_uint_t w, mpd_uint_t m)
{
	*a = ((mpd_uuint_t) *a * w) % m;
	*b = ((mpd_uuint_t) *b * w) % m;
}

static INLINE void
std_mulmod2(mpd_uint_t *a0, mpd_uint_t b0, mpd_uint_t *a1, mpd_uint_t b1,
            mpd_uint_t m)
{
	*a0 = ((mpd_uuint_t) *a0 * b0) % m;
	*a1 = ((mpd_uuint_t) *a1 * b1) % m;
}
/* END HAVE_UINT64_T */
#else
/* LEGACY_COMPILER */
static inline mpd_uint_t
std_mulmod(mpd_uint_t a, mpd_uint_t b, mpd_uint_t m)
{
	mpd_uint_t hi, lo, q, r;
	_mpd_mul_words(&hi, &lo, a, b);
	_mpd_div_words(&q, &r, hi, lo, m);
	return r;
}

static inline void
std_mulmod2c(mpd_uint_t *a, mpd_uint_t *b, mpd_uint_t w, mpd_uint_t m)
{
	*a = std_mulmod(*a, w, m);
	*b = std_mulmod(*b, w, m);
}

static inline void
std_mulmod2(mpd_uint_t *a0, mpd_uint_t b0, mpd_uint_t *a1, mpd_uint_t b1,
            mpd_uint_t m)
{
	*a0 = std_mulmod(*a0, b0, m);
	*a1 = std_mulmod(*a1, b1, m);
}
/* END LEGACY_COMPILER */
#endif

static INLINE mpd_uint_t
std_powmod(mpd_uint_t base, mpd_uint_t exp, mpd_uint_t umod)
{
	mpd_uint_t r = 1;

	while (exp > 0) {
		if (exp & 1)
			r = std_mulmod(r, base, umod);
		base = std_mulmod(base, base, umod);
		exp >>= 1;
	}

	return r;
}
#endif /* ANSI CONFIG_32 */


/**************************************************************************/
/*                    Pentium Pro modular arithmetic                      */
/**************************************************************************/

/*
 * Description of the algorithm in apfloat.pdf, Chapter 7.1.1, by Mikko Tommila.
 * http://www.apfloat.org/apfloat/2.41/
 *
 * Remarks:
 *
 *   - dinvmod points to an array of three uint32_t, which is interpreted
 *     as an 80 bit long double.
 *   - Intel compilers prior to version 11 do not seem to handle the
 *     __GNUC__ inline assembly correctly.
 *   - random tests are provided in tests/extended/ppro_mulmod.c
 */

#if defined(PPRO)
#if defined(ASM)

/* all operands < dmod */
static inline mpd_uint_t
ppro_mulmod(mpd_uint_t a, mpd_uint_t b, double *dmod, uint32_t *dinvmod)
{
	mpd_uint_t retval;

	asm (
		"fildl	%2\n\t"
		"fildl	%1\n\t"
		"fmulp	%%st, %%st(1)\n\t"
		"fldt	(%4)\n\t"
		"fmul	%%st(1), %%st\n\t"
		"flds	%5\n\t"
		"fadd	%%st, %%st(1)\n\t"
		"fsubrp	%%st, %%st(1)\n\t"
		"fldl	(%3)\n\t"
		"fmulp	%%st, %%st(1)\n\t"
		"fsubrp	%%st, %%st(1)\n\t"
		"fistpl	%0\n\t"
		: "=m" (retval)
		: "m" (a), "m" (b), "r" (dmod), "r" (dinvmod), "m" (MPD_TWO63)
		: "st", "memory"
	);

	return retval;
}

/* all operands < dmod */
static inline void
ppro_mulmod2c(mpd_uint_t *a0, mpd_uint_t *a1, mpd_uint_t w,
              double *dmod, uint32_t *dinvmod)
{
	asm (
		"fildl	%2\n\t"
		"fildl	(%1)\n\t"
		"fmul	%%st(1), %%st\n\t"
		"fxch	%%st(1)\n\t"
		"fildl	(%0)\n\t"
		"fmulp	%%st, %%st(1) \n\t"
		"fldt	(%4)\n\t"
		"flds	%5\n\t"
		"fld	%%st(2)\n\t"
		"fmul	%%st(2)\n\t"
		"fadd	%%st(1)\n\t"
		"fsub	%%st(1)\n\t"
		"fmull	(%3)\n\t"
		"fsubrp %%st, %%st(3)\n\t"
		"fxch   %%st(2)\n\t"
		"fistpl	(%0)\n\t"
		"fmul	%%st(2)\n\t"
		"fadd	%%st(1)\n\t"
		"fsubp  %%st, %%st(1)\n\t"
		"fmull  (%3)\n\t"
		"fsubrp %%st, %%st(1)\n\t"
		"fistpl (%1)\n\t"
		: : "r" (a0), "r" (a1), "m" (w),
		    "r" (dmod), "r" (dinvmod),
		    "m" (MPD_TWO63)
		: "st", "memory"
	);
}

/* all operands < dmod */
static inline void
ppro_mulmod2(mpd_uint_t *a0, mpd_uint_t b0, mpd_uint_t *a1, mpd_uint_t b1,
             double *dmod, uint32_t *dinvmod)
{
	asm (
		"fildl	%3\n\t"
		"fildl	(%2)\n\t"
		"fmulp	%%st, %%st(1)\n\t"
		"fildl	%1\n\t"
		"fildl	(%0)\n\t"
		"fmulp	%%st, %%st(1)\n\t"
		"fldt	(%5)\n\t"
		"fld	%%st(2)\n\t"
		"fmul	%%st(1), %%st\n\t"
		"fxch	%%st(1)\n\t"
		"fmul	%%st(2), %%st\n\t"
		"flds	%6\n\t"
		"fldl	(%4)\n\t"
		"fxch	%%st(3)\n\t"
		"fadd	%%st(1), %%st\n\t"
		"fxch	%%st(2)\n\t"
		"fadd	%%st(1), %%st\n\t"
		"fxch	%%st(2)\n\t"
		"fsub	%%st(1), %%st\n\t"
		"fxch	%%st(2)\n\t"
		"fsubp	%%st, %%st(1)\n\t"
		"fxch	%%st(1)\n\t"
		"fmul	%%st(2), %%st\n\t"
		"fxch	%%st(1)\n\t"
		"fmulp	%%st, %%st(2)\n\t"
		"fsubrp	%%st, %%st(3)\n\t"
		"fsubrp	%%st, %%st(1)\n\t"
		"fxch	%%st(1)\n\t"
		"fistpl	(%2)\n\t"
		"fistpl	(%0)\n\t"
		: : "r" (a0), "m" (b0), "r" (a1), "m" (b1),
		    "r" (dmod), "r" (dinvmod),
		    "m" (MPD_TWO63)
		: "st", "memory"
	);
}
/* END PPRO GCC ASM */
#elif defined(MASM)

/* all operands < dmod */
static INLINE mpd_uint_t __cdecl
ppro_mulmod(mpd_uint_t a, mpd_uint_t b, double *dmod, uint32_t *dinvmod)
{
	mpd_uint_t retval;

	__asm {
		mov	eax, dinvmod
		mov	edx, dmod
		fild	b
		fild	a
		fmulp	st(1), st
		fld	TBYTE PTR [eax]
		fmul	st, st(1)
		fld	MPD_TWO63
		fadd	st(1), st
		fsubp	st(1), st
		fld	QWORD PTR [edx]
		fmulp	st(1), st
		fsubp	st(1), st
		fistp	retval
	}

	return retval;
}

/* all operands < dmod */
static INLINE mpd_uint_t __cdecl
ppro_mulmod2c(mpd_uint_t *a0, mpd_uint_t *a1, mpd_uint_t w,
              double *dmod, uint32_t *dinvmod)
{
	__asm {
		mov	ecx, dmod
		mov	edx, a1
		mov	ebx, dinvmod
		mov	eax, a0
		fild	w
		fild	DWORD PTR [edx]
		fmul	st, st(1)
		fxch	st(1)
		fild	DWORD PTR [eax]
		fmulp	st(1), st
		fld	TBYTE PTR [ebx]
		fld	MPD_TWO63
		fld	st(2)
		fmul	st, st(2)
		fadd	st, st(1)
		fsub	st, st(1)
		fmul	QWORD PTR [ecx]
		fsubp	st(3), st
		fxch	st(2)
		fistp	DWORD PTR [eax]
		fmul	st, st(2)
		fadd	st, st(1)
		fsubrp	st(1), st
		fmul	QWORD PTR [ecx]
		fsubp	st(1), st
		fistp	DWORD PTR [edx]
	}
}

/* all operands < dmod */
static INLINE void __cdecl
ppro_mulmod2(mpd_uint_t *a0, mpd_uint_t b0, mpd_uint_t *a1, mpd_uint_t b1,
             double *dmod, uint32_t *dinvmod)
{
	__asm {
		mov	ecx, dmod
		mov	edx, a1
		mov	ebx, dinvmod
		mov	eax, a0
		fild	b1
		fild	DWORD PTR [edx]
		fmulp	st(1), st
		fild	b0
		fild	DWORD PTR [eax]
		fmulp	st(1), st
		fld	TBYTE PTR [ebx]
		fld	st(2)
		fmul	st, st(1)
		fxch	st(1)
		fmul	st, st(2)
		fld	DWORD PTR MPD_TWO63
		fld	QWORD PTR [ecx]
		fxch	st(3)
		fadd	st, st(1)
		fxch	st(2)
		fadd	st, st(1)
		fxch	st(2)
		fsub	st, st(1)
		fxch	st(2)
		fsubrp	st(1), st
		fxch	st(1)
		fmul	st, st(2)
		fxch	st(1)
		fmulp	st(2), st
		fsubp	st(3), st
		fsubp	st(1), st
		fxch	st(1)
		fistp	DWORD PTR [edx]
		fistp	DWORD PTR [eax]
	}
}
#endif /* PPRO MASM (_MSC_VER) */


/* all operands < dmod */
static INLINE mpd_uint_t
ppro_powmod(mpd_uint_t base, mpd_uint_t exp, double *dmod, uint32_t *dinvmod)
{
	mpd_uint_t r = 1;

	while (exp > 0) {
		if (exp & 1)
			r = ppro_mulmod(r, base, dmod, dinvmod);
		base = ppro_mulmod(base, base, dmod, dinvmod);
		exp >>= 1;
	}

	return r;
}
#endif /* PPRO */
#endif /* CONFIG_32 */


#endif /* UMODARITH_H */



