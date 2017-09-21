#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "node.h"
#include "lexical.h"
#include "syntax.h"
#include "compile.h"
#include "io.h"

struct lng_node_unit_t* lng_node(const unsigned char* text) {
	size_t tokens_size;
	struct lng_token_t* tokens = lng_lexical(text, &tokens_size);
	if (!tokens) {
		return 0;
	}
	struct lng_syntax_t syntax;
	syntax.token_count = tokens_size;
	syntax.text = text;
	syntax.test = 1;
	syntax.tokens = syntax.token = tokens;
	struct lng_node_unit_t* node = lng_syntax_unit(&syntax);
	if (!node) {
		free(tokens);
		return 0;
	}
	node->text = text;
	node->tokens = tokens;
	node->vtable->head((struct lng_node_t*) node);
	node->vtable->body((struct lng_node_t*) node);
	return node;
}

void lng_node_free(struct lng_node_unit_t* self) {
	free(self->tokens);
	free(self);
}

unsigned char* lng_compile(struct lng_node_unit_t** nodes, uint8_t node_size) {
	struct lng_compile_t compiler;
	unsigned int n;
	for (n = 0; n < node_size; n++) {
		struct lng_node_unit_t* node = nodes[n];
		node->vtable->link((struct lng_node_t*) node, &compiler);
	}
	size_t bytes_size = 0;
	{
		bytes_size++;
		bytes_size += 4;
		for (n = 0; n < node_size; n++) {
			struct lng_node_unit_t* unit = nodes[n];
			if (unit->package && unit->package->name->size > 0) {
				bytes_size += 4 + unit->package->name->size + 1 + unit->clazz->name->size;
			} else {
				bytes_size += 4 + unit->clazz->name->size;
			}
			bytes_size += 2;
			bytes_size += 2;
		}
		for (n = 0; n < node_size; n++) {
			struct lng_node_unit_t* unit = nodes[n];
			size_t m, msize = unit->clazz->method_count;
			struct lng_node_method_t* method = unit->clazz->methods;
			for (m = 0; m < msize; m++) {
				if (method->declares > 0) {
					bytes_size += 5;
				}
				bytes_size += method->cmd->vtable->size((struct lng_node_t*) method->cmd);
				method++;
			}
		}
		bytes_size++;
	}
	unsigned char* bytes = (unsigned char*) malloc(bytes_size);
	if (!bytes) {
		return 0;
	}
	unsigned char* bytes_next = bytes;
	{
		lng_io_uint8_write_inline(bytes_next, 0xBE);
		lng_io_uint32_write_inline(bytes_next, node_size);
		for (n = 0; n < node_size; n++) {
			struct lng_node_unit_t* unit = nodes[n];
			const unsigned char* text = unit->text;
			if (unit->package && unit->package->name->size > 0) {
				struct lng_token_t* pkgname = unit->package->name;
				struct lng_token_t* classname = unit->clazz->name;
				unsigned int size = pkgname->size + 1 + classname->size;
				lng_io_uint32_write_inline(bytes_next, size);
				lng_io_uchars_write_inline(bytes_next, text + pkgname->begin, pkgname->size);
				lng_io_uint8_write_inline(bytes_next, '.');
				lng_io_uchars_write_inline(bytes_next, text + classname->begin, classname->size);
			} else {
				struct lng_token_t* classname = unit->clazz->name;
				uint32 size = classname->size;
				lng_io_uint32_write_inline(bytes_next, size);
				lng_io_uchars_write_inline(bytes_next, text + classname->begin, classname->size);
			}
			lng_io_uint16_write_inline(bytes_next, unit->clazz->field_count);
			uint16_t m, msize = unit->clazz->field_count;
//            struct lng_node_field_t* fields = unit->clazz->fields;
			for (m = 0; m < msize; m++) {
				assert(0);
			}
			lng_io_uint16_write_inline(bytes_next, unit->clazz->method_count);
		}
		for (n = 0; n < node_size; n++) {
			struct lng_node_unit_t* unit = nodes[n];
			size_t m, msize = unit->clazz->method_count;
			struct lng_node_method_t* method = unit->clazz->methods;
			for (m = 0; m < msize; m++) {
				if (method->declares > 0) {
					lng_opcode_stack_push(bytes_next, method->declares);
				}
				method->cmd->vtable->build((struct lng_node_t*) method->cmd, &bytes_next);
				method++;
			}
		}
		lng_io_uint8_write_inline(bytes_next, 0xFF);
	}
	if (bytes + bytes_size != bytes_next) {
		assert(0);
		free(bytes);
		return 0;
	}
	return bytes;
}
