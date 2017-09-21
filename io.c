#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "platform.h"
#include "io.h"

#ifdef __WIN32__

#else 

char* lng_io_tmp(const unsigned char* bytes, int36 size) {
	FILE* file = fopen("/tmp/cg.cu", "wb");
	if (!file) {
		return 0;
	}
	uint36 len = size < 0 ? strlen((const char*) bytes) : size;
	if (fwrite(bytes, 1, len, file) != len) {
		fclose(file);
		return 0;
	}
	fclose(file);
	return strdup("/tmp/cg.cu");
}

#endif

unsigned char* lng_io_file(const char* filename) {
	FILE* file = fopen(filename, "rb");
	if (!file) {
		return 0;
	}
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);
	unsigned char* data = (unsigned char*) malloc(size + 1);
	if (!data) {
		fclose(file);
		return 0;
	}
	size_t readed = fread(data, 1, size, file);
	if (readed != size) {
		free(data);
		fclose(file);
		return 0;
	}
	data[size] = 0;
	fclose(file);
	return data;
}

void lng_io_int_write(unsigned char* bytes, int value) {
	unsigned char neg = value < 0;
	unsigned int uvalue = neg ? abs(value) : value;
	*bytes++ = (neg ? 0x80 : 0) + ((uvalue >> 24) & 0x7F);
	*bytes++ = (uvalue >> 16) & 0xFF;
	*bytes++ = (uvalue >> 8) & 0xFF;
	*bytes++ = (uvalue) & 0xFF;
}

void lng_io_uint32_write(unsigned char* bytes, unsigned int value) {
	*(bytes++) = (value >> 24) & 0xFF;
	*(bytes++) = (value >> 16) & 0xFF;
	*(bytes++) = (value >> 8) & 0xFF;
	*(bytes++) = (value) & 0xFF;
}

unsigned int lng_io_uint_read(const unsigned char* bytes) {
	unsigned char i1 = *(bytes++);
	unsigned char i2 = *(bytes++);
	unsigned char i3 = *(bytes++);
	unsigned char i4 = *(bytes++);
	return (i1 << 24) + (i2 << 16) + (i3 << 8) + i4;
}

void lng_io_uint64_write(unsigned char* bytes, uint64 value) {
	*bytes++ = (value >> 56) & 0xFF;
	*bytes++ = (value >> 48) & 0xFF;
	*bytes++ = (value >> 40) & 0xFF;
	*bytes++ = (value >> 32) & 0xFF;
	*bytes++ = (value >> 24) & 0xFF;
	*bytes++ = (value >> 16) & 0xFF;
	*bytes++ = (value >> 8) & 0xFF;
	*bytes++ = (value) & 0xFF;
}

uint64 lng_io_ulong_read(unsigned char* bytes) {
	uint64 a = *bytes++;
	uint64 b = *bytes++;
	uint64 c = *bytes++;
	uint64 d = *bytes++;
	uint32 e = *bytes++;
	uint32 f = *bytes++;
	uint32 g = *bytes++;
	uint32 h = *bytes++;
	return (a << 56) + (b << 48) + (c << 40) + (d << 32) + (e << 24) + (f << 16) + (g << 8) + h;
}
/*
 void lng_io_double_write(unsigned char* bytes, double v) {
 int ebits = 11, fbits = 52;
 int bias = (1 << (ebits - 1)) - 1;
 unsigned char s;
 double f;
 int e;
 if (isnan(v)) {
 e = (1 << bias) - 1;
 f = 1;
 s = 0;
 } else if (isinf(v)) {
 e = (1 << bias) - 1;
 f = 0;
 s = (v < 0) ? 1 : 0;
 } else if (v == 0) {
 e = 0;
 f = 0;
 s = (isinf(1 / v) == -1) ? 1 : 0;
 } else {
 s = v < 0;
 v = v < 0 ? -v : v;
 if (v >= pow(2, 1 - bias)) {
 double ln2 = 0.6931471805599453;
 double aux = floor(log(v) / ln2);
 double ln = aux > bias ? bias : aux;
 e = (int) (ln + bias);
 f = v * pow(2, fbits - ln) - pow(2, fbits);
 } else {
 e = 0;
 f = v / pow(2, 1 - bias - fbits);
 }
 }
 unsigned char bits[64];
 int i, n, m, index = 0;
 for (i = 0; i < fbits; i++) {
 bits[index++] = ((int) f) % 2 == 1 ? 1 : 0;
 f = floor(f / 2);
 }
 for (i = 0; i < ebits; i++) {
 bits[index++] = e % 2 == 1 ? 1 : 0;
 e = (int) floor(e / 2);
 }
 bits[index] = s;
 for (n = 0; n < 8; n++) {
 int value = 0;
 for (m = 0; m < 8; m++) {
 if (bits[n * 8 + m]) {
 value += pow(2, m);
 }
 }
 *bytes++ = (unsigned char) (value & 0xff);
 }
 }
 
 double lng_io_double_read(const unsigned char* bytes) {
 int ebits = 11, fbits = 52;
 unsigned char bits[64];
 int n, i, j, index = 0;
 for (i = 0; i < 8; i++) {
 unsigned char b = *bytes++;
 for (j = 0; j < 8; j++) {
 bits[index++] = b % 2 == 1 ? 1 : 0;
 b = b >> 1;
 }
 }
 int bias = (1 << (ebits - 1)) - 1;
 int s = bits[63] ? -1 : 1;
 int e = 0;
 for (n = 62; n >= 64 - ebits - 1; n--) {
 if (bits[n]) {
 e += pow(2, ebits - 1 - 62 + n);
 }
 }
 long f = 0;
 int imax = 64 - ebits - 2;
 for (n = imax; n >= 0; n--) {
 if (bits[n]) {
 f += pow(2, n);
 }
 }
 if (e == (1 << ebits) - 1) {
 return f != 0 ? NAN : s * 1. / 0.;
 } else if (e > 0) {
 return s * pow(2, e - bias) * (1 + f / pow(2, fbits));
 } else if (f != 0) {
 return s * pow(2, -(bias - 1)) * (f / pow(2, fbits));
 } else {
 return s * 0;
 }
 }*/

