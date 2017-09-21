#ifndef LNG_LINKER_H
#define LNG_LINKER_H

#include "treemap.h"

struct lng_linker_t {
    struct b_treemap_t* classes;
};

struct lng_linker_class_t {
    unsigned char* name;
	struct b_treemap_t* fields;
	struct b_treemap_t* methods;
	byte* binary;
	byte* opcodes;
};

struct lng_linker_method_t {
    unsigned char* name;
};

struct lng_linker_field_t {
    unsigned char* name;
};

struct lng_linker_t* lng_linker_new();

void lng_linker_free(struct lng_linker_t* self);

void lng_linker_class_free(struct lng_linker_class_t* self);

void lng_linker_field_free(struct lng_linker_field_t* self);

void lng_linker_method_free(struct lng_linker_method_t* self);

unsigned char lng_linker_add(struct lng_linker_t* self, unsigned char* bytes);

unsigned char* lng_linker_compile(struct lng_linker_t* self, uint36* size);

void lng_linker_test();

#endif
