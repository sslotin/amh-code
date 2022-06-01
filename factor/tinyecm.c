// gcc -O3 -march=native -lm tinyecm.c -o tecm
// (crashes for some reason)

/*
Copyright (c) 2020, Ben Buhrow
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
*/

/* portions of this file use the following license: */
/*
 * java-math-library is a Java library focused on number theory, but not necessarily limited to it. It is based on the PSIQS 4.0 factoring project.
 * Copyright (C) 2018 Tilman Neumann (www.tilman-neumann.de)
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program;
 * if not, see <http://www.gnu.org/licenses/>.
 */

 /*

 Ported to C and released 7/31/19
 Ben Buhrow

 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <math.h>

#define D 120

#ifndef BITS
#define BITS 64
#endif

#if BITS==64

// full strength mul/sqr redc
__inline uint64_t mulredcx(uint64_t x, uint64_t y, uint64_t n, uint64_t nhat)
{
    if (n & 0x8000000000000000)
    {
        __asm__(
            "mulx %2, %%r10, %%r11	\n\t"
            "movq %%r10, %%rax		\n\t"
            "xorq %%r8, %%r8 \n\t"
            "xorq %%r12, %%r12 \n\t"
            "mulq %3 \n\t"
            "mulq %4 \n\t"
            "addq %%r10, %%rax \n\t"
            "adcq %%r11, %%rdx \n\t"
            "cmovae %4, %%r12 \n\t"
            "subq %4, %%rdx \n\t"
            "cmovc %%r12, %%r8 \n\t"
            "addq %%r8, %%rdx \n\t"
            : "=&d"(x)
            : "0"(x), "r"(y), "r"(nhat), "r"(n)
            : "rax", "r8", "r10", "r11", "r12", "cc");
    }
    else
    {
        __asm__(
            "mulx %2, %%r10, %%r11	\n\t"
            "movq %3, %%rax		\n\t"
            "xorq %%r8, %%r8 \n\t"
            "mulq %%r10 \n\t"
            "mulq %4 \n\t"
            "addq %%r10, %%rax \n\t"
            "adcq %%r11, %%rdx \n\t"
            "subq %4, %%rdx \n\t"
            "cmovc %4, %%r8 \n\t"
            "addq %%r8, %%rdx \n\t"
            : "=d"(x)
            : "0"(x), "r"(y), "r"(nhat), "r"(n)
            : "rax", "r8", "r10", "r11", "cc");

    }
    return x;
}

__inline void mulredcx2(uint64_t x, uint64_t y, uint64_t x2, uint64_t y2,
    uint64_t *r1, uint64_t *r2, uint64_t n, uint64_t nhat)
{
    // attempt to do two independent mulredc faster than individually.
    // mostly by using mulx/adcx/adox to process two carry chains 
    // separately.
    // so far, it is very slightly worse.  Perhaps because it
    // uses more registers and has more instructions (an extra
    // movq/subq to negate n and movq at the end), and more function
    // arguments.
    // also there is no reciprocal throughput advantage to using 
    // independent adc/ox vs. serial addq/adcq: they are all latency 1.

    if (n & 0x8000000000000000)
    {
        __asm__(
            "mulx %3, %%r10, %%r11	\n\t"
            "movq %4, %%rdx		\n\t"
            "movq $0, %%rbx \n\t"
            "mulx %5, %%r12, %%r13	\n\t"
            "movq %6, %%rdx		\n\t"
            "mulx %%r10, %%rax, %%r8 \n\t"
            "mulx %%r12, %%rdx, %%r15 \n\t"
            "subq %7, %%rbx \n\t"
            "mulx %7, %%r14, %%r15 \n\t"
            "mulq %7 \n\t"
            "xorq %%r8, %%r8 \n\t"
            "xorq %1, %1 \n\t"
            "adcx %%r12, %%r14 \n\t"
            "adox %%r10, %%rax \n\t"
            "adcx %%r13, %%r15 \n\t"
            "adox %%r11, %%rdx \n\t"
            "cmovnc %7, %%r8 \n\t"
            "cmovno %7, %1 \n\t"
            "xorq %%rax, %%rax \n\t"
            "xorq %%r14, %%r14 \n\t"
            "adcx %%rbx, %%r15 \n\t"
            "adox %%rbx, %%rdx \n\t"
            "cmovnc %%r8, %%rax \n\t"
            "cmovno %1, %%r14 \n\t"
            "addq %%r14, %%rdx \n\t"
            "addq %%rax, %%r15 \n\t"
            "movq %%r15, %1\n\t"
            : "=d"(x), "=r"(x2)
            : "0"(x), "r"(y), "1"(x2), "r"(y2), "r"(nhat), "r"(n)
            : "rax", "r8", "r10", "r11", "r12", "r13", "r14", "r15", "rbx", "cc");
    }
    else
    {
        __asm__(
            "mulx %3, %%r10, %%r11	\n\t"
            "movq %4, %%rdx		\n\t"
            "movq $0, %%rbx \n\t"
            "mulx %5, %%r12, %%r13	\n\t"
            "movq %6, %%rdx		\n\t"
            "mulx %%r10, %%rax, %%r8 \n\t"
            "mulx %%r12, %%rdx, %%r15 \n\t"
            "subq %7, %%rbx \n\t"
            "mulx %7, %%r14, %%r15 \n\t"
            "mulq %7 \n\t"
            "xorq %%r8, %%r8 \n\t"
            "adcx %%r12, %%r14 \n\t"
            "adox %%r10, %%rax \n\t"
            "adcx %%r13, %%r15 \n\t"
            "adox %%r11, %%rdx \n\t"
            "xorq %%r14, %%r14 \n\t"
            "adcx %%rbx, %%r15 \n\t"
            "adox %%rbx, %%rdx \n\t"
            "cmovnc %7, %%r8 \n\t"
            "cmovno %7, %%r14 \n\t"
            "addq %%r14, %%rdx \n\t"
            "addq %%r8, %%r15 \n\t"
            "movq %%r15, %1\n\t"
            : "=d"(x), "=r"(x2)
            : "0"(x), "r"(y), "1"(x2), "r"(y2), "r"(nhat), "r"(n)
            : "rax", "r8", "r10", "r11", "r12", "r13", "r14", "r15", "rbx", "cc");
    }

    *r1 = x;
    *r2 = x2;
    return;
}

__inline uint64_t sqrredcx(uint64_t x, uint64_t n, uint64_t nhat)
{
    if (n & 0x8000000000000000)
    {
        __asm__(
            "mulx %1, %%r10, %%r11	\n\t"
            "movq %%r10, %%rax		\n\t"
            "xorq %%r8, %%r8 \n\t"
            "xorq %%r12, %%r12 \n\t"
            "mulq %2 \n\t"
            "mulq %3 \n\t"
            "addq %%r10, %%rax \n\t"
            "adcq %%r11, %%rdx \n\t"
            "cmovae %3, %%r12 \n\t"
            "subq %3, %%rdx \n\t"
            "cmovc %%r12, %%r8 \n\t"
            "addq %%r8, %%rdx \n\t"
            : "=&d"(x)
            : "0"(x), "r"(nhat), "r"(n)
            : "rax", "r8", "r10", "r11", "r12", "cc");
    }
    else
    {
        __asm__(
            "mulx %1, %%r10, %%r11	\n\t"
            "movq %2, %%rax		\n\t"
            "xorq %%r8, %%r8 \n\t"
            "mulq %%r10 \n\t"
            "mulq %3 \n\t"
            "addq %%r10, %%rax \n\t"
            "adcq %%r11, %%rdx \n\t"
            "subq %3, %%rdx \n\t"
            "cmovc %3, %%r8 \n\t"
            "addq %%r8, %%rdx \n\t"
            : "=d"(x)
            : "0"(x), "r"(nhat), "r"(n)
            : "rax", "r8", "r10", "r11", "cc");

    }
    return x;
}

__inline void sqrredcx2(uint64_t x, uint64_t x2, 
    uint64_t *r1, uint64_t *r2, uint64_t n, uint64_t nhat)
{
    // attempt to do two independent mulredc faster than individually.
    // mostly by using mulx/adcx/adox to process two carry chains 
    // separately.
    // so far, it is very slightly worse.  Perhaps because it does
    // use many more registers and has more instructions (an extra
    // movq/subq to negate n, movq at the end, and more function
    // arguments.
    // we get reciprocal throughput advantage on skyx on the cmov's,
    // the xor's, and the final adds
    // maybe we need to do more of a staggered approach where we
    // do the first muls, then when doing the adcx/adcx start the
    // second set of muls

    if (n & 0x8000000000000000)
    {
        __asm__(
            "mulx %2, %%r10, %%r11	\n\t"
            "movq %3, %%rdx		\n\t"
            "movq $0, %%rbx \n\t"
            "mulx %3, %%r12, %%r13	\n\t"
            "movq %4, %%rdx		\n\t"
            "mulx %%r12, %%r14, %%r15 \n\t"
            "mulx %%r10, %%rax, %%r8 \n\t"  /* output rax ready for mulq */
            "movq %%r14, %%rdx		\n\t"
            "subq %5, %%rbx \n\t"
            "mulx %5, %%r14, %%r15 \n\t"    /* mulq must go 2nd because it commits rdx */
            "mulq %5 \n\t"
            "xorq %%r8, %%r8 \n\t"
            "xorq %1, %1 \n\t"
            "adcx %%r12, %%r14 \n\t"
            "adox %%r10, %%rax \n\t"
            "adcx %%r13, %%r15 \n\t"
            "adox %%r11, %%rdx \n\t"
            "cmovnc %5, %%r8 \n\t"
            "cmovno %5, %1 \n\t"
            "xorq %%rax, %%rax \n\t"
            "xorq %%r14, %%r14 \n\t"
            "adcx %%rbx, %%r15 \n\t"
            "adox %%rbx, %%rdx \n\t"
            "cmovnc %%r8, %%rax \n\t"
            "cmovno %1, %%r14 \n\t"
            "addq %%r14, %%rdx \n\t"
            "addq %%rax, %%r15 \n\t"
            "movq %%r15, %1\n\t"
            : "=d"(x), "=r"(x2)
            : "0"(x), "1"(x2), "r"(nhat), "r"(n)
            : "rax", "r8", "r10", "r11", "r12", "r13", "r14", "r15", "rbx", "cc");
    }
    else
    {
        __asm__(
            "mulx %2, %%r10, %%r11	\n\t"
            "movq %3, %%rdx		\n\t"
            "movq $0, %%rbx \n\t"
            "mulx %3, %%r12, %%r13	\n\t"
            "movq %4, %%rdx		\n\t"
            "mulx %%r12, %%r14, %%r15 \n\t"
            "mulx %%r10, %%rax, %%r8 \n\t"  /* output rax ready for mulq */
            "movq %%r14, %%rdx		\n\t"
            "subq %5, %%rbx \n\t"
            "mulx %5, %%r14, %%r15 \n\t"    /* mulq must go 2nd because it commits rdx */
            "mulq %5 \n\t"
            "xorq %%r8, %%r8 \n\t"
            "adcx %%r12, %%r14 \n\t"
            "adox %%r10, %%rax \n\t"
            "adcx %%r13, %%r15 \n\t"
            "adox %%r11, %%rdx \n\t"
            "xorq %%r14, %%r14 \n\t"
            "adcx %%rbx, %%r15 \n\t"
            "adox %%rbx, %%rdx \n\t"
            "cmovnc %5, %%r8 \n\t"
            "cmovno %5, %%r14 \n\t"
            "addq %%r14, %%rdx \n\t"
            "addq %%r8, %%r15 \n\t"
            "movq %%r15, %1\n\t"
            : "=d"(x), "=r"(x2)
            : "0"(x), "1"(x2), "r"(nhat), "r"(n)
            : "rax", "r8", "r10", "r11", "r12", "r13", "r14", "r15", "rbx", "cc");
    }

    *r1 = x;
    *r2 = x2;
    return;
}

#elif BITS >= 62

// don't have to check for addmod overflow at the end
__inline uint64_t mulredcx(uint64_t x, uint64_t y, uint64_t n, uint64_t nhat)
{
    __asm__(
        "mulx %2, %%r10, %%r11	\n\t"
        "movq %3, %%rax		\n\t"
        "xorq %%r8, %%r8 \n\t"
        "mulq %%r10 \n\t"
        "mulq %4 \n\t"
        "addq %%r10, %%rax \n\t"
        "adcq %%r11, %%rdx \n\t"
        "subq %4, %%rdx \n\t"
        "cmovc %4, %%r8 \n\t"
        "addq %%r8, %%rdx \n\t"
        : "=d"(x)
        : "0"(x), "r"(y), "r"(nhat), "r"(n)
        : "rax", "r8", "r10", "r11", "cc");

    return x;
}

__inline void mulredcx2(uint64_t x, uint64_t y, uint64_t x2, uint64_t y2, 
    uint64_t *r1, uint64_t *r2, uint64_t n, uint64_t nhat)
{
    __asm__(
        "mulx %3, %%r10, %%r11	\n\t"
        "movq %4, %%rdx		\n\t"
        "mulx %5, %%r12, %%r13	\n\t"
        "movq %6, %%rdx		\n\t"
        "mulx %%r10, %%rax, %%r8 \n\t"
        "mulx %%r12, %%rdx, %%r15 \n\t"
        "mulx %7, %%r14, %%r15 \n\t"
        "mulq %7 \n\t"
        "movq $0, %%rbx \n\t"
        "subq %7, %%rbx \n\t"
        "xorq %%r8, %%r8 \n\t"
        "adcx %%r12, %%r14 \n\t"
        "adcx %%r13, %%r15 \n\t"
        "adox %%r10, %%rax \n\t"
        "adox %%r11, %%rdx \n\t"
        "xorq %%r14, %%r14 \n\t"
        "adcx %%rbx, %%r15 \n\t"
        "adox %%rbx, %%rdx \n\t"
        "cmovnc %7, %%r8 \n\t"
        "cmovno %7, %%r14 \n\t"
        "addq %%r14, %%rdx \n\t"
        "addq %%r8, %%r15 \n\t"
        "movq %%r15, %1\n\t"
        : "=d"(x), "=r"(x2)
        : "0"(x), "r"(y), "1"(x2), "r"(y2), "r"(nhat), "r"(n)
        : "rax", "r8", "r10", "r11", "r12", "r13", "r14", "r15", "rbx", "cc");

    *r1 = x;
    *r2 = x2;
    return;
}

__inline uint64_t sqrredcx(uint64_t x, uint64_t n, uint64_t nhat)
{
    __asm__(
        "mulx %1, %%r10, %%r11	\n\t"
        "movq %2, %%rax		\n\t"
        "xorq %%r8, %%r8 \n\t"
        "mulq %%r10 \n\t"
        "mulq %3 \n\t"
        "addq %%r10, %%rax \n\t"
        "adcq %%r11, %%rdx \n\t"
        "subq %3, %%rdx \n\t"
        "cmovc %3, %%r8 \n\t"
        "addq %%r8, %%rdx \n\t"
        : "=d"(x)
        : "0"(x), "r"(nhat), "r"(n)
        : "rax", "r8", "r10", "r11", "cc");

    return x;
}

#else

// can be more lazy and not do final (mod n) at all.
// average curve count goes up a bit, but still faster overall.
__inline uint64_t mulredcx(uint64_t x, uint64_t y, uint64_t n, uint64_t nhat)
{
    __asm__(
        "mulx %2, %%r10, %%r11	\n\t"
        "movq %3, %%rax		\n\t"
        "xorq %%r8, %%r8 \n\t"
        "mulq %%r10 \n\t"
        "mulq %4 \n\t"
        "addq %%r10, %%rax \n\t"
        "adcq %%r11, %%rdx \n\t"
        : "=d"(x)
        : "0"(x), "r"(y), "r"(nhat), "r"(n)
        : "rax", "r8", "r10", "r11", "cc");

    return x;
}

__inline uint64_t sqrredcx(uint64_t x, uint64_t n, uint64_t nhat)
{
    __asm__(
        "mulx %1, %%r10, %%r11	\n\t"
        "movq %2, %%rax		\n\t"
        "xorq %%r8, %%r8 \n\t"
        "mulq %%r10 \n\t"
        "mulq %3 \n\t"
        "addq %%r10, %%rax \n\t"
        "adcq %%r11, %%rdx \n\t"
        : "=d"(x)
        : "0"(x), "r"(nhat), "r"(n)
        : "rax", "r8", "r10", "r11", "cc");

    return x;
}

#endif

__inline uint64_t mulredc63x(uint64_t x, uint64_t y, uint64_t n, uint64_t nhat)
{
    __asm__(
        "mulx %2, %%r10, %%r11	\n\t"
        "movq %3, %%rax		\n\t"
        "xorq %%r8, %%r8 \n\t"
        "mulq %%r10 \n\t"
        "mulq %4 \n\t"
        "addq %%r10, %%rax \n\t"
        "adcq %%r11, %%rdx \n\t"
        "subq %4, %%rdx \n\t"
        "cmovc %4, %%r8 \n\t"
        "addq %%r8, %%rdx \n\t"
        : "=d"(x)
        : "0"(x), "r"(y), "r"(nhat), "r"(n)
        : "rax", "r8", "r10", "r11", "cc");

    return x;
}

__inline uint64_t sqrredc63x(uint64_t x, uint64_t n, uint64_t nhat)
{
	__asm__(
		"mulx %1, %%r10, %%r11	\n\t"
		"movq %2, %%rax		\n\t"
		"xorq %%r8, %%r8 \n\t"
		"mulq %%r10 \n\t"
		"mulq %3 \n\t"
		"addq %%r10, %%rax \n\t"
		"adcq %%r11, %%rdx \n\t"
		"subq %3, %%rdx \n\t"
		"cmovc %3, %%r8 \n\t"
		"addq %%r8, %%rdx \n\t"
		: "=d"(x)
		: "0"(x), "r"(nhat), "r"(n)
		: "rax", "r8", "r10", "r11", "cc");

	return x;
}


//#define DEBUG 1
typedef struct
{
	uint64_t X;
	uint64_t Z;
} uecm_pt;

typedef struct
{
	uint64_t sum1;
	uint64_t diff1;
	uint64_t sum2;
	uint64_t diff2;
	uint64_t tt1;
	uint64_t tt2;
	uint64_t tt3;
	uint64_t tt4;
	uint64_t tt5;
	uint64_t s;
	uint64_t n;
	uecm_pt pt1;
	uecm_pt pt2;
	uecm_pt pt3;
	uecm_pt pt4;
	uecm_pt pt5;
	uint32_t sigma;

	uecm_pt Pa;
	uecm_pt Pd;
	uecm_pt Pad;
	uecm_pt Pb[60];
	uint64_t Paprod;
	uint64_t Pbprod[60];

	uint64_t stg2acc;
	uint32_t stg1Add;
	uint32_t stg1Doub;
	uint32_t paired;
	uint32_t ptadds;
	uint64_t numprimes;
	uint64_t A;
	uint32_t last_pid;
	uint32_t amin;

	uint32_t stg1_max;
	uint32_t stg2_max;

} uecm_work;

static const uint32_t map[61] = {
	0, 1, 2, 0, 0, 0, 0, 3, 0, 0,
	0, 4, 0, 5, 0, 0, 0, 6, 0, 7,
	0, 0, 0, 8, 0, 0, 0, 0, 0, 9,
	0, 10, 0, 0, 0, 0, 0, 11, 0, 0,
	0, 12, 0, 13, 0, 0, 0, 14, 0, 15,
	0, 0, 0, 16, 0, 0, 0, 0, 0, 17,
	18 };

#define NUMP 801
static const uint32_t primes[NUMP] = {
2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31,
37, 41, 43, 47, 53, 59, 61, 67, 71, 73,
79, 83, 89, 97, 101, 103, 107, 109, 113, 127,
131, 137, 139, 149, 151, 157, 163, 167, 173, 179,
181, 191, 193, 197, 199, 211, 223, 227, 229, 233,
239, 241, 251, 257, 263, 269, 271, 277, 281, 283,
293, 307, 311, 313, 317, 331, 337, 347, 349, 353,
359, 367, 373, 379, 383, 389, 397, 401, 409, 419,
421, 431, 433, 439, 443, 449, 457, 461, 463, 467,
479, 487, 491, 499, 503, 509, 521, 523, 541, 547,
557, 563, 569, 571, 577, 587, 593, 599, 601, 607,
613, 617, 619, 631, 641, 643, 647, 653, 659, 661,
673, 677, 683, 691, 701, 709, 719, 727, 733, 739,
743, 751, 757, 761, 769, 773, 787, 797, 809, 811,
821, 823, 827, 829, 839, 853, 857, 859, 863, 877,
881, 883, 887, 907, 911, 919, 929, 937, 941, 947,
953, 967, 971, 977, 983, 991, 997, 1009, 1013, 1019,
1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069, 1087,
1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151, 1153,
1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223, 1229,
1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291, 1297,
1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373, 1381,
1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453,
1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511, 1523,
1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583, 1597,
1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657, 1663,
1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733, 1741,
1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811, 1823,
1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889, 1901,
1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987, 1993,
1997, 1999, 2003, 2011, 2017, 2027, 2029, 2039, 2053, 2063,
2069, 2081, 2083, 2087, 2089, 2099, 2111, 2113, 2129, 2131,
2137, 2141, 2143, 2153, 2161, 2179, 2203, 2207, 2213, 2221,
2237, 2239, 2243, 2251, 2267, 2269, 2273, 2281, 2287, 2293,
2297, 2309, 2311, 2333, 2339, 2341, 2347, 2351, 2357, 2371,
2377, 2381, 2383, 2389, 2393, 2399, 2411, 2417, 2423, 2437,
2441, 2447, 2459, 2467, 2473, 2477, 2503, 2521, 2531, 2539,
2543, 2549, 2551, 2557, 2579, 2591, 2593, 2609, 2617, 2621,
2633, 2647, 2657, 2659, 2663, 2671, 2677, 2683, 2687, 2689,
2693, 2699, 2707, 2711, 2713, 2719, 2729, 2731, 2741, 2749,
2753, 2767, 2777, 2789, 2791, 2797, 2801, 2803, 2819, 2833,
2837, 2843, 2851, 2857, 2861, 2879, 2887, 2897, 2903, 2909,
2917, 2927, 2939, 2953, 2957, 2963, 2969, 2971, 2999, 3001,
3011, 3019, 3023, 3037, 3041, 3049, 3061, 3067, 3079, 3083,
3089, 3109, 3119, 3121, 3137, 3163, 3167, 3169, 3181, 3187,
3191, 3203, 3209, 3217, 3221, 3229, 3251, 3253, 3257, 3259,
3271, 3299, 3301, 3307, 3313, 3319, 3323, 3329, 3331, 3343,
3347, 3359, 3361, 3371, 3373, 3389, 3391, 3407, 3413, 3433,
3449, 3457, 3461, 3463, 3467, 3469, 3491, 3499, 3511, 3517,
3527, 3529, 3533, 3539, 3541, 3547, 3557, 3559, 3571, 3581,
3583, 3593, 3607, 3613, 3617, 3623, 3631, 3637, 3643, 3659,
3671, 3673, 3677, 3691, 3697, 3701, 3709, 3719, 3727, 3733,
3739, 3761, 3767, 3769, 3779, 3793, 3797, 3803, 3821, 3823,
3833, 3847, 3851, 3853, 3863, 3877, 3881, 3889, 3907, 3911,
3917, 3919, 3923, 3929, 3931, 3943, 3947, 3967, 3989, 4001,
4003, 4007, 4013, 4019, 4021, 4027, 4049, 4051, 4057, 4073,
4079, 4091, 4093, 4099, 4111, 4127, 4129, 4133, 4139, 4153,
4157, 4159, 4177, 4201, 4211, 4217, 4219, 4229, 4231, 4241,
4243, 4253, 4259, 4261, 4271, 4273, 4283, 4289, 4297, 4327,
4337, 4339, 4349, 4357, 4363, 4373, 4391, 4397, 4409, 4421,
4423, 4441, 4447, 4451, 4457, 4463, 4481, 4483, 4493, 4507,
4513, 4517, 4519, 4523, 4547, 4549, 4561, 4567, 4583, 4591,
4597, 4603, 4621, 4637, 4639, 4643, 4649, 4651, 4657, 4663,
4673, 4679, 4691, 4703, 4721, 4723, 4729, 4733, 4751, 4759,
4783, 4787, 4789, 4793, 4799, 4801, 4813, 4817, 4831, 4861,
4871, 4877, 4889, 4903, 4909, 4919, 4931, 4933, 4937, 4943,
4951, 4957, 4967, 4969, 4973, 4987, 4993, 4999, 5003, 5009,
5011, 5021, 5023, 5039, 5051, 5059, 5077, 5081, 5087, 5099,
5101, 5107, 5113, 5119, 5147, 5153, 5167, 5171, 5179, 5189,
5197, 5209, 5227, 5231, 5233, 5237, 5261, 5273, 5279, 5281,
5297, 5303, 5309, 5323, 5333, 5347, 5351, 5381, 5387, 5393,
5399, 5407, 5413, 5417, 5419, 5431, 5437, 5441, 5443, 5449,
5471, 5477, 5479, 5483, 5501, 5503, 5507, 5519, 5521, 5527,
5531, 5557, 5563, 5569, 5573, 5581, 5591, 5623, 5639, 5641,
5647, 5651, 5653, 5657, 5659, 5669, 5683, 5689, 5693, 5701,
5711, 5717, 5737, 5741, 5743, 5749, 5779, 5783, 5791, 5801,
5807, 5813, 5821, 5827, 5839, 5843, 5849, 5851, 5857, 5861,
5867, 5869, 5879, 5881, 5897, 5903, 5923, 5927, 5939, 5953,
5981, 5987, 6007, 6011, 6029, 6037, 6043, 6047, 6053, 6067,
6073, 6079, 6089, 6091, 6101, 6113, 6121, 6131, 6133, 6143,
};

