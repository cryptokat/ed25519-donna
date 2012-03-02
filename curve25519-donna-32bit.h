/*
	Public domain by Andrew M. <liquidsun@gmail.com>
	See: https://github.com/floodyberry/curve25519-donna
	
	32 bit integer curve25519 implementation
*/

typedef uint32_t bignum25519[10];
typedef uint32_t bignum25519align16[12];


/* Copy a bignum25519 to another: out = in */
static void DONNA_INLINE
curve25519_copy(bignum25519 out, const bignum25519 in) {
	out[0] = in[0];
	out[1] = in[1];
	out[2] = in[2];
	out[3] = in[3];
	out[4] = in[4];
	out[5] = in[5];
	out[6] = in[6];
	out[7] = in[7];
	out[8] = in[8];
	out[9] = in[9];
}

/* Sum two numbers: out += in */
static void DONNA_INLINE
curve25519_add(bignum25519 out, const bignum25519 in) {
	out[0] += in[0];
	out[1] += in[1];
	out[2] += in[2];
	out[3] += in[3];
	out[4] += in[4];
	out[5] += in[5];
	out[6] += in[6];
	out[7] += in[7];
	out[8] += in[8];
	out[9] += in[9];
}

static void DONNA_INLINE
curve25519_add_reduce(bignum25519 out, const bignum25519 in) {
	uint32_t c;
	out[0] += in[0]    ; c = (out[0] >> 26); out[0] &= 0x3ffffff;
	out[1] += in[1] + c; c = (out[1] >> 25); out[1] &= 0x1ffffff;
	out[2] += in[2] + c; c = (out[2] >> 26); out[2] &= 0x3ffffff;
	out[3] += in[3] + c; c = (out[3] >> 25); out[3] &= 0x1ffffff;
	out[4] += in[4] + c; c = (out[4] >> 26); out[4] &= 0x3ffffff;
	out[5] += in[5] + c; c = (out[5] >> 25); out[5] &= 0x1ffffff;
	out[6] += in[6] + c; c = (out[6] >> 26); out[6] &= 0x3ffffff;
	out[7] += in[7] + c; c = (out[7] >> 25); out[7] &= 0x1ffffff;
	out[8] += in[8] + c; c = (out[8] >> 26); out[8] &= 0x3ffffff;
	out[9] += in[9] + c; c = (out[9] >> 25); out[9] &= 0x1ffffff;
	out[0] +=    19 * c;
}

/* out = out - in */
#define curve25519_subtract_reduce curve25519_subtract
static void DONNA_INLINE
curve25519_subtract(bignum25519 out, const bignum25519 in) {
	uint32_t r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,c;

	r0 = 0x7ffffda + out[0] - in[0];
	r1 = 0x3fffffe + out[1] - in[1];
	r2 = 0x7fffffe + out[2] - in[2];
	r3 = 0x3fffffe + out[3] - in[3];
	r4 = 0x7fffffe + out[4] - in[4];
	r5 = 0x3fffffe + out[5] - in[5];
	r6 = 0x7fffffe + out[6] - in[6];
	r7 = 0x3fffffe + out[7] - in[7];
	r8 = 0x7fffffe + out[8] - in[8];
	r9 = 0x3fffffe + out[9] - in[9];

	         c = (r0 >> 26); r0 &= 0x3ffffff;
	r1 += c; c = (r1 >> 25); r1 &= 0x1ffffff;
	r2 += c; c = (r2 >> 26); r2 &= 0x3ffffff;
	r3 += c; c = (r3 >> 25); r3 &= 0x1ffffff;
	r4 += c; c = (r4 >> 26); r4 &= 0x3ffffff;
	r5 += c; c = (r5 >> 25); r5 &= 0x1ffffff;
	r6 += c; c = (r6 >> 26); r6 &= 0x3ffffff;
	r7 += c; c = (r7 >> 25); r7 &= 0x1ffffff;
	r8 += c; c = (r8 >> 26); r8 &= 0x3ffffff;
	r9 += c; c = (r9 >> 25); r9 &= 0x1ffffff;
	r0 += 19 * c;

	out[0] = r0;
	out[1] = r1;
	out[2] = r2;
	out[3] = r3;
	out[4] = r4;
	out[5] = r5;
	out[6] = r6;
	out[7] = r7;
	out[8] = r8;
	out[9] = r9;
}