void lng_io_chars_write(unsigned char* bytes, char* text, int64 len) {
	if (len < 0) {
		char* aux = text;
		char c = *(aux++);
		while (c) {
			*(bytes++) = c;
			c = *(aux++);
		}
	} else {
		int n;
		char* aux = text;
		for (n = 0; n < len; n++) {
			*(bytes++) = *(aux++);
		}
	}
}

unsigned char* lng_io_chars_read(unsigned char* bytes, unsigned int len) {
	unsigned char* data = (unsigned char*) malloc(len + 1);
	unsigned char* aux = data;
	if (!data) {
		return 0;
	}
	data[len] = 0;
	unsigned int n;
	for (n = 0; n < len; n++) {
		*(aux++) = *(bytes++);
	}
	return data;
}

int lng_io_string_to_int(char *s, long size) {
	int n = 0, sign = 1;
	if (*s == '-') {
		sign = -1;
		s++;
		size--;
	}
	if (size < 0) {
		for (; *s; s++) {
			n = (n * 10) + (*s - '0');
		}
	} else {
		int m;
		for (m = 0; m < size; m++) {
			n = (n * 10) + (*(s++) - '0');
		}
	}
	return sign * n;
}

uint32 lng_io_string_to_uint(char *s, long size) {
	int n, m = 0;
	for (n = 0; n < size; n++) {
		m = (m * 10) + (*(s++) - '0');
	}
	return m;
}

char* lng_io_uint_to_string(unsigned int value, char *digits, int base) {
	char *s, *p;
	s = "0123456789abcdefghijklmnopqrstuvwxyz";
	if (base == 0)
		base = 10;
	if (digits == NULL || base < 2 || base > 36)
		return NULL;
	if (value < (unsigned int) base) {
		digits[0] = s[value];
		digits[1] = '\0';
	} else {
		for (p = lng_io_uint_to_string(value / ((unsigned int) base), digits, base); *p; p++) {
		}
		lng_io_uint_to_string(value % ((unsigned int) base), p, base);
	}
	return digits;
}

char* lng_io_int_to_string(int value, char *digits, int base) {
	char *d;
	unsigned int u;
	d = digits;
	if (base == 0)
		base = 10;
	if (digits == NULL || base < 2 || base > 36)
		return NULL;
	if (value < 0) {
		*d++ = '-';
		u = -((unsigned) value);
	} else
		u = value;
	lng_io_uint_to_string(u, d, base);
	return digits;
}

//void lng_io_double_to_int64(double x, int* exp, int64_t* frac) {
//	double xf = fabs(frexp(x, exp)) - 0.5;
//	if (xf < 0.0) {
//		*frac = 0;
//		return;
//	}
//	long long f = 1 + xf * 2.0 * (9223372036854775807LL - 1);
//	if (x < 0.0) {
//		f = -f;
//	}
//	*frac = f;
//}
//
//double lng_io_int64_to_double(int exp, int64_t frac) {
//	double xf, x;
//	if (frac == 0) {
//		return 0.0;
//	}
//	xf = ((double) (llabs(frac) - 1) / (9223372036854775807LL - 1)) / 2.0;
//	x = ldexp(xf + 0.5, exp);
//	if (frac < 0) {
//		x = -x;
//	}
//	return x;
//}