// local functions
void uadd(uint64_t rho, uecm_work *work, uecm_pt *P1, uecm_pt *p2,
	uecm_pt *Pin, uecm_pt *Pout);
void udup(uint64_t rho, uecm_work *work, uint64_t insum, uint64_t indiff, uecm_pt *P);
void uprac(uint64_t rho, uecm_work *work, uecm_pt *P, uint64_t c, double v);
int ucheck_factor(uint64_t Z, uint64_t n, uint64_t * f);
void ubuild(uecm_pt *P, uint64_t rho, uecm_work *work, uint32_t sigma);

void uecm_stage1(uint64_t rho, uecm_work *work, uecm_pt *P);
void uecm_stage2(uecm_pt *P, uint64_t rho, uecm_work *work);
void uecm_stage2_D60(uecm_pt* P, uint64_t rho, uecm_work* work);
void uecm_stage2_D120(uecm_pt *P, uint64_t rho, uecm_work *work);
void uecm_stage2_D120_inv(uecm_pt *P, uint64_t rho, uecm_work *work);
void uecm_stage2_w30u2(uecm_pt* P, uint64_t rho, uecm_work* work);
void uecm_stage2_w60u1(uecm_pt* P, uint64_t rho, uecm_work* work);

__inline void addsubmod(uint64_t x, uint64_t y, uint64_t *s, uint64_t *d, uint64_t n)
{
    // attempt to do simultaneous add/sub faster than individually.
    // so far, it is very slightly worse.
    *d = 0 - y;
    *s = x;
    __asm__(
        "xorq %%r10, %%r10 \n\t"    /* zero r10 */
        "xorq %%r8, %%r8 \n\t"      /* zero r8 and clear CF and OF */
        "adcx %3, %1\n\t"           /* d = x - y, set CF */
        "adox %4, %0\n\t"           /* s = x + y, set OF */
        "cmovnc %2, %%r8\n\t"       /* sub overflow ? n : 0 */
        "cmovo %2, %%r10\n\t"       /* add overflow ? n : 0 */
        "addq %%r8, %1\n\t"         /* x = x - y mod n */
        "subq %%r10, %0\n\t"        /* y = y + x mod n */
        :"+r" (*s), "+r" (*d)
        : "r"(n), "r"(x), "r"(y)
        : "r8", "r10", "cc"
    );

    return;
}

__inline uint64_t submod(uint64_t a, uint64_t b, uint64_t n)
{
	__asm__(
		"xorq %%r8, %%r8 \n\t"
		"subq %1, %0 \n\t"
		"cmovc %2, %%r8 \n\t"
		"addq %%r8, %0 \n\t"
		: "+r"(a)
		: "r"(b), "r"(n)
		: "r8", "cc");

	return a;
}

#if BITS==64
__inline uint64_t addmod(uint64_t x, uint64_t y, uint64_t n)
{
    uint64_t t = x - n;
    x += y;
    __asm__("add %2, %1\n\t"
        "cmovc %1, %0\n\t"
        :"+r" (x), "+&r" (t)
        : "r" (y)
        : "cc"
    );
    return x;
}
#else
// this works as long as inputs <= 62 bits so that the adds don't overflow.
// it is ok to have the result larger than n.
#define addmod(x, y, n) ((x) + (y))
#endif

__inline uint64_t u64div(uint64_t c, uint64_t n)
{
	__asm__("divq %4"
		: "=a"(c), "=d"(n)
		: "1"(c), "0"(0), "r"(n));

	return n;
}

uint64_t spDivide(uint64_t *q, uint64_t *r, uint64_t u[2], uint64_t v)
{
	*r = u[1];
	*q = u[0];
	__asm__("divq %4"
		: "=a"(*q), "=d"(*r)
		: "1"(*r), "0"(*q), "r"(v));

	return 0;
}

void spMultiply(uint64_t u, uint64_t v, uint64_t *product, uint64_t *carry)
{
	*product = v;
	*carry = u;

	__asm__("movq %2, %%rax	\n\t"
		"mulq %3	\n\t"
		"movq %%rax, %0		\n\t"
		"movq %%rdx, %1		\n\t"
		: "=r"(*product), "=r"(*carry)
		: "1"(*carry), "0"(*product)
		: "rax", "rdx", "cc");

	return;
}

void spMulMod(uint64_t u, uint64_t v, uint64_t m, uint64_t *w)
{
	uint64_t p[2];
	uint64_t q;

	spMultiply(u, v, &p[0], &p[1]);
	spDivide(&q, w, p, m);

	return;
}

uint64_t spGCD(uint64_t x, uint64_t y)
{
	uint64_t a, b, c;
	a = x; b = y;
	while (b != 0)
	{
		c = a % b;
		a = b;
		b = c;
	}
	return a;
}

// quick and easy RNG
uint64_t LCGSTATE;
uint32_t spRand(uint32_t lower, uint32_t upper)
{
	// advance the state of the LCG and return the appropriate result
	LCGSTATE = 6364136223846793005ULL * LCGSTATE + 1442695040888963407ULL;
	return lower + (uint32_t)(
		(double)(upper - lower) * (double)(LCGSTATE >> 32) / 4294967296.0);
}

#define INV_2_POW_64 5.4210108624275221700372640043497e-20
uint64_t spRand64(uint64_t lower, uint64_t upper)
{
    // advance the state of the LCG and return the appropriate result
    LCGSTATE = 6364136223846793005ULL * LCGSTATE + 1442695040888963407ULL;
    return lower + (uint64_t)(
        (double)(upper - lower) * ((double)(LCGSTATE) * INV_2_POW_64));
}

void uadd(uint64_t rho, uecm_work *work, uecm_pt *P1, uecm_pt *P2,
	uecm_pt *Pin, uecm_pt *Pout)
{
	// compute:
	//x+ = z- * [(x1-z1)(x2+z2) + (x1+z1)(x2-z2)]^2
	//z+ = x- * [(x1-z1)(x2+z2) - (x1+z1)(x2-z2)]^2
	// where:
	//x- = original x
	//z- = original z
	// given the sums and differences of the original points (stored in work structure).
	work->diff1 = submod(P1->X, P1->Z, work->n);
	work->sum1 = addmod(P1->X, P1->Z, work->n);
	work->diff2 = submod(P2->X, P2->Z, work->n);
	work->sum2 = addmod(P2->X, P2->Z, work->n);
    //addsubmod(P1->X, P1->Z, &work->sum1, &work->diff1, work->n);
    //addsubmod(P2->X, P2->Z, &work->sum2, &work->diff2, work->n);

	work->tt1 = mulredcx(work->diff1, work->sum2, work->n, rho);	//U
	work->tt2 = mulredcx(work->sum1, work->diff2, work->n, rho);	//V
    //mulredcx2(work->diff1, work->sum2, work->sum1, work->diff2, 
    //    &work->tt1, &work->tt2, work->n, rho);

	work->tt3 = addmod(work->tt1, work->tt2, work->n);
	work->tt4 = submod(work->tt1, work->tt2, work->n);
    //addsubmod(work->tt1, work->tt2, &work->tt3, &work->tt4, work->n);
	work->tt1 = sqrredcx(work->tt3, work->n, rho);	//(U + V)^2
	work->tt2 = sqrredcx(work->tt4, work->n, rho);	//(U - V)^2
    //sqrredcx2(work->tt3, work->tt4, 
    //    &work->tt1, &work->tt2, work->n, rho);

	if (Pin == Pout)
	{
		uint64_t tmp;
		Pout->Z = mulredcx(work->tt1, Pin->Z, work->n, rho);		//Z * (U + V)^2
		Pout->X = mulredcx(work->tt2, Pin->X, work->n, rho);		//x * (U - V)^2
        //mulredcx2(work->tt1, Pin->Z, work->tt2, Pin->X,
        //    &Pout->Z, &Pout->X, work->n, rho);

		tmp = Pout->Z;
		Pout->Z = Pout->X;
		Pout->X = tmp;
	}
	else
	{
		Pout->X = mulredcx(work->tt1, Pin->Z, work->n, rho);		//Z * (U + V)^2
		Pout->Z = mulredcx(work->tt2, Pin->X, work->n, rho);		//x * (U - V)^2
        //mulredcx2(work->tt1, Pin->Z, work->tt2, Pin->X,
        //    &Pout->X, &Pout->Z, work->n, rho);
	}
	work->stg1Add++;
	return;
}

void udup(uint64_t rho, uecm_work *work,
	uint64_t insum, uint64_t indiff, uecm_pt *P)
{
	work->tt1 = sqrredcx(indiff, work->n, rho);			// U=(x1 - z1)^2
	work->tt2 = sqrredcx(insum, work->n, rho);			// V=(x1 + z1)^2
    //sqrredcx2(indiff, insum,
    //    &work->tt1, &work->tt2, work->n, rho);

	P->X = mulredcx(work->tt1, work->tt2, work->n, rho);			// x=U*V

	work->tt3 = submod(work->tt2, work->tt1, work->n);			// w = V-U
	work->tt2 = mulredcx(work->tt3, work->s, work->n, rho);		// w = (A+2)/4 * w
	work->tt2 = addmod(work->tt2, work->tt1, work->n);			// w = w + U
	P->Z = mulredcx(work->tt2, work->tt3, work->n, rho);			// Z = w*(V-U)
	work->stg1Doub++;
	return;
}

//#define PRACOPT
//#define TRACEPRAC
#ifdef TRACEPRAC
#include "gmp.h"
#endif

void uprac70(uint64_t rho, uecm_work *work, uecm_pt *P)
{
	uint64_t s1, s2, d1, d2;
	uint64_t swp1, swp2;
	int i;
	//static const uint8_t steps[116] = { 
	//	0,6,0,6,0,6,0,4,6,0,4,6,0,4,4,6,
	//	0,4,4,6,0,5,4,6,0,3,3,4,6,0,3,5,
	//	4,6,0,3,4,3,4,6,0,5,5,4,6,0,5,3,
	//	3,4,6,0,3,3,4,3,4,6,0,5,3,3,3,3,
	//	3,3,3,3,4,3,3,4,6,0,5,4,3,3,4,6,
	//	0,3,4,3,5,4,6,0,5,3,3,3,4,6,0,5,
	//	4,3,5,4,6,0,5,5,3,3,4,6,0,4,3,3,
	//	3,5,4,6};

    // case 6 is always followed by 0 except for the very last one.
    static const uint8_t steps[95] = {
        0,6,6,6,4,6,4,6,4,4,6,4,4,6,5,4,
        6,3,3,4,6,3,5,4,6,3,4,3,4,6,5,5,
        4,6,5,3,3,4,6,3,3,4,3,4,6,5,3,3,
        3,3,3,3,3,3,4,3,3,4,6,5,4,3,3,4,
        6,3,4,3,5,4,6,5,3,3,3,4,6,5,4,3,
        5,4,6,5,5,3,3,4,6,4,3,3,3,5,4};

#ifdef TRACEPRAC
    mpz_t A, B, C, T, S, p;
    mpz_init(A);
    mpz_init(B);
    mpz_init(C);
    mpz_init(T);
    mpz_init(S);
    mpz_init(p);

    mpz_set_ui(A, 1);
    mpz_set_ui(B, 1);
    mpz_set_ui(C, 1);
    mpz_set_ui(T, 1);
    mpz_set_ui(S, 1);
    mpz_set_ui(p, 1);
#endif

	for (i = 0; i < 95; i++)
	{
		if (steps[i] == 0)
		{
			work->pt1.X = work->pt2.X = work->pt3.X = P->X;
			work->pt1.Z = work->pt2.Z = work->pt3.Z = P->Z;

			d1 = submod(work->pt1.X, work->pt1.Z, work->n);
			s1 = addmod(work->pt1.X, work->pt1.Z, work->n);
			udup(rho, work, s1, d1, &work->pt1);

#ifdef TRACEPRAC
            gmp_printf("A = B = C = P = %Zd\n", p);
            mpz_add(A, A, A);
            gmp_printf("A = 2 * A (%Zd)\n", A);
#endif
		}
		else if (steps[i] == 3)
		{
			// integrate step 4 followed by swap(1,2)
			uadd(rho, work, &work->pt2, &work->pt1, &work->pt3, &work->pt4);		// T = B + A (C)
            
#ifdef TRACEPRAC
            mpz_add(T, B, A);
            gmp_printf("T = B + A (%Zd)\n", T);

            mpz_set(S, A);
            mpz_set(A, T);
            mpz_set(T, C);
            mpz_set(C, B);
            mpz_set(B, S);

            gmp_printf("S = A (%Zd)\n", S);
            gmp_printf("A = T (%Zd)\n", A);
            gmp_printf("T = C (%Zd)\n", T);
            gmp_printf("C = B (%Zd)\n", C);
            gmp_printf("B = S (%Zd)\n", B);
#endif

			swp1 = work->pt1.X;
            swp2 = work->pt1.Z;
			work->pt1.X = work->pt4.X;
            work->pt1.Z = work->pt4.Z;
			work->pt4.X = work->pt3.X;
            work->pt4.Z = work->pt3.Z;
			work->pt3.X = work->pt2.X;
            work->pt3.Z = work->pt2.Z;
			work->pt2.X = swp1;
            work->pt2.Z = swp2;
		}
		else if (steps[i] == 4)
		{
			uadd(rho, work, &work->pt2, &work->pt1, &work->pt3, &work->pt4);		// T = B + A (C)

#ifdef TRACEPRAC
            mpz_add(T, B, A);
            gmp_printf("T = B + A (%Zd)\n", T);

            mpz_set(S, B);
            mpz_set(B, T);
            mpz_set(T, C);
            mpz_set(C, S);

            gmp_printf("S = B (%Zd)\n", S);
            gmp_printf("B = T (%Zd)\n", B);
            gmp_printf("T = C (%Zd)\n", T);
            gmp_printf("C = S (%Zd)\n", S);
#endif

			swp1 = work->pt2.X;
            swp2 = work->pt2.Z;
			work->pt2.X = work->pt4.X;
            work->pt2.Z = work->pt4.Z;
			work->pt4.X = work->pt3.X;
            work->pt4.Z = work->pt3.Z;
			work->pt3.X = swp1;
			work->pt3.Z = swp2;
		}
		else if (steps[i] == 5)
		{
			d2 = submod(work->pt1.X, work->pt1.Z, work->n);
			s2 = addmod(work->pt1.X, work->pt1.Z, work->n);

			uadd(rho, work, &work->pt2, &work->pt1, &work->pt3, &work->pt2);		// B = B + A (C)
			udup(rho, work, s2, d2, &work->pt1);		// A = 2A

#ifdef TRACEPRAC
            mpz_add(B, B, A);
            mpz_add(A, A, A);
            gmp_printf("B = B + A (%Zd)\n", B);
            gmp_printf("A = 2 * A (%Zd)\n", A);
#endif
		}
		else if (steps[i] == 6)
		{
			uadd(rho, work, &work->pt1, &work->pt2, &work->pt3, P);		// A = A + B (C)

            work->pt1.X = work->pt2.X = work->pt3.X = P->X;
            work->pt1.Z = work->pt2.Z = work->pt3.Z = P->Z;

            d1 = submod(work->pt1.X, work->pt1.Z, work->n);
            s1 = addmod(work->pt1.X, work->pt1.Z, work->n);
            udup(rho, work, s1, d1, &work->pt1);

#ifdef TRACEPRAC
            mpz_add(p, A, B);
            mpz_set(A, p);
            mpz_set(B, p);
            mpz_set(C, p);
            gmp_printf("p = A + B (%Zd)\n", p);
            gmp_printf("A = B = C = P = %Zd\n", p);
            mpz_add(A, A, A);
            gmp_printf("A = 2 * A (%Zd)\n", A);
#endif
		}
	}

    uadd(rho, work, &work->pt1, &work->pt2, &work->pt3, P);		// A = A + B (C)
    
#ifdef TRACEPRAC
    mpz_add(p, A, B);
    gmp_printf("p = A + B (%Zd)\n", p);
    exit(1);
#endif

	return;

}

void uprac85(uint64_t rho, uecm_work *work, uecm_pt *P)
{
	uint64_t s1, s2, d1, d2;
	uint64_t swp;
	int i;
	static const uint8_t steps[146] = { 
		0,6,0,6,0,6,0,6,0,4,
		6,0,4,6,0,4,4,6,0,4,
		4,6,0,5,4,6,0,3,3,4,
		6,0,3,5,4,6,0,3,4,3,
		4,6,0,5,5,4,6,0,5,3,
		3,4,6,0,3,3,4,3,4,6,
		0,4,3,4,3,5,3,3,3,3,
		3,3,3,3,4,6,0,3,3,3,
		3,3,3,3,3,3,4,3,4,3,
		4,6,0,3,4,3,5,4,6,0,
		5,3,3,3,4,6,0,5,4,3,
		5,4,6,0,4,3,3,3,5,4,
		6,0,4,3,5,3,3,4,6,0,
		3,3,3,3,5,4,6,0,3,3,
		3,4,3,3,4,6 };

	for (i = 0; i < 146; i++)
	{
		if (steps[i] == 0)
		{
			work->pt1.X = work->pt2.X = work->pt3.X = P->X;
			work->pt1.Z = work->pt2.Z = work->pt3.Z = P->Z;

			d1 = submod(work->pt1.X, work->pt1.Z, work->n);
			s1 = addmod(work->pt1.X, work->pt1.Z, work->n);
			udup(rho, work, s1, d1, &work->pt1);
		}
		else if (steps[i] == 3)
		{
			// integrate step 4 followed by swap(1,2)
			uadd(rho, work, &work->pt2, &work->pt1, &work->pt3, &work->pt4);		// T = B + A (C)

			swp = work->pt1.X;
			work->pt1.X = work->pt4.X;
			work->pt4.X = work->pt3.X;
			work->pt3.X = work->pt2.X;
			work->pt2.X = swp;
			swp = work->pt1.Z;
			work->pt1.Z = work->pt4.Z;
			work->pt4.Z = work->pt3.Z;
			work->pt3.Z = work->pt2.Z;
			work->pt2.Z = swp;
		}
		else if (steps[i] == 4)
		{
			uadd(rho, work, &work->pt2, &work->pt1, &work->pt3, &work->pt4);		// T = B + A (C)

			swp = work->pt2.X;
			work->pt2.X = work->pt4.X;
			work->pt4.X = work->pt3.X;
			work->pt3.X = swp;
			swp = work->pt2.Z;
			work->pt2.Z = work->pt4.Z;
			work->pt4.Z = work->pt3.Z;
			work->pt3.Z = swp;
		}
		else if (steps[i] == 5)
		{
			d2 = submod(work->pt1.X, work->pt1.Z, work->n);
			s2 = addmod(work->pt1.X, work->pt1.Z, work->n);

			uadd(rho, work, &work->pt2, &work->pt1, &work->pt3, &work->pt2);		// B = B + A (C)
			udup(rho, work, s2, d2, &work->pt1);		// A = 2A
		}
		else if (steps[i] == 6)
		{
			uadd(rho, work, &work->pt1, &work->pt2, &work->pt3, P);		// A = A + B (C)
		}

	}

	return;

}

void uprac(uint64_t rho, uecm_work *work, uecm_pt *P, uint64_t c, double v)
{
	uint64_t d, e, r;
	int i;
	uint64_t s1, s2, d1, d2;
	uint64_t swp;

	d = c;
	r = (uint64_t)((double)d * v + 0.5);

	s1 = work->sum1;
	s2 = work->sum2;
	d1 = work->diff1;
	d2 = work->diff2;

	d = c - r;
	e = 2 * r - c;

	// the first one is always a doubling
	// point1 is [1]P
	work->pt1.X = work->pt2.X = work->pt3.X = P->X;
	work->pt1.Z = work->pt2.Z = work->pt3.Z = P->Z;

	d1 = submod(work->pt1.X, work->pt1.Z, work->n);
	s1 = addmod(work->pt1.X, work->pt1.Z, work->n);

	// point2 is [2]P
#ifdef PRACOPT
	printf("0,");
#endif
	
	udup(rho, work, s1, d1, &work->pt1);

	while (d != e)
	{
		if (d < e)
		{
#ifdef PRACOPT
			printf("1,");
#endif
			r = d;
			d = e;
			e = r;
			swp = work->pt1.X;
			work->pt1.X = work->pt2.X;
			work->pt2.X = swp;
			swp = work->pt1.Z;
			work->pt1.Z = work->pt2.Z;
			work->pt2.Z = swp;
		}

		if ((d + 3) / 4 <= e)
		{
#ifdef PRACOPT
			printf("4,");
#endif
			d -= e;

			uadd(rho, work, &work->pt2, &work->pt1, &work->pt3, &work->pt4);		// T = B + A (C)

			swp = work->pt2.X;
			work->pt2.X = work->pt4.X;
			work->pt4.X = work->pt3.X;
			work->pt3.X = swp;
			swp = work->pt2.Z;
			work->pt2.Z = work->pt4.Z;
			work->pt4.Z = work->pt3.Z;
			work->pt3.Z = swp;
		}
		else if ((d + e) % 2 == 0)
		{
#ifdef PRACOPT
			printf("5,");
#endif
			d = (d - e) / 2;

			d2 = submod(work->pt1.X, work->pt1.Z, work->n);
			s2 = addmod(work->pt1.X, work->pt1.Z, work->n);

			uadd(rho, work, &work->pt2, &work->pt1, &work->pt3, &work->pt2);		// B = B + A (C)
			udup(rho, work, s2, d2, &work->pt1);		// A = 2A
		}
		else
		{
#ifdef PRACOPT
		printf("a,");
#endif
			printf("unhandled case in prac\n");
			exit(1);
		}
	}

#ifdef PRACOPT
	printf("b,");
#endif
	uadd(rho, work, &work->pt1, &work->pt2, &work->pt3, P);		// A = A + B (C)

	return;

}

uint64_t modinv_64(uint64_t a, uint64_t p) {

	/* thanks to the folks at www.mersenneforum.org */

	uint64_t ps1, ps2, parity, dividend, divisor, rem, q, t;

	q = 1;
	rem = a;
	dividend = p;
	divisor = a;
	ps1 = 1;
	ps2 = 0;
	parity = 0;

	while (divisor > 1) {
		rem = dividend - divisor;
		t = rem - divisor;
		if (rem >= divisor) {
			q += ps1; rem = t; t -= divisor;
			if (rem >= divisor) {
				q += ps1; rem = t; t -= divisor;
				if (rem >= divisor) {
					q += ps1; rem = t; t -= divisor;
					if (rem >= divisor) {
						q += ps1; rem = t; t -= divisor;
						if (rem >= divisor) {
							q += ps1; rem = t; t -= divisor;
							if (rem >= divisor) {
								q += ps1; rem = t; t -= divisor;
								if (rem >= divisor) {
									q += ps1; rem = t; t -= divisor;
									if (rem >= divisor) {
										q += ps1; rem = t;
										if (rem >= divisor) {
											q = dividend / divisor;
											rem = dividend % divisor;
											q *= ps1;
										}
									}
								}
							}
						}
					}
				}
			}
		}

		q += ps2;
		parity = ~parity;
		dividend = divisor;
		divisor = rem;
		ps2 = ps1;
		ps1 = q;
	}

	if (parity == 0)
		return ps1;
	else
		return p - ps1;
}

uint64_t modinv_64_v(uint64_t a, uint64_t p) {

	/* thanks to the folks at www.mersenneforum.org */

	uint64_t ps1, ps2, parity, dividend, divisor, rem, q, t;

	q = 1;
	rem = a;
	dividend = p;
	divisor = a;
	ps1 = 1;
	ps2 = 0;
	parity = 0;
    int i = 0;
    
    printf("iteration %d:\n", i);
    printf("\trem = %lu\n", rem);
    printf("\tdividend = %lu\n", dividend);
    printf("\tdivisor = %lu\n", divisor);
    printf("\tq = %lu\n", q);
    printf("\tps1 = %lu\n", ps1);
    printf("\tps2 = %lu\n", ps2);
    printf("\tparity = %lu\n", parity);

	while (divisor > 1) {
        
        
		rem = dividend - divisor;
		t = rem - divisor;
		if (rem >= divisor) {
			q += ps1; rem = t; t -= divisor;
			if (rem >= divisor) {
				q += ps1; rem = t; t -= divisor;
				if (rem >= divisor) {
					q += ps1; rem = t; t -= divisor;
					if (rem >= divisor) {
						q += ps1; rem = t; t -= divisor;
						if (rem >= divisor) {
							q += ps1; rem = t; t -= divisor;
							if (rem >= divisor) {
								q += ps1; rem = t; t -= divisor;
								if (rem >= divisor) {
									q += ps1; rem = t; t -= divisor;
									if (rem >= divisor) {
										q += ps1; rem = t;
										if (rem >= divisor) {
											q = dividend / divisor;
											rem = dividend % divisor;
											q *= ps1;
										}
									}
								}
							}
						}
					}
				}
			}
		}

        i++;
		q += ps2;
		parity = ~parity;
		dividend = divisor;
		divisor = rem;
		ps2 = ps1;
		ps1 = q;
        
        printf("iteration %d:\n", i);
        printf("\trem = %lu\n", rem);
        printf("\tdividend = %lu\n", dividend);
        printf("\tdivisor = %lu\n", divisor);
        printf("\tq = %lu\n", q);
        printf("\tps1 = %lu\n", ps1);
        printf("\tps2 = %lu\n", ps2);
        printf("\tparity = %lu\n", parity);
	}

	if (parity == 0)
		return ps1;
	else
		return p - ps1;
}