/* Multiply two numbers: out = in2 * in */
static void
curve25519_mul(bignum25519 out, const bignum25519 in2, const bignum25519 in) {
	uint32_t r0,r1,r2,r3,r4,r5,r6,r7,r8,r9;
	uint32_t s0,s1,s2,s3,s4,s5,s6,s7,s8,s9;
	uint64_t m0,m1,m2,m3,m4,m5,m6,m7,m8,m9,c;
	uint32_t p;

	r0 = in[0];
	r1 = in[1];
	r2 = in[2];
	r3 = in[3];
	r4 = in[4];
	r5 = in[5];
	r6 = in[6];
	r7 = in[7];
	r8 = in[8];
	r9 = in[9];

	s0 = in2[0];
	s1 = in2[1];
	s2 = in2[2];
	s3 = in2[3];
	s4 = in2[4];
	s5 = in2[5];
	s6 = in2[6];
	s7 = in2[7];
	s8 = in2[8];
	s9 = in2[9];

	m1 = mul32x32_64(r0, s1) + mul32x32_64(r1, s0);
	m3 = mul32x32_64(r0, s3) + mul32x32_64(r1, s2) + mul32x32_64(r2, s1) + mul32x32_64(r3, s0);
	m5 = mul32x32_64(r0, s5) + mul32x32_64(r1, s4) + mul32x32_64(r2, s3) + mul32x32_64(r3, s2) + mul32x32_64(r4, s1) + mul32x32_64(r5, s0);
	m7 = mul32x32_64(r0, s7) + mul32x32_64(r1, s6) + mul32x32_64(r2, s5) + mul32x32_64(r3, s4) + mul32x32_64(r4, s3) + mul32x32_64(r5, s2) + mul32x32_64(r6, s1) + mul32x32_64(r7, s0);
	m9 = mul32x32_64(r0, s9) + mul32x32_64(r1, s8) + mul32x32_64(r2, s7) + mul32x32_64(r3, s6) + mul32x32_64(r4, s5) + mul32x32_64(r5, s4) + mul32x32_64(r6, s3) + mul32x32_64(r7, s2) + mul32x32_64(r8, s1) + mul32x32_64(r9, s0);

	r1 *= 2;
	r3 *= 2;
	r5 *= 2;
	r7 *= 2;

	m0 = mul32x32_64(r0, s0);
	m2 = mul32x32_64(r0, s2) + mul32x32_64(r1, s1) + mul32x32_64(r2, s0);
	m4 = mul32x32_64(r0, s4) + mul32x32_64(r1, s3) + mul32x32_64(r2, s2) + mul32x32_64(r3, s1) + mul32x32_64(r4, s0);
	m6 = mul32x32_64(r0, s6) + mul32x32_64(r1, s5) + mul32x32_64(r2, s4) + mul32x32_64(r3, s3) + mul32x32_64(r4, s2) + mul32x32_64(r5, s1) + mul32x32_64(r6, s0);
	m8 = mul32x32_64(r0, s8) + mul32x32_64(r1, s7) + mul32x32_64(r2, s6) + mul32x32_64(r3, s5) + mul32x32_64(r4, s4) + mul32x32_64(r5, s3) + mul32x32_64(r6, s2) + mul32x32_64(r7, s1) + mul32x32_64(r8, s0);

	r1 *= 19;
	r2 *= 19;
	r3 = (r3 / 2) * 19;
	r4 *= 19;
	r5 = (r5 / 2) * 19;
	r6 *= 19;
	r7 = (r7 / 2) * 19;
	r8 *= 19;
	r9 *= 19;

	m1 += (mul32x32_64(r9, s2) + mul32x32_64(r8, s3) + mul32x32_64(r7, s4) + mul32x32_64(r6, s5) + mul32x32_64(r5, s6) + mul32x32_64(r4, s7) + mul32x32_64(r3, s8) + mul32x32_64(r2, s9));
	m3 += (mul32x32_64(r9, s4) + mul32x32_64(r8, s5) + mul32x32_64(r7, s6) + mul32x32_64(r6, s7) + mul32x32_64(r5, s8) + mul32x32_64(r4, s9));
	m5 += (mul32x32_64(r9, s6) + mul32x32_64(r8, s7) + mul32x32_64(r7, s8) + mul32x32_64(r6, s9));
	m7 += (mul32x32_64(r9, s8) + mul32x32_64(r8, s9));

	r3 *= 2;
	r5 *= 2;
	r7 *= 2;
	r9 *= 2;

	m0 += (mul32x32_64(r9, s1) + mul32x32_64(r8, s2) + mul32x32_64(r7, s3) + mul32x32_64(r6, s4) + mul32x32_64(r5, s5) + mul32x32_64(r4, s6) + mul32x32_64(r3, s7) + mul32x32_64(r2, s8) + mul32x32_64(r1, s9));
	m2 += (mul32x32_64(r9, s3) + mul32x32_64(r8, s4) + mul32x32_64(r7, s5) + mul32x32_64(r6, s6) + mul32x32_64(r5, s7) + mul32x32_64(r4, s8) + mul32x32_64(r3, s9));
	m4 += (mul32x32_64(r9, s5) + mul32x32_64(r8, s6) + mul32x32_64(r7, s7) + mul32x32_64(r6, s8) + mul32x32_64(r5, s9));
	m6 += (mul32x32_64(r9, s7) + mul32x32_64(r8, s8) + mul32x32_64(r7, s9));
	m8 += (mul32x32_64(r9, s9));

	                             r0 = (uint32_t)m0 & 0x3ffffff; c = (m0 >> 26);
	m1 += c;                     r1 = (uint32_t)m1 & 0x1ffffff; c = (m1 >> 25);
	m2 += c;                     r2 = (uint32_t)m2 & 0x3ffffff; c = (m2 >> 26);
	m3 += c;                     r3 = (uint32_t)m3 & 0x1ffffff; c = (m3 >> 25);
	m4 += c;                     r4 = (uint32_t)m4 & 0x3ffffff; c = (m4 >> 26);
	m5 += c;                     r5 = (uint32_t)m5 & 0x1ffffff; c = (m5 >> 25);
	m6 += c;                     r6 = (uint32_t)m6 & 0x3ffffff; c = (m6 >> 26);
	m7 += c;                     r7 = (uint32_t)m7 & 0x1ffffff; c = (m7 >> 25);
	m8 += c;                     r8 = (uint32_t)m8 & 0x3ffffff; c = (m8 >> 26);
	m9 += c;                     r9 = (uint32_t)m9 & 0x1ffffff; p = (uint32_t)(m9 >> 25);
	m0 = r0 + mul32x32_64(p,19); r0 = (uint32_t)m0 & 0x3ffffff; p = (uint32_t)(m0 >> 26);
	r1 += p;      p = r1 >> 25; r1 &= 0x1ffffff;
	r2 += p;      p = r2 >> 26; r2 &= 0x3ffffff;
	r3 += p;      p = r3 >> 25; r3 &= 0x1ffffff;
	r4 += p;      p = r4 >> 26; r4 &= 0x3ffffff;
	r5 += p;      p = r5 >> 25; r5 &= 0x1ffffff;
	r6 += p;      p = r6 >> 26; r6 &= 0x3ffffff;
	r7 += p;      p = r7 >> 25; r7 &= 0x1ffffff;
	r8 += p;      p = r8 >> 26; r8 &= 0x3ffffff;
	r9 += p;      p = r9 >> 25; r9 &= 0x1ffffff;
	r0 += p * 19;

	out[0] = r0;
	out[1] = r1;
	out[2] = r2;
	out[3] = r3;
	out[4] = r4;
	out[5] = r5;
	out[6] = r6;
	out[7] = r7;
	out[8] = r8;
	out[9] = r9;
}


