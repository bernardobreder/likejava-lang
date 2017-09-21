#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "platform.h"
#include "lexical.h"
#include "syntax.h"
#include "node.h"
#include "compile.h"
#include "io.h"
#include "vm.h"

struct lng_node_t* lng_node_find(struct lng_node_t* root, unsigned char type) {
	register struct lng_node_t* parent = root;
	while (parent) {
		if (parent->vtable->classid == type) {
			return parent;
		}
		parent = parent->parent;
	}
	return 0;
}

int32_t lng_node_declare_find(struct lng_node_t* parent, struct lng_token_t* name_token) {
	uint64 name_begin = name_token->begin;
	uint32_t name_size = name_token->size;
	uint32_t index = 0;
	struct lng_node_unit_t* unit_node = (struct lng_node_unit_t*) lng_node_find(parent, LNG_NODE_UNIT_TYPE);
	const unsigned char* text = unit_node->text;
	struct lng_node_block_t* block_node = (struct lng_node_block_t*) lng_node_find(parent, LNG_NODE_BLOCK_TYPE);
	while (block_node) {
		struct lng_node_linked_t* declare = block_node->declares;
		while (declare) {
			struct lng_node_declare_t* declare_node = (struct lng_node_declare_t*) declare->node;
			if (declare_node->name->size == name_size) {
				const unsigned char* name = text + declare_node->name->begin;
				if (name[0] == text[name_begin]) {
					if (name_size == 1 || !strncmp((const char*) name, (const char*) text + name_begin, name_size)) {
						return index;
					}
				}
			}
			index++;
			declare = declare->next;
		}
		block_node = (struct lng_node_block_t*) lng_node_find(block_node->parent, LNG_NODE_BLOCK_TYPE);
	}
	return -1;
}

void lng_node_unit_head(struct lng_node_unit_t* self) {
	self->package->vtable->head((struct lng_node_t*) self->package);
	struct lng_node_import_t* import = self->imports;
	while (import->vtable) {
		import->vtable->head((struct lng_node_t*) import);
		import++;
	}
	self->clazz->vtable->head((struct lng_node_t*) self->clazz);
}

void lng_node_unit_body(struct lng_node_unit_t* self) {
	self->package->vtable->body((struct lng_node_t*) self->package);
	struct lng_node_import_t* import = self->imports;
	while (import->vtable) {
		import->vtable->body((struct lng_node_t*) import);
		import++;
	}
	self->clazz->vtable->body((struct lng_node_t*) self->clazz);
}

void lng_node_unit_link(struct lng_node_unit_t* self, struct lng_compile_t* compiler) {
	self->package->vtable->link((struct lng_node_t*) self->package, compiler);
	struct lng_node_import_t* import = self->imports;
	while (import->vtable) {
		import->vtable->link((struct lng_node_t*) import, compiler);
		import++;
	}
	self->clazz->vtable->link((struct lng_node_t*) self->clazz, compiler);
}

uint32_t lng_node_unit_size(struct lng_node_unit_t* self) {
	uint32_t result = 1;
//	if (self->package && self->package->name->size > 0) {
//		struct lng_token_t* pkgname = self->package->name;
//		struct lng_token_t* classname = self->clazz->name;
//		result += 4 + pkgname->size + 1 + classname->size;
//	} else {
//		struct lng_token_t* classname = self->clazz->name;
//		result += 4 + classname->size;
//	}
//	result += self->clazz->vtable->size((struct lng_node_t*) self->clazz);
//	result += 1;
	return result;
}

void lng_node_unit_build(struct lng_node_unit_t* self, unsigned char** bytes) {
//	char* text = (char*) compile->syntax->text;
//	*(compile->bytes_next++) = 0xBB;
//	self->package->vtable->build((struct lng_node_t*) self->package);
//	{
//		if (self->package && self->package->name->size > 0) {
//			struct lng_token_t* pkgname = self->package->name;
//			struct lng_token_t* classname = self->clazz->name;
//			uint32_t size = pkgname->size + 1 + classname->size;
//			lng_io_uint32_write(compile->bytes_next, size);
//			compile->bytes_next += 4;
//			lng_io_chars_write(compile->bytes_next, text + pkgname->begin, pkgname->size);
//			compile->bytes_next += pkgname->size;
//			*(compile->bytes_next++) = '.';
//			lng_io_chars_write(compile->bytes_next, text + classname->begin, classname->size);
//			compile->bytes_next += classname->size;
//		} else {
//			struct lng_token_t* classname = self->clazz->name;
//			uint32_t size = classname->size;
//			lng_io_uint64_write(compile->bytes_next, size);
//			compile->bytes_next += 4;
//			lng_io_chars_write(compile->bytes_next, text + classname->begin, size);
//			compile->bytes_next += size;
//		}
//	}
//	self->clazz->vtable->build((struct lng_node_t*) self->clazz);
//	*(compile->bytes_next++) = 0xFF;
}

void lng_node_package_head(struct lng_node_package_t* self) {
}

void lng_node_package_body(struct lng_node_package_t* self) {
}

void lng_node_package_link(struct lng_node_package_t* self, struct lng_compile_t* compiler) {
}

uint32_t lng_node_package_size(struct lng_node_unit_t* self) {
	return 0;
}

void lng_node_package_build(struct lng_node_package_t* self, unsigned char** bytes) {
}

void lng_node_import_head(struct lng_node_import_t* self) {
}

void lng_node_import_link(struct lng_node_import_t* self, struct lng_compile_t* compiler) {
}

uint32_t lng_node_import_size(struct lng_node_unit_t* self) {
	return 0;
}

void lng_node_import_body(struct lng_node_import_t* self) {
}

void lng_node_import_build(struct lng_node_import_t* self, unsigned char** bytes) {
}

void lng_node_class_head(struct lng_node_class_t* self) {
	struct lng_node_field_t* field = self->fields;
	while (field->vtable) {
		field->vtable->head((struct lng_node_t*) field);
		field++;
	}
	struct lng_node_method_t* method = self->methods;
	while (method->vtable) {
		method->vtable->head((struct lng_node_t*) method);
		method++;
	}
}