int modinv_64_3(uint64_t *r, uint64_t a, uint64_t p) {

	/* thanks to the folks at www.mersenneforum.org */

	uint64_t ps1, ps2, parity, dividend, divisor, rem, q, t;

	q = 1;
	rem = a;
	dividend = p;
	divisor = a;
	ps1 = 1;
	ps2 = 0;
	parity = 0;

	while (divisor > 1) {
        
        
		rem = dividend - divisor;
		t = rem - divisor;
		if (rem >= divisor) {
			q += ps1; rem = t; t -= divisor;
			if (rem >= divisor) {
				q += ps1; rem = t; t -= divisor;
				if (rem >= divisor) {
					q += ps1; rem = t; t -= divisor;
					if (rem >= divisor) {
						q += ps1; rem = t; t -= divisor;
						if (rem >= divisor) {
							q += ps1; rem = t; t -= divisor;
							if (rem >= divisor) {
								q += ps1; rem = t; t -= divisor;
								if (rem >= divisor) {
									q += ps1; rem = t; t -= divisor;
									if (rem >= divisor) {
										q += ps1; rem = t;
										if (rem >= divisor) {
											q = dividend / divisor;
											rem = dividend % divisor;
											q *= ps1;
										}
									}
								}
							}
						}
					}
				}
			}
		}

		q += ps2;
		parity = ~parity;
		dividend = divisor;
		divisor = rem;
		ps2 = ps1;
		ps1 = q;
	}
    
    if (rem == 0)
    {
        *r = q;
        return 0;
    }
    else if (parity == 0)
    {
        *r = ps1;
		return 1;
    }
	else
    {
        * r = p - ps1;
		return 1;
    }
}

void ubuild(uecm_pt *P, uint64_t rho, uecm_work *work, uint32_t sigma)
{
	uint64_t t1, t2, t3, t4, t5, s[3];
	uint64_t u, v, n;
	n = work->n;

	if (sigma == 0)
	{
		work->sigma = spRand(7, (uint32_t)-1);
	}
	else
	{
		work->sigma = sigma;
	}
	sigma = work->sigma;

    //printf("sigma = %u\n", sigma);
    //printf("n = %lu\n", n);
    //printf("rho = %lu\n", rho);

	u = sigma;
	u = u64div(u, n);

	t1 = 4;
	t1 = u64div(t1, n);

	v = mulredcx(u, t1, n, rho);		// v = 4*sigma

	u = mulredcx(u, u, n, rho);
	t1 = 5;
	t1 = u64div(t1, n);
	u = submod(u, t1, n);			// u = sigma^2 - 5

	t1 = mulredcx(u, u, n, rho);
	P->X = mulredcx(t1, u, n, rho);	// x = u^3

	t1 = mulredcx(v, v, n, rho);
	P->Z = mulredcx(t1, v, n, rho);	// z = v^3

    //printf("X = %lu\n", P->X);
    //printf("Z = %lu\n", P->Z);

	//compute parameter A
	t1 = submod(v, u, n);			// (v - u)
	t2 = mulredcx(t1, t1, n, rho);
	t4 = mulredcx(t2, t1, n, rho);	// (v - u)^3

    //printf("(v-u) = %lu\n", t1);
    //printf("(v-u)^2 = %lu\n", t2);
    //printf("(v-u)^3 = %lu\n", t4);

	t1 = 3;
	t1 = u64div(t1, n);
	t2 = mulredcx(t1, u, n, rho);	// 3u
	t3 = addmod(t2, v, n);			// 3u + v

    //printf("3u + v = %lu\n", t3);

	t1 = mulredcx(t3, t4, n, rho);	// a = (v-u)^3 * (3u + v)

    //printf("a = (v-u)^3 * (3u + v) = %lu\n", t1);

	t2 = 16;
	t2 = u64div(t2, n);
	t3 = mulredcx(P->X, t2, n, rho);	// 16*u^3
	t4 = mulredcx(t3, v, n, rho);	// 16*u^3*v

    //printf("16*u^3*v = %lu\n", t4);

	// u holds the denom, t1 holds the numer
	// accomplish the division by multiplying by the modular inverse
	t2 = 1;
	t4 = mulredcx(t4, t2, n, rho);	// take t4 out of monty rep
	t1 = mulredcx(t1, t2, n, rho);	// take t1 out of monty rep

	t3 = modinv_64(t4, n);
	spMulMod(t3, t1, n, &work->s);
	work->s = u64div(work->s, n);

    //printf("s = (v-u)^3 * (3u + v) / (16*u^3*v) = %lu\n", work->s);

	return;
}

int tinyecm(uint64_t n, uint64_t *f, uint32_t B1, uint32_t B2, uint32_t curves)
{
	//attempt to factor n with the elliptic curve method
	//following brent and montgomery's papers, and CP's book
	int i, j;
	int curve;
	int result;
	uecm_work work;
	uecm_pt P;
	uint32_t sigma;
	uint64_t rho, x, tmp1;
    //uint32_t goodsigma[31] = {
    //    5907, 6468, 3233, 2302, 4513, 6028, 5830, 3305, 254, 8503, 3625, 2406, 7879, 2811, 5115, 2550, 4997, 8914, 7974,
    //    4395, 5965, 7695, 470, 5078, 6958, 1929, 9141, 3485, 5396, 5304, 6005 };
    
    // n must be at least 40 bits to use partially prebuilt curves.
    // small sigma that are more likely to successfully factor n, 
    // especially 11, as noted in Kruppa's talk.
    int use_prebuilt_curves = 1;
    uint32_t smallsigma[8] = { 11, 61, 56, 81, 83, 7, 30, 51 };
    uint64_t Z[8] = { 85184, 14526784, 11239424, 34012224, 36594368, 21952, 1728000, 8489664 };
    uint64_t X[8] = { 1560896ULL, 51312965696ULL, 30693697091ULL, 281784327616ULL,
        326229015104ULL, 85184ULL, 716917375ULL, 17495004736ULL };
    uint64_t u_v[8] = {373248ULL, 41854210048ULL, 24566036643ULL, 242037319168ULL, 
        281268868608ULL, 4096ULL, 465484375ULL, 13686220288ULL };
    uint64_t u3_v[8] = { 392, 11392, 9617, 19992, 20984, 160, 2805, 7992 };
    uint64_t v[8] = { 44, 244, 224, 324, 332, 28, 120, 204 };
    uint64_t d[8] = { 1098870784ULL, 200325818077184ULL, 110006210374144ULL, 1460769954361344ULL,
        1732928528232448ULL, 38162432ULL, 1376481360000ULL, 57103695458304ULL };

	x = (((n + 2) & 4) << 1) + n; // here x*a==1 mod 2**4
	x *= 2 - n * x;               // here x*a==1 mod 2**8
	x *= 2 - n * x;               // here x*a==1 mod 2**16
	x *= 2 - n * x;               // here x*a==1 mod 2**32         
	x *= 2 - n * x;               // here x*a==1 mod 2**64

    // check size of n and scale sigma down if necessary
    // (so that udiv64 works).
    int numlz = __builtin_clzll(n);
    
    if (numlz >= 24)
    {
        use_prebuilt_curves = 0;
        //printf("bitsize = %d, using random curves\n", 64 - numlz);
    }

	rho = (uint64_t)0 - x;
	work.n = n;
	work.stg1_max = B1;
	work.stg2_max = B2;

	if (0)
	{
        // printing out coprime residues
		j = 3;
		for (i = 3; i <= 105; i++)
		{
			if (i == 105)
			{
				printf("%d, ", j++);
			}
			else if ((spGCD(i, 105) == 1) && (i & 1))
			{
				printf("%d, ", j++);
			}
			else
			{
				printf("0, ");
			}
			if (i % 10 == 0)
				printf("\n");
		}
		printf("\n");
		exit(1);
	}

	if (0)
	{
        // a mini pair generator for use in uecm_stage2_D* or uecm_stage2_D*_inv
        // where * can be 60 or 120.
		uint32_t A = 150;
		uint32_t d = 120;
		uint32_t b1 = B1;
		uint32_t p;
		int k = 0;
        int b;
		int m = 0;

        // printing out coprime residues
        printf("b-vals: ");
        b = 0;
        for (i = 1; i < (d/2 + (d%2)); i++)
        {
            if (spGCD(i, d) == 1)
            {
                printf("%d, ", i);
                b++;
            }
        }
        printf("\n");
        printf("need %d b-vals: minimum %u muls\n", b, b * 6 + b);

		i = 0;
		while (primes[i] < b1)
			i++;

        k = 0;
		printf("starting A = %u, d = %u, prime = %u\n", A, d, primes[i]);
		while (A < B2)
		{
			int found;

			//printf("A = %u\n", A);
			while (primes[i] < A)
			{
				uint32_t s1 = A - primes[i];
				j = 1;
				while (primes[i + j] < A)
				{
					j++;
				}

				found = 0;
				while (primes[i+j] < (A + d/2))
				{
					if ((primes[i + j] - A) == s1)
					{
						//printf("%u,%u,%u\n", s1,primes[i+j],primes[i]);
						printf("%u,", s1);
						k++;
						found = 1;
						break;
					}
					j++;
				}
				if (!found)
				{
					//printf("%u,%u\n", s1, primes[i]);
					printf("%u,", s1);
					k++;
				}
				i++;
			}

			while (primes[i] < (A + d / 2))
			{
				j = 1;
				found = 0;
				while (primes[i - j] > (A - d / 2))
				{
					if ((A - primes[i - j]) == (primes[i] - A))
					{
						found = 1;
						break;
					}
					j++;
				}
				if (!found)
				{
					//printf("%u,%u\n", s1, primes[i]);
					printf("%u,", primes[i] - A);
					k++;
				}
				i++;
			}
			A += d;
            if (A < B2)
            {
                printf("0,");
                k++;
                m++;
            }
		}
		printf("\nlength %d, %d adds\n", k-m, m);
        printf("L=1: %u accum muls + %u setup muls = %u total muls\n", 
            (k - m) * 2 + m * 6 + m, 
            b * 6 + b, 
            (k - m) * 2 + m * 6 + m + b * 6 + b);
		exit(1);
	}

    if (0)
    {
        // for printing out pre-built constants of specific curves
        // that have been identified as more likely to factor general inputs.
        //for (curve = 0; curve < 32; curve++)
        //{
        //    ubuild(&P, rho, &work, goodsigma[curve]);
        //}
        //
        exit(0);
    }
	
	*f = 1;
	for (curve = 0; curve < curves; curve++)
	{
		uint64_t p;

		work.stg1Add = 0;
		work.stg1Doub = 0;
		work.last_pid = 0;

        if ((curve < 8) && (use_prebuilt_curves))
        {
            //ubuild(&P, rho, &work, goodsigma[curve]); // sigma);
            work.sigma = smallsigma[curve];
            // lookup point
            P.X = X[curve];
            P.Z = Z[curve];
            // some computation left to do for S parameter for this 'n'
            uint64_t num;
            spMulMod((n - u_v[curve]), u3_v[curve], n, &num);
            uint64_t dem = d[curve];
            dem = modinv_64(dem, n);
            spMulMod(num, dem, n, &work.s);
            work.s = u64div(work.s, n);      // put into Monty rep.
            P.X = u64div(P.X, n);            // put into Monty rep.
            P.Z = u64div(P.Z, n);            // put into Monty rep.
        }
        else
        {
            if (numlz >= 32)
            {
                work.sigma = spRand(7, (1ULL << (64 - numlz - 1)) - 1);
                //printf("bitsize = %d, using small curve %lu\n", 
                //    64 - numlz, work.sigma);
                ubuild(&P, rho, &work, work.sigma);
            }
            else
            {
                ubuild(&P, rho, &work, 0);
            }
        }

		uecm_stage1(rho, &work, &P);
		result = ucheck_factor(P.Z, n, &tmp1);

		if (result == 1)
		{
			*f = tmp1;
			break;
		}

		if (B2 > B1)
		{
			//uecm_stage2_D120_inv(&P, rho, &work);
            uecm_stage2_D120(&P, rho, &work);
            //uecm_stage2_D60(&P, rho, &work);

            //if (B1 < 60)
            //    uecm_stage2_w30u2(&P, rho, &work);
            //else
            //    uecm_stage2_w60u1(&P, rho, &work);

			result = ucheck_factor(work.stg2acc, n, &tmp1);

			if (result == 1)
			{
				*f = tmp1;
				break;
			}
		}
	}

	return curve;
}

void uecm_stage1(uint64_t rho, uecm_work *work, uecm_pt *P)
{
	int i;
	uint64_t q;
	uint64_t stg1 = (uint64_t)work->stg1_max;

	// handle the only even case 
	q = 2;
	while (q < stg1)
	{
		work->diff1 = submod(P->X, P->Z, work->n);
		work->sum1 = addmod(P->X, P->Z, work->n);
		udup(rho, work, work->sum1, work->diff1, P);
		q *= 2;
	}

    if (stg1 == 27)
    {
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 5, 0.618033988749894903);
        uprac(rho, work, P, 5, 0.618033988749894903);
        uprac(rho, work, P, 7, 0.618033988749894903);
        uprac(rho, work, P, 11, 0.580178728295464130);
        uprac(rho, work, P, 13, 0.618033988749894903);
        uprac(rho, work, P, 17, 0.618033988749894903);
        uprac(rho, work, P, 19, 0.618033988749894903);
        uprac(rho, work, P, 23, 0.522786351415446049);
        i = 9;
    }
    else if (stg1 == 29)
    {
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 5, 0.618033988749894903);
        uprac(rho, work, P, 5, 0.618033988749894903);
        uprac(rho, work, P, 7, 0.618033988749894903);
        uprac(rho, work, P, 11, 0.580178728295464130);
        uprac(rho, work, P, 13, 0.618033988749894903);
        uprac(rho, work, P, 17, 0.618033988749894903);
        uprac(rho, work, P, 19, 0.618033988749894903);
        uprac(rho, work, P, 23, 0.522786351415446049);
        uprac(rho, work, P, 29, 0.548409048446403258);
        i = 10;
    }
    else if (stg1 == 31)
    {
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 5, 0.618033988749894903);
        uprac(rho, work, P, 5, 0.618033988749894903);
        uprac(rho, work, P, 7, 0.618033988749894903);
        uprac(rho, work, P, 11, 0.580178728295464130);
        uprac(rho, work, P, 13, 0.618033988749894903);
        uprac(rho, work, P, 17, 0.618033988749894903);
        uprac(rho, work, P, 19, 0.618033988749894903);
        uprac(rho, work, P, 23, 0.522786351415446049);
        uprac(rho, work, P, 29, 0.548409048446403258);
        uprac(rho, work, P, 31, 0.618033988749894903);
        i = 11;
    }
    else if (stg1 == 37)
    {
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 5, 0.618033988749894903);
        uprac(rho, work, P, 5, 0.618033988749894903);
        uprac(rho, work, P, 7, 0.618033988749894903);
        uprac(rho, work, P, 11, 0.580178728295464130);
        uprac(rho, work, P, 13, 0.618033988749894903);
        uprac(rho, work, P, 17, 0.618033988749894903);
        uprac(rho, work, P, 19, 0.618033988749894903);
        uprac(rho, work, P, 23, 0.522786351415446049);
        uprac(rho, work, P, 29, 0.548409048446403258);
        uprac(rho, work, P, 31, 0.618033988749894903);
        uprac(rho, work, P, 37, 0.580178728295464130);
        i = 12;
    }
    else if (stg1 == 41)
    {
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 5, 0.618033988749894903);
        uprac(rho, work, P, 5, 0.618033988749894903);
        uprac(rho, work, P, 7, 0.618033988749894903);
        uprac(rho, work, P, 11, 0.580178728295464130);
        uprac(rho, work, P, 13, 0.618033988749894903);
        uprac(rho, work, P, 17, 0.618033988749894903);
        uprac(rho, work, P, 19, 0.618033988749894903);
        uprac(rho, work, P, 23, 0.522786351415446049);
        uprac(rho, work, P, 29, 0.548409048446403258);
        uprac(rho, work, P, 31, 0.618033988749894903);
        uprac(rho, work, P, 37, 0.580178728295464130);
        uprac(rho, work, P, 41, 0.548409048446403258);
        i = 13;
    }
    else if (stg1 == 43)
    {
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 5, 0.618033988749894903);
        uprac(rho, work, P, 5, 0.618033988749894903);
        uprac(rho, work, P, 7, 0.618033988749894903);
        uprac(rho, work, P, 11, 0.580178728295464130);
        uprac(rho, work, P, 13, 0.618033988749894903);
        uprac(rho, work, P, 17, 0.618033988749894903);
        uprac(rho, work, P, 19, 0.618033988749894903);
        uprac(rho, work, P, 23, 0.522786351415446049);
        uprac(rho, work, P, 29, 0.548409048446403258);
        uprac(rho, work, P, 31, 0.618033988749894903);
        uprac(rho, work, P, 37, 0.580178728295464130);
        uprac(rho, work, P, 41, 0.548409048446403258);
        uprac(rho, work, P, 43, 0.618033988749894903);
        i = 14;
    }
    else if (stg1 == 47)
    {
        uprac(rho, work, P, 3, 0.618033988749894903);
        uprac(rho, work, P, 3, 0.618033988749894903);
        uprac(rho, work, P, 3, 0.618033988749894903);
        uprac(rho, work, P, 5, 0.618033988749894903);
        uprac(rho, work, P, 5, 0.618033988749894903);
        uprac(rho, work, P, 7, 0.618033988749894903);
        uprac(rho, work, P, 11, 0.580178728295464130);
        uprac(rho, work, P, 13, 0.618033988749894903);
        uprac(rho, work, P, 17, 0.618033988749894903);
        uprac(rho, work, P, 19, 0.618033988749894903);
        uprac(rho, work, P, 23, 0.522786351415446049);
        uprac(rho, work, P, 29, 0.548409048446403258);
        uprac(rho, work, P, 31, 0.618033988749894903);
        uprac(rho, work, P, 37, 0.580178728295464130);
        uprac(rho, work, P, 41, 0.548409048446403258);
        uprac(rho, work, P, 43, 0.618033988749894903);
        uprac(rho, work, P, 47, 0.548409048446403258);
        i = 15;
    }
    else if (stg1 == 59)
    {
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 3, 0.61803398874989485);
        uprac(rho, work, P, 5, 0.618033988749894903);
        uprac(rho, work, P, 5, 0.618033988749894903);
        uprac(rho, work, P, 7, 0.618033988749894903);
        uprac(rho, work, P, 7, 0.618033988749894903);
        uprac(rho, work, P, 11, 0.580178728295464130);
        uprac(rho, work, P, 13, 0.618033988749894903);
        uprac(rho, work, P, 17, 0.618033988749894903);
        uprac(rho, work, P, 19, 0.618033988749894903);
        uprac(rho, work, P, 23, 0.522786351415446049);
        uprac(rho, work, P, 29, 0.548409048446403258);
        uprac(rho, work, P, 31, 0.618033988749894903);
        uprac(rho, work, P, 1961, 0.552936068843375);	// 37 * 53
        uprac(rho, work, P, 41, 0.548409048446403258);
        uprac(rho, work, P, 43, 0.618033988749894903);
        uprac(rho, work, P, 47, 0.548409048446403258);
        uprac(rho, work, P, 59, 0.548409048446403258);
        i = 17;
    }
    else if (stg1 == 70)
	{
		uprac70(rho, work, P);
		i = 19;
	}
	else if (stg1 >= 85)
	{
		// call prac with best ratios found by a deep search.
		// some composites are cheaper than their 
		// constituent primes.
		uprac85(rho, work, P);
		if (stg1 < 100)
		{
			// paired into a composite for larger bounds
			uprac(rho, work, P, 61, 0.522786351415446049);
		}
		i = 23;

		if (stg1 >= 125)
		{
			//printf("\nstart 125\n");
			uprac(rho, work, P, 5, 0.618033988749894903);
			uprac(rho, work, P, 11, 0.580178728295464130);
			uprac(rho, work, P, 89, 0.618033988749894903);
			uprac(rho, work, P, 97, 0.723606797749978936);
			uprac(rho, work, P, 101, 0.556250337855490828);
			uprac(rho, work, P, 107, 0.580178728295464130);
			uprac(rho, work, P, 109, 0.548409048446403258);
			uprac(rho, work, P, 113, 0.618033988749894903);

			if (stg1 < 130)
			{
				//printf("\nlast 103\n");
                uprac(rho, work, P, 61, 0.522786351415446049);
				uprac(rho, work, P, 103, 0.632839806088706269);
			}
			
			i = 30;
		}
		
		if (stg1 >= 165)
		{
			//printf("\nstart 165\n");
			uprac(rho, work, P, 7747, 0.552188778811121); // 61 x 127
			uprac(rho, work, P, 131, 0.618033988749894903);
			uprac(rho, work, P, 14111, 0.632839806088706);	// 103 x 137
			uprac(rho, work, P, 20989, 0.620181980807415);	// 139 x 151
			uprac(rho, work, P, 157, 0.640157392785047019);
			uprac(rho, work, P, 163, 0.551390822543526449);

			if (stg1 < 200)
			{
				//printf("\nlast 149\n");
				uprac(rho, work, P, 149, 0.580178728295464130);
			}
			i = 38;
		}
		
		if (stg1 >= 205)
		{
			//printf("\nstart 205\n");
			uprac(rho, work, P, 13, 0.618033988749894903);
			uprac(rho, work, P, 167, 0.580178728295464130);
			uprac(rho, work, P, 173, 0.612429949509495031);
			uprac(rho, work, P, 179, 0.618033988749894903);
			uprac(rho, work, P, 181, 0.551390822543526449);
			uprac(rho, work, P, 191, 0.618033988749894903);
			uprac(rho, work, P, 193, 0.618033988749894903);
			uprac(rho, work, P, 29353, 0.580178728295464);	// 149 x 197
			uprac(rho, work, P, 199, 0.551390822543526449);
			//printf("\n");
			//exit(1);
			i = 46;
		}
	}

	work->last_pid = i;
	return;
}

//#define DEBUG_STG2

#if D==60
static const int numb1_70 = 201;
static uint8_t b1_70[201] = { 19,17,11,7,1,13,0,23,19,13,11,1,7,17,29,0,29,19,17,13,11,1,23,0,29,19,13,7,1,11,23,0,23,19,17,13,1,7,29,0,23,17,11,7,1,19,29,0,29,19,17,11,7,1,13,0,23,19,11,7,1,13,0,29,23,13,7,1,17,0,29,23,17,13,11,1,0,29,17,13,7,1,11,19,0,23,17,11,7,1,19,0,23,13,1,11,17,19,29,0,17,13,11,7,0,23,19,11,1,7,17,0,23,19,13,7,1,29,0,29,19,17,11,1,13,0,23,19,17,13,7,1,0,19,17,7,1,11,23,0,29,17,13,7,1,19,0,13,11,7,1,17,29,0,29,23,11,17,0,29,11,1,13,17,19,23,0,23,19,17,11,1,13,29,0,19,7,1,13,23,29,0,23,19,11,7,17,29,0,29,23,13,7,17,19,0,17,13,11,1,23 };

static const int numb1_85 = 242;
static uint8_t b1_85[242] = { 1,13,0,23,19,13,11,1,7,17,29,0,29,19,17,13,11,1,23,0,29,19,13,7,1,11,23,0,23,19,17,13,1,7,29,0,23,17,11,7,1,19,29,0,29,19,17,11,7,1,13,0,23,19,11,7,1,13,0,29,23,13,7,1,17,0,29,23,17,13,11,1,0,29,17,13,7,1,11,19,0,23,17,11,7,1,19,0,23,13,1,11,17,19,29,0,17,13,11,7,0,23,19,11,1,7,17,0,23,19,13,7,1,29,0,29,19,17,11,1,13,0,23,19,17,13,7,1,0,19,17,7,1,11,23,0,29,17,13,7,1,19,0,13,11,7,1,17,29,0,29,23,11,17,0,29,11,1,13,17,19,23,0,23,19,17,11,1,13,29,0,19,7,1,13,23,29,0,23,19,11,7,17,29,0,29,23,13,7,17,19,0,17,13,11,1,23,0,29,23,17,11,7,13,19,0,29,19,7,1,17,0,29,23,19,17,13,11,1,0,19,17,1,23,29,0,23,17,13,11,7,1,19,29,0,17,7,1,11,13,19,29 };

static const int numb1_125 = 354;
static uint8_t b1_125[354] = { 23,19,13,11,1,7,17,29,0,29,19,17,13,11,1,23,0,29,19,13,7,1,11,23,0,23,19,17,13,1,7,29,0,23,17,11,7,1,19,29,0,29,19,17,11,7,1,13,0,23,19,11,7,1,13,0,29,23,13,7,1,17,0,29,23,17,13,11,1,0,29,17,13,7,1,11,19,0,23,17,11,7,1,19,0,23,13,1,11,17,19,29,0,17,13,11,7,0,23,19,11,1,7,17,0,23,19,13,7,1,29,0,29,19,17,11,1,13,0,23,19,17,13,7,1,0,19,17,7,1,11,23,0,29,17,13,7,1,19,0,13,11,7,1,17,29,0,29,23,11,17,0,29,11,1,13,17,19,23,0,23,19,17,11,1,13,29,0,19,7,1,13,23,29,0,23,19,11,7,17,29,0,29,23,13,7,17,19,0,17,13,11,1,23,0,29,23,17,11,7,13,19,0,29,19,7,1,17,0,29,23,19,17,13,11,1,0,19,17,1,23,29,0,23,17,13,11,7,1,19,29,0,17,7,1,11,13,19,29,0,19,17,1,7,11,13,23,0,29,11,13,17,23,0,29,13,11,7,1,17,19,23,0,29,23,17,13,1,0,29,23,19,13,1,7,11,0,19,13,7,11,17,29,0,23,17,13,0,29,19,11,7,1,0,19,17,1,7,11,23,0,23,13,11,7,1,17,19,29,0,23,19,17,11,1,0,23,13,1,7,11,29,0,17,13,7,1,11,29,0,23,13,7,1,17,29,0,17,13,7,1,29,0,29,19,11,7,0,29,23,11,7,1,19 };