static void
curve25519_square_times(bignum25519 out, const bignum25519 in, int count) {
	uint32_t r0,r1,r2,r3,r4,r5,r6,r7,r8,r9;
	uint32_t d6,d7,d8,d9;
	uint64_t m0,m1,m2,m3,m4,m5,m6,m7,m8,m9,c;
	uint32_t p;

	r0 = in[0];
	r1 = in[1];
	r2 = in[2];
	r3 = in[3];
	r4 = in[4];
	r5 = in[5];
	r6 = in[6];
	r7 = in[7];
	r8 = in[8];
	r9 = in[9];

	do {
		m0 = mul32x32_64(r0, r0);
		r0 *= 2;
		m1 = mul32x32_64(r0, r1);
		m2 = mul32x32_64(r0, r2) + mul32x32_64(r1, r1 * 2);
		r1 *= 2;
		m3 = mul32x32_64(r0, r3) + mul32x32_64(r1, r2    );
		m4 = mul32x32_64(r0, r4) + mul32x32_64(r1, r3 * 2) + mul32x32_64(r2, r2);
		r2 *= 2;
		m5 = mul32x32_64(r0, r5) + mul32x32_64(r1, r4    ) + mul32x32_64(r2, r3);
		m6 = mul32x32_64(r0, r6) + mul32x32_64(r1, r5 * 2) + mul32x32_64(r2, r4) + mul32x32_64(r3, r3 * 2);
		r3 *= 2;
		m7 = mul32x32_64(r0, r7) + mul32x32_64(r1, r6    ) + mul32x32_64(r2, r5) + mul32x32_64(r3, r4    );
		m8 = mul32x32_64(r0, r8) + mul32x32_64(r1, r7 * 2) + mul32x32_64(r2, r6) + mul32x32_64(r3, r5 * 2) + mul32x32_64(r4, r4    );
		m9 = mul32x32_64(r0, r9) + mul32x32_64(r1, r8    ) + mul32x32_64(r2, r7) + mul32x32_64(r3, r6    ) + mul32x32_64(r4, r5 * 2);

		d6 = r6 * 19;
		d7 = r7 * 2 * 19;
		d8 = r8 * 19;
		d9 = r9 * 2 * 19;

		m0 += (mul32x32_64(d9, r1    ) + mul32x32_64(d8, r2    ) + mul32x32_64(d7, r3    ) + mul32x32_64(d6, r4 * 2) + mul32x32_64(r5, r5 * 2 * 19));
		m1 += (mul32x32_64(d9, r2 / 2) + mul32x32_64(d8, r3    ) + mul32x32_64(d7, r4    ) + mul32x32_64(d6, r5 * 2));
		m2 += (mul32x32_64(d9, r3    ) + mul32x32_64(d8, r4 * 2) + mul32x32_64(d7, r5 * 2) + mul32x32_64(d6, r6    ));
		m3 += (mul32x32_64(d9, r4    ) + mul32x32_64(d8, r5 * 2) + mul32x32_64(d7, r6    ));
		m4 += (mul32x32_64(d9, r5 * 2) + mul32x32_64(d8, r6 * 2) + mul32x32_64(d7, r7    ));
		m5 += (mul32x32_64(d9, r6    ) + mul32x32_64(d8, r7 * 2));
		m6 += (mul32x32_64(d9, r7 * 2) + mul32x32_64(d8, r8    ));
		m7 += (mul32x32_64(d9, r8    ));
		m8 += (mul32x32_64(d9, r9    ));

									 r0 = (uint32_t)m0 & 0x3ffffff; c = (m0 >> 26);
		m1 += c;                     r1 = (uint32_t)m1 & 0x1ffffff; c = (m1 >> 25);
		m2 += c;                     r2 = (uint32_t)m2 & 0x3ffffff; c = (m2 >> 26);
		m3 += c;                     r3 = (uint32_t)m3 & 0x1ffffff; c = (m3 >> 25);
		m4 += c;                     r4 = (uint32_t)m4 & 0x3ffffff; c = (m4 >> 26);
		m5 += c;                     r5 = (uint32_t)m5 & 0x1ffffff; c = (m5 >> 25);
		m6 += c;                     r6 = (uint32_t)m6 & 0x3ffffff; c = (m6 >> 26);
		m7 += c;                     r7 = (uint32_t)m7 & 0x1ffffff; c = (m7 >> 25);
		m8 += c;                     r8 = (uint32_t)m8 & 0x3ffffff; c = (m8 >> 26);
		m9 += c;                     r9 = (uint32_t)m9 & 0x1ffffff; p = (uint32_t)(m9 >> 25);
		m0 = r0 + mul32x32_64(p,19); r0 = (uint32_t)m0 & 0x3ffffff; p = (uint32_t)(m0 >> 26);
		r1 += p;      p = r1 >> 25; r1 &= 0x1ffffff;
		r2 += p;      p = r2 >> 26; r2 &= 0x3ffffff;
		r3 += p;      p = r3 >> 25; r3 &= 0x1ffffff;
		r4 += p;      p = r4 >> 26; r4 &= 0x3ffffff;
		r5 += p;      p = r5 >> 25; r5 &= 0x1ffffff;
		r6 += p;      p = r6 >> 26; r6 &= 0x3ffffff;
		r7 += p;      p = r7 >> 25; r7 &= 0x1ffffff;
		r8 += p;      p = r8 >> 26; r8 &= 0x3ffffff;
		r9 += p;      p = r9 >> 25; r9 &= 0x1ffffff;
		r0 += p * 19;
	} while (--count);

	out[0] = r0;
	out[1] = r1;
	out[2] = r2;
	out[3] = r3;
	out[4] = r4;
	out[5] = r5;
	out[6] = r6;
	out[7] = r7;
	out[8] = r8;
	out[9] = r9;
}