void lng_node_class_body(struct lng_node_class_t* self) {
	struct lng_node_field_t* field = self->fields;
	while (field->vtable) {
		field->vtable->body((struct lng_node_t*) field);
		field++;
	}
	struct lng_node_method_t* method = self->methods;
	while (method->vtable) {
		method->vtable->body((struct lng_node_t*) method);
		method++;
	}
}

void lng_node_class_link(struct lng_node_class_t* self, struct lng_compile_t* compiler) {
	struct lng_node_field_t* field = self->fields;
	while (field->vtable) {
		field->vtable->link((struct lng_node_t*) field, compiler);
		field++;
	}
	struct lng_node_method_t* method = self->methods;
	while (method->vtable) {
		method->vtable->link((struct lng_node_t*) method, compiler);
		method++;
	}
}

uint32_t lng_node_class_size(struct lng_node_class_t* self) {
	uint32_t result = 0;
//	result += 4;
//	{
//		if (self->field_count > 0) {
//			struct lng_node_field_t* field = self->fields;
//			while (field->vtable) {
//				result += field->vtable->size((struct lng_node_t*) field);
//				field++;
//			}
//		}
//	}
//	{
//		result += 4;
//	}
//	{
//		if (self->method_count > 0) {
//			struct lng_node_method_t* method = self->methods;
//			while (method->vtable) {
//				struct lng_token_t* name = method->name;
//				int32 size = name->size;
//				result += 4 + size;
//				method++;
//			}
//		}
//	}
//	{
//		if (self->method_count > 0) {
//			struct lng_node_method_t* method = self->methods;
//			while (method->vtable) {
//				result += method->vtable->size((struct lng_node_t*) method);
//				method++;
//			}
//		}
//	}
	return result;
}

void lng_node_class_build(struct lng_node_class_t* self, unsigned char** bytes) {
//	char* text = (char*) compile->syntax->text;
//	{
//		int32 size = self->field_count;
//		lng_io_uint32_write(compile->bytes_next, size);
//		compile->bytes_next += 4;
//	}
//	{
//		if (self->field_count > 0) {
//			struct lng_node_field_t* field = self->fields;
//			while (field->vtable) {
//				field->vtable->build((struct lng_node_t*) field);
//				field++;
//			}
//		}
//	}
//	{
//		int32 size = self->method_count;
//		lng_io_uint32_write(compile->bytes_next, size);
//		compile->bytes_next += 4;
//	}
//	{
//		if (self->method_count > 0) {
//			struct lng_node_method_t* method = self->methods;
//			while (method->vtable) {
//				struct lng_token_t* name = method->name;
//				int32 size = name->size;
//				lng_io_uint32_write(compile->bytes_next, size);
//				compile->bytes_next += 4;
//				lng_io_chars_write(compile->bytes_next, text + name->begin, size);
//				compile->bytes_next += size;
//				method++;
//			}
//		}
//	}
//	{
//		if (self->method_count > 0) {
//			struct lng_node_method_t* method = self->methods;
//			while (method->vtable) {
//				method->vtable->build((struct lng_node_t*) method);
//				method++;
//			}
//		}
//	}
}

void lng_node_field_head(struct lng_node_field_t* self) {
}

void lng_node_field_body(struct lng_node_field_t* self) {
}

void lng_node_field_link(struct lng_node_field_t* self, struct lng_compile_t* compiler) {
}

uint32_t lng_node_field_size(struct lng_node_field_t* self) {
	return 0;
}

void lng_node_field_build(struct lng_node_field_t* self, unsigned char** bytes) {
}

void lng_node_method_head(struct lng_node_method_t* self) {
	self->type->vtable->head((struct lng_node_t*) self->type);
	struct lng_node_param_t* param = self->parameters;
	while (param->vtable) {
		param->vtable->head((struct lng_node_t*) param);
		param++;
	}
	self->cmd->vtable->head((struct lng_node_t*) self->cmd);
}

void lng_node_method_body(struct lng_node_method_t* self) {
	self->type->vtable->body((struct lng_node_t*) self->type);
	struct lng_node_param_t* param = self->parameters;
	while (param->vtable) {
		param->vtable->body((struct lng_node_t*) param);
		param++;
	}
	self->cmd->vtable->body((struct lng_node_t*) self->cmd);
}

void lng_node_method_link(struct lng_node_method_t* self, struct lng_compile_t* compiler) {
	self->type->vtable->link((struct lng_node_t*) self->type, compiler);
	struct lng_node_param_t* param = self->parameters;
	while (param->vtable) {
		param->vtable->link((struct lng_node_t*) param, compiler);
		param++;
	}
	self->cmd->vtable->link((struct lng_node_t*) self->cmd, compiler);
}

uint32_t lng_node_method_size(struct lng_node_method_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = self->cmd->vtable->size((struct lng_node_t*) self->cmd);
	}
	return self->bytes_sizeof;
}

void lng_node_method_build(struct lng_node_method_t* self, unsigned char** bytes) {
//	self->cmd->vtable->build((struct lng_node_t*) self->cmd);
}

void lng_node_type_int_head(struct lng_node_type_int_t* self) {
}

void lng_node_type_int_body(struct lng_node_type_int_t* self) {
}

void lng_node_type_int_link(struct lng_node_type_int_t* self, struct lng_compile_t* compiler) {
}

uint32_t lng_node_type_int_size(struct lng_node_type_int_t* self) {
	return 0;
}

void lng_node_type_int_build(struct lng_node_type_int_t* self, unsigned char** bytes) {
}

void lng_node_type_bool_head(struct lng_node_type_bool_t* self) {
}

void lng_node_type_bool_body(struct lng_node_type_bool_t* self) {
}

void lng_node_type_bool_link(struct lng_node_type_bool_t* self, struct lng_compile_t* compiler) {
}

uint32_t lng_node_type_bool_size(struct lng_node_type_bool_t* self) {
	return 0;
}

void lng_node_type_bool_build(struct lng_node_type_bool_t* self, unsigned char** bytes) {
}

void lng_node_type_str_head(struct lng_node_type_str_t* self) {
}

