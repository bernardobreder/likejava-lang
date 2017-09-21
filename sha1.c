#include <string.h>
#include "sha1.h"

#define SHA1_K0 0x5a827999
#define SHA1_K20 0x6ed9eba1
#define SHA1_K40 0x8f1bbcdc
#define SHA1_K60 0xca62c1d6

const unsigned char sha1InitState[] = { 0x01, 0x23, 0x45, 0x67, // H0
		0x89, 0xab, 0xcd, 0xef, // H1
		0xfe, 0xdc, 0xba, 0x98, // H2
		0x76, 0x54, 0x32, 0x10, // H3
		0xf0, 0xe1, 0xd2, 0xc3 // H4
		};

void sha1_new(sha1_t *s) {
	memcpy(s->state.b, sha1InitState, SHA1_HASH_LENGTH);
	s->byteCount = 0;
	s->bufferOffset = 0;
}

static unsigned int sha1_rol32(unsigned int number, unsigned char bits) {
	return ((number << bits) | (number >> (32 - bits)));
}

static void sha1_hash_block(sha1_t *s) {
	unsigned char i;
	unsigned int a, b, c, d, e, t;
	a = s->state.w[0];
	b = s->state.w[1];
	c = s->state.w[2];
	d = s->state.w[3];
	e = s->state.w[4];
	for (i = 0; i < 80; i++) {
		if (i >= 16) {
			t = s->buffer.w[(i + 13) & 15] ^ s->buffer.w[(i + 8) & 15] ^ s->buffer.w[(i + 2) & 15] ^ s->buffer.w[i & 15];
			s->buffer.w[i & 15] = sha1_rol32(t, 1);
		}
		if (i < 20) {
			t = (d ^ (b & (c ^ d))) + SHA1_K0;
		} else if (i < 40) {
			t = (b ^ c ^ d) + SHA1_K20;
		} else if (i < 60) {
			t = ((b & c) | (d & (b | c))) + SHA1_K40;
		} else {
			t = (b ^ c ^ d) + SHA1_K60;
		}
		t += sha1_rol32(a, 5) + e + s->buffer.w[i & 15];
		e = d;
		d = c;
		c = sha1_rol32(b, 30);
		b = a;
		a = t;
	}
	s->state.w[0] += a;
	s->state.w[1] += b;
	s->state.w[2] += c;
	s->state.w[3] += d;
	s->state.w[4] += e;
}

static void sha1_add_uncounted(sha1_t *s, unsigned char data) {
	s->buffer.b[s->bufferOffset ^ 3] = data;
	s->bufferOffset++;
	if (s->bufferOffset == SHA1_BLOCK_LENGTH) {
		sha1_hash_block(s);
		s->bufferOffset = 0;
	}
}

void sha1_writebyte(sha1_t *s, unsigned char data) {
	++s->byteCount;
	sha1_add_uncounted(s, data);
}

void sha1_write(sha1_t *s, const char *data, size_t len) {
	for (; len--;) {
		sha1_writebyte(s, (unsigned char) *data++);
	}
}

static void sha1_pad(sha1_t *s) {
	// Implement SHA-1 padding (fips180-2 §5.1.1)
	// Pad with 0x80 followed by 0x00 until the end of the block
	sha1_add_uncounted(s, 0x80);
	while (s->bufferOffset != 56)
		sha1_add_uncounted(s, 0x00);
	// Append length in the last 8 bytes
	sha1_add_uncounted(s, 0); // We're only using 32 bit lengths
	sha1_add_uncounted(s, 0); // But SHA-1 supports 64 bit lengths
	sha1_add_uncounted(s, 0); // So zero pad the top bits
	sha1_add_uncounted(s, s->byteCount >> 29); // Shifting to multiply by 8
	sha1_add_uncounted(s, s->byteCount >> 21); // as SHA-1 supports bitstreams as well as
	sha1_add_uncounted(s, s->byteCount >> 13); // byte.
	sha1_add_uncounted(s, s->byteCount >> 5);
	sha1_add_uncounted(s, s->byteCount << 3);
}

unsigned char* sha1_result(sha1_t *s) {
	int i;
	// Pad to complete the last block
	sha1_pad(s);
	// Swap byte order back
	for (i = 0; i < 5; i++) {
		unsigned int a, b;
		a = s->state.w[i];
		b = a << 24;
		b |= (a << 8) & 0x00ff0000;
		b |= (a >> 8) & 0x0000ff00;
		b |= a >> 24;
		s->state.w[i] = b;
	}
	// Return pointer to hash (20 characters)
	return s->state.b;
}