/* Take a little-endian, 32-byte number and expand it into polynomial form */
static void
curve25519_expand(bignum25519 out, const unsigned char in[32]) {
	#define F(n,start,shift,mask) \
		out[n] = \
			((((uint32_t) in[start + 0]) | \
			((uint32_t) in[start + 1]) << 8 | \
			((uint32_t) in[start + 2]) << 16 | \
			((uint32_t) in[start + 3]) << 24) >> shift) & mask;

	F(0, 0, 0, 0x3ffffff);
	F(1, 3, 2, 0x1ffffff);
	F(2, 6, 3, 0x3ffffff);
	F(3, 9, 5, 0x1ffffff);
	F(4, 12, 6, 0x3ffffff);
	F(5, 16, 0, 0x1ffffff);
	F(6, 19, 1, 0x3ffffff);
	F(7, 22, 3, 0x1ffffff);
	F(8, 25, 4, 0x3ffffff);
	F(9, 28, 6, 0x1ffffff);
	#undef F
}

/* Take a fully reduced polynomial form number and contract it into a
 * little-endian, 32-byte array
 */
static void
curve25519_contract(unsigned char out[32], const bignum25519 in) {
	bignum25519 f;
	curve25519_copy(f, in);

	#define carry_pass() \
		f[1] += f[0] >> 26; f[0] &= 0x3ffffff; \
		f[2] += f[1] >> 25; f[1] &= 0x1ffffff; \
		f[3] += f[2] >> 26; f[2] &= 0x3ffffff; \
		f[4] += f[3] >> 25; f[3] &= 0x1ffffff; \
		f[5] += f[4] >> 26; f[4] &= 0x3ffffff; \
		f[6] += f[5] >> 25; f[5] &= 0x1ffffff; \
		f[7] += f[6] >> 26; f[6] &= 0x3ffffff; \
		f[8] += f[7] >> 25; f[7] &= 0x1ffffff; \
		f[9] += f[8] >> 26; f[8] &= 0x3ffffff;

	#define carry_pass_full() \
		carry_pass() \
		f[0] += 19 * (f[9] >> 25); f[9] &= 0x1ffffff;

	#define carry_pass_final() \
		carry_pass() \
		f[9] &= 0x1ffffff;

	carry_pass_full()
	carry_pass_full()

	/* now t is between 0 and 2^255-1, properly carried. */
	/* case 1: between 0 and 2^255-20. case 2: between 2^255-19 and 2^255-1. */
	f[0] += 19;
	carry_pass_full()

	/* now between 19 and 2^255-1 in both cases, and offset by 19. */
	f[0] += (1 << 26) - 19;
	f[1] += (1 << 25) - 1;
	f[2] += (1 << 26) - 1;
	f[3] += (1 << 25) - 1;
	f[4] += (1 << 26) - 1;
	f[5] += (1 << 25) - 1;
	f[6] += (1 << 26) - 1;
	f[7] += (1 << 25) - 1;
	f[8] += (1 << 26) - 1;
	f[9] += (1 << 25) - 1;

	/* now between 2^255 and 2^256-20, and offset by 2^255. */
	carry_pass_final()

	#undef carry_pass
	#undef carry_full
	#undef carry_final

	f[1] <<= 2;
	f[2] <<= 3;
	f[3] <<= 5;
	f[4] <<= 6;
	f[6] <<= 1;
	f[7] <<= 3;
	f[8] <<= 4;
	f[9] <<= 6;

	#define F(i, s) \
		out[s+0] |= (unsigned char )(f[i] & 0xff); \
		out[s+1] = (unsigned char )((f[i] >> 8) & 0xff); \
		out[s+2] = (unsigned char )((f[i] >> 16) & 0xff); \
		out[s+3] = (unsigned char )((f[i] >> 24) & 0xff);

	out[0] = 0;
	out[16] = 0;
	F(0,0);
	F(1,3);
	F(2,6);
	F(3,9);
	F(4,12);
	F(5,16);
	F(6,19);
	F(7,22);
	F(8,25);
	F(9,28);
	#undef F
}