static const int numb1_165 = 463;
static uint8_t b1_165[463] = { 13,7,1,11,19,0,29,17,13,11,7,1,23,0,29,23,19,17,7,11,13,0,29,23,13,11,7,1,19,0,23,19,11,1,13,29,0,23,19,17,13,1,7,11,29,0,19,17,1,7,23,29,0,29,23,13,7,1,17,19,0,29,19,17,13,7,1,23,0,29,19,11,1,7,13,23,0,29,23,19,11,7,17,0,29,19,17,13,11,1,23,0,23,19,17,13,7,11,29,0,23,19,13,7,11,17,0,29,23,11,7,1,13,19,0,29,19,17,11,7,13,23,0,23,17,11,13,0,29,19,13,7,1,17,23,0,29,23,11,1,17,19,0,29,23,19,17,13,1,7,0,19,13,7,1,29,0,17,13,11,7,1,19,0,29,19,17,13,11,7,1,23,0,29,17,11,7,1,19,23,0,23,19,13,11,7,1,17,0,23,17,13,11,19,29,0,19,17,7,1,13,0,23,17,13,11,1,0,29,13,1,7,11,17,19,0,19,13,7,11,29,0,29,7,1,13,17,19,23,0,29,23,13,11,1,0,19,17,13,11,1,29,0,29,23,19,17,7,1,0,17,13,7,1,19,23,0,29,13,11,7,1,17,0,29,7,1,11,17,0,29,23,19,17,11,7,1,0,23,19,13,1,7,17,0,17,1,11,19,23,29,0,29,23,1,11,13,0,23,19,7,17,0,29,23,17,13,11,7,1,19,0,29,19,11,7,17,0,29,23,19,17,1,13,0,29,23,19,1,7,17,0,23,13,1,17,29,0,29,1,11,19,23,0,23,19,11,1,7,29,0,11,1,17,0,17,13,11,1,7,23,29,0,23,19,11,13,17,0,29,1,7,13,19,23,0,29,17,13,1,11,0,29,13,7,0,23,19,17,13,11,0,29,23,13,11,7,1,17,19,0,29,19,17,7,13,23,0,29,23,17,1,11,13,0,29,23,19,11,1,7,13,0,19,13,11,1,17,23,0,19,17,7,11,13,23,0,23,19,11,7,17,29,0,29,17,13,7,0,19,17,13,7,1,29,0,29,23,7,1,11,13,19 };

static const int numb1_205 = 566;
static uint8_t b1_205[566] = { 29,17,13,11,7,1,23,0,29,23,19,17,7,11,13,0,29,23,13,11,7,1,19,0,23,19,11,1,13,29,0,23,19,17,13,1,7,11,29,0,19,17,1,7,23,29,0,29,23,13,7,1,17,19,0,29,19,17,13,7,1,23,0,29,19,11,1,7,13,23,0,29,23,19,11,7,17,0,29,19,17,13,11,1,23,0,23,19,17,13,7,11,29,0,23,19,13,7,11,17,0,29,23,11,7,1,13,19,0,29,19,17,11,7,13,23,0,23,17,11,13,0,29,19,13,7,1,17,23,0,29,23,11,1,17,19,0,29,23,19,17,13,1,7,0,19,13,7,1,29,0,17,13,11,7,1,19,0,29,19,17,13,11,7,1,23,0,29,17,11,7,1,19,23,0,23,19,13,11,7,1,17,0,23,17,13,11,19,29,0,19,17,7,1,13,0,23,17,13,11,1,0,29,13,1,7,11,17,19,0,19,13,7,11,29,0,29,7,1,13,17,19,23,0,29,23,13,11,1,0,19,17,13,11,1,29,0,29,23,19,17,7,1,0,17,13,7,1,19,23,0,29,13,11,7,1,17,0,29,7,1,11,17,0,29,23,19,17,11,7,1,0,23,19,13,1,7,17,0,17,1,11,19,23,29,0,29,23,1,11,13,0,23,19,7,17,0,29,23,17,13,11,7,1,19,0,29,19,11,7,17,0,29,23,19,17,1,13,0,29,23,19,1,7,17,0,23,13,1,17,29,0,29,1,11,19,23,0,23,19,11,1,7,29,0,11,1,17,0,17,13,11,1,7,23,29,0,23,19,11,13,17,0,29,1,7,13,19,23,0,29,17,13,1,11,0,29,13,7,0,23,19,17,13,11,0,29,23,13,11,7,1,17,19,0,29,19,17,7,13,23,0,29,23,17,1,11,13,0,29,23,19,11,1,7,13,0,19,13,11,1,17,23,0,19,17,7,11,13,23,0,23,19,11,7,17,29,0,29,17,13,7,0,19,17,13,7,1,29,0,29,23,7,1,11,13,19,0,29,13,11,7,1,17,19,0,23,1,11,17,19,29,0,29,19,17,7,1,11,13,23,0,23,7,17,19,29,0,23,17,7,11,29,0,19,17,1,7,11,23,0,19,17,7,13,23,0,13,11,1,7,23,0,29,23,17,1,19,0,29,23,17,7,1,11,0,19,17,11,7,0,17,13,11,7,1,0,29,1,11,17,0,17,11,1,13,23,0,29,23,13,11,7,19,0,29,19,17,1,11,0,23,19,13,1,7 };
#else

// assumes starting at A=120 and covering the whole range; needs a special case first giant step.
// it is like a special-case w=120, U=1, L=1 PAIR
// 13 giant steps (13 adds + 13 muls) and 186 accums = 463 muls
//static const int special_70 = 1;
//static const int numb1_70 = 199;
//static uint8_t b1_70[199] = {49, 47, 41, 37, 31, 23, 19, 17, 13, 11, 7, 29, 43, 0, 59, 49, 47, 43, 41, 29, 17, 13, 11, 7, 1, 23, 31, 37, 53, 0, 53, 49, 47, 43, 29, 23, 13, 11, 7, 1, 19, 37, 41, 59, 0, 59, 49, 47, 41, 37, 31, 23, 19, 17, 13, 1, 7, 11, 29, 43, 0, 59, 53, 43, 37, 31, 29, 23, 13, 7, 1, 17, 19, 41, 47, 0, 59, 47, 43, 37, 29, 19, 11, 1, 7, 13, 23, 31, 41, 49, 53, 0, 53, 43, 31, 29, 19, 17, 13, 11, 1, 23, 37, 41, 47, 0, 53, 49, 41, 31, 23, 19, 13, 7, 11, 17, 37, 59, 0, 59, 49, 47, 41, 31, 29, 19, 17, 11, 7, 13, 23, 37, 43, 0, 49, 47, 37, 29, 19, 13, 7, 1, 17, 23, 31, 59, 0, 43, 41, 37, 31, 29, 23, 19, 17, 13, 1, 7, 47, 53, 0, 59, 41, 31, 17, 13, 11, 7, 1, 19, 43, 47, 49, 53, 0, 49, 37, 29, 17, 11, 7, 1, 19, 23, 41, 47, 53, 59, 0, 59, 53, 43, 23, 17, 13, 11, 19, 29, 41};

// assumes starting at A=150 and missing the primes between 70 and 90
// it is like a special-case w=120, U=1, L=1 PAIR
// 14 giant steps (14 adds + 14 muls) and 172 accums = 442 muls
static const int special_70 = 0;
static const int numb1_70 = 186;
static uint8_t b1_70[186] = { 53,49,47,43,41,37,23,19,13,11,1,7,17,29,31,0,59,47,43,41,37,31,29,19,13,7,1,11,23,0,59,53,43,41,37,31,23,17,11,7,1,19,29,49,0,53,49,47,43,31,23,19,11,7,1,13,37,59,0,59,53,43,37,31,29,23,17,13,11,1,47,0,59,49,41,31,23,17,11,7,1,19,37,47,0,59,49,47,43,41,31,17,13,11,7,37,0,53,49,43,37,23,19,13,7,1,29,31,41,59,0,59,49,47,41,23,19,17,13,7,1,43,53,0,59,49,43,37,29,17,13,7,1,19,47,53,0,59,53,49,47,43,31,29,23,11,17,0,47,43,41,37,31,23,19,17,11,1,13,29,53,0,59,47,41,37,31,23,19,11,7,17,29,0,53,47,43,41,17,13,11,1,23,31,37,49 };

// with map[] already included... didn't really help speed at all.
//static const int special_70 = 0;
//static const int numb1_70 = 186;
//static uint8_t b1_70[186] = {16, 15, 14, 13, 12, 11, 8, 7, 5, 4, 1, 3, 6, 9, 10, 0, 17, 14, 13, 12, 11, 10, 9, 7, 5, 3, 1, 4, 8, 0, 17, 16, 13, 12, 11, 10, 8, 6, 4, 3, 1, 7, 9, 15, 0, 16, 15, 14, 13, 10, 8, 7, 4, 3, 1, 5, 11, 17, 0, 17, 16, 13, 11, 10, 9, 8, 6, 5, 4, 1, 14, 0, 17, 15, 12, 10, 8, 6, 4, 3, 1, 7, 11, 14, 0, 17, 15, 14, 13, 12, 10, 6, 5, 4, 3, 11, 0, 16, 15, 13, 11, 8, 7, 5, 3, 1, 9, 10, 12, 17, 0, 17, 15, 14, 12, 8, 7, 6, 5, 3, 1, 13, 16, 0, 17, 15, 13, 11, 9, 6, 5, 3, 1, 7, 14, 16, 0, 17, 16, 15, 14, 13, 10, 9, 8, 4, 6, 0, 14, 13, 12, 11, 10, 8, 7, 6, 4, 1, 5, 9, 16, 0, 17, 14, 12, 11, 10, 8, 7, 4, 3, 6, 9, 0, 16, 14, 13, 12, 6, 5, 4, 1, 8, 10, 11, 15};

static const int numb1_85 = 225;
static uint8_t b1_85[225] = { 1,53,49,47,43,41,37,23,19,13,11,1,7,17,29,31,0,59,47,43,41,37,31,29,19,13,7,1,11,23,0,59,53,43,41,37,31,23,17,11,7,1,19,29,49,0,53,49,47,43,31,23,19,11,7,1,13,37,59,0,59,53,43,37,31,29,23,17,13,11,1,47,0,59,49,41,31,23,17,11,7,1,19,37,47,0,59,49,47,43,41,31,17,13,11,7,37,0,53,49,43,37,23,19,13,7,1,29,31,41,59,0,59,49,47,41,23,19,17,13,7,1,43,53,0,59,49,43,37,29,17,13,7,1,19,47,53,0,59,53,49,47,43,31,29,23,11,17,0,47,43,41,37,31,23,19,17,11,1,13,29,53,0,59,47,41,37,31,23,19,11,7,17,29,0,53,47,43,41,17,13,11,1,23,31,37,49,0,53,47,43,41,29,19,7,1,17,31,37,49,59,0,49,43,37,19,17,1,23,29,47,53,0,59,53,43,41,31,17,7,1,11,13,19,29 };

static const int numb1_125 = 319;
static uint8_t b1_125[319] = { 23,19,13,11,1,7,17,29,31,0,59,47,43,41,37,31,29,19,13,7,1,11,23,0,59,53,43,41,37,31,23,17,11,7,1,19,29,49,0,53,49,47,43,31,23,19,11,7,1,13,37,59,0,59,53,43,37,31,29,23,17,13,11,1,47,0,59,49,41,31,23,17,11,7,1,19,37,47,0,59,49,47,43,41,31,17,13,11,7,37,0,53,49,43,37,23,19,13,7,1,29,31,41,59,0,59,49,47,41,23,19,17,13,7,1,43,53,0,59,49,43,37,29,17,13,7,1,19,47,53,0,59,53,49,47,43,31,29,23,11,17,0,47,43,41,37,31,23,19,17,11,1,13,29,53,0,59,47,41,37,31,23,19,11,7,17,29,0,53,47,43,41,17,13,11,1,23,31,37,49,0,53,47,43,41,29,19,7,1,17,31,37,49,59,0,49,43,37,19,17,1,23,29,47,53,0,59,53,43,41,31,17,7,1,11,13,19,29,0,59,53,49,47,37,29,11,13,17,23,31,0,59,43,41,37,29,23,17,13,1,31,47,0,59,53,49,47,41,37,31,19,13,7,11,17,29,43,0,47,29,19,11,7,1,41,43,59,0,53,49,37,23,13,11,7,1,17,19,29,41,43,59,0,59,49,41,37,23,13,1,7,11,29,43,47,53,0,59,53,49,31,23,13,7,1,17,29,43,47,0,59,31,29,19,11,7,37,49,53 };

static const int numb1_165 = 425;
static uint8_t b1_165[425] = { 13,7,1,11,19,47,59,0,59,49,43,37,31,29,23,19,17,7,11,13,47,53,0,53,47,41,37,31,23,19,11,1,13,29,43,59,0,53,49,41,37,31,19,17,1,7,23,29,47,59,0,59,53,47,43,41,29,19,17,13,7,1,23,31,49,0,53,47,41,37,29,23,19,11,7,17,31,43,49,59,0,47,43,41,37,23,19,17,13,7,11,29,53,0,53,49,43,37,29,23,11,7,1,13,19,31,41,0,53,49,47,43,37,31,23,17,11,13,41,0,59,47,43,37,31,29,23,11,1,17,19,41,0,59,53,19,13,7,1,29,43,47,49,0,53,49,47,41,29,19,17,13,11,7,1,23,31,43,59,0,53,49,41,37,23,19,13,11,7,1,17,43,47,0,47,43,41,31,19,17,7,1,13,37,49,0,59,49,37,29,13,1,7,11,17,19,41,47,53,0,49,47,31,29,7,1,13,17,19,23,37,59,0,47,37,31,19,17,13,11,1,29,41,43,53,0,59,41,17,13,7,1,19,23,31,47,49,53,0,59,53,47,43,31,29,7,1,11,17,37,41,49,0,49,43,37,23,19,13,1,7,17,0,59,49,41,37,31,29,23,1,11,13,53,0,53,43,41,37,29,23,17,13,11,7,1,19,31,49,0,53,43,31,29,23,19,17,1,13,37,41,59,0,53,43,37,31,23,13,1,17,29,59,0,59,49,41,37,23,19,11,1,7,29,0,59,43,17,13,11,1,7,23,29,37,41,49,0,49,47,43,41,29,1,7,13,19,23,31,59,0,59,49,47,31,29,13,7,37,41,43,0,49,41,29,23,13,11,7,1,17,19,31,43,53,0,53,47,43,37,29,23,17,1,11,13,31,41,49,59,0,53,47,41,19,13,11,1,17,23,43,0,53,49,47,37,23,19,11,7,17,29,31,43,0,53,31,19,17,13,7,1,29,37,59 };

static const int numb1_205 = 511;
static uint8_t b1_205[511] = { 1,23,41,0,59,53,49,47,37,23,19,17,13,1,7,29,43,0,53,49,41,31,29,19,17,11,7,1,13,37,59,0,49,47,29,23,13,7,1,17,31,37,43,0,59,49,47,43,37,31,29,17,13,7,1,11,19,53,0,59,53,49,41,37,23,13,1,11,17,19,29,43,47,0,53,49,47,43,23,19,11,1,7,17,37,41,0,59,53,41,37,31,29,19,17,11,1,13,43,47,0,53,47,41,19,17,7,1,11,23,31,43,59,0,59,53,41,31,13,11,7,1,17,29,37,0,49,43,37,29,11,1,13,17,19,23,41,0,59,49,47,43,41,37,31,19,7,1,13,23,29,53,0,53,49,43,41,37,31,29,23,13,7,17,19,47,59,0,49,47,37,29,23,17,11,7,13,19,31,41,53,0,59,43,29,23,19,17,13,11,1,41,0,59,37,31,23,17,13,11,7,1,19,29,43,53,0,49,47,43,41,31,19,17,1,7,11,13,23,0,47,43,37,29,13,11,7,1,17,19,23,31,59,0,59,37,31,29,23,19,13,1,7,11,41,47,53,0,53,49,43,31,23,17,13,41,59,0,59,53,31,19,17,1,7,11,23,37,47,49,0,59,53,47,43,41,37,31,23,19,17,11,1,0,59,53,49,47,31,17,13,7,1,11,29,37,0,53,43,31,17,13,7,1,29,41,49,0,53,49,41,29,23,11,7,1,19,31,47,0,47,43,41,29,23,19,7,1,11,49,0,59,31,29,23,17,11,7,1,13,41,43,0,59,43,37,17,1,7,11,13,19,41,49,0,59,53,43,41,37,31,29,23,13,11,1,47,0,59,53,47,31,19,17,13,1,7,11,29,37,43,49,0,49,43,41,31,17,13,7,11,23,37,53,0,53,49,41,23,19,13,11,7,1,17,37,59,0,49,47,43,37,31,29,23,1,7,41,0,59,43,41,37,31,17,13,11,7,47,49,0,59,49,47,37,31,29,19,17,7,1,0,53,47,37,19,13,1,11,31,41,0,49,47,37,23,17,13,11,7,19,31,53,0,59,53,47,29,13,11,7,1,23,41,0,49,47,41,37,19,11,13,17,23,29,31,43,0,59,29,19,13,1,41,43,47,53,0,59,53,43,41,37,23,17,11,7,1,13,29,49 };

#endif

void uecm_stage2_D60(uecm_pt *P, uint64_t rho, uecm_work *work)
{
	int b;
	int i, j, k, m;
	uecm_pt *Pa = &work->Pa;
	uecm_pt *Pb = work->Pb;
	uecm_pt *Pd;
	uint64_t acc = work->stg2acc;
	uint8_t *barray;
	uint32_t numb;

	work->paired = 0;
	work->numprimes = 0;
	work->ptadds = 0;
	work->stg1Add = 0;
	work->stg1Doub = 0;

	//stage 2 init
	//Q = P = result of stage 1
	//compute [d]Q for 0 < d <= D
	Pd = &Pb[map[D]];

	// [1]Q
	Pb[1].Z = P->Z;
	Pb[1].X = P->X;
	work->Pbprod[1] = mulredcx(Pb[1].X, Pb[1].Z, work->n, rho);

	// [2]Q
	Pb[2].Z = P->Z;
	Pb[2].X = P->X;
	work->diff1 = submod(P->X, P->Z, work->n);
	work->sum1 = addmod(P->X, P->Z, work->n);
	udup(rho, work, work->sum1, work->diff1, &Pb[2]);
	work->Pbprod[2] = mulredcx(Pb[2].X, Pb[2].Z, work->n, rho);

	// Calculate all Pb: the following is specialized for D=60
	// [2]Q + [1]Q([1]Q) = [3]Q
	uadd(rho, work, &Pb[1], &Pb[2], &Pb[1], &Pb[3]);		// <-- temporary

	// 2*[3]Q = [6]Q
	work->diff1 = submod(Pb[3].X, Pb[3].Z, work->n);
	work->sum1 = addmod(Pb[3].X, Pb[3].Z, work->n);
	udup(rho, work, work->sum1, work->diff1, &work->pt3);	// pt3 = [6]Q

	// [3]Q + [2]Q([1]Q) = [5]Q
	uadd(rho, work, &Pb[3], &Pb[2], &Pb[1], &work->pt1);	// <-- pt1 = [5]Q
	Pb[3].X = work->pt1.X;
	Pb[3].Z = work->pt1.Z;

	// [6]Q + [5]Q([1]Q) = [11]Q
	uadd(rho, work, &work->pt3, &work->pt1, &Pb[1], &Pb[4]);	// <-- [11]Q

	// [11]Q + [6]Q([5]Q) = [17]Q
	uadd(rho, work, &work->pt3, &Pb[4], &Pb[3], &Pb[map[17]]);

	// [17]Q + [6]Q([11]Q) = [23]Q
	uadd(rho, work, &work->pt3, &Pb[map[17]], &Pb[map[11]], &Pb[map[23]]);

	// [23]Q + [6]Q([17]Q) = [29]Q
	uadd(rho, work, &work->pt3, &Pb[map[23]], &Pb[map[17]], &Pb[map[29]]);

	// [6]Q + [1]Q([5]Q) = [7]Q
	uadd(rho, work, &work->pt3, &Pb[1], &work->pt1, &Pb[3]);	// <-- [7]Q

	// [7]Q + [6]Q([1]Q) = [13]Q
	uadd(rho, work, &work->pt3, &Pb[map[7]], &Pb[1], &Pb[map[13]]);

	// [13]Q + [6]Q([7]Q) = [19]Q
	uadd(rho, work, &work->pt3, &Pb[map[13]], &Pb[map[7]], &Pb[map[19]]);

	// temporary - make [4]Q
	work->diff1 = submod(Pb[2].X, Pb[2].Z, work->n);
	work->sum1 = addmod(Pb[2].X, Pb[2].Z, work->n);
	udup(rho, work, work->sum1, work->diff1, &work->pt3);	// pt3 = [4]Q

	// Pd = [w]Q
	// [17]Q + [13]Q([4]Q) = [30]Q
	uadd(rho, work, &Pb[map[17]], &Pb[map[13]], &work->pt3, &work->Pad);	// <-- [30]Q

	// make all of the Pbprod's
	//for (i = 3; i < 19; i++)
	for (i = 3; i < 10; i++)
	{
		work->Pbprod[i] = mulredcx(Pb[i].X, Pb[i].Z, work->n, rho);
	}

	//first A value: first multiple of D greater than B1
	work->A = work->stg1_max / D; // +(work->stg1_max % D != 0);
	work->A *= D;
	work->A += D / 2;		// should be 90.  get there by 3*[30]Q

	//initialize info needed for giant step
	work->A = 90;
	work->diff1 = submod(work->Pad.X, work->Pad.Z, work->n);
	work->sum1 = addmod(work->Pad.X, work->Pad.Z, work->n);
	udup(rho, work, work->sum1, work->diff1, Pd);	// Pd = [60]Q
	uadd(rho, work, Pd, &work->Pad, &work->Pad, Pa);	// Pa = [90]Q

	// adjustment of Pa and Pad for larger B1.
	// Currently we have Pa=90, Pd=60, Pad=30
	if (work->stg1_max == 125)
	{
		// need Pa = 150, Pad = 90
		work->pt1.X = Pa->X;
		work->pt1.Z = Pa->Z;

		// 90+60
		uadd(rho, work, Pa, Pd, &work->Pad, Pa);

		//Pad holds the previous Pa
		work->Pad.X = work->pt1.X;
		work->Pad.Z = work->pt1.Z;

	}
	else if (work->stg1_max == 165)
	{
		// need Pa = 180, Pad = 120
		work->diff1 = submod(Pa->X, Pa->Z, work->n);
		work->sum1 = addmod(Pa->X, Pa->Z, work->n);
		udup(rho, work, work->sum1, work->diff1, Pa);	// Pa = [180]Q

		work->diff1 = submod(Pd->X, Pd->Z, work->n);
		work->sum1 = addmod(Pd->X, Pd->Z, work->n);
		udup(rho, work, work->sum1, work->diff1, &work->Pad);	// Pad = [120]Q

	}
	else if (work->stg1_max == 205)
	{
		// need Pa = 240, Pad = 180
		work->pt1.X = Pa->X;
		work->pt1.Z = Pa->Z;

		// 90+30
		uadd(rho, work, Pa, &work->Pad, Pd, Pa);
		work->diff1 = submod(Pa->X, Pa->Z, work->n);
		work->sum1 = addmod(Pa->X, Pa->Z, work->n);
		udup(rho, work, work->sum1, work->diff1, Pa);	// Pa = [240]Q

		work->diff1 = submod(work->pt1.X, work->pt1.Z, work->n);
		work->sum1 = addmod(work->pt1.X, work->pt1.Z, work->n);
		udup(rho, work, work->sum1, work->diff1, &work->Pad);	// Pad = [180]Q
	}

	//initialize accumulator and Paprod
	acc = u64div(1, work->n);
	work->Paprod = mulredcx(Pa->X, Pa->Z, work->n, rho);

	if (work->stg1_max == 70)
	{
		barray = b1_70;
		numb = numb1_70;
	}
	else if (work->stg1_max == 85)
	{
		barray = b1_85;
		numb = numb1_85;
	}
	else if (work->stg1_max == 125)
	{
		barray = b1_125;
		numb = numb1_125;
	}
	else if (work->stg1_max == 165)
	{
		barray = b1_165;
		numb = numb1_165;
	}
	else if (work->stg1_max == 205)
	{
		barray = b1_205;
		numb = numb1_205;
	}

	for (i = 0; i < numb; i++)
	{
		if (barray[i] == 0)
		{
			//giant step - use the addition formula for ECM
			work->pt1.X = Pa->X;
			work->pt1.Z = Pa->Z;

			//Pa + Pd
			uadd(rho, work, Pa, Pd, &work->Pad, Pa);

			//Pad holds the previous Pa
			work->Pad.X = work->pt1.X;
			work->Pad.Z = work->pt1.Z;

			//and Paprod
			work->Paprod = mulredcx(Pa->X, Pa->Z, work->n, rho);

			i++;
		}

		//we accumulate XrZd - XdZr = (Xr - Xd) * (Zr + Zd) + XdZd - XrZr
		//in CP notation, Pa -> (Xr,Zr), Pb -> (Xd,Zd)

		b = barray[i];
		// accumulate the cross product  (zimmerman syntax).
		// page 342 in C&P
		work->tt1 = submod(Pa->X, Pb[map[b]].X, work->n);
		work->tt2 = addmod(Pa->Z, Pb[map[b]].Z, work->n);
		work->tt3 = mulredcx(work->tt1, work->tt2, work->n, rho);
		work->tt1 = addmod(work->tt3, work->Pbprod[map[b]], work->n);
		work->tt2 = submod(work->tt1, work->Paprod, work->n);
		acc = mulredcx(acc, work->tt2, work->n, rho);

	}

	work->stg2acc = acc;

	return;
}