void lng_node_type_str_body(struct lng_node_type_str_t* self) {
}

void lng_node_type_str_link(struct lng_node_type_str_t* self, struct lng_compile_t* compiler) {
}

uint32_t lng_node_type_str_size(struct lng_node_type_str_t* self) {
	return 0;
}

void lng_node_type_str_build(struct lng_node_type_str_t* self, unsigned char** bytes) {
}

void lng_node_param_head(struct lng_node_param_t* self) {
	self->type->vtable->head((struct lng_node_t*) self->type);
}

void lng_node_param_body(struct lng_node_param_t* self) {
	self->type->vtable->body((struct lng_node_t*) self->type);
}

void lng_node_param_link(struct lng_node_param_t* self, struct lng_compile_t* compiler) {
	self->type->vtable->link((struct lng_node_t*) self->type, compiler);
}

uint32_t lng_node_param_size(struct lng_node_unit_t* self) {
	return 0;
}

void lng_node_param_build(struct lng_node_param_t* self, unsigned char** bytes) {
}

void lng_node_block_head(struct lng_node_block_t* self) {
	if (self->cmds) {
		struct lng_node_command_t** cmds = self->cmds;
		while (*cmds) {
			struct lng_node_command_t* cmd = *(cmds++);
			cmd->vtable->head((struct lng_node_t*) cmd);
		}
	}
}

void lng_node_block_body(struct lng_node_block_t* self) {
	if (self->cmds) {
		struct lng_node_command_t** cmds = self->cmds;
		while (*cmds) {
			struct lng_node_command_t* cmd = *(cmds++);
			cmd->vtable->body((struct lng_node_t*) cmd);
		}
	}
}

void lng_node_block_link(struct lng_node_block_t* self, struct lng_compile_t* compiler) {
	if (self->cmds) {
		struct lng_node_command_t** cmds = self->cmds;
		while (*cmds) {
			compiler->stack_offset = 0;
			struct lng_node_command_t* cmd = *(cmds++);
			cmd->vtable->link((struct lng_node_t*) cmd, compiler);
		}
	}
}

uint32_t lng_node_block_size(struct lng_node_block_t* self) {
	if (!self->bytes_sizeof) {
		if (self->cmds) {
			struct lng_node_command_t** cmds = self->cmds;
			while (*cmds) {
				struct lng_node_command_t* cmd = *(cmds++);
				self->bytes_sizeof += cmd->vtable->size((struct lng_node_t*) cmd);
			}
		}
	}
	return self->bytes_sizeof;
}

void lng_node_block_build(struct lng_node_block_t* self, unsigned char** bytes) {
	if (self->cmds) {
		struct lng_node_command_t** cmds = self->cmds;
		while (*cmds) {
			struct lng_node_command_t* cmd = *(cmds++);
			cmd->vtable->build((struct lng_node_t*) cmd, bytes);
		}
	}
}

void lng_node_return_head(struct lng_node_return_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
}

void lng_node_return_body(struct lng_node_return_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
	struct lng_node_method_t* method_node = (struct lng_node_method_t*) lng_node_find(self->parent, LNG_NODE_METHOD_TYPE);
	self->pop_size = method_node->declares;
}

void lng_node_return_link(struct lng_node_return_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
}

uint32_t lng_node_return_size(struct lng_node_return_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = 5 + self->left->vtable->size((struct lng_node_t*) self->left);
	}
	return self->bytes_sizeof;
}

void lng_node_return_build(struct lng_node_return_t* self, unsigned char** bytes) {
	self->left->vtable->build((struct lng_node_t*) self->left, bytes);
	lng_opcode_int_return(*bytes, self->pop_size);
}

void lng_node_expression_head(struct lng_node_expression_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
}

void lng_node_expression_body(struct lng_node_expression_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
}

void lng_node_expression_link(struct lng_node_expression_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
	compiler->stack_offset--;
}

uint32_t lng_node_expression_size(struct lng_node_expression_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = 1 + self->left->vtable->size((struct lng_node_t*) self->left);
	}
	return self->bytes_sizeof;
}

void lng_node_expression_build(struct lng_node_expression_t* self, unsigned char** bytes) {
	self->left->vtable->build((struct lng_node_t*) self->left, bytes);
	lng_opcode_stack_popone(*bytes);
}

void lng_node_declare_head(struct lng_node_declare_t* self) {
	self->type->vtable->head((struct lng_node_t*) self->type);
	self->value->vtable->head((struct lng_node_t*) self->value);
	struct lng_node_method_t* method_node = (struct lng_node_method_t*) lng_node_find(self->parent, LNG_NODE_METHOD_TYPE);
	method_node->declares++;
}

void lng_node_declare_body(struct lng_node_declare_t* self) {
	self->type->vtable->body((struct lng_node_t*) self->type);
	self->value->vtable->body((struct lng_node_t*) self->value);
	self->index = lng_node_declare_find(self->parent, self->name);
}

void lng_node_declare_link(struct lng_node_declare_t* self, struct lng_compile_t* compiler) {
	self->type->vtable->link((struct lng_node_t*) self->type, compiler);
	self->index += compiler->stack_offset;
	self->value->vtable->link((struct lng_node_t*) self->value, compiler);
}

uint32_t lng_node_declare_size(struct lng_node_declare_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = 5 + self->value->vtable->size((struct lng_node_t*) self->value);
	}
	return self->bytes_sizeof;
}

void lng_node_declare_build(struct lng_node_declare_t* self, unsigned char** bytes) {
	self->value->vtable->build((struct lng_node_t*) self->value, bytes);
	lng_opcode_stack_store(*bytes, self->index + 1);
}

void lng_node_int_head(struct lng_node_int_t* self) {
}

void lng_node_int_body(struct lng_node_int_t* self) {
}

void lng_node_int_link(struct lng_node_int_t* self, struct lng_compile_t* compiler) {
	compiler->stack_offset++;
}

uint32_t lng_node_int_size(struct lng_node_int_t* self) {
	return 5;
}

void lng_node_int_build(struct lng_node_int_t* self, unsigned char** bytes) {
	lng_opcode_int_load(*bytes, self->value);
}

