#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "stringbuilder.h"

void stringbuilder_new(struct stringbuilder_t* self, unsigned char* bytes, size_t max) {
    self->bytes = bytes;
	self->size = 0;
    self->max = max - 1;
    bytes[max - 1] = 0;
    
}

unsigned char* stringbuilder_indexof(struct stringbuilder_t* self, unsigned char c, size_t offset, size_t* length) {
    if (length) {
        unsigned char* data = (unsigned char*) strchr((char*) self->bytes + offset, c);
        if (!data) {
            return 0;
        }
        *length = data - self->bytes + offset;
    }
    return self->bytes + offset;
}

void stringbuilder_append_chars(struct stringbuilder_t* self, unsigned char* buffer, size_t length) {
	if (length <= 0) {
		return;
	}
    if (self->size == self->max) {
        return;
    }
    if (self->max - self->size < length) {
        length = self->max - self->size;
    }
    memcpy(self->bytes + self->size, buffer, length);
    self->size += length;
}

void stringbuilder_append_uint(struct stringbuilder_t* self, unsigned int value) {
    if (self->size == self->max) {
        return;
    }
	int i = (int) log10((double) value);
    size_t len = i + 1;
    if (self->max - self->size < len) {
        len = self->max - self->size;
    }
	while (value > 0) {
        if (self->max - self->size > i) {
            self->bytes[self->size + i] = (value % 10) + '0';
	    }
    	value = value / 10;
		i = i - 1;
	}
    self->size += len;
}

void stringbuilder_append_char(struct stringbuilder_t* self, unsigned char c) {
    if (self->size == self->max) {
        return;
    }
	self->bytes[self->size++] = c;
}

const unsigned char* stringbuilder_tostring(struct stringbuilder_t* self, size_t* length) {
    if (length) {
        *length = self->size;
    }
	return self->bytes;
}