void uecm_stage2_D120(uecm_pt *P, uint64_t rho, uecm_work *work)
{
	int b;
	int i, j, k, m;
	uecm_pt *Pa = &work->Pa;
	uecm_pt *Pb = work->Pb;
	uecm_pt *Pd;
	uint64_t acc = work->stg2acc;
	uint8_t *barray;
	uint32_t numb;
    uint64_t Pbprod[19];
    uint64_t Paprod;
    uint64_t n = work->n;
    uint64_t sum1;
    uint64_t diff1;
    uecm_pt Pad;

	work->paired = 0;
	work->numprimes = 0;
	work->ptadds = 0;
	work->stg1Add = 0;
	work->stg1Doub = 0;

	//stage 2 init
	//Q = P = result of stage 1
	//compute [d]Q for 0 < d <= D
	Pd = &Pb[map[60]];

    // Pb setup
    {
        // [1]Q
        Pb[1].Z = P->Z;
        Pb[1].X = P->X;
        Pbprod[1] = mulredcx(Pb[1].X, Pb[1].Z, n, rho);

        // [2]Q
        Pb[2].Z = P->Z;
        Pb[2].X = P->X;
        diff1 = submod(P->X, P->Z, n);
        sum1 = addmod(P->X, P->Z, n);
        udup(rho, work, sum1, diff1, &Pb[2]);
        Pbprod[2] = mulredcx(Pb[2].X, Pb[2].Z, n, rho);

        // Calculate all Pb: the following is specialized for D=60
        // [2]Q + [1]Q([1]Q) = [3]Q
        uadd(rho, work, &Pb[1], &Pb[2], &Pb[1], &Pb[3]);		// <-- temporary

        // 2*[3]Q = [6]Q
        diff1 = submod(Pb[3].X, Pb[3].Z, n);
        sum1 = addmod(Pb[3].X, Pb[3].Z, n);
        udup(rho, work, sum1, diff1, &work->pt3);	// pt3 = [6]Q

        // [3]Q + [2]Q([1]Q) = [5]Q
        uadd(rho, work, &Pb[3], &Pb[2], &Pb[1], &work->pt1);	// <-- pt1 = [5]Q
        Pb[3].X = work->pt1.X;
        Pb[3].Z = work->pt1.Z;

        // [6]Q + [5]Q([1]Q) = [11]Q
        uadd(rho, work, &work->pt3, &work->pt1, &Pb[1], &Pb[4]);	// <-- [11]Q

        i = 3;
        k = 4;
        j = 5;
        while ((j + 12) < (60))
        {
            // [j+6]Q + [6]Q([j]Q) = [j+12]Q
            uadd(rho, work, &work->pt3, &Pb[k], &Pb[i], &Pb[map[j + 12]]);
            i = k;
            k = map[j + 12];
            j += 6;
        }

        // [6]Q + [1]Q([5]Q) = [7]Q
        uadd(rho, work, &work->pt3, &Pb[1], &work->pt1, &Pb[3]);	// <-- [7]Q
        i = 1;
        k = 3;
        j = 1;
        while ((j + 12) < (60))
        {
            // [j+6]Q + [6]Q([j]Q) = [j+12]Q
            uadd(rho, work, &work->pt3, &Pb[k], &Pb[i], &Pb[map[j + 12]]);
            i = k;
            k = map[j + 12];
            j += 6;
        }

        // Pd = [2w]Q
        // [31]Q + [29]Q([2]Q) = [60]Q
        uadd(rho, work, &Pb[9], &Pb[10], &Pb[2], Pd);	// <-- [60]Q
        work->ptadds++;

        // make all of the Pbprod's
        for (i = 3; i < 19; i++)
        {
            Pbprod[i] = mulredcx(Pb[i].X, Pb[i].Z, n, rho);
        }
    }

    // Pa setup
    {
        //initialize info needed for giant step
        // temporary - make [4]Q
        diff1 = submod(Pb[2].X, Pb[2].Z, n);
        sum1 = addmod(Pb[2].X, Pb[2].Z, n);
        udup(rho, work, sum1, diff1, &work->pt3);	// pt3 = [4]Q

        // Pd = [w]Q
        // [17]Q + [13]Q([4]Q) = [30]Q
        uadd(rho, work, &Pb[map[17]], &Pb[map[13]], &work->pt3, &Pad);	// <-- [30]Q

        // [60]Q + [30]Q([30]Q) = [90]Q
        uadd(rho, work, Pd, &Pad, &Pad, Pa);
        work->pt1.X = Pa->X;
        work->pt1.Z = Pa->Z;

        // [90]Q + [30]Q([60]Q) = [120]Q
        uadd(rho, work, Pa, &Pad, Pd, Pa);

        // adjustment of Pa and Pad for larger B1.
        // Currently we have Pa=120, Pd=60, Pad=30
        if ((work->stg1_max > 70) || !special_70)
        {
            // Pd = [120]Q
            Pd->X = Pa->X;
            Pd->Z = Pa->Z;

            // [120]Q + [30]Q([90]Q) = [150]Q
            uadd(rho, work, Pa, &Pad, &work->pt1, Pa);

            // now we have Pa=150, Pd=120, Pad=30 and are ready for strides of 120
        }

        if (work->stg1_max == 165)
        {
            // need Pa = 180, Pad = 60
            // [150]Q + [30]Q([120]Q) = [180]Q
            uadd(rho, work, Pa, &Pad, Pd, Pa);

            diff1 = submod(Pad.X, Pad.Z, n);
            sum1 = addmod(Pad.X, Pad.Z, n);
            udup(rho, work, sum1, diff1, &Pad);	// Pad = [60]Q
        }
        else if (work->stg1_max == 205)
        {
            // need Pa = 210, Pad = 90.
            // have pt1 = 90

            diff1 = submod(Pad.X, Pad.Z, n);
            sum1 = addmod(Pad.X, Pad.Z, n);
            udup(rho, work, sum1, diff1, &Pad);	// Pad = [60]Q

            // [150]Q + [60]Q([90]Q) = [210]Q
            uadd(rho, work, Pa, &Pad, &work->pt1, Pa);
            Pad.X = work->pt1.X;
            Pad.Z = work->pt1.Z;
        }

        Paprod = mulredcx(Pa->X, Pa->Z, n, rho);
    }

	//initialize accumulator
	acc = u64div(1, n);
	
    int first = 0;
	if (work->stg1_max == 70)
	{
        first = special_70;
		barray = b1_70;
		numb = numb1_70;
	}
	else if (work->stg1_max == 85)
	{
		barray = b1_85;
		numb = numb1_85;
	}
	else if (work->stg1_max == 125)
	{
		barray = b1_125;
		numb = numb1_125;
	}
	else if (work->stg1_max == 165)
	{
		barray = b1_165;
		numb = numb1_165;
	}
	else if (work->stg1_max == 205)
	{
		barray = b1_205;
		numb = numb1_205;
	}

	for (i = 0; i < numb; i++)
	{
        uint64_t x, z, t1, t2, t3;

		if (barray[i] == 0)
		{
            if (first)
            {
                // Currently we have Pa=120, Pd=60, Pad=30
                // need to get Pd=120, Pa=240, and Pad = Pa - 120, then
                // that condition will hold for the rest of stage 2.
                // this is a special case for B1=70.
                // to do that, we double Pa and set both
                // Pad and Pd to the old Pa
                x = Pa->X;
                z = Pa->Z;
                diff1 = submod(Pa->X, Pa->Z, n);
                sum1 = addmod(Pa->X, Pa->Z, n);
                udup(rho, work, sum1, diff1, Pa);	
                Pad.X = Pd->X = x;
                Pad.Z = Pd->Z = z;
                first = 0;
            }
            else
            {
                //giant step - use the addition formula for ECM
                x = Pa->X;
                z = Pa->Z;

                //Pa + Pd
                uadd(rho, work, Pa, Pd, &Pad, Pa);

                //Pad holds the previous Pa
                Pad.X = x;
                Pad.Z = z;

                //and Paprod
                Paprod = mulredcx(Pa->X, Pa->Z, n, rho);
            }
			i++;
		}

		//we accumulate XrZd - XdZr = (Xr - Xd) * (Zr + Zd) + XdZd - XrZr
		//in CP notation, Pa -> (Xr,Zr), Pb -> (Xd,Zd)

		b = barray[i];
		// accumulate the cross product  (zimmerman syntax).
		// page 342 in C&P
		t1 = submod(Pa->X, Pb[map[b]].X, n);
		t2 = addmod(Pa->Z, Pb[map[b]].Z, n);
		t3 = mulredcx(t1, t2, n, rho);
		t1 = addmod(t3, Pbprod[map[b]], n);
		t2 = submod(t1, Paprod, n);
		acc = mulredcx(acc, t2, n, rho);

	}

	work->stg2acc = acc;

	return;
}

void uecm_stage2_D120_inv(uecm_pt *P, uint64_t rho, uecm_work *work)
{
    int b;
    int i, j, k, m;
    uecm_pt Pap;
    uecm_pt *Pa = &Pap;
    uint64_t Pa_inv;
    uecm_pt *Pb = work->Pb;
    uecm_pt *Pd;
    uecm_pt Pd_norm;
    uint64_t acc = work->stg2acc;
    uint8_t *barray;
    uint32_t numb;
    int noerr;

    work->paired = 0;
    work->numprimes = 0;
    work->ptadds = 0;
    work->stg1Add = 0;
    work->stg1Doub = 0;

    //stage 2 init
    //Q = P = result of stage 1
    //compute [d]Q for 0 < d <= D
    Pd = &Pb[map[60]];

    // [1]Q
    Pb[1].Z = P->Z;
    Pb[1].X = P->X;
    work->Pbprod[1] = mulredcx(Pb[1].X, Pb[1].Z, work->n, rho);

    // [2]Q
    Pb[2].Z = P->Z;
    Pb[2].X = P->X;
    work->diff1 = submod(P->X, P->Z, work->n);
    work->sum1 = addmod(P->X, P->Z, work->n);
    udup(rho, work, work->sum1, work->diff1, &Pb[2]);
    work->Pbprod[2] = mulredcx(Pb[2].X, Pb[2].Z, work->n, rho);

    // Calculate all Pb: the following is specialized for D=60
    // [2]Q + [1]Q([1]Q) = [3]Q
    uadd(rho, work, &Pb[1], &Pb[2], &Pb[1], &Pb[3]);		// <-- temporary

    // 2*[3]Q = [6]Q
    work->diff1 = submod(Pb[3].X, Pb[3].Z, work->n);
    work->sum1 = addmod(Pb[3].X, Pb[3].Z, work->n);
    udup(rho, work, work->sum1, work->diff1, &work->pt3);	// pt3 = [6]Q

    // [3]Q + [2]Q([1]Q) = [5]Q
    uadd(rho, work, &Pb[3], &Pb[2], &Pb[1], &work->pt1);	// <-- pt1 = [5]Q
    Pb[3].X = work->pt1.X;
    Pb[3].Z = work->pt1.Z;

    // [6]Q + [5]Q([1]Q) = [11]Q
    uadd(rho, work, &work->pt3, &work->pt1, &Pb[1], &Pb[4]);	// <-- [11]Q

    i = 3;
    k = 4;
    j = 5;
    while ((j + 12) < (60))
    {
        // [j+6]Q + [6]Q([j]Q) = [j+12]Q
        uadd(rho, work, &work->pt3, &Pb[k], &Pb[i], &Pb[map[j + 12]]);
        i = k;
        k = map[j + 12];
        j += 6;
    }

    // [6]Q + [1]Q([5]Q) = [7]Q
    uadd(rho, work, &work->pt3, &Pb[1], &work->pt1, &Pb[3]);	// <-- [7]Q
    i = 1;
    k = 3;
    j = 1;
    while ((j + 12) < (60))
    {
        // [j+6]Q + [6]Q([j]Q) = [j+12]Q
        uadd(rho, work, &work->pt3, &Pb[k], &Pb[i], &Pb[map[j + 12]]);
        i = k;
        k = map[j + 12];
        j += 6;
    }

    // Pd = [2w]Q
    // [31]Q + [29]Q([2]Q) = [60]Q
    uadd(rho, work, &Pb[9], &Pb[10], &Pb[2], Pd);	// <-- [60]Q
    work->ptadds++;

    //initialize info needed for giant step
    // temporary - make [4]Q
    work->diff1 = submod(Pb[2].X, Pb[2].Z, work->n);
    work->sum1 = addmod(Pb[2].X, Pb[2].Z, work->n);
    udup(rho, work, work->sum1, work->diff1, &work->pt3);	// pt3 = [4]Q

    // Pd = [w]Q
    // [17]Q + [13]Q([4]Q) = [30]Q
    uadd(rho, work, &Pb[map[17]], &Pb[map[13]], &work->pt3, &work->Pad);	// <-- [30]Q

    // [60]Q + [30]Q([30]Q) = [90]Q
    uadd(rho, work, Pd, &work->Pad, &work->Pad, Pa);
    work->pt1.X = Pa->X;
    work->pt1.Z = Pa->Z;

    // [90]Q + [30]Q([60]Q) = [120]Q
    uadd(rho, work, Pa, &work->Pad, Pd, Pa);
    Pd->X = Pa->X;
    Pd->Z = Pa->Z;

    // [120]Q + [30]Q([90]Q) = [150]Q
    uadd(rho, work, Pa, &work->Pad, &work->pt1, Pa);

    // adjustment of Pa and Pad for larger B1.
    // Currently we have Pa=150, Pd=120, Pad=30
    if (work->stg1_max == 165)
    {
        // need Pa = 180, Pad = 60
        // [150]Q + [30]Q([120]Q) = [180]Q
        uadd(rho, work, Pa, &work->Pad, Pd, Pa);

        work->diff1 = submod(work->Pad.X, work->Pad.Z, work->n);
        work->sum1 = addmod(work->Pad.X, work->Pad.Z, work->n);
        udup(rho, work, work->sum1, work->diff1, &work->Pad);	// Pad = [60]Q
    }
    else if (work->stg1_max == 205)
    {
        // need Pa = 210, Pad = 90.
        // have pt1 = 90

        work->diff1 = submod(work->Pad.X, work->Pad.Z, work->n);
        work->sum1 = addmod(work->Pad.X, work->Pad.Z, work->n);
        udup(rho, work, work->sum1, work->diff1, &work->Pad);	// Pad = [60]Q

        // [150]Q + [60]Q([90]Q) = [210]Q
        uadd(rho, work, Pa, &work->Pad, &work->pt1, Pa);
        work->Pad.X = work->pt1.X;
        work->Pad.Z = work->pt1.Z;
    }
    
    //initialize accumulator
    acc = u64div(1, work->n);
    
    // invert all Pb's.  This also converts Pd = Pb[18]: save the non-inverted Pd.
    Pd_norm.X = Pd->X;
    Pd_norm.Z = Pd->Z;
    
    // Slow explicit inversion...
    if (0)  // this case works, but lowers averge curves/input slightly
    {
        for (i = 1; i < 19; i++)
        {
            uint64_t tmp;
            
            // take out of monty rep
            Pb[i].X = mulredcx(Pb[i].X, 1, work->n, rho);
            Pb[i].Z = mulredcx(Pb[i].Z, 1, work->n, rho);
            
            // invert Z
            //Pb[i].Z = modinv_64(Pb[i].Z, work->n);
            noerr = modinv_64_3(&Pb[i].Z, Pb[i].Z, work->n);
            if (noerr == 0)
            {
                work->stg2acc = Pb[i].Z;
                return;
            }
            
            // combine with X
            spMulMod(Pb[i].Z, Pb[i].X, work->n, &tmp);
            
            // back into monty
            Pb[i].X = u64div(tmp, work->n);
        }
    }
    else // this case works the same as explicit conversion
    {
        uint64_t B[19];
        // here, we have temporary space for B, A is put into the unused Pbprod, and C is Pb.Z.
        
        // faster batch inversion in three phases, as follows:
        // first, set A1 = z1 and Ai = zi * A(i-1) so that Ai = prod(j=1,i,zj).
        work->Pbprod[1] = Pb[1].Z;
        for (i = 2; i < 19; i++)
        {
            work->Pbprod[i] = mulredcx(Pb[i].Z, work->Pbprod[i - 1], work->n, rho);
        }
        
        // now we have to take An out of monty rep so we can invert it.
        B[18] = mulredcx(work->Pbprod[18], 1, work->n, rho);

        // then, invert An and store in Bn and set Bi = z(i+1) * B(i+1) for i < n.
        B[18] = modinv_64(B[18], work->n);
        
        // now put it back into Monty rep.
        B[18] = u64div(B[18], work->n);
        
        // and continue.
        for (i = 17; i >= 0; i--)
        {
            B[i] = mulredcx(Pb[i+1].Z, B[i+1], work->n, rho);
        }
        // Now we have Bi = prod(j=1,i,zj^-1).
        // finally, set C1 = B1 and Ci = A(i-1) * B(i) for i > 1.
        // Then Ci = zi^-1 for i > 1.
        Pb[1].Z = B[1];
        for (i = 2; i < 19; i++)
        {
            Pb[i].Z = mulredcx(B[i], work->Pbprod[i - 1], work->n, rho);
        }
        // each phase takes n-1 multiplications so we have 3n-3 total multiplications
        // and one inversion mod N.
        // but we still have to combine with the X coord.
        for (i = 1; i < 19; i++)
        {
            Pb[i].X = mulredcx(Pb[i].X, Pb[i].Z, work->n, rho);
        }
    }
    
    // prepare Pa_inv
    work->Paprod = mulredcx(Pa->Z, 1, work->n, rho);
    //work->Paprod = modinv_64(work->Paprod, work->n);
    noerr = modinv_64_3(&work->Paprod, work->Paprod, work->n);
    if (noerr == 0)
    {
        work->stg2acc = work->Paprod;
        return;
    }
    work->Paprod = u64div(work->Paprod, work->n);
    Pa_inv = mulredcx(Pa->X, work->Paprod, work->n, rho);     

    if (work->stg1_max == 70)
    {
        barray = b1_70;
        numb = numb1_70;
    }
    else if (work->stg1_max == 85)
    {
        barray = b1_85;
        numb = numb1_85;
    }
    else if (work->stg1_max == 125)
    {
        barray = b1_125;
        numb = numb1_125;
    }
    else if (work->stg1_max == 165)
    {
        barray = b1_165;
        numb = numb1_165;
    }
    else if (work->stg1_max == 205)
    {
        barray = b1_205;
        numb = numb1_205;
    }

    for (i = 0; i < numb; i++)
    {
        if (barray[i] == 0)
        {
            uint64_t tmp;
            
            //giant step - use the addition formula for ECM
            work->pt1.X = Pa->X;
            work->pt1.Z = Pa->Z;

            //Pa + Pd
            uadd(rho, work, Pa, &Pd_norm, &work->Pad, Pa);

            //Pad holds the previous Pa
            work->Pad.X = work->pt1.X;
            work->Pad.Z = work->pt1.Z;

            // prepare Pa_inv
            work->Paprod = mulredcx(Pa->Z, 1, work->n, rho);
            noerr = modinv_64_3(&tmp, work->Paprod, work->n);
            if (noerr == 0)
            {
                work->stg2acc = tmp;
                return;
            }

            work->Paprod = u64div(tmp, work->n);
            Pa_inv = mulredcx(Pa->X, work->Paprod, work->n, rho);          
    
            i++;
        }

        //we accumulate XrZd - XdZr = (Xr - Xd) * (Zr + Zd) + XdZd - XrZr
        //in CP notation, Pa -> (Xr,Zr), Pb -> (Xd,Zd)

        b = barray[i];
        // accumulate the cross product directly now that Zr and Zd == 1 mod n
        work->tt1 = submod(Pa_inv, Pb[map[b]].X, work->n);
        acc = mulredcx(acc, work->tt1, work->n, rho);
    }

    work->stg2acc = acc;

    return;
}

int ucheck_factor(uint64_t Z, uint64_t n, uint64_t * f)
{
	int status;

	*f = spGCD(Z, n);

	status = 0;
	if (*f > 1)
	{
		if (*f == n)
		{
			*f = 0;
			status = 0;
		}
		else
		{
			status = 1;
		}
	}

	return status;
}

double my_difftime(struct timeval * start, struct timeval * end)
{
	double secs;
	double usecs;

	if (start->tv_sec == end->tv_sec) {
		secs = 0;
		usecs = end->tv_usec - start->tv_usec;
	}
	else {
		usecs = 1000000 - start->tv_usec;
		secs = end->tv_sec - (start->tv_sec + 1);
		usecs += end->tv_usec;
		if (usecs >= 1000000) {
			usecs -= 1000000;
			secs += 1;
		}
	}

	return secs + usecs / 1000000.;
}

uint64_t gcd64(uint64_t x, uint64_t y)
{
    uint64_t a, b, c;
    a = x; b = y;
    while (b != 0)
    {
        c = a % b;
        a = b;
        b = c;
    }
    return a;
}

static int pairmap_sz47_w30_u2_x25 = 116;
static uint8_t pairmap_v47_w30_u2_x25[116] =
{ 1, 1, 1, 2, 2, 2, 3, 3, 3, 5, 4, 5, 5, 5, 7, 7, 6, 3, 0, 6, 5, 3, 6, 3, 6, 7, 3, 7, 8, 9, 8, 9, 0, 8, 7, 8, 8, 10, 10, 7, 10, 0, 10, 7, 10, 10, 10, 7, 11, 11, 13, 13, 12, 7, 9, 0, 11, 12, 11, 13, 13, 13, 12, 15, 11, 0, 14, 14, 14, 16, 15, 16, 13, 13, 0, 16, 16, 16, 13, 16, 16, 13, 16, 13, 18, 19, 15, 0, 18, 18, 19, 18, 15, 18, 19, 19, 20, 20, 21, 0, 20, 20, 21, 21, 21, 23, 19, 0, 22, 21, 21, 19, 21, 21, 23, 21 };
static uint8_t pairmap_u47_w30_u2_x25[116] =
{ 1, 7, 13, 7, 11, 19, 7, 17, 31, 1, 43, 17, 31, 49, 1, 17, 53, 19, 0, 1, 41, 7, 37, 13, 43, 17, 11, 49, 37, 13, 53, 29, 0, 11, 49, 31, 53, 11, 17, 7, 53, 0, 11, 1, 31, 47, 53, 29, 31, 53, 1, 13, 47, 11, 11, 0, 31, 11, 53, 1, 13, 17, 53, 19, 7, 0, 7, 23, 59, 13, 47, 49, 11, 1, 0, 7, 13, 17, 11, 41, 49, 29, 59, 7, 23, 23, 1, 0, 19, 23, 1, 43, 19, 49, 29, 43, 19, 53, 29, 0, 13, 19, 23, 47, 53, 29, 19, 0, 7, 49, 1, 13, 13, 17, 29, 19 };

static int pairmap_sz59_w30_u2_x25 = 136;
static uint8_t pairmap_v59_w30_u2_x25[136] =
{ 1, 2, 2, 2, 2, 2, 3, 3, 4, 5, 5, 5, 5, 5, 7, 7, 6, 3, 0, 6, 5, 6, 6, 7, 3, 7, 8, 9, 8, 9, 0, 8, 7, 8, 8, 10, 10, 7, 10, 0, 10, 7, 10, 10, 10, 7, 11, 11, 13, 13, 12, 7, 9, 0, 11, 12, 11, 13, 13, 13, 12, 15, 11, 0, 14, 14, 14, 16, 15, 16, 13, 13, 0, 16, 16, 16, 13, 16, 16, 13, 16, 13, 18, 19, 15, 0, 18, 18, 19, 18, 15, 18, 19, 19, 20, 20, 21, 0, 20, 20, 21, 21, 21, 23, 19, 0, 22, 21, 22, 22, 23, 24, 23, 19, 23, 25, 0, 24, 24, 24, 21, 26, 27, 27, 26, 26, 0, 27, 27, 25, 27, 25, 27, 27, 27, 27 };
static uint8_t pairmap_u59_w30_u2_x25[136] =
{ 1, 7, 11, 17, 19, 23, 17, 31, 23, 1, 13, 17, 31, 49, 1, 17, 53, 19, 0, 1, 41, 37, 43, 17, 11, 49, 37, 13, 53, 29, 0, 11, 49, 31, 53, 11, 17, 7, 53, 0, 11, 1, 31, 47, 53, 29, 31, 53, 1, 13, 47, 11, 11, 0, 31, 11, 53, 1, 13, 17, 53, 19, 7, 0, 7, 23, 59, 13, 47, 49, 11, 1, 0, 7, 13, 17, 11, 41, 49, 29, 59, 7, 23, 23, 1, 0, 19, 23, 1, 43, 19, 49, 29, 43, 19, 53, 29, 0, 13, 19, 23, 47, 53, 29, 19, 0, 7, 49, 29, 49, 29, 7, 43, 13, 47, 1, 0, 23, 49, 59, 17, 29, 7, 11, 43, 49, 0, 29, 19, 17, 17, 13, 13, 11, 7, 1 };