void lng_node_str_head(struct lng_node_str_t* self) {
}

void lng_node_str_body(struct lng_node_str_t* self) {
}

void lng_node_str_link(struct lng_node_str_t* self, struct lng_compile_t* compiler) {
}

uint32_t lng_node_str_size(struct lng_node_str_t* self) {
	return 5;
}

void lng_node_str_build(struct lng_node_str_t* self, unsigned char** bytes) {
	// TODO
	assert(0);
}

void lng_node_array_head(struct lng_node_array_t* self) {
}

void lng_node_array_body(struct lng_node_array_t* self) {
}

void lng_node_array_link(struct lng_node_array_t* self, struct lng_compile_t* compiler) {
}

uint32_t lng_node_array_size(struct lng_node_array_t* self) {
	return 5;
}

void lng_node_array_build(struct lng_node_array_t* self, unsigned char** bytes) {
	// TODO
	assert(0);
}

void lng_node_assign_head(struct lng_node_assign_t* self) {
}

void lng_node_assign_body(struct lng_node_assign_t* self) {
}

void lng_node_assign_link(struct lng_node_assign_t* self, struct lng_compile_t* compiler) {
}

uint32_t lng_node_assign_size(struct lng_node_assign_t* self) {
	return 5;
}

void lng_node_assign_build(struct lng_node_assign_t* self, unsigned char** bytes) {
	// TODO
	assert(0);
}

void lng_node_bool_head(struct lng_node_bool_t* self) {
}

void lng_node_bool_body(struct lng_node_bool_t* self) {
}

void lng_node_bool_link(struct lng_node_bool_t* self, struct lng_compile_t* compiler) {
	compiler->stack_offset++;
}

uint32_t lng_node_bool_size(struct lng_node_bool_t* self) {
	return 1;
}

void lng_node_bool_build(struct lng_node_bool_t* self, unsigned char** bytes) {
	if (self->value) {
		lng_opcode_bool_true(*bytes);
	} else {
		lng_opcode_bool_false(*bytes);
	}
}

void lng_node_neg_head(struct lng_node_neg_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
}

void lng_node_neg_body(struct lng_node_neg_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
}

void lng_node_neg_link(struct lng_node_neg_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
}

uint32_t lng_node_neg_size(struct lng_node_neg_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = 5 + 1 + self->left->vtable->size((struct lng_node_t*) self->left);
	}
	return self->bytes_sizeof;
}

void lng_node_neg_build(struct lng_node_neg_t* self, unsigned char** bytes) {
	self->left->vtable->build((struct lng_node_t*) self->left, bytes);
	lng_opcode_int_load(*bytes, -1);
	lng_opcode_int_mul(*bytes);
}

void lng_node_id_head(struct lng_node_id_t* self) {
}

void lng_node_id_body(struct lng_node_id_t* self) {
	int32_t index = lng_node_declare_find(self->parent, self->name);
	if (index < 0) {
		struct lng_node_unit_t* unit_node = (struct lng_node_unit_t*) lng_node_find(self->parent, LNG_NODE_UNIT_TYPE);
		unit_node->errors[unit_node->error_index++] = "not found a variable";
		index = 0;
	}
	self->index = index;
}

void lng_node_id_link(struct lng_node_id_t* self, struct lng_compile_t* compiler) {
	self->index += compiler->stack_offset;
	compiler->stack_offset++;
}

uint32_t lng_node_id_size(struct lng_node_id_t* self) {
	return 5;
}

void lng_node_id_build(struct lng_node_id_t* self, unsigned char** bytes) {
	lng_opcode_stack_load(*bytes, self->index);
}

void lng_node_ternary_head(struct lng_node_ternary_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
	self->center->vtable->head((struct lng_node_t*) self->center);
	self->right->vtable->head((struct lng_node_t*) self->right);
}

void lng_node_ternary_body(struct lng_node_ternary_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
	self->center->vtable->body((struct lng_node_t*) self->center);
	self->right->vtable->body((struct lng_node_t*) self->right);
}

void lng_node_ternary_link(struct lng_node_ternary_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
	assert(0);
	// compiler->stack_offset++;
	self->center->vtable->link((struct lng_node_t*) self->center, compiler);
	self->right->vtable->link((struct lng_node_t*) self->right, compiler);
}

uint32_t lng_node_ternary_size(struct lng_node_ternary_t* self) {
	// TODO
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = self->left->vtable->size((struct lng_node_t*) self->left) + self->right->vtable->size((struct lng_node_t*) self->right) + 1;
	}
	return self->bytes_sizeof;
}

void lng_node_ternary_build(struct lng_node_ternary_t* self, unsigned char** bytes) {
	// TODO
}

void lng_node_and_head(struct lng_node_and_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
	self->right->vtable->head((struct lng_node_t*) self->right);
}

void lng_node_and_body(struct lng_node_and_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
	self->right->vtable->body((struct lng_node_t*) self->right);
}

void lng_node_and_link(struct lng_node_and_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
	self->right->vtable->link((struct lng_node_t*) self->right, compiler);
	compiler->stack_offset--;
}

uint32_t lng_node_and_size(struct lng_node_and_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = self->left->vtable->size((struct lng_node_t*) self->left) + self->right->vtable->size((struct lng_node_t*) self->right) + 1;
	}
	return self->bytes_sizeof;
}

void lng_node_and_build(struct lng_node_and_t* self, unsigned char** bytes) {
	self->left->vtable->build((struct lng_node_t*) self->left, bytes);
	self->right->vtable->build((struct lng_node_t*) self->right, bytes);
	lng_opcode_int_sum(*bytes);
}

void lng_node_or_head(struct lng_node_or_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
	self->right->vtable->head((struct lng_node_t*) self->right);
}

void lng_node_or_body(struct lng_node_or_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
	self->right->vtable->body((struct lng_node_t*) self->right);
}

void lng_node_or_link(struct lng_node_or_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
	self->right->vtable->link((struct lng_node_t*) self->right, compiler);
	compiler->stack_offset--;
}

