#ifndef LNG_IO_H
#define LNG_IO_H

#include "platform.h"

#ifndef	SEEK_END
#define SEEK_END	(2)
#endif

#define lng_io_int32_read_inline(BYTES, VALUE) \
if ((*(BYTES) & 0x80)) { \
VALUE = -((*((BYTES)++) & 0x7F) << 24); \
VALUE -= (*((BYTES)++) << 16); \
VALUE -= (*((BYTES)++) << 8); \
VALUE -= *((BYTES)++); \
} else { \
VALUE = (*((BYTES)++) << 24); \
VALUE += (*((BYTES)++) << 16); \
VALUE += (*((BYTES)++) << 8); \
VALUE += *((BYTES)++); \
}

#define lng_io_int32_see_inline(BYTES) (((BYTES)[0] & 0x80) ? (-(((BYTES)[0] & 0x7F) << 24) - ((BYTES)[1] << 16) - ((BYTES)[2] << 8) - ((BYTES)[3])) : (((BYTES)[0] << 24) + ((BYTES)[1] << 16) + ((BYTES)[2] << 8) + ((BYTES)[3])))

#define lng_io_uint32_see_inline(BYTES) ((BYTES)[0] << 24) + ((BYTES)[1] << 16) + ((BYTES)[2] << 8) + ((BYTES)[3])

#define lng_io_uint64_see_inline(BYTES) ((uint64)(BYTES)[0]<<56) + ((uint64)(BYTES)[1]<<48) + ((uint64)(BYTES)[2]<<40) + ((uint64)(BYTES)[3]<<32) + ((BYTES)[4]<<24) + ((BYTES)[5]<<16) + ((BYTES)[6]<<8) + ((BYTES)[7])

#define lng_io_uint16_read_inline(B,V) V=*B++<<8; V+=*B++;

#define lng_io_uint32_read_inline(B,V) V=*B++<<24; V+=*B++<<16; V+=*B++<<8; V+=*B++;

#define lng_io_int32_write_inline(BYTES, VALUE) \
if (VALUE < 0) { \
	*(BYTES)++ = (((-VALUE) >> 24) & 0x7F) + 0x80; \
	*(BYTES)++ = ((-VALUE) >> 16) & 0xFF; \
	*(BYTES)++ = ((-VALUE) >> 8) & 0xFF; \
	*(BYTES)++ = (-VALUE) & 0xFF; \
} else { \
	*(BYTES)++ = ((VALUE) >> 24); \
	*(BYTES)++ = ((VALUE) >> 16) & 0xFF; \
	*(BYTES)++ = ((VALUE) >> 8) & 0xFF; \
	*(BYTES)++ = (VALUE) & 0xFF; \
}

#define lng_io_uint64_read_inline(VALUE, BYTES) \
VALUE = ((uint64)(*((BYTES)++)) << 56); \
VALUE += ((uint64)(*((BYTES)++)) << 48); \
VALUE += ((uint64)(*((BYTES)++)) << 40); \
VALUE += ((uint64)(*((BYTES)++)) << 32); \
VALUE += (*((BYTES)++) << 24); \
VALUE += (*((BYTES)++) << 16); \
VALUE += (*((BYTES)++) << 8); \
VALUE += *((BYTES)++);

#define lng_io_uint64_write_inline(BYTES, VALUE) \
*(BYTES)++ = (((uint64)(VALUE)) >> 56); \
*(BYTES)++ = (((uint64)(VALUE)) >> 48); \
*(BYTES)++ = (((uint64)(VALUE)) >> 40); \
*(BYTES)++ = (((uint64)(VALUE)) >> 32); \
*(BYTES)++ = (((uint32)(VALUE)) >> 24); \
*(BYTES)++ = (((uint32)(VALUE)) >> 16) & 0xFF; \
*(BYTES)++ = (((uint32)(VALUE)) >> 8) & 0xFF; \
*(BYTES)++ = ((uint32)(VALUE)) & 0xFF;

#define lng_io_uint16_write_inline(B,V) *B++=((V)>>8)&0xFF; *B++=(V)&0xFF;

#define lng_io_uint32_write_inline(B,V) *B++=((V)>>24)&0xFF; *B++=((V)>>16)&0xFF; *B++=((V)>>8)&0xFF; *B++=(V)&0xFF;

#define lng_io_uint8_write_inline(B,V) *B++=V;

#define lng_io_uchars_write_inline(B,STR,SIZE) memcpy(B,STR,SIZE); B+=SIZE;

#define lng_io_string_write_inline(B,STR,SIZE) lng_io_uint32_write_inline(B,SIZE); lng_io_uchars_write_inline(B,STR,SIZE);

char* lng_io_tmp(const unsigned char* bytes, int36 size);

unsigned char* lng_io_file(const char* filename);

void lng_io_int_write(unsigned char* bytes, int value);

void lng_io_uint32_write(unsigned char* bytes, unsigned int value);

unsigned int lng_io_uint_read(const unsigned char* bytes);

void lng_io_uint64_write(unsigned char* bytes, uint64 value);

uint64 lng_io_ulong_read(unsigned char* bytes);

void lng_io_double_write(unsigned char* bytes, double v);

double lng_io_double_read(const unsigned char* bytes);

int lng_io_string_to_int(char *s, long size);

uint32 lng_io_string_to_uint(char *s, long size);

void lng_io_chars_write(unsigned char* bytes, char* text, int64 len);

unsigned char* lng_io_chars_read(unsigned char* bytes, unsigned int len);

char* lng_io_uint_to_string(unsigned int value, char *digits, int base);

char* lng_io_int_to_string(int value, char *digits, int base);

#define lng_io_double_to_bytes_inline(V) ((unsigned char*) &(V))

#define lng_io_bytes_to_double_inline(B) (*((double*)(B)))

#endif