// U=2 means more pairs and fewer accumulates, but w=30 means more adds.
// the whole sequence is laid out so we can count the total multiplies.
// each 0 in the u-array is 4 adds and 4 muls = 28 muls
// 13 0's = 364 muls.  (157 - 13) * 2 = 288 muls (total 652).  
// so the giant steps are  more expensive than the accumulates.
static int pairmap_sz70_w30_u2_x25 = 157;
static uint8_t pairmap_v70_w30_u2_x25[157] =
{ 2, 2, 2, 2, 3, 3, 3, 5, 5, 5, 5, 5, 7, 7, 6, 3, 0, 6, 5, 3, 6, 6, 7, 3, 7, 8, 9, 8, 9, 0, 8, 7, 8, 8, 10, 10, 7, 10, 0, 10, 7, 10, 10, 10, 7, 11, 11, 13, 13, 12, 7, 9, 0, 11, 12, 11, 13, 13, 13, 12, 15, 11, 0, 14, 14, 14, 16, 15, 16, 13, 13, 0, 16, 16, 16, 13, 16, 16, 13, 16, 13, 18, 19, 15, 0, 18, 18, 19, 18, 15, 18, 19, 19, 20, 20, 21, 0, 20, 20, 21, 21, 21, 23, 19, 0, 22, 21, 22, 22, 23, 24, 23, 19, 23, 25, 0, 24, 24, 24, 21, 26, 27, 27, 26, 26, 0, 27, 28, 29, 28, 25, 25, 0, 27, 25, 28, 28, 25, 28, 29, 25, 30, 31, 30, 25, 27, 0, 27, 27, 31, 31, 29, 31, 31, 29, 27, 29 };
static uint8_t pairmap_u70_w30_u2_x25[157] =
{ 7, 11, 17, 19, 7, 17, 31, 1, 13, 17, 31, 49, 1, 17, 53, 19, 0, 1, 41, 7, 37, 43, 17, 11, 49, 37, 13, 53, 29, 0, 11, 49, 31, 53, 11, 17, 7, 53, 0, 11, 1, 31, 47, 53, 29, 31, 53, 1, 13, 47, 11, 11, 0, 31, 11, 53, 1, 13, 17, 53, 19, 7, 0, 7, 23, 59, 13, 47, 49, 11, 1, 0, 7, 13, 17, 11, 41, 49, 29, 59, 7, 23, 23, 1, 0, 19, 23, 1, 43, 19, 49, 29, 43, 19, 53, 29, 0, 13, 19, 23, 47, 53, 29, 19, 0, 7, 49, 29, 49, 29, 7, 43, 13, 47, 1, 0, 23, 49, 59, 17, 29, 7, 11, 43, 49, 0, 11, 37, 11, 49, 17, 13, 0, 37, 1, 31, 37, 17, 47, 37, 13, 43, 17, 49, 29, 29, 0, 1, 7, 7, 1, 29, 19, 17, 13, 11, 11 };

static int pairmap_sz70_w30_u2_x50 = 298;
static uint8_t pairmap_v70_w30_u2_x50[298] =
{2, 2, 2, 2, 3, 3, 3, 5, 5, 5, 5, 5, 7, 7, 6, 3, 0, 6, 5, 3, 6, 6, 7, 3, 7, 8, 9, 8, 9, 0, 8, 7, 8, 8, 10, 10, 7, 10, 0, 10, 7, 10, 10, 10, 7, 11, 11, 13, 13, 12, 7, 9, 0, 11, 12, 11, 13, 13, 13, 12, 15, 11, 0, 14, 14, 14, 16, 15, 16, 13, 13, 0, 16, 16, 16, 13, 16, 16, 13, 16, 13, 18, 19, 15, 0, 18, 18, 19, 18, 15, 18, 19, 19, 20, 20, 21, 0, 20, 20, 21, 21, 21, 23, 19, 0, 22, 21, 22, 22, 23, 24, 23, 19, 23, 25, 0, 24, 24, 24, 21, 26, 27, 27, 26, 26, 0, 27, 28, 29, 28, 25, 25, 0, 27, 25, 28, 28, 25, 28, 29, 25, 30, 31, 30, 25, 27, 0, 27, 27, 31, 30, 31, 27, 29, 33, 33, 29, 0, 32, 29, 32, 33, 34, 34, 34, 29, 31, 0, 34, 33, 35, 34, 36, 31, 35, 37, 37, 0, 35, 35, 37, 37, 38, 37, 35, 38, 39, 0, 38, 37, 40, 40, 40, 41, 0, 39, 40, 40, 37, 37, 42, 41, 43, 43, 39, 0, 41, 39, 42, 43, 43, 41, 44, 44, 39, 41, 0, 44, 44, 47, 47, 46, 43, 41, 0, 45, 46, 45, 45, 47, 48, 48, 48, 49, 45, 0, 48, 48, 45, 49, 51, 51, 45, 45, 0, 49, 51, 52, 47, 49, 52, 47, 49, 49, 47, 49, 0, 52, 52, 52, 52, 55, 49, 49, 0, 54, 54, 51, 56, 51, 56, 56, 57, 51, 51, 0, 55, 55, 58, 57, 58, 58, 55, 53, 0, 58, 58, 57, 55, 55, 60, 59, 61, 57, 61, 59, 61, 61};
static uint8_t pairmap_u70_w30_u2_x50[298] =
{7, 11, 17, 19, 7, 17, 31, 1, 13, 17, 31, 49, 1, 17, 53, 19, 0, 1, 41, 7, 37, 43, 17, 11, 49, 37, 13, 53, 29, 0, 11, 49, 31, 53, 11, 17, 7, 53, 0, 11, 1, 31, 47, 53, 29, 31, 53, 1, 13, 47, 11, 11, 0, 31, 11, 53, 1, 13, 17, 53, 19, 7, 0, 7, 23, 59, 13, 47, 49, 11, 1, 0, 7, 13, 17, 11, 41, 49, 29, 59, 7, 23, 23, 1, 0, 19, 23, 1, 43, 19, 49, 29, 43, 19, 53, 29, 0, 13, 19, 23, 47, 53, 29, 19, 0, 7, 49, 29, 49, 29, 7, 43, 13, 47, 1, 0, 23, 49, 59, 17, 29, 7, 11, 43, 49, 0, 11, 37, 11, 49, 17, 13, 0, 37, 1, 31, 37, 17, 47, 37, 13, 43, 17, 49, 29, 29, 0, 1, 7, 7, 43, 19, 11, 11, 11, 23, 29, 0, 43, 17, 47, 29, 11, 23, 43, 1, 1, 0, 1, 59, 7, 43, 7, 11, 49, 13, 29, 0, 41, 43, 11, 13, 1, 37, 1, 31, 19, 0, 47, 7, 37, 43, 47, 29, 0, 59, 37, 41, 23, 19, 13, 59, 11, 23, 13, 0, 59, 7, 37, 13, 43, 19, 49, 53, 29, 29, 0, 1, 29, 7, 19, 53, 11, 17, 0, 31, 13, 47, 49, 7, 1, 11, 19, 7, 23, 0, 1, 13, 23, 23, 23, 29, 13, 1, 0, 47, 23, 1, 1, 1, 31, 19, 19, 7, 17, 17, 0, 7, 11, 19, 31, 1, 13, 17, 0, 13, 41, 23, 7, 19, 41, 43, 19, 7, 29, 0, 31, 59, 1, 43, 17, 29, 11, 7, 0, 1, 17, 53, 23, 19, 17, 49, 19, 1, 11, 29, 23, 17};

// now each giant step is 2 adds and 2 muls, again we have 13 giant steps.
// 13 * (12 + 2) + (178 - 13) * 2 = 512 muls
static int pairmap_sz70_w60_u1_x25 = 178;
static uint8_t pairmap_v70_w60_u1_x25[178] =
{1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 1, 1, 3, 1, 1, 1, 0, 3, 3, 3, 3, 2, 4, 2, 4, 2, 4, 2, 2, 2, 0, 4, 4, 4, 4, 4, 3, 3, 3, 3, 0, 5, 5, 5, 5, 4, 6, 4, 6, 4, 4, 6, 4, 6, 6, 4, 4, 4, 4, 0, 6, 6, 6, 5, 7, 5, 5, 5, 0, 7, 7, 7, 7, 8, 8, 6, 6, 6, 6, 6, 6, 6, 6, 0, 8, 8, 8, 8, 9, 7, 9, 9, 9, 7, 7, 7, 7, 0, 9, 9, 9, 10, 10, 8, 8, 0, 10, 10, 10, 11, 9, 11, 9, 9, 9, 9, 9, 9, 9, 0, 11, 11, 11, 11, 11, 11, 12, 10, 10, 10, 0, 12, 12, 13, 13, 11, 13, 13, 11, 13, 11, 11, 11, 0, 13, 13, 14, 14, 12, 12, 12, 12, 12, 0, 14, 14, 14, 15, 13, 15, 13, 15, 13, 13, 13, 13, 13, 13, 0, 15, 14, 14, 14, 16, 14};
static uint8_t pairmap_u70_w60_u1_x25[178] =
{7, 11, 17, 19, 31, 37, 47, 1, 17, 31, 1, 11, 17, 23, 41, 41, 53, 49, 13, 0, 7, 17, 31, 37, 7, 7, 13, 13, 49, 49, 31, 37, 47, 0, 1, 19, 23, 37, 41, 11, 29, 47, 1, 0, 17, 31, 37, 53, 1, 1, 13, 13, 17, 31, 31, 47, 53, 59, 11, 19, 41, 49, 0, 13, 17, 41, 37, 37, 7, 17, 41, 0, 7, 29, 41, 47, 17, 43, 7, 23, 41, 49, 59, 29, 11, 1, 0, 19, 37, 41, 47, 7, 11, 31, 49, 53, 37, 47, 29, 1, 0, 1, 43, 49, 29, 49, 23, 37, 0, 23, 47, 53, 29, 31, 49, 11, 17, 23, 43, 47, 41, 19, 0, 23, 29, 37, 43, 47, 59, 41, 47, 59, 19, 0, 53, 59, 11, 13, 19, 31, 41, 43, 59, 47, 53, 29, 0, 11, 53, 11, 37, 19, 43, 47, 53, 17, 0, 1, 7, 37, 13, 17, 17, 29, 43, 7, 19, 41, 47, 49, 1, 0, 7, 19, 29, 41, 59, 11};

static int pairmap_sz85_w60_u1_x25 = 209;
static uint8_t pairmap_v85_w60_u1_x25[209] =
{1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 1, 3, 1, 1, 1, 0, 3, 3, 3, 3, 2, 4, 2, 4, 2, 4, 2, 2, 0, 4, 4, 4, 4, 4, 3, 3, 3, 3, 0, 5, 5, 5, 5, 4, 6, 4, 6, 4, 4, 6, 4, 6, 6, 4, 4, 4, 4, 0, 6, 6, 6, 5, 7, 5, 5, 5, 0, 7, 7, 7, 7, 8, 8, 6, 6, 6, 6, 6, 6, 6, 6, 0, 8, 8, 8, 8, 9, 7, 9, 9, 9, 7, 7, 7, 7, 0, 9, 9, 9, 10, 10, 8, 8, 0, 10, 10, 10, 11, 9, 11, 9, 9, 9, 9, 9, 9, 9, 0, 11, 11, 11, 11, 11, 11, 12, 10, 10, 10, 0, 12, 12, 13, 13, 11, 13, 13, 11, 13, 11, 11, 11, 0, 13, 13, 14, 14, 12, 12, 12, 12, 12, 0, 14, 14, 14, 15, 13, 15, 13, 15, 13, 13, 13, 13, 13, 13, 0, 15, 15, 14, 16, 16, 16, 14, 14, 0, 16, 15, 17, 17, 15, 17, 15, 15, 0, 17, 17, 18, 18, 16, 18, 16, 18, 16, 18, 16, 16, 16, 16, 0, 18, 17, 17, 17, 19, 17, 17};
static uint8_t pairmap_u85_w60_u1_x25[209] =
{7, 11, 17, 19, 31, 1, 13, 17, 31, 1, 11, 17, 23, 41, 37, 53, 13, 0, 7, 17, 31, 37, 7, 7, 13, 13, 49, 49, 31, 37, 0, 1, 19, 23, 37, 41, 11, 29, 47, 1, 0, 17, 31, 37, 53, 1, 1, 13, 13, 17, 31, 31, 47, 53, 59, 11, 19, 41, 49, 0, 13, 17, 41, 37, 37, 7, 17, 41, 0, 7, 29, 41, 47, 17, 43, 7, 23, 41, 49, 59, 29, 11, 1, 0, 19, 37, 41, 47, 7, 11, 31, 49, 53, 37, 47, 29, 1, 0, 1, 43, 49, 29, 49, 23, 37, 0, 23, 47, 53, 29, 31, 49, 11, 17, 23, 43, 47, 41, 19, 0, 23, 29, 37, 43, 47, 59, 41, 47, 59, 19, 0, 53, 59, 11, 13, 19, 31, 41, 43, 59, 47, 53, 29, 0, 11, 53, 11, 37, 19, 43, 47, 53, 17, 0, 1, 7, 37, 13, 17, 17, 29, 43, 7, 19, 41, 47, 49, 1, 0, 7, 19, 11, 11, 23, 47, 19, 29, 0, 29, 13, 13, 53, 59, 59, 1, 17, 0, 31, 49, 13, 23, 41, 41, 43, 43, 47, 47, 49, 31, 19, 7, 0, 11, 29, 59, 53, 47, 37, 1};

static int pairmap_sz125_w60_u1_x25 = 300;
static uint8_t pairmap_v125_w60_u1_x25[300] =
{2, 2, 2, 2, 2, 2, 2, 3, 3, 1, 1, 1, 1, 0, 3, 3, 3, 3, 3, 4, 2, 4, 2, 4, 2, 2, 2, 0, 4, 4, 4, 4, 4, 3, 3, 3, 0, 5, 5, 5, 5, 4, 6, 4, 6, 4, 4, 6, 4, 6, 6, 4, 4, 4, 4, 0, 6, 6, 6, 5, 7, 5, 5, 5, 0, 7, 7, 7, 7, 8, 8, 6, 6, 6, 6, 6, 6, 6, 6, 0, 8, 8, 8, 8, 9, 7, 9, 9, 9, 7, 7, 7, 7, 0, 9, 9, 9, 10, 10, 8, 8, 0, 10, 10, 10, 11, 9, 11, 9, 9, 9, 9, 9, 9, 9, 0, 11, 11, 11, 11, 11, 11, 12, 10, 10, 10, 0, 12, 12, 13, 13, 11, 13, 13, 11, 13, 11, 11, 11, 0, 13, 13, 14, 14, 12, 12, 12, 12, 12, 0, 14, 14, 14, 15, 13, 15, 13, 15, 13, 13, 13, 13, 13, 13, 0, 15, 15, 14, 16, 16, 16, 14, 14, 0, 16, 15, 17, 17, 15, 17, 15, 15, 0, 17, 17, 18, 18, 16, 18, 16, 18, 16, 18, 16, 16, 16, 16, 0, 18, 18, 17, 19, 19, 17, 17, 17, 0, 19, 18, 20, 20, 18, 20, 18, 20, 18, 18, 18, 0, 20, 20, 20, 19, 21, 21, 19, 19, 21, 19, 0, 21, 21, 20, 20, 20, 20, 20, 20, 20, 0, 22, 22, 22, 23, 21, 23, 23, 21, 21, 21, 21, 21, 21, 0, 23, 23, 23, 24, 24, 24, 22, 22, 22, 22, 22, 0, 24, 24, 25, 25, 23, 25, 23, 23, 23, 23, 23, 0, 25, 24, 26, 24, 24, 24, 24, 0, 26, 26, 26, 27, 25, 25, 25, 27, 27, 25, 25, 25, 27, 25};
static uint8_t pairmap_u125_w60_u1_x25[300] =
{1, 13, 17, 31, 43, 49, 53, 1, 41, 19, 31, 37, 53, 0, 7, 17, 31, 37, 49, 7, 13, 13, 49, 49, 17, 31, 37, 0, 1, 19, 23, 37, 41, 29, 47, 1, 0, 17, 31, 37, 53, 1, 1, 13, 13, 17, 31, 31, 47, 53, 59, 11, 19, 41, 49, 0, 13, 17, 41, 37, 37, 7, 17, 41, 0, 7, 29, 41, 47, 17, 43, 7, 23, 41, 49, 59, 29, 11, 1, 0, 19, 37, 41, 47, 7, 11, 31, 49, 53, 37, 47, 29, 1, 0, 1, 43, 49, 29, 49, 23, 37, 0, 23, 47, 53, 29, 31, 49, 11, 17, 23, 43, 47, 41, 19, 0, 23, 29, 37, 43, 47, 59, 41, 47, 59, 19, 0, 53, 59, 11, 13, 19, 31, 41, 43, 59, 47, 53, 29, 0, 11, 53, 11, 37, 19, 43, 47, 53, 17, 0, 1, 7, 37, 13, 17, 17, 29, 43, 7, 19, 41, 47, 49, 1, 0, 7, 19, 11, 11, 23, 47, 19, 29, 0, 29, 13, 13, 53, 59, 59, 1, 17, 0, 31, 49, 13, 23, 41, 41, 43, 43, 47, 47, 49, 31, 19, 7, 0, 11, 31, 1, 19, 47, 53, 59, 37, 0, 17, 7, 7, 13, 17, 29, 31, 59, 1, 53, 23, 0, 7, 43, 59, 11, 11, 23, 37, 41, 59, 31, 0, 13, 43, 19, 29, 37, 41, 49, 43, 7, 0, 29, 37, 41, 7, 43, 47, 49, 53, 1, 11, 29, 37, 59, 0, 7, 29, 41, 7, 29, 41, 17, 23, 37, 43, 59, 0, 17, 23, 23, 29, 47, 47, 17, 31, 59, 49, 11, 0, 23, 1, 1, 19, 7, 17, 59, 0, 19, 23, 49, 1, 19, 23, 49, 59, 53, 47, 43, 31, 31, 29};

static int pairmap_sz165_w60_u1_x25 = 387;
static uint8_t pairmap_v165_w60_u1_x25[387] =
{2, 2, 3, 3, 3, 3, 3, 3, 1, 0, 3, 3, 2, 4, 2, 4, 4, 2, 4, 2, 2, 0, 4, 4, 4, 4, 4, 3, 3, 3, 3, 0, 5, 5, 5, 5, 4, 6, 4, 6, 4, 4, 6, 4, 6, 6, 4, 4, 4, 4, 0, 6, 6, 6, 5, 7, 5, 5, 5, 0, 7, 7, 7, 7, 8, 8, 6, 6, 6, 6, 6, 6, 6, 6, 0, 8, 8, 8, 8, 9, 7, 9, 9, 9, 7, 7, 7, 7, 0, 9, 9, 9, 10, 10, 8, 8, 0, 10, 10, 10, 11, 9, 11, 9, 9, 9, 9, 9, 9, 9, 0, 11, 11, 11, 11, 11, 11, 12, 10, 10, 10, 0, 12, 12, 13, 13, 11, 13, 13, 11, 13, 11, 11, 11, 0, 13, 13, 14, 14, 12, 12, 12, 12, 12, 0, 14, 14, 14, 15, 13, 15, 13, 15, 13, 13, 13, 13, 13, 13, 0, 15, 15, 14, 16, 16, 16, 14, 14, 0, 16, 15, 17, 17, 15, 17, 15, 15, 0, 17, 17, 18, 18, 16, 18, 16, 18, 16, 18, 16, 16, 16, 16, 0, 18, 18, 17, 19, 19, 17, 17, 17, 0, 19, 18, 20, 20, 18, 20, 18, 20, 18, 18, 18, 0, 20, 20, 20, 19, 21, 21, 19, 19, 21, 19, 0, 21, 21, 20, 20, 20, 20, 20, 20, 20, 0, 22, 22, 22, 23, 21, 23, 23, 21, 21, 21, 21, 21, 21, 0, 23, 23, 23, 24, 24, 24, 22, 22, 22, 22, 22, 0, 24, 24, 25, 25, 23, 25, 23, 23, 23, 23, 23, 0, 25, 24, 26, 24, 24, 24, 24, 0, 26, 26, 26, 27, 25, 25, 25, 25, 25, 25, 25, 0, 27, 28, 28, 26, 28, 26, 28, 26, 26, 26, 26, 0, 28, 28, 29, 29, 29, 29, 27, 29, 27, 27, 0, 29, 29, 30, 30, 28, 30, 28, 28, 0, 30, 30, 31, 29, 31, 29, 31, 29, 29, 29, 0, 31, 30, 32, 30, 32, 32, 30, 30, 30, 30, 30, 0, 32, 31, 33, 31, 31, 31, 31, 31, 31, 0, 33, 33, 33, 34, 32, 34, 32, 34, 32, 32, 32, 32, 32, 0, 34, 34, 33, 35, 35, 35, 35, 33, 33, 35, 33, 35, 35, 33, 35};
static uint8_t pairmap_u165_w60_u1_x25[387] = 
{1, 13, 1, 11, 17, 29, 41, 43, 53, 0, 7, 37, 7, 7, 13, 13, 29, 49, 49, 31, 37, 0, 1, 19, 23, 37, 41, 11, 43, 47, 1, 0, 17, 31, 37, 53, 1, 1, 13, 13, 17, 31, 31, 47, 53, 59, 11, 19, 41, 49, 0, 13, 17, 41, 37, 37, 7, 17, 41, 0, 7, 29, 41, 47, 17, 43, 7, 23, 41, 49, 59, 29, 11, 1, 0, 19, 37, 41, 47, 7, 11, 31, 49, 53, 37, 47, 29, 1, 0, 1, 43, 49, 29, 49, 23, 37, 0, 23, 47, 53, 29, 31, 49, 11, 17, 23, 43, 47, 41, 19, 0, 23, 29, 37, 43, 47, 59, 41, 47, 59, 19, 0, 53, 59, 11, 13, 19, 31, 41, 43, 59, 47, 53, 29, 0, 11, 53, 11, 37, 19, 43, 47, 53, 17, 0, 1, 7, 37, 13, 17, 17, 29, 43, 7, 19, 41, 47, 49, 1, 0, 7, 19, 11, 11, 23, 47, 19, 29, 0, 29, 13, 13, 53, 59, 59, 1, 17, 0, 31, 49, 13, 23, 41, 41, 43, 43, 47, 47, 49, 31, 19, 7, 0, 11, 31, 1, 19, 47, 53, 59, 37, 0, 17, 7, 7, 13, 17, 29, 31, 59, 1, 53, 23, 0, 7, 43, 59, 11, 11, 23, 37, 41, 59, 31, 0, 13, 43, 19, 29, 37, 41, 49, 43, 7, 0, 29, 37, 41, 7, 43, 47, 49, 53, 1, 11, 29, 37, 59, 0, 7, 29, 41, 7, 29, 41, 17, 23, 37, 43, 59, 0, 17, 23, 23, 29, 47, 47, 17, 31, 59, 49, 11, 0, 23, 1, 1, 19, 7, 17, 59, 0, 19, 23, 49, 1, 43, 47, 19, 23, 49, 31, 29, 0, 11, 11, 19, 31, 31, 59, 59, 17, 43, 47, 53, 0, 43, 47, 1, 29, 31, 47, 53, 53, 37, 23, 0, 47, 49, 19, 31, 37, 47, 59, 41, 0, 1, 7, 7, 17, 17, 23, 43, 11, 37, 49, 0, 37, 19, 19, 41, 47, 49, 13, 31, 37, 59, 29, 0, 53, 11, 37, 13, 41, 59, 43, 29, 1, 0, 11, 19, 47, 29, 43, 43, 47, 53, 7, 11, 23, 19, 17, 0, 1, 53, 31, 31, 11, 13, 19, 41, 47, 53, 37, 29, 23, 17, 1};