uint32_t lng_node_or_size(struct lng_node_or_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = self->left->vtable->size((struct lng_node_t*) self->left) + self->right->vtable->size((struct lng_node_t*) self->right) + 1;
	}
	return self->bytes_sizeof;
}

void lng_node_or_build(struct lng_node_or_t* self, unsigned char** bytes) {
	self->left->vtable->build((struct lng_node_t*) self->left, bytes);
	self->right->vtable->build((struct lng_node_t*) self->right, bytes);
	lng_opcode_bool_or(*bytes);
}

void lng_node_eq_head(struct lng_node_eq_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
	self->right->vtable->head((struct lng_node_t*) self->right);
}

void lng_node_eq_body(struct lng_node_eq_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
	self->right->vtable->body((struct lng_node_t*) self->right);
}

void lng_node_eq_link(struct lng_node_eq_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
	self->right->vtable->link((struct lng_node_t*) self->right, compiler);
	compiler->stack_offset--;
}

uint32_t lng_node_eq_size(struct lng_node_eq_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = self->left->vtable->size((struct lng_node_t*) self->left) + self->right->vtable->size((struct lng_node_t*) self->right) + 1;
	}
	return self->bytes_sizeof;
}

void lng_node_eq_build(struct lng_node_eq_t* self, unsigned char** bytes) {
	self->left->vtable->build((struct lng_node_t*) self->left, bytes);
	self->right->vtable->build((struct lng_node_t*) self->right, bytes);
	lng_opcode_int_eq(*bytes);
}

void lng_node_ne_head(struct lng_node_ne_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
	self->right->vtable->head((struct lng_node_t*) self->right);
}

void lng_node_ne_body(struct lng_node_ne_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
	self->right->vtable->body((struct lng_node_t*) self->right);
}

void lng_node_ne_link(struct lng_node_ne_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
	self->right->vtable->link((struct lng_node_t*) self->right, compiler);
	compiler->stack_offset--;
}

uint32_t lng_node_ne_size(struct lng_node_ne_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = self->left->vtable->size((struct lng_node_t*) self->left) + self->right->vtable->size((struct lng_node_t*) self->right) + 1;
	}
	return self->bytes_sizeof;
}

void lng_node_ne_build(struct lng_node_ne_t* self, unsigned char** bytes) {
	self->left->vtable->build((struct lng_node_t*) self->left, bytes);
	self->right->vtable->build((struct lng_node_t*) self->right, bytes);
	lng_opcode_int_ne(*bytes);
}

void lng_node_gt_head(struct lng_node_gt_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
	self->right->vtable->head((struct lng_node_t*) self->right);
}

void lng_node_gt_body(struct lng_node_gt_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
	self->right->vtable->body((struct lng_node_t*) self->right);
}

void lng_node_gt_link(struct lng_node_gt_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
	self->right->vtable->link((struct lng_node_t*) self->right, compiler);
	compiler->stack_offset--;
}

uint32_t lng_node_gt_size(struct lng_node_gt_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = self->left->vtable->size((struct lng_node_t*) self->left) + self->right->vtable->size((struct lng_node_t*) self->right) + 1;
	}
	return self->bytes_sizeof;
}

void lng_node_gt_build(struct lng_node_gt_t* self, unsigned char** bytes) {
	self->left->vtable->build((struct lng_node_t*) self->left, bytes);
	self->right->vtable->build((struct lng_node_t*) self->right, bytes);
	lng_opcode_int_gt(*bytes);
}

void lng_node_ge_head(struct lng_node_ge_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
	self->right->vtable->head((struct lng_node_t*) self->right);
}

void lng_node_ge_body(struct lng_node_ge_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
	self->right->vtable->body((struct lng_node_t*) self->right);
}

void lng_node_ge_link(struct lng_node_ge_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
	self->right->vtable->link((struct lng_node_t*) self->right, compiler);
	compiler->stack_offset--;
}

uint32_t lng_node_ge_size(struct lng_node_ge_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = self->left->vtable->size((struct lng_node_t*) self->left) + self->right->vtable->size((struct lng_node_t*) self->right) + 1;
	}
	return self->bytes_sizeof;
}

void lng_node_ge_build(struct lng_node_ge_t* self, unsigned char** bytes) {
	self->left->vtable->build((struct lng_node_t*) self->left, bytes);
	self->right->vtable->build((struct lng_node_t*) self->right, bytes);
	lng_opcode_int_ge(*bytes);
}

void lng_node_lt_head(struct lng_node_lt_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
	self->right->vtable->head((struct lng_node_t*) self->right);
}

void lng_node_lt_body(struct lng_node_lt_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
	self->right->vtable->body((struct lng_node_t*) self->right);
}

void lng_node_lt_link(struct lng_node_lt_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
	self->right->vtable->link((struct lng_node_t*) self->right, compiler);
	compiler->stack_offset--;
}

uint32_t lng_node_lt_size(struct lng_node_lt_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = self->left->vtable->size((struct lng_node_t*) self->left) + self->right->vtable->size((struct lng_node_t*) self->right) + 1;
	}
	return self->bytes_sizeof;
}

void lng_node_lt_build(struct lng_node_lt_t* self, unsigned char** bytes) {
	self->left->vtable->build((struct lng_node_t*) self->left, bytes);
	self->right->vtable->build((struct lng_node_t*) self->right, bytes);
	lng_opcode_int_lt(*bytes);
}

void lng_node_le_head(struct lng_node_le_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
	self->right->vtable->head((struct lng_node_t*) self->right);
}

void lng_node_le_body(struct lng_node_le_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
	self->right->vtable->body((struct lng_node_t*) self->right);
}

void lng_node_le_link(struct lng_node_le_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
	self->right->vtable->link((struct lng_node_t*) self->right, compiler);
	compiler->stack_offset--;
}

uint32_t lng_node_le_size(struct lng_node_le_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = self->left->vtable->size((struct lng_node_t*) self->left) + self->right->vtable->size((struct lng_node_t*) self->right) + 1;
	}
	return self->bytes_sizeof;
}