static void DONNA_INLINE
curve25519_move_conditional(bignum25519 out, const bignum25519 in, uint32_t flag) {
	const uint32_t nb = flag - 1, b = ~nb;
	out[0] = (out[0] & nb) | (in[0] & b);
	out[1] = (out[1] & nb) | (in[1] & b);
	out[2] = (out[2] & nb) | (in[2] & b);
	out[3] = (out[3] & nb) | (in[3] & b);
	out[4] = (out[4] & nb) | (in[4] & b);
	out[5] = (out[5] & nb) | (in[5] & b);
	out[6] = (out[6] & nb) | (in[6] & b);
	out[7] = (out[7] & nb) | (in[7] & b);
	out[8] = (out[8] & nb) | (in[8] & b);
	out[9] = (out[9] & nb) | (in[9] & b);
}

/*
 * Maybe swap the contents of two bignum25519 arrays (@a and @b), each 5 elements
 * long. Perform the swap iff @swap is non-zero.
 */
static void DONNA_INLINE
curve25519_swap_conditional(bignum25519 a, bignum25519 b, uint32_t iswap) {
	const uint32_t swap = (uint32_t)(-(int32_t)iswap);
	uint32_t x0,x1,x2,x3,x4,x5,x6,x7,x8,x9;

	x0 = swap & (a[0] ^ b[0]); a[0] ^= x0; b[0] ^= x0;
	x1 = swap & (a[1] ^ b[1]); a[1] ^= x1; b[1] ^= x1;
	x2 = swap & (a[2] ^ b[2]); a[2] ^= x2; b[2] ^= x2;
	x3 = swap & (a[3] ^ b[3]); a[3] ^= x3; b[3] ^= x3;
	x4 = swap & (a[4] ^ b[4]); a[4] ^= x4; b[4] ^= x4;
	x5 = swap & (a[5] ^ b[5]); a[5] ^= x5; b[5] ^= x5;
	x6 = swap & (a[6] ^ b[6]); a[6] ^= x6; b[6] ^= x6;
	x7 = swap & (a[7] ^ b[7]); a[7] ^= x7; b[7] ^= x7;
	x8 = swap & (a[8] ^ b[8]); a[8] ^= x8; b[8] ^= x8;
	x9 = swap & (a[9] ^ b[9]); a[9] ^= x9; b[9] ^= x9;
}