static int pairmap_sz205_w60_u1_x25 = 471;
static uint8_t pairmap_v205_w60_u1_x25[471] =
{2, 2, 2, 2, 3, 3, 3, 2, 4, 2, 4, 4, 4, 2, 2, 2, 0, 4, 4, 4, 4, 4, 3, 3, 3, 0, 5, 5, 5, 5, 4, 6, 4, 6, 4, 4, 6, 4, 6, 6, 4, 4, 4, 4, 0, 6, 6, 6, 5, 7, 5, 5, 5, 0, 7, 7, 7, 7, 8, 8, 6, 6, 6, 6, 6, 6, 6, 6, 0, 8, 8, 8, 8, 9, 7, 9, 9, 9, 7, 7, 7, 7, 0, 9, 9, 9, 10, 10, 8, 8, 0, 10, 10, 10, 11, 9, 11, 9, 9, 9, 9, 9, 9, 9, 0, 11, 11, 11, 11, 11, 11, 12, 10, 10, 10, 0, 12, 12, 13, 13, 11, 13, 13, 11, 13, 11, 11, 11, 0, 13, 13, 14, 14, 12, 12, 12, 12, 12, 0, 14, 14, 14, 15, 13, 15, 13, 15, 13, 13, 13, 13, 13, 13, 0, 15, 15, 14, 16, 16, 16, 14, 14, 0, 16, 15, 17, 17, 15, 17, 15, 15, 0, 17, 17, 18, 18, 16, 18, 16, 18, 16, 18, 16, 16, 16, 16, 0, 18, 18, 17, 19, 19, 17, 17, 17, 0, 19, 18, 20, 20, 18, 20, 18, 20, 18, 18, 18, 0, 20, 20, 20, 19, 21, 21, 19, 19, 21, 19, 0, 21, 21, 20, 20, 20, 20, 20, 20, 20, 0, 22, 22, 22, 23, 21, 23, 23, 21, 21, 21, 21, 21, 21, 0, 23, 23, 23, 24, 24, 24, 22, 22, 22, 22, 22, 0, 24, 24, 25, 25, 23, 25, 23, 23, 23, 23, 23, 0, 25, 24, 26, 24, 24, 24, 24, 0, 26, 26, 26, 27, 25, 25, 25, 25, 25, 25, 25, 0, 27, 28, 28, 26, 28, 26, 28, 26, 26, 26, 26, 0, 28, 28, 29, 29, 29, 29, 27, 29, 27, 27, 0, 29, 29, 30, 30, 28, 30, 28, 28, 0, 30, 30, 31, 29, 31, 29, 31, 29, 29, 29, 0, 31, 30, 32, 30, 32, 32, 30, 30, 30, 30, 30, 0, 32, 31, 33, 31, 31, 31, 31, 31, 31, 0, 33, 33, 33, 34, 32, 34, 32, 34, 32, 32, 32, 32, 32, 0, 34, 34, 33, 35, 35, 33, 33, 33, 33, 0, 35, 34, 34, 36, 36, 34, 34, 34, 34, 34, 34, 0, 36, 36, 37, 35, 35, 35, 35, 35, 35, 35, 0, 37, 37, 37, 36, 36, 36, 36, 36, 36, 0, 38, 38, 38, 38, 37, 39, 37, 37, 37, 37, 37, 0, 39, 39, 39, 39, 39, 40, 38, 38, 0, 40, 40, 41, 41, 39, 41, 39, 39, 39, 39, 39, 39, 0, 41, 42, 42, 42, 40, 40, 0, 42, 42, 42, 42, 43, 43, 41, 41, 41, 41, 41, 0, 43, 43, 42, 42, 44, 44, 42, 42, 42};
static uint8_t pairmap_u205_w60_u1_x25[471] = 
{1, 11, 17, 29, 7, 17, 37, 7, 7, 13, 13, 29, 49, 31, 37, 41, 0, 1, 19, 23, 37, 41, 11, 47, 1, 0, 17, 31, 37, 53, 1, 1, 13, 13, 17, 31, 31, 47, 53, 59, 11, 19, 41, 49, 0, 13, 17, 41, 37, 37, 7, 17, 41, 0, 7, 29, 41, 47, 17, 43, 7, 23, 41, 49, 59, 29, 11, 1, 0, 19, 37, 41, 47, 7, 11, 31, 49, 53, 37, 47, 29, 1, 0, 1, 43, 49, 29, 49, 23, 37, 0, 23, 47, 53, 29, 31, 49, 11, 17, 23, 43, 47, 41, 19, 0, 23, 29, 37, 43, 47, 59, 41, 47, 59, 19, 0, 53, 59, 11, 13, 19, 31, 41, 43, 59, 47, 53, 29, 0, 11, 53, 11, 37, 19, 43, 47, 53, 17, 0, 1, 7, 37, 13, 17, 17, 29, 43, 7, 19, 41, 47, 49, 1, 0, 7, 19, 11, 11, 23, 47, 19, 29, 0, 29, 13, 13, 53, 59, 59, 1, 17, 0, 31, 49, 13, 23, 41, 41, 43, 43, 47, 47, 49, 31, 19, 7, 0, 11, 31, 1, 19, 47, 53, 59, 37, 0, 17, 7, 7, 13, 17, 29, 31, 59, 1, 53, 23, 0, 7, 43, 59, 11, 11, 23, 37, 41, 59, 31, 0, 13, 43, 19, 29, 37, 41, 49, 43, 7, 0, 29, 37, 41, 7, 43, 47, 49, 53, 1, 11, 29, 37, 59, 0, 7, 29, 41, 7, 29, 41, 17, 23, 37, 43, 59, 0, 17, 23, 23, 29, 47, 47, 17, 31, 59, 49, 11, 0, 23, 1, 1, 19, 7, 17, 59, 0, 19, 23, 49, 1, 43, 47, 19, 23, 49, 31, 29, 0, 11, 11, 19, 31, 31, 59, 59, 17, 43, 47, 53, 0, 43, 47, 1, 29, 31, 47, 53, 53, 37, 23, 0, 47, 49, 19, 31, 37, 47, 59, 41, 0, 1, 7, 7, 17, 17, 23, 43, 11, 37, 49, 0, 37, 19, 19, 41, 47, 49, 13, 31, 37, 59, 29, 0, 53, 11, 37, 13, 41, 59, 43, 29, 1, 0, 11, 19, 47, 29, 43, 43, 47, 53, 7, 11, 23, 19, 17, 0, 1, 53, 31, 31, 53, 41, 47, 37, 17, 0, 13, 1, 29, 41, 43, 11, 13, 19, 49, 59, 23, 0, 1, 29, 37, 1, 11, 17, 19, 29, 53, 23, 0, 17, 41, 43, 23, 7, 29, 53, 49, 47, 0, 7, 17, 19, 49, 31, 37, 1, 7, 17, 23, 49, 0, 17, 29, 37, 53, 59, 41, 47, 13, 0, 11, 49, 1, 13, 17, 17, 23, 43, 53, 59, 37, 29, 0, 29, 11, 17, 49, 41, 7, 0, 7, 13, 23, 29, 19, 41, 59, 13, 23, 43, 1, 0, 1, 13, 11, 37, 53, 41, 29, 17, 1};

void uecm_stage2_w30u2(uecm_pt* P, uint64_t rho, uecm_work* work)
{
    int a, b;
    int i, j, k, m;
    uint32_t w = 30;
    uint32_t U = 2;
    uint32_t L = 4;
    uecm_pt Pa[8];
    uecm_pt* Pb = work->Pb;
    uecm_pt* Pd = &work->Pd;
    uint64_t Paprod[8];
    uint64_t acc = work->stg2acc;
    uint64_t amin = (work->stg1_max + w) / (2 * w);
    uint8_t* pairmap_u;
    uint8_t* pairmap_v;
    uint32_t numb;
    uint32_t A;

    work->paired = 0;
    work->numprimes = 0;
    work->ptadds = 0;
    work->stg1Add = 0;
    work->stg1Doub = 0;

    //stage 2 init
    //Q = P = result of stage 1
    //compute [d]Q for 0 < d <= D

    // compute Pb and Pbprod. 
    // This is the same for w=30/U=2 or w=60/U=1
    {
        // [1]Q
        Pb[1].Z = P->Z;
        Pb[1].X = P->X;
        work->Pbprod[1] = mulredcx(Pb[1].X, Pb[1].Z, work->n, rho);

        // [2]Q
        Pb[2].Z = P->Z;
        Pb[2].X = P->X;
        work->diff1 = submod(P->X, P->Z, work->n);
        work->sum1 = addmod(P->X, P->Z, work->n);
        udup(rho, work, work->sum1, work->diff1, &Pb[2]);
        work->Pbprod[2] = mulredcx(Pb[2].X, Pb[2].Z, work->n, rho);

        // Calculate all Pb: the following is specialized for D=60
        // [2]Q + [1]Q([1]Q) = [3]Q
        uadd(rho, work, &Pb[1], &Pb[2], &Pb[1], &Pb[3]);		// <-- temporary

        // 2*[3]Q = [6]Q
        work->diff1 = submod(Pb[3].X, Pb[3].Z, work->n);
        work->sum1 = addmod(Pb[3].X, Pb[3].Z, work->n);
        udup(rho, work, work->sum1, work->diff1, &work->pt3);	// pt3 = [6]Q

        // [3]Q + [2]Q([1]Q) = [5]Q
        uadd(rho, work, &Pb[3], &Pb[2], &Pb[1], &work->pt1);	// <-- pt1 = [5]Q
        Pb[3].X = work->pt1.X;
        Pb[3].Z = work->pt1.Z;

        // [6]Q + [5]Q([1]Q) = [11]Q
        uadd(rho, work, &work->pt3, &work->pt1, &Pb[1], &Pb[4]);	// <-- [11]Q

        i = 3;
        k = 4;
        j = 5;
        while ((j + 12) < (60))
        {
            // [j+6]Q + [6]Q([j]Q) = [j+12]Q
            uadd(rho, work, &work->pt3, &Pb[k], &Pb[i], &Pb[map[j + 12]]);
            i = k;
            k = map[j + 12];
            j += 6;
        }

        // [6]Q + [1]Q([5]Q) = [7]Q
        uadd(rho, work, &work->pt3, &Pb[1], &work->pt1, &Pb[3]);	// <-- [7]Q
        i = 1;
        k = 3;
        j = 1;
        while ((j + 12) < (60))
        {
            // [j+6]Q + [6]Q([j]Q) = [j+12]Q
            uadd(rho, work, &work->pt3, &Pb[k], &Pb[i], &Pb[map[j + 12]]);
            i = k;
            k = map[j + 12];
            j += 6;
        }

        // make all of the Pbprod's
        for (i = 3; i < 19; i++)
        {
            work->Pbprod[i] = mulredcx(Pb[i].X, Pb[i].Z, work->n, rho);
        }
    }

    // [31]Q + [29]Q([2]Q) = [60]Q
    A = 60;
    uadd(rho, work, &Pb[9], &Pb[10], &Pb[2], &Pa[0]);	// <-- [60]Q

    work->ptadds++;
    //initialize info needed for giant step
    // temporary - make [4]Q
    work->diff1 = submod(Pb[2].X, Pb[2].Z, work->n);
    work->sum1 = addmod(Pb[2].X, Pb[2].Z, work->n);
    udup(rho, work, work->sum1, work->diff1, &work->pt3);	// pt3 = [4]Q

    // Pd = Pad = [w]Q
    // [17]Q + [13]Q([4]Q) = [30]Q
    uadd(rho, work, &Pb[map[17]], &Pb[map[13]], &work->pt3, &work->Pad);	// <-- [30]Q
    Pd->X = work->Pad.X;
    Pd->Z = work->Pad.Z;

    while (A < 2 * amin * w)
    {
        work->pt1.X = Pa[0].X;
        work->pt1.Z = Pa[0].Z;
        uadd(rho, work, &work->pt1, Pd, &work->Pad, &Pa[0]);	// Pa = Pd + Pa
        work->Pad.X = work->pt1.X;
        work->Pad.Z = work->pt1.Z;

        work->pt1.X = Pa[0].X;
        work->pt1.Z = Pa[0].Z;
        uadd(rho, work, &work->pt1, Pd, &work->Pad, &Pa[0]);	// Pa = Pd + Pa
        work->Pad.X = work->pt1.X;
        work->Pad.Z = work->pt1.Z;

        A += 2 * w;
    }

    // now fill in the rest of the A array
    Paprod[0] = mulredcx(Pa[0].X, Pa[0].Z, work->n, rho);
    for (i = 1; i < (2 * L); i++)
    {
        work->pt1.X = Pa[i-1].X;
        work->pt1.Z = Pa[i-1].Z;
        uadd(rho, work, &Pa[i-1], Pd, &work->Pad, &Pa[i]);	// Pa = Pd + Pa
        work->Pad.X = work->pt1.X;
        work->Pad.Z = work->pt1.Z;
        Paprod[i] = mulredcx(Pa[i].X, Pa[i].Z, work->n, rho);
    }

    //initialize accumulator and Paprod
    acc = u64div(1, work->n);

    if (work->stg1_max == 47)
    {
        pairmap_u = pairmap_u47_w30_u2_x25;
        pairmap_v = pairmap_v47_w30_u2_x25;
        numb = pairmap_sz47_w30_u2_x25;
    }
    else if (work->stg1_max == 59)
    {
        pairmap_u = pairmap_u59_w30_u2_x25;
        pairmap_v = pairmap_v59_w30_u2_x25;
        numb = pairmap_sz59_w30_u2_x25;
    }
    else
    {
        pairmap_u = pairmap_u70_w30_u2_x25;
        pairmap_v = pairmap_v70_w30_u2_x25;
        numb = pairmap_sz70_w30_u2_x25;
    }

    for (i = 0; i < numb; i++)
    {
        if (pairmap_u[i] == 0)
        {
            // get L more A's
            Pa[0].X = Pa[4].X;
            Pa[1].X = Pa[5].X;
            Pa[2].X = Pa[6].X;
            Pa[3].X = Pa[7].X;
            Pa[0].Z = Pa[4].Z;
            Pa[1].Z = Pa[5].Z;
            Pa[2].Z = Pa[6].Z;
            Pa[3].Z = Pa[7].Z;
            Paprod[0] = Paprod[4];
            Paprod[1] = Paprod[5];
            Paprod[2] = Paprod[6];
            Paprod[3] = Paprod[7];

            uadd(rho, work, &Pa[3], Pd, &Pa[2], &Pa[4]);
            uadd(rho, work, &Pa[4], Pd, &Pa[3], &Pa[5]);
            uadd(rho, work, &Pa[5], Pd, &Pa[4], &Pa[6]);
            uadd(rho, work, &Pa[6], Pd, &Pa[5], &Pa[7]);

            Paprod[4] = mulredcx(Pa[4].X, Pa[4].Z, work->n, rho);
            Paprod[5] = mulredcx(Pa[5].X, Pa[5].Z, work->n, rho);
            Paprod[6] = mulredcx(Pa[6].X, Pa[6].Z, work->n, rho);
            Paprod[7] = mulredcx(Pa[7].X, Pa[7].Z, work->n, rho);

            amin += U;
            i++;
        }

        //we accumulate XrZd - XdZr = (Xr - Xd) * (Zr + Zd) + XdZd - XrZr
        //in CP notation, Pa -> (Xr,Zr), Pb -> (Xd,Zd)

        b = pairmap_u[i];
        a = pairmap_v[i] - amin;
        // accumulate the cross product  (zimmerman syntax).
        // page 342 in C&P
        work->tt1 = submod(Pa[a].X, Pb[map[b]].X, work->n);
        work->tt2 = addmod(Pa[a].Z, Pb[map[b]].Z, work->n);
        work->tt3 = mulredcx(work->tt1, work->tt2, work->n, rho);
        work->tt1 = addmod(work->tt3, work->Pbprod[map[b]], work->n);
        work->tt2 = submod(work->tt1, Paprod[a], work->n);
        acc = mulredcx(acc, work->tt2, work->n, rho);

    }

    work->stg2acc = acc;

    return;
}

void uecm_stage2_w60u1(uecm_pt* P, uint64_t rho, uecm_work* work)
{
    int a, b;
    int i, j, k, m;
    uint32_t w = 60;
    uint32_t U = 1;
    uint32_t L = 2;
    uecm_pt Pa[4];
    uecm_pt* Pb = work->Pb;
    uecm_pt* Pd = &work->Pd;
    uint64_t Paprod[4];
    uint64_t acc = work->stg2acc;
    uint64_t amin = (work->stg1_max + w) / (2 * w);
    uint8_t* pairmap_u;
    uint8_t* pairmap_v;
    uint32_t numb;
    uint32_t A;

    work->paired = 0;
    work->numprimes = 0;
    work->ptadds = 0;
    work->stg1Add = 0;
    work->stg1Doub = 0;

    //stage 2 init
    //Q = P = result of stage 1
    //compute [d]Q for 0 < d <= D

    // compute Pb and Pbprod. 
    // This is the same for w=30/U=2 or w=60/U=1
    {
        // [1]Q
        Pb[1].Z = P->Z;
        Pb[1].X = P->X;
        work->Pbprod[1] = mulredcx(Pb[1].X, Pb[1].Z, work->n, rho);

        // [2]Q
        Pb[2].Z = P->Z;
        Pb[2].X = P->X;
        work->diff1 = submod(P->X, P->Z, work->n);
        work->sum1 = addmod(P->X, P->Z, work->n);
        udup(rho, work, work->sum1, work->diff1, &Pb[2]);
        work->Pbprod[2] = mulredcx(Pb[2].X, Pb[2].Z, work->n, rho);

        // Calculate all Pb: the following is specialized for D=60
        // [2]Q + [1]Q([1]Q) = [3]Q
        uadd(rho, work, &Pb[1], &Pb[2], &Pb[1], &Pb[3]);		// <-- temporary

        // 2*[3]Q = [6]Q
        work->diff1 = submod(Pb[3].X, Pb[3].Z, work->n);
        work->sum1 = addmod(Pb[3].X, Pb[3].Z, work->n);
        udup(rho, work, work->sum1, work->diff1, &work->pt3);	// pt3 = [6]Q

        // [3]Q + [2]Q([1]Q) = [5]Q
        uadd(rho, work, &Pb[3], &Pb[2], &Pb[1], &work->pt1);	// <-- pt1 = [5]Q
        Pb[3].X = work->pt1.X;
        Pb[3].Z = work->pt1.Z;

        // [6]Q + [5]Q([1]Q) = [11]Q
        uadd(rho, work, &work->pt3, &work->pt1, &Pb[1], &Pb[4]);	// <-- [11]Q

        i = 3;
        k = 4;
        j = 5;
        while ((j + 12) < (60))
        {
            // [j+6]Q + [6]Q([j]Q) = [j+12]Q
            uadd(rho, work, &work->pt3, &Pb[k], &Pb[i], &Pb[map[j + 12]]);
            i = k;
            k = map[j + 12];
            j += 6;
        }

        // [6]Q + [1]Q([5]Q) = [7]Q
        uadd(rho, work, &work->pt3, &Pb[1], &work->pt1, &Pb[3]);	// <-- [7]Q
        i = 1;
        k = 3;
        j = 1;
        while ((j + 12) < (60))
        {
            // [j+6]Q + [6]Q([j]Q) = [j+12]Q
            uadd(rho, work, &work->pt3, &Pb[k], &Pb[i], &Pb[map[j + 12]]);
            i = k;
            k = map[j + 12];
            j += 6;
        }

        // make all of the Pbprod's
        for (i = 3; i < 19; i++)
        {
            work->Pbprod[i] = mulredcx(Pb[i].X, Pb[i].Z, work->n, rho);
        }
    }

    // [31]Q + [29]Q([2]Q) = [60]Q
    A = 60;
    uadd(rho, work, &Pb[9], &Pb[10], &Pb[2], &Pa[0]);	// <-- [60]Q
    // save [60]Q
    Pa[1].X = Pa[0].X;
    Pa[1].Z = Pa[0].Z;

    // temporary - make [4]Q
    work->diff1 = submod(Pb[2].X, Pb[2].Z, work->n);
    work->sum1 = addmod(Pb[2].X, Pb[2].Z, work->n);
    udup(rho, work, work->sum1, work->diff1, &work->pt3);	// pt3 = [4]Q

    // Pd = Pad = [w]Q
    // [17]Q + [13]Q([4]Q) = [30]Q
    uadd(rho, work, &Pb[map[17]], &Pb[map[13]], &work->pt3, &work->Pad);	// <-- [30]Q
    Pd->X = work->Pad.X;
    Pd->Z = work->Pad.Z;

    work->pt1.X = Pa[0].X;
    work->pt1.Z = Pa[0].Z;
    uadd(rho, work, &work->pt1, Pd, &work->Pad, &Pa[0]);	// Pa = 30 + Pa
    work->Pad.X = work->pt1.X;
    work->Pad.Z = work->pt1.Z;

    work->pt1.X = Pa[0].X;
    work->pt1.Z = Pa[0].Z;
    uadd(rho, work, &work->pt1, Pd, &work->Pad, &Pa[0]);	// Pa = 30 + Pa
    work->Pad.X = work->pt1.X;
    work->Pad.Z = work->pt1.Z;

    A = 120;
    Pd->X = work->Pad.X = Pa[1].X;
    Pd->Z = work->Pad.Z = Pa[1].Z;

    while (A < 2 * amin * w)
    {
        work->pt1.X = Pa[0].X;
        work->pt1.Z = Pa[0].Z;
        uadd(rho, work, &work->pt1, Pd, &work->Pad, &Pa[0]);	// Pa = Pd + Pa
        work->Pad.X = work->pt1.X;
        work->Pad.Z = work->pt1.Z;

        work->pt1.X = Pa[0].X;
        work->pt1.Z = Pa[0].Z;
        uadd(rho, work, &work->pt1, Pd, &work->Pad, &Pa[0]);	// Pa = Pd + Pa
        work->Pad.X = work->pt1.X;
        work->Pad.Z = work->pt1.Z;

        A += 2 * w;
    }

    // now fill in the rest of the A array
    Paprod[0] = mulredcx(Pa[0].X, Pa[0].Z, work->n, rho);
    for (i = 1; i < (2 * L); i++)
    {
        work->pt1.X = Pa[i - 1].X;
        work->pt1.Z = Pa[i - 1].Z;
        uadd(rho, work, &Pa[i - 1], Pd, &work->Pad, &Pa[i]);	// Pa = Pd + Pa
        work->Pad.X = work->pt1.X;
        work->Pad.Z = work->pt1.Z;
        Paprod[i] = mulredcx(Pa[i].X, Pa[i].Z, work->n, rho);
    }

    //initialize accumulator and Paprod
    acc = u64div(1, work->n);

    if (work->stg1_max == 70)
    {
        pairmap_u = pairmap_u70_w60_u1_x25;
        pairmap_v = pairmap_v70_w60_u1_x25;
        numb = pairmap_sz70_w60_u1_x25;
    }
    if (work->stg1_max == 85)
    {
        pairmap_u = pairmap_u85_w60_u1_x25;
        pairmap_v = pairmap_v85_w60_u1_x25;
        numb = pairmap_sz85_w60_u1_x25;
    }
    if (work->stg1_max == 125)
    {
        pairmap_u = pairmap_u125_w60_u1_x25;
        pairmap_v = pairmap_v125_w60_u1_x25;
        numb = pairmap_sz125_w60_u1_x25;
    }
    if (work->stg1_max == 165)
    {
        pairmap_u = pairmap_u165_w60_u1_x25;
        pairmap_v = pairmap_v165_w60_u1_x25;
        numb = pairmap_sz165_w60_u1_x25;
    }
    else
    {
        pairmap_u = pairmap_u205_w60_u1_x25;
        pairmap_v = pairmap_v205_w60_u1_x25;
        numb = pairmap_sz205_w60_u1_x25;
    }



    for (i = 0; i < numb; i++)
    {
        if (pairmap_u[i] == 0)
        {
            // get L more A's
            Pa[0].X = Pa[2].X;
            Pa[1].X = Pa[3].X;
            Pa[0].Z = Pa[2].Z;
            Pa[1].Z = Pa[3].Z;
            Paprod[0] = Paprod[2];
            Paprod[1] = Paprod[3];

            uadd(rho, work, &Pa[1], Pd, &Pa[0], &Pa[2]);
            uadd(rho, work, &Pa[2], Pd, &Pa[1], &Pa[3]);

            Paprod[2] = mulredcx(Pa[2].X, Pa[2].Z, work->n, rho);
            Paprod[3] = mulredcx(Pa[3].X, Pa[3].Z, work->n, rho);

            amin += U;
            i++;
        }

        //we accumulate XrZd - XdZr = (Xr - Xd) * (Zr + Zd) + XdZd - XrZr
        //in CP notation, Pa -> (Xr,Zr), Pb -> (Xd,Zd)

        b = pairmap_u[i];
        a = pairmap_v[i] - amin;
        // accumulate the cross product  (zimmerman syntax).
        // page 342 in C&P
        work->tt1 = submod(Pa[a].X, Pb[map[b]].X, work->n);
        work->tt2 = addmod(Pa[a].Z, Pb[map[b]].Z, work->n);
        work->tt3 = mulredcx(work->tt1, work->tt2, work->n, rho);
        work->tt1 = addmod(work->tt3, work->Pbprod[map[b]], work->n);
        work->tt2 = submod(work->tt1, Paprod[a], work->n);
        acc = mulredcx(acc, work->tt2, work->n, rho);

    }

    work->stg2acc = acc;

    return;
}

/**
 * Trial division factor algorithm replacing division by multiplications.
 *
 * Instead of dividing N by consecutive primes, we store the reciprocals of those primes, too,
 * and multiply N by those reciprocals. Only if such a result is near to an integer we need
 * to do a division.
 *
 * Assuming that we want to identify "near integers" with a precision of 2^-d.
 * Then the approach works for primes p if bitLength(p) >= bitLength(N) - 53 + d.
 *
 * @authors Thilo Harich + Tilman Neumann
 */

 /*

 Ported to C and released 7/31/19
 Ben Buhrow

 */

#include <math.h>

#define DISCRIMINATOR_BITS 10 // experimental result
#define DISCRIMINATOR  1.0 / (1 << DISCRIMINATOR_BITS)
#define FACTORLIMIT (1<<21) + 1000
#define MAXPRIMES 155678

int leh_primes[MAXPRIMES];
double reciprocals[MAXPRIMES];

/**
    * Create a trial division algorithm that is capable of finding factors up to factorLimit.
    * @param factorLimit
    */


void TDiv63InverseSetup() {
    
    int i;
    for (i = 0; i < MAXPRIMES; i++) {
        reciprocals[i] = 1.0 / leh_primes[i];
    }
    
}


int tdiv_inverse(int64_t N, int pLimit) {
    int i = 0;
    int lbits = __builtin_clzll(N);
    int Nbits = 64 - lbits;

    int pMinBits = Nbits - 53 + DISCRIMINATOR_BITS;
    if (pMinBits > 0) {
        // for the smallest primes we must do standard trial division
        int pMin = 1 << pMinBits;
        //printf("standard trial division to limit %d on Nbits = %d, leading bits %d, top limit %d\n",
        //	pMin, Nbits, lbits, pLimit);

        for (; leh_primes[i] < pMin; i++) {
            if (N%leh_primes[i] == 0) {
                return leh_primes[i];
            }
        }
    }

    // Now the primes are big enough to apply trial division by inverses
    for (; leh_primes[i] <= pLimit; i++) {

        int64_t nDivPrime = (int64_t)(N*reciprocals[i] + DISCRIMINATOR);
        //if (N == 346425669865991LL && primes[i] == 70163)
        //	printf("nDivPrime = %ld, test = %ld\n", nDivPrime, nDivPrime * primes[i]);

        if (nDivPrime * (int64_t)leh_primes[i] == N) {
            // nDivPrime is very near to an integer
            if (N%leh_primes[i] == 0) {
                //printf("Found factor %d\n", primes[i]);
                return leh_primes[i];
            }
        }
    }

    // nothing found up to pLimit
    return 0;
}

/*
 * java-math-library is a Java library focused on number theory, but not necessarily limited to it. It is based on the PSIQS 4.0 factoring project.
 * Copyright (C) 2018 Tilman Neumann (www.tilman-neumann.de)
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program;
 * if not, see <http://www.gnu.org/licenses/>.
 */

 /*

 Ported to C and released 7/31/19
 Ben Buhrow

 */

 /** This is a constant that is below 1 for rounding up double values to long. */
#define SQRTBOUND ((1 << 21)+1)

const double ROUND_UP_DOUBLE = 0.9999999665;
static double sqRoot[SQRTBOUND];
static double sqrtInv[SQRTBOUND];
static int initialized = 0;

