#ifndef STRINGBUILDER_T
#define STRINGBUILDER_T

struct stringbuilder_t {
	unsigned char* bytes;
	size_t size;
    size_t max;
};

void stringbuilder_new(struct stringbuilder_t* self, unsigned char* bytes, size_t max);

unsigned char* stringbuilder_indexof(struct stringbuilder_t* self, unsigned char c, size_t offset, size_t* length);

void stringbuilder_append_chars(struct stringbuilder_t* self, unsigned char* buffer, size_t length);

void stringbuilder_append_char(struct stringbuilder_t* self, unsigned char c);

void stringbuilder_append_uint(struct stringbuilder_t* self, unsigned int value);

const unsigned char* stringbuilder_tostring(struct stringbuilder_t* self, size_t* length);

#endif