void lng_node_le_build(struct lng_node_le_t* self, unsigned char** bytes) {
	self->left->vtable->build((struct lng_node_t*) self->left, bytes);
	self->right->vtable->build((struct lng_node_t*) self->right, bytes);
	lng_opcode_int_le(*bytes);
}

void lng_node_sum_head(struct lng_node_sum_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
	self->right->vtable->head((struct lng_node_t*) self->right);
}

void lng_node_sum_body(struct lng_node_sum_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
	self->right->vtable->body((struct lng_node_t*) self->right);
}

void lng_node_sum_link(struct lng_node_sum_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
	self->right->vtable->link((struct lng_node_t*) self->right, compiler);
	compiler->stack_offset--;
}

uint32_t lng_node_sum_size(struct lng_node_sum_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = self->left->vtable->size((struct lng_node_t*) self->left) + self->right->vtable->size((struct lng_node_t*) self->right) + 1;
	}
	return self->bytes_sizeof;
}

void lng_node_sum_build(struct lng_node_sum_t* self, unsigned char** bytes) {
	self->left->vtable->build((struct lng_node_t*) self->left, bytes);
	self->right->vtable->build((struct lng_node_t*) self->right, bytes);
	lng_opcode_int_sum(*bytes);
}

void lng_node_sub_head(struct lng_node_sub_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
	self->right->vtable->head((struct lng_node_t*) self->right);
}

void lng_node_sub_body(struct lng_node_sub_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
	self->right->vtable->body((struct lng_node_t*) self->right);
}

void lng_node_sub_link(struct lng_node_sub_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
	self->right->vtable->link((struct lng_node_t*) self->right, compiler);
	compiler->stack_offset--;
}

uint32_t lng_node_sub_size(struct lng_node_sub_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = self->left->vtable->size((struct lng_node_t*) self->left) + self->right->vtable->size((struct lng_node_t*) self->right) + 1;
	}
	return self->bytes_sizeof;
}

void lng_node_sub_build(struct lng_node_sub_t* self, unsigned char** bytes) {
	self->left->vtable->build((struct lng_node_t*) self->left, bytes);
	self->right->vtable->build((struct lng_node_t*) self->right, bytes);
	lng_opcode_int_sub(*bytes);
}

void lng_node_mul_head(struct lng_node_mul_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
	self->right->vtable->head((struct lng_node_t*) self->right);
}

void lng_node_mul_body(struct lng_node_mul_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
	self->right->vtable->body((struct lng_node_t*) self->right);
}

void lng_node_mul_link(struct lng_node_mul_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
	self->right->vtable->link((struct lng_node_t*) self->right, compiler);
	compiler->stack_offset--;
}

uint32_t lng_node_mul_size(struct lng_node_mul_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = self->left->vtable->size((struct lng_node_t*) self->left) + self->right->vtable->size((struct lng_node_t*) self->right) + 1;
	}
	return self->bytes_sizeof;
}

void lng_node_mul_build(struct lng_node_mul_t* self, unsigned char** bytes) {
	self->left->vtable->build((struct lng_node_t*) self->left, bytes);
	self->right->vtable->build((struct lng_node_t*) self->right, bytes);
	lng_opcode_int_mul(*bytes);
}

void lng_node_div_head(struct lng_node_div_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
	self->right->vtable->head((struct lng_node_t*) self->right);
}

void lng_node_div_body(struct lng_node_div_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
	self->right->vtable->body((struct lng_node_t*) self->right);
}

void lng_node_div_link(struct lng_node_div_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
	self->right->vtable->link((struct lng_node_t*) self->right, compiler);
	compiler->stack_offset--;
}

uint32_t lng_node_div_size(struct lng_node_div_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = self->left->vtable->size((struct lng_node_t*) self->left) + self->right->vtable->size((struct lng_node_t*) self->right) + 1;
	}
	return self->bytes_sizeof;
}

void lng_node_div_build(struct lng_node_div_t* self, unsigned char** bytes) {
	self->left->vtable->build((struct lng_node_t*) self->left, bytes);
	self->right->vtable->build((struct lng_node_t*) self->right, bytes);
	lng_opcode_int_div(*bytes);
}

void lng_node_not_head(struct lng_node_not_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
}

void lng_node_not_body(struct lng_node_not_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
}

void lng_node_not_link(struct lng_node_not_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
}

uint32_t lng_node_not_size(struct lng_node_not_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = self->left->vtable->size((struct lng_node_t*) self->left) + 1;
	}
	return self->bytes_sizeof;
}

void lng_node_not_build(struct lng_node_not_t* self, unsigned char** bytes) {
	self->left->vtable->build((struct lng_node_t*) self->left, bytes);
	// TODO lng_opcode_int_div(*bytes);
}

void lng_node_preinc_head(struct lng_node_preinc_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
}

void lng_node_preinc_body(struct lng_node_preinc_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
}

void lng_node_preinc_link(struct lng_node_preinc_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
	if (self->left->vtable->classid == LNG_NODE_ID_TYPE) {
		self->index = ((struct lng_node_id_t*) self->left)->index;
	} else {
		assert(0);
	}
}

uint32_t lng_node_preinc_size(struct lng_node_preinc_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = 5;
	}
	return self->bytes_sizeof;
}

void lng_node_preinc_build(struct lng_node_preinc_t* self, unsigned char** bytes) {
	lng_opcode_int_preinc(*bytes, self->index);
}

void lng_node_predec_head(struct lng_node_predec_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
}

void lng_node_predec_body(struct lng_node_predec_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
}

void lng_node_predec_link(struct lng_node_predec_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
	if (self->left->vtable->classid == LNG_NODE_ID_TYPE) {
		self->index = ((struct lng_node_id_t*) self->left)->index ;
	} else {
		assert(0);
	}
}

uint32_t lng_node_predec_size(struct lng_node_predec_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = 5;
	}
	return self->bytes_sizeof;
}

void lng_node_predec_build(struct lng_node_predec_t* self, unsigned char** bytes) {
	lng_opcode_int_predec(*bytes, self->index);
}

void lng_node_posinc_head(struct lng_node_posinc_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
}

void lng_node_posinc_body(struct lng_node_posinc_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
}

