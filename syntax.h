#ifndef LNG_SYNTAX_H
#define LNG_SYNTAX_H

#include "lexical.h"
#include "node.h"

struct lng_syntax_t {
	const unsigned char* text;
	struct lng_token_t* tokens;
	struct lng_token_t* token;
	size_t token_count;
	unsigned char test;
	unsigned char* memory;
	size_t memory_size;
};

struct lng_node_unit_t* lng_syntax_unit(struct lng_syntax_t* self);

struct lng_syntax_t* lng_syntax_new(const unsigned char* text);

void lng_syntax_free(struct lng_syntax_t* self);

struct lng_node_unit_t* lng_syntax_execute(struct lng_syntax_t* self);

struct lng_node_package_t* lng_syntax_package(struct lng_syntax_t* self);

struct lng_node_import_t* lng_syntax_imports(struct lng_syntax_t* self);

struct lng_node_import_t* lng_syntax_import(struct lng_syntax_t* self);

struct lng_node_class_t* lng_syntax_class(struct lng_syntax_t* self);

struct lng_node_t* lng_syntax_field(struct lng_syntax_t* self);

struct lng_node_t* lng_syntax_method(struct lng_syntax_t* self);

struct lng_node_param_t* lng_syntax_parameters(struct lng_syntax_t* self);

struct lng_node_command_t* lng_syntax_command(struct lng_syntax_t* self);

struct lng_node_type_t* lng_syntax_type(struct lng_syntax_t* self);

struct lng_node_block_t* lng_syntax_block(struct lng_syntax_t* self);

struct lng_node_t* lng_syntax_if(struct lng_syntax_t* self);

struct lng_node_t* lng_syntax_while(struct lng_syntax_t* self);

struct lng_node_t* lng_syntax_repeat(struct lng_syntax_t* self);

struct lng_node_t* lng_syntax_for(struct lng_syntax_t* self);

struct lng_node_t* lng_syntax_declare(struct lng_syntax_t* self);

struct lng_node_expression_t* lng_syntax_expression(struct lng_syntax_t* self);

struct lng_node_value_t* lng_syntax_expr(struct lng_syntax_t* self);

struct lng_node_value_t* lng_syntax_ternary(struct lng_syntax_t* self);

struct lng_node_value_t* lng_syntax_and(struct lng_syntax_t* self);

struct lng_node_value_t* lng_syntax_or(struct lng_syntax_t* self);

struct lng_node_value_t* lng_syntax_compare(struct lng_syntax_t* self);

struct lng_node_value_t* lng_syntax_sum(struct lng_syntax_t* self);

struct lng_node_value_t* lng_syntax_mul(struct lng_syntax_t* self);

struct lng_node_value_t* lng_syntax_unary(struct lng_syntax_t* self);

struct lng_node_value_t* lng_syntax_literal(struct lng_syntax_t* self);

void lng_syntax_test();

#endif