int64_t lehmanOdd(int kBegin, int kLimit, double sqrt4N, int64_t N, int64_t fourN) {
    int k;
    for (k = kBegin; k <= kLimit; k += 6) {
        int64_t a = (int64_t)(sqrt4N * sqRoot[k] + ROUND_UP_DOUBLE);
        // make a == (k+N) (mod 4)
        const int64_t kPlusN = k + N;
        if ((kPlusN & 3) == 0) {
            a += ((kPlusN - a) & 7);
        }
        else {
            a += ((kPlusN - a) & 3);
        }
        const int64_t test = a * a - k * fourN;
        //const int64_t b = (int64_t)sqrt(test);
        //if (b*b == test) {
        //	return (int64_t)gcd64(a + b, N);
        //}
        //{
        //	/* Step 1, reduce to 18% of inputs */
        //	int64_t m = test & 127;
        //	if ((m * 0x8bc40d7d) & (m * 0xa1e2f5d1) & 0x14020a)  continue;
        //	/* Step 2, reduce to 7% of inputs (mod 99 reduces to 4% but slower) */
        //	//m = test % 240; if ((m * 0xfa445556) & (m * 0x8021feb1) & 0x614aaa0f) continue;
        //	/* m = n % 99; if ((m*0x5411171d) & (m*0xe41dd1c7) & 0x80028a80) return 0; */
        //	/* Step 3, do the square root instead of any more rejections */
        //	const int64_t b = (int64_t)sqrt(test);
        //	if (b*b == test) {
        //		return gcd64(a + b, N);
        //	}
        //}
        //if (issq1024[test & 1023])
        //{
        //	if (issq4199[test % 3465] & 2)
        //	{
        //		if (issq4199[test % 4199] & 1)
        //		{
        const int64_t b = (int64_t)sqrt(test);
        if (b*b == test) {
            return gcd64(a + b, N);
        }
        //		}
        //	}
        //}
        //int t2 = test & 31;
        //if (t2 == 0 || t2 == 1 || t2 == 4 ||
        //	t2 == 9 || t2 == 16 || t2 == 17 || t2 == 25)
        //{
        //	const int64_t b = (int64_t)sqrt(test);
        //	if (b*b == test) {
        //		return (int64_t)gcd64(a + b, N);
        //	}
        //}
    }
    return -1;
}

int64_t lehmanEven(int kBegin, int kEnd, double sqrt4N, int64_t N, int64_t fourN) {
    int k;
    for (k = kBegin; k <= kEnd; k += 6) {
        // k even -> a must be odd
        const int64_t a = (int64_t)(sqrt4N * sqRoot[k] + ROUND_UP_DOUBLE) | (int64_t)1;
        const int64_t test = a * a - k * fourN;
        //const int64_t b = (int64_t)sqrt(test);
        //if (b*b == test) {
        //	return (int64_t)gcd64(a + b, N);
        //}
        //{
        //	/* Step 1, reduce to 18% of inputs */
        //	int64_t m = test & 127;
        //	if ((m * 0x8bc40d7d) & (m * 0xa1e2f5d1) & 0x14020a)  continue;
        //	/* Step 2, reduce to 7% of inputs (mod 99 reduces to 4% but slower) */
        //	//m = test % 240; if ((m * 0xfa445556) & (m * 0x8021feb1) & 0x614aaa0f) continue;
        //	/* m = n % 99; if ((m*0x5411171d) & (m*0xe41dd1c7) & 0x80028a80) return 0; */
        //	/* Step 3, do the square root instead of any more rejections */
        //	const int64_t b = (int64_t)sqrt(test);
        //	if (b*b == test) {
        //		return gcd64(a + b, N);
        //	}
        //}
        //if (issq1024[test & 1023])
        //{
        //	if (issq4199[test % 3465] & 2)
        //	{
        //		if (issq4199[test % 4199] & 1)
        //		{
        const int64_t b = (int64_t)sqrt(test);
        if (b*b == test) {
            return gcd64(a + b, N);
        }
        //		}
        //	}
        //}
        //int t2 = test & 31;
        //if (t2 == 0 || t2 == 1 || t2 == 4 ||
        //	t2 == 9 || t2 == 16 || t2 == 17 || t2 == 25)
        //{
        //	const int64_t b = (int64_t)sqrt(test);
        //	if (b*b == test) {
        //		return (int64_t)gcd64(a + b, N);
        //	}
        //}
    }
    return -1;
}


/**
 * Fast implementation of Lehman's factor algorithm.
 * Works flawlessly for N up to 60 bit.<br><br>
 *
 * It is quite surprising that the exact sqrt test of <code>test = a^2 - 4kN</code> works for N >= 45 bit.
 * At that size, both a^2 and 4kN start to overflow Long.MAX_VALUE.
 * But the error - comparing correct results vs. long results - is just the same for both a^2 and 4kN
 * (and a multiple of 2^64).
 *  Thus <code>test</code> is correct and <code>b</code> is correct, too. <code>a</code> is correct anyway.
 *
 * @authors Tilman Neumann + Thilo Harich
 */

 /*

 Ported to C and released 7/31/19
 Ben Buhrow

 */

#define get_bit(x, b) ((x) & (1 << (b)))
#define set_bit(b) ((1 << (b)))

uint32_t *tiny_soe(uint32_t limit, uint32_t *nump)
{
    // simple sieve of erathosthenes for small limits
    uint8_t *flags;
    uint32_t* primes;
    uint32_t prime;
    uint64_t i, j;
    int it;
    size_t sz = (size_t)((double)limit / 8);

    // allocate flags to sieve
    flags = (uint8_t *)malloc(sz * sizeof(uint8_t));
    if (flags == NULL)
    {
        printf("error allocating flags\n");
        exit(1);
    }

    // allocate an estimate for primes array
    *nump = (uint32_t)((double)limit / log((double)limit) * 1.1);
    primes = (uint32_t*)malloc(*nump * sizeof(uint32_t));

    memset(flags, 0xff, sz);
    primes[0] = 2;
    it = 1;
    prime = 2;

    // sieve using primes less than the sqrt of the desired limit
    i = 1;
    while ((i < sz * 8) && (prime < (uint32_t)sqrt((double)limit)))
    {
        if (get_bit(flags[i/8], i%8) > 0)
        {
            prime = (uint32_t)(2 * i + 1);
            //printf("sieving prime %u ", prime);
            for (j = i + (uint64_t)prime; j < (uint64_t)limit; j += prime)
                flags[j/8] &= ~(uint8_t)(set_bit(j%8));
            //printf("stopped at %lu\n", j);

            primes[it] = prime;
            it++;
        }
        i++;
    }

    // everything left marked is prime
    for (; i < sz * 8; i++)
    {
        if (get_bit(flags[i / 8], i % 8) > 0)
        {
            uint64_t p = (uint64_t)(2 * i + 1);
            if (p > (uint64_t)limit)
                break;
            primes[it] = (uint32_t)p;
            it++;
        }
    }
    
    free(flags);
    *nump = it;
    return primes;
}

uint64_t LehmanFactor(uint64_t uN, int DoTrialFirst)
{
    int i;
    int k;
    int j;
    int64_t N = (int64_t)uN;
    int64_t fourN;
    double sqrt4N;
    int64_t factor;
    double sixthRootTerm;

    if (!initialized)
    {
        // Precompute sqrts for all possible k. 2^21 entries are enough for N~2^63.
        int kMax = 1 << 21;
        for (i = 1; i < SQRTBOUND; i++) {
            double sqrtI = sqrt((double)i);
            sqRoot[i] = sqrtI;
            sqrtInv[i] = 1.0 / sqrtI;
        }

        tiny_soe(FACTORLIMIT, (uint32_t*)leh_primes);
        TDiv63InverseSetup();
        //printf("prime list generated\n");
        //printf("PMAX = %lu, NUM_P = %lu\n", P_MAX, NUM_P);

        initialized = 1;
        //printf("sqrt tables generated\n");
        //printf("check of sqrt(-1): %f, %d\n", sqrt(-1.), (int)sqrt(-1));

        //MakeIssq();
        //printf("issqr tables generated\n");
    }

    const int cbrt = (int)pow(N, 1. / 3);

    // do trial division before Lehman loop, up to the cube root
    if (DoTrialFirst)
    {
        //i = 0;
        //while (PRIMES[i] < cbrt)
        //{
        //	if ((N % PRIMES[i]) == 0)
        //		return PRIMES[i];
        //	i++;
        //}
        if ((factor = tdiv_inverse(N, cbrt)) > 1) return factor;
    }


    fourN = N << 2;
    sqrt4N = sqrt(fourN);

    // kLimit must be 0 mod 6, since we also want to search above of it
    const int kLimit = ((cbrt + 6) / 6) * 6;
    // For kTwoA = kLimit / 64 the range for a is at most 2. We make it 0 mod 6, too.
    const int kTwoA = (((cbrt >> 6) + 6) / 6) * 6;

    // We are investigating solutions of a^2 - sqrt(k*n) = y^2 in three k-ranges:
    // * The "small range" is 1 <= k < kTwoA, where we may have more than two 'a'-solutions per k.
    //   Thus, an inner 'a'-loop is required.
    // * The "middle range" is kTwoA <= k < kLimit, where we have at most two possible 'a' values per k.
    // * The "high range" is kLimit <= k < 2*kLimit. This range is not required for the correctness
    //   of the algorithm, but investigating it for some k==0 (mod 6) improves performance.

    // We start with the middle range cases k == 0 (mod 6) and k == 3 (mod 6),
    // which have the highest chance to find a factor.
    if ((factor = lehmanEven(kTwoA, kLimit, sqrt4N, N, fourN)) > 1) return factor;
    if ((factor = lehmanOdd(kTwoA + 3, kLimit, sqrt4N, N, fourN)) > 1) return factor;

    // Now investigate the small range
    sixthRootTerm = 0.25 * pow(N, 1 / 6.0); // double precision is required for stability
    for (k = 1; k < kTwoA; k++) {
        int64_t a;
        const int64_t fourkN = k * fourN;
        const double sqrt4kN = sqrt4N * sqRoot[k];
        // only use long values
        const int64_t aStart = (int64_t)(sqrt4kN + ROUND_UP_DOUBLE); // much faster than ceil() !
        int64_t aLimit = (int64_t)(sqrt4kN + sixthRootTerm * sqrtInv[k]);
        int64_t aStep;
        if ((k & 1) == 0) {
            // k even -> make sure aLimit is odd
            aLimit |= 1LL;
            aStep = 2;
        }
        else {
            const int64_t kPlusN = k + N;
            if ((kPlusN & 3) == 0) {
                aStep = 8;
                aLimit += ((kPlusN - aLimit) & 7);
            }
            else {
                aStep = 4;
                aLimit += ((kPlusN - aLimit) & 3);
            }
        }

        // processing the a-loop top-down is faster than bottom-up
        for (a = aLimit; a >= aStart; a -= aStep) {
            const int64_t test = a * a - fourkN;
            // Here test<0 is possible because of double to long cast errors in the 'a'-computation.
            // But then b = sqrt(test) gives NaN (sic!) => NaN*NaN != test => no errors.
            const int64_t b = (int64_t)sqrt(test);
            if (b*b == test) {
                return (int64_t)gcd64(a + b, N);
            }
            //{
            //	/* Step 1, reduce to 18% of inputs */
            //	int64_t m = test & 127;
            //	if ((m * 0x8bc40d7d) & (m * 0xa1e2f5d1) & 0x14020a)  continue;
            //	/* Step 2, reduce to 7% of inputs (mod 99 reduces to 4% but slower) */
            //	//m = test % 240; if ((m * 0xfa445556) & (m * 0x8021feb1) & 0x614aaa0f) continue;
            //	/* m = n % 99; if ((m*0x5411171d) & (m*0xe41dd1c7) & 0x80028a80) return 0; */
            //	/* Step 3, do the square root instead of any more rejections */
            //	const int64_t b = (int64_t)sqrt(test);
            //	if (b*b == test) {
            //		return gcd64(a + b, N);
            //	}
            //}

        }
    }

    // k == 0 (mod 6) has the highest chance to find a factor; checking it in the high range boosts performance
    if ((factor = lehmanEven(kLimit, kLimit << 1, sqrt4N, N, fourN)) > 1) return factor;

    // Complete middle range
    if ((factor = lehmanOdd(kTwoA + 1, kLimit, sqrt4N, N, fourN)) > 1) return factor;
    if ((factor = lehmanEven(kTwoA + 2, kLimit, sqrt4N, N, fourN)) > 1) return factor;
    if ((factor = lehmanEven(kTwoA + 4, kLimit, sqrt4N, N, fourN)) > 1) return factor;
    if ((factor = lehmanOdd(kTwoA + 5, kLimit, sqrt4N, N, fourN)) > 1) return factor;

    // do trial division after Lehman loop ?
    const int rt2 = sqrt(N);

    // do trial division after Lehman loop
    if (!DoTrialFirst)
    {
        //i = 0;
        //while (PRIMES[i] < cbrt)
        //{
        //	if ((N % PRIMES[i]) == 0)
        //		return PRIMES[i];
        //	i++;
        //}
        if ((factor = tdiv_inverse(N, cbrt)) > 1) return factor;
    }

    // If sqrt(4kN) is very near to an exact integer then the fast ceil() in the 'aStart'-computation
    // may have failed. Then we need a "correction loop":
    for (k = kTwoA + 1; k <= kLimit; k++) {
        int64_t a = (int64_t)(sqrt4N * sqRoot[k] + ROUND_UP_DOUBLE) - 1;
        int64_t test = a * a - k * fourN;
        int64_t b = (int64_t)sqrt(test);
        if (b*b == test) {
            return gcd64(a + b, N);
        }
        //{
        //	/* Step 1, reduce to 18% of inputs */
        //	int64_t m = test & 127;
        //	if ((m * 0x8bc40d7d) & (m * 0xa1e2f5d1) & 0x14020a)  continue;
        //	/* Step 2, reduce to 7% of inputs (mod 99 reduces to 4% but slower) */
        //	//m = test % 240; if ((m * 0xfa445556) & (m * 0x8021feb1) & 0x614aaa0f) continue;
        //	/* m = n % 99; if ((m*0x5411171d) & (m*0xe41dd1c7) & 0x80028a80) return 0; */
        //	/* Step 3, do the square root instead of any more rejections */
        //	const int64_t b = (int64_t)sqrt(test);
        //	if (b*b == test) {
        //		return gcd64(a + b, N);
        //	}
        //}
    }

    return 0; // fail
}

int bin_search_uint32(int idp, int idm, uint32_t q, uint32_t* input)
{
    int next = (idp + idm) / 2;

    while ((idp - idm) > 10)
    {
        if (input[next] > q)
        {
            idp = next;
            next = (next + idm) / 2;
        }
        else
        {
            idm = next;
            next = (idp + next) / 2;
        }
    }

    for (next = idm; next < idm + 10; next++)
        if (input[next] >= q)
            return next;

    if (input[next] != q)
        next = -1;

    return next;
}

static uint32_t* primes_list;
static int primes_generated = 0;

void generate_semiprime_list(int num, int bits)
{
    //generate a list of 'num' semiprimes, each of size 'bits'
    //save to semiprimes.dat

    FILE* out;
    int i;
    char fname[80];
    uint64_t tmp1, tmp2, tmp3;
    uint32_t nump;

    sprintf(fname, "semiprimes_%dbit.dat", bits);
    out = fopen(fname, "w");
    if (out == NULL)
    {
        printf("couldn't open %s for writing\n", fname);
        return;
    }

    if (primes_generated == 0)
    {
        printf("generating prime list\n");
        primes_list = tiny_soe(4294967295, &primes_generated);
        printf("found %u primes\n", primes_generated);
    }

    for (i = 0; i < num; i++)
    {
        tmp3 = spRand64(0, 1ULL << (bits / 2));
        tmp3 |= (1ULL << (bits / 2 - 1));

        int id = bin_search_uint32(primes_generated, 0, tmp3, primes_list);
        tmp1 = primes_list[id];

        if (bits & 1)
        {
            tmp3 = spRand64(0, 1ULL << (bits / 2 + 1));
            tmp3 |= (1ULL << (bits / 2));
        }
        else
        {
            tmp3 = spRand64(0, 1ULL << (bits / 2));
            tmp3 |= (1ULL << (bits / 2 - 1));
        }
        id = bin_search_uint32(primes_generated, 0, tmp3, primes_list);
        tmp2 = primes_list[id];
        
        tmp3 = tmp1 * tmp2;
        fprintf(out, "%lu,%lu,%lu\n", tmp3, tmp1, tmp2);
    }
    fclose(out);

    printf("generated %d semiprimes in file semiprimes.dat\n", num);
    return;
}

int main(int argc, char **argv)
{
	FILE *in;
	double t_time;
	int i = 0, num, correct, avg_curves;
	struct timeval gstart;
	struct timeval gstop;
	int nf;
	int num_files;
	char filenames[15][80];
    uint64_t inputs[100000];
    uint64_t knowns1[100000], knowns2[100000];
    int generate_test_files = 0;

    gettimeofday(&gstart, NULL);
    srand(42);
    LCGSTATE = 65537 * (uint64_t)(rand() + 1) * (gstart.tv_usec + 1);
    printf("randstate is %lu\n", LCGSTATE);

    if (generate_test_files)
    {
        generate_semiprime_list(100000, 42);
        generate_semiprime_list(100000, 44);
        generate_semiprime_list(100000, 46);
        generate_semiprime_list(100000, 48);
        generate_semiprime_list(100000, 50);
        generate_semiprime_list(100000, 52);
        generate_semiprime_list(100000, 54);
        generate_semiprime_list(100000, 56);
        generate_semiprime_list(100000, 58);
        generate_semiprime_list(100000, 60);
        generate_semiprime_list(100000, 62);
        generate_semiprime_list(100000, 64);
    }

    // testing larger semiprime inputs.
    // note: only one of these if blocks should be active at a time.
    if (1)
    {
        strcpy(filenames[i++], "semiprimes_42bit.dat"); // 0
        strcpy(filenames[i++], "semiprimes_44bit.dat"); // 1
        strcpy(filenames[i++], "semiprimes_46bit.dat"); // 2 
        strcpy(filenames[i++], "semiprimes_48bit.dat"); // 3 
        strcpy(filenames[i++], "semiprimes_50bit.dat"); // 4 
        strcpy(filenames[i++], "semiprimes_52bit.dat"); // 5 
        strcpy(filenames[i++], "semiprimes_54bit.dat"); // 6 
        strcpy(filenames[i++], "semiprimes_56bit.dat"); // 7 
        strcpy(filenames[i++], "semiprimes_58bit.dat"); // 8 
        strcpy(filenames[i++], "semiprimes_60bit.dat"); // 9 
        strcpy(filenames[i++], "semiprimes_62bit.dat"); // 10 
        strcpy(filenames[i++], "semiprimes_64bit.dat"); // 11 
        num_files = i;
    }
    // testing small semiprime inputs
    else if (0)
    {
        strcpy(filenames[i++], "testdata_24bit");   // 0
        strcpy(filenames[i++], "testdata_26bit");   // 1
        strcpy(filenames[i++], "testdata_28bit");   // 2
        strcpy(filenames[i++], "testdata_30bit");   // 3 
        strcpy(filenames[i++], "testdata_32bit");   // 4 
        num_files = i;
    }
    // testing larger random inputs
    else if (0)
    {
        strcpy(filenames[i++], "testdata_42bit");   // 0 
        strcpy(filenames[i++], "testdata_44bit");   // 1 
        strcpy(filenames[i++], "testdata_46bit");   // 2 
        strcpy(filenames[i++], "testdata_48bit");   // 3 
        strcpy(filenames[i++], "testdata_50bit");   // 4 
        strcpy(filenames[i++], "testdata_52bit");   // 5 
        strcpy(filenames[i++], "testdata_54bit");   // 6
        strcpy(filenames[i++], "testdata_56bit");   // 7
        strcpy(filenames[i++], "testdata_58bit");   // 8
        strcpy(filenames[i++], "testdata_60bit");   // 9
        strcpy(filenames[i++], "testdata_62bit");   // 10
        strcpy(filenames[i++], "testdata_64bit");   // 11
        num_files = i;
    }

	
	// tinyecm test
	for (nf = -1; nf < num_files; nf++)
	{
		char buf[1024];
		int curves;
		int B1;
		uint64_t known1, known2;
        uint64_t in64;

        // a mixed strategy uses curves at several B1 
        // values to attempt each input.  If your true
        // inputs of interest span a range of sizes and/or
        // are known to not be semiprime (their factors
        // are not necessarily the same size), this
        // approach is probably best.  If they are all
        // one size and semiprime then the B1 values can 
        // be tuned better.
        int use_mixed_strategy = 0;
        int bitsize = 0;

        // read in the first few test inputs to determine
        // the size of inputs in this file (we assume they
        // are all approximately the same).
        if ((use_mixed_strategy == 0) && (nf >= 0))
        {
            in = fopen(filenames[nf], "r");

            for (i = 0; i < 1; i++)
            {
                fgets(buf, 1024, in);
                sscanf(buf, "%lu, %lu, %lu", &in64, &known1, &known2);
                bitsize = 64 - __builtin_clzll(in64);
            }

            fclose(in);

            if (bitsize == 0)
            {
                B1 = 70;
                curves = 0;
            }
            else if (bitsize <= 24)
            {
                B1 = 27;
                curves = 16;
            }
            else if (bitsize <= 26)
            {
                B1 = 31;
                curves = 16;
            }
            else if (bitsize <= 28)
            {
                B1 = 37;
                curves = 16;
            }
            else if (bitsize <= 32)
            {
                B1 = 41;
                curves = 16;
            }
#if 0
            else if (bitsize <= 44)
            {
                B1 = 47;
                curves = 16;
            }
            else if (bitsize <= 48)
            {
                B1 = 59;
                curves = 16;
            }
            else if (bitsize <= 52)
            {
                B1 = 70;
                curves = 16;
            }
            else if (bitsize <= 56)
            {
                B1 = 125;
                curves = 20;
            }
            else if (bitsize <= 58)
            {
                B1 = 165;
                curves = 16;
            }
#else
            else if (bitsize <= 52)
            {
                B1 = 70;
                curves = 24;
            }
            else if (bitsize <= 54)
            {
                B1 = 85;
                curves = 24;
            }
            else if (bitsize <= 58)
            {
                B1 = 125;
                curves = 32;
            }
#endif
            else if (bitsize <= 60)
            {
                B1 = 165;
                curves = 32;
            }
            else if (bitsize <= 64)
            {
                B1 = 205;
                curves = 40;
            }
            else
            {
                B1 = 70;
                curves = 1024;
            }
            
            printf("choosing B1 = %d, curves = %d\n", B1, curves);
        }

		gettimeofday(&gstart, NULL);

		if (nf < 0)
		{
			in = fopen(filenames[0], "r");
			correct = 0;
			num = 100000;
			for (i = 0; i < num; i++)
			{
				uint64_t in64;
				uint64_t outf;

				fgets(buf, 1024, in);
				sscanf(buf, "%lu, %lu, %lu", &in64, &known1, &known2);
				B1 += in64 + known1 + known2;
			}

			fclose(in);

			gettimeofday(&gstop, NULL);
			t_time = my_difftime(&gstart, &gstop);

			printf("file overhead was %2.2f sec (%d)\n", t_time, B1);
		}
		else
		{
            int num_mixed_states;
            int mixed_b2mult;
            // mixed strategy for "standard" curves 42-64 bits
            //int curves_array[7] = { 2, 2, 4, 8, 8, 8, 16 };
            //int B1_array[7] = { 47, 59, 70, 85, 125, 165, 205 };
            //num_mixed_states = 7;

            // mixed strategy for ultra-small inputs (24-32 bits)
            int curves_array[5] = { 2, 2, 2, 4, 8 };
            int B1_array[5] = { 27, 29, 31, 37, 41 };
            num_mixed_states = 5;
            mixed_b2mult = 0;

			in = fopen(filenames[nf], "r");
			printf("testing file: %s\n", filenames[nf]);

			correct = 0;
			num = 100000;
			avg_curves = 0;

            for (i = 0; i < num; i++)
            {
                int j;
                uint64_t outf;

                fgets(buf, 1024, in);
                sscanf(buf, "%lu, %lu, %lu", &in64, &known1, &known2);
                inputs[i] = in64;
                knowns1[i] = known1;
                knowns2[i] = known2;
            }
            fclose(in);

            gettimeofday(&gstart, NULL);

            int k;
            for (k = 0; k < 1; k++)
            {
                for (i = 0; i < num; i++)
                {
                    int j;
                    uint64_t outf;
                    in64 = inputs[i];

                    //fgets(buf, 1024, in);
                    //sscanf(buf, "%lu, %lu, %lu", &in64, &known1, &known2);
                    //
                    //outf = LehmanFactor(inputs[i], 1); 
                    //if ((outf == knowns1[i]) ||
                    //    (outf == knowns2[i]))
                    //{
                    //    correct++;
                    //}
                    //continue;

                    if (use_mixed_strategy)
                    {
                        for (j = 0; j < num_mixed_states; j++)
                        {
                            if (curves_array[j] == 0)
                                continue;

                            avg_curves += tinyecm(in64, &outf, B1_array[j],
                                B1_array[j] * mixed_b2mult, curves_array[j]);

                            if ((outf == knowns1[i]) ||
                                (outf == knowns2[i]))
                            {
                                correct++;
                                break;
                            }
                        }
                    }
                    else
                    {
                        if (B1 > 160)
                            avg_curves += tinyecm(in64, &outf, B1, 35 * B1, curves);
                        else if (B1 < 70)
                            avg_curves += tinyecm(in64, &outf, B1, 25 * B1, curves);
                        else
                            avg_curves += tinyecm(in64, &outf, B1, 25 * B1, curves);

                        if ((outf == knowns1[i]) ||
                            (outf == knowns2[i]))
                        {
                            correct++;
                        }
                    }
                }
            }

			gettimeofday(&gstop, NULL);
			t_time = my_difftime(&gstart, &gstop);

			printf("tinyecm got %d of %d correct in %2.2f sec average %1.2f curves/input\n",
				correct / k, num, t_time, (double)avg_curves / (double)num);
			printf("percent correct = %.2f\n", 
                100.0*(double)correct / (double)num / (double)k);
			printf("average time per input = %.2f ms\n", 1000 * t_time / (double)num);
		}
	}

	return 0;
}