void lng_node_posinc_link(struct lng_node_posinc_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
	if (self->left->vtable->classid == LNG_NODE_ID_TYPE) {
		self->index = ((struct lng_node_id_t*) self->left)->index ;
	} else {
		assert(0);
	}
}

uint32_t lng_node_posinc_size(struct lng_node_posinc_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = 5;
	}
	return self->bytes_sizeof;
}

void lng_node_posinc_build(struct lng_node_posinc_t* self, unsigned char** bytes) {
	lng_opcode_int_posinc(*bytes, self->index);
}

void lng_node_posdec_head(struct lng_node_posdec_t* self) {
	self->left->vtable->head((struct lng_node_t*) self->left);
}

void lng_node_posdec_body(struct lng_node_posdec_t* self) {
	self->left->vtable->body((struct lng_node_t*) self->left);
}

void lng_node_posdec_link(struct lng_node_posdec_t* self, struct lng_compile_t* compiler) {
	self->left->vtable->link((struct lng_node_t*) self->left, compiler);
	if (self->left->vtable->classid == LNG_NODE_ID_TYPE) {
		self->index = ((struct lng_node_id_t*) self->left)->index ;
	} else {
		assert(0);
	}
}

uint32_t lng_node_posdec_size(struct lng_node_posdec_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof = 5;
	}
	return self->bytes_sizeof;
}

void lng_node_posdec_build(struct lng_node_posdec_t* self, unsigned char** bytes) {
	lng_opcode_int_posdec(*bytes, self->index);
}

void lng_node_if_head(struct lng_node_if_t* self) {
	self->cond->vtable->head((struct lng_node_t*) self->cond);
	self->true_cmd->vtable->head((struct lng_node_t*) self->true_cmd);
	if (self->false_cmd) {
		self->false_cmd->vtable->head((struct lng_node_t*) self->false_cmd);
	}
}

void lng_node_if_body(struct lng_node_if_t* self) {
	self->cond->vtable->body((struct lng_node_t*) self->cond);
	self->true_cmd->vtable->body((struct lng_node_t*) self->true_cmd);
	if (self->false_cmd) {
		self->false_cmd->vtable->body((struct lng_node_t*) self->false_cmd);
	}
}

void lng_node_if_link(struct lng_node_if_t* self, struct lng_compile_t* compiler) {
	self->cond->vtable->link((struct lng_node_t*) self->cond, compiler);
	compiler->stack_offset--;
	self->true_cmd->vtable->link((struct lng_node_t*) self->true_cmd, compiler);
	if (self->false_cmd) {
		self->false_cmd->vtable->link((struct lng_node_t*) self->false_cmd, compiler);
	}
}

uint32_t lng_node_if_size(struct lng_node_if_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof += self->cond->vtable->size((struct lng_node_t*) self->cond);
		self->bytes_sizeof += 5;
		self->bytes_sizeof += self->true_cmd->vtable->size((struct lng_node_t*) self->true_cmd);
		if (self->false_cmd) {
			self->bytes_sizeof += 5;
			self->bytes_sizeof += self->false_cmd->vtable->size((struct lng_node_t*) self->false_cmd);
		}
	}
	return self->bytes_sizeof;
}

void lng_node_if_build(struct lng_node_if_t* self, unsigned char** bytes) {
	uint32_t true_cmd_size = self->true_cmd->vtable->size((struct lng_node_t*) self->true_cmd);
	self->cond->vtable->build((struct lng_node_t*) self->cond, bytes);
	if (self->false_cmd) {
		uint32_t false_cmd_size = self->false_cmd->vtable->size((struct lng_node_t*) self->false_cmd);
		uint32_t jmp_false_index = true_cmd_size + 5;
		lng_opcode_jmp_false(*bytes, jmp_false_index);
		self->true_cmd->vtable->build((struct lng_node_t*) self->true_cmd, bytes);
		uint32_t jmp_index = false_cmd_size + 5;
		lng_opcode_jmp(*bytes, jmp_index);
		self->false_cmd->vtable->build((struct lng_node_t*) self->false_cmd, bytes);
	} else {
		lng_opcode_jmp_false(*bytes, true_cmd_size);
		self->true_cmd->vtable->build((struct lng_node_t*) self->true_cmd, bytes);
	}
}

void lng_node_while_head(struct lng_node_while_t* self) {
	self->cond->vtable->head((struct lng_node_t*) self->cond);
	self->cmd->vtable->head((struct lng_node_t*) self->cmd);
}

void lng_node_while_body(struct lng_node_while_t* self) {
	self->cond->vtable->body((struct lng_node_t*) self->cond);
	self->cmd->vtable->body((struct lng_node_t*) self->cmd);
}

void lng_node_while_link(struct lng_node_while_t* self, struct lng_compile_t* compiler) {
	self->cond->vtable->link((struct lng_node_t*) self->cond, compiler);
	compiler->stack_offset--;
	self->cmd->vtable->link((struct lng_node_t*) self->cmd, compiler);
}

uint32_t lng_node_while_size(struct lng_node_while_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof += self->cond->vtable->size((struct lng_node_t*) self->cond);
		self->bytes_sizeof += 5;
		self->bytes_sizeof += self->cmd->vtable->size((struct lng_node_t*) self->cmd);
		self->bytes_sizeof += 5;
	}
	return self->bytes_sizeof;
}

void lng_node_while_build(struct lng_node_while_t* self, unsigned char** bytes) {
	uint32_t cond_size = self->cond->vtable->size((struct lng_node_t*) self->cond);
	uint32_t cmd_size = self->cmd->vtable->size((struct lng_node_t*) self->cmd);
	self->cond->vtable->build((struct lng_node_t*) self->cond, bytes);
	uint32_t jmp_false_index = cmd_size + 5;
	lng_opcode_jmp_false(*bytes, jmp_false_index);
	self->cmd->vtable->build((struct lng_node_t*) self->cmd, bytes);
	int32_t jmp_index = -(5 + cmd_size + 5 + cond_size);
	lng_opcode_jmp(*bytes, jmp_index);
}

void lng_node_repeat_head(struct lng_node_repeat_t* self) {
	self->cmd->vtable->head((struct lng_node_t*) self->cmd);
	self->cond->vtable->head((struct lng_node_t*) self->cond);
}

void lng_node_repeat_body(struct lng_node_repeat_t* self) {
	self->cmd->vtable->body((struct lng_node_t*) self->cmd);
	self->cond->vtable->body((struct lng_node_t*) self->cond);
}

void lng_node_repeat_link(struct lng_node_repeat_t* self, struct lng_compile_t* compiler) {
	self->cmd->vtable->link((struct lng_node_t*) self->cmd, compiler);
	self->cond->vtable->link((struct lng_node_t*) self->cond, compiler);
	compiler->stack_offset--;
}

uint32_t lng_node_repeat_size(struct lng_node_repeat_t* self) {
	if (!self->bytes_sizeof) {
		self->bytes_sizeof += self->cond->vtable->size((struct lng_node_t*) self->cond);
		self->bytes_sizeof += self->cmd->vtable->size((struct lng_node_t*) self->cmd);
		self->bytes_sizeof += 5;
	}
	return self->bytes_sizeof;
}

void lng_node_repeat_build(struct lng_node_repeat_t* self, unsigned char** bytes) {
	uint32_t cmd_size = self->cmd->vtable->size((struct lng_node_t*) self->cmd);
	uint32_t cond_size = self->cond->vtable->size((struct lng_node_t*) self->cond);
	self->cmd->vtable->build((struct lng_node_t*) self->cmd, bytes);
	self->cond->vtable->build((struct lng_node_t*) self->cond, bytes);
	uint32_t jmp_index = -(5 + cond_size + cmd_size);
	lng_opcode_jmp_true(*bytes, jmp_index);
}

void lng_node_forn_head(struct lng_node_forn_t* self) {
	if (self->init) {
		self->init->vtable->head((struct lng_node_t*) self->init);
	}
	if (self->cond) {
		self->cond->vtable->head((struct lng_node_t*) self->cond);
	}
	if (self->inc) {
		self->inc->vtable->head((struct lng_node_t*) self->inc);
	}
	self->cmd->vtable->head((struct lng_node_t*) self->cmd);
}

void lng_node_forn_body(struct lng_node_forn_t* self) {
	if (self->init) {
		self->init->vtable->body((struct lng_node_t*) self->init);
	}
	if (self->cond) {
		self->cond->vtable->body((struct lng_node_t*) self->cond);
	}
	if (self->inc) {
		self->inc->vtable->body((struct lng_node_t*) self->inc);
	}
	self->cmd->vtable->body((struct lng_node_t*) self->cmd);
}

void lng_node_forn_link(struct lng_node_forn_t* self, struct lng_compile_t* compiler) {
	if (self->init) {
		self->init->vtable->link((struct lng_node_t*) self->init, compiler);
		compiler->stack_offset--;
	}
	if (self->cond) {
		self->cond->vtable->link((struct lng_node_t*) self->cond, compiler);
		compiler->stack_offset--;
	}
	if (self->inc) {
		self->inc->vtable->link((struct lng_node_t*) self->inc, compiler);
		compiler->stack_offset--;
	}
	self->cmd->vtable->link((struct lng_node_t*) self->cmd, compiler);
}

uint32_t lng_node_forn_size(struct lng_node_forn_t* self) {
	if (!self->bytes_sizeof) {
		if (self->init) {
			self->bytes_sizeof += self->init->vtable->size((struct lng_node_t*) self->init);
		}
		if (self->cond) {
			self->bytes_sizeof += self->cond->vtable->size((struct lng_node_t*) self->cond);
			self->bytes_sizeof += 5;
		}
		self->bytes_sizeof += self->cmd->vtable->size((struct lng_node_t*) self->cmd);
		if (self->inc) {
			self->bytes_sizeof += self->inc->vtable->size((struct lng_node_t*) self->inc);
		}
		self->bytes_sizeof += 5;
	}
	return self->bytes_sizeof;
}

void lng_node_forn_build(struct lng_node_forn_t* self, unsigned char** bytes) {
	uint32_t inc_size = self->inc ? self->inc->vtable->size((struct lng_node_t*) self->inc) : 0;
	uint32_t cond_size = self->cond ? self->cond->vtable->size((struct lng_node_t*) self->cond) : 0;
	uint32_t cmd_size = self->cmd ? self->cmd->vtable->size((struct lng_node_t*) self->cmd) : 0;
	if (self->init) {
		self->init->vtable->build((struct lng_node_t*) self->init, bytes);
	}
	if (self->cond) {
		self->cond->vtable->build((struct lng_node_t*) self->cond, bytes);
		lng_opcode_jmp_false(*bytes, 5 + cmd_size + inc_size + 5);
	}
	self->cmd->vtable->build((struct lng_node_t*) self->cmd, bytes);
	if (self->inc) {
		self->inc->vtable->build((struct lng_node_t*) self->inc, bytes);
	}
	uint32_t jmp_index = -(5 + inc_size + cmd_size + (self->cond ? 5 : 0) + cond_size);
	lng_opcode_jmp(*bytes, jmp_index);
}

void lng_node_foreach_head(struct lng_node_foreach_t* self) {
	// TOOD
	assert(0);
}

void lng_node_foreach_body(struct lng_node_foreach_t* self) {
	// TOOD
	assert(0);
	self->cmd->vtable->body((struct lng_node_t*) self->cmd);
}

void lng_node_foreach_link(struct lng_node_foreach_t* self, struct lng_compile_t* compiler) {
	// TOOD
	assert(0);
	self->cmd->vtable->link((struct lng_node_t*) self->cmd, compiler);
}

uint32_t lng_node_foreach_size(struct lng_node_foreach_t* self) {
	if (!self->bytes_sizeof) {
		// TOOD
		assert(0);
		self->bytes_sizeof += self->cmd->vtable->size((struct lng_node_t*) self->cmd);
	}
	return self->bytes_sizeof;
}

void lng_node_foreach_build(struct lng_node_foreach_t* self, unsigned char** bytes) {
	// TOOD
	assert(0);
